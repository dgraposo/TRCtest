#include <stdio.h>
#include <stdbool.h>
#include <json-c/json.h>
#include <string.h>
#include <curl/curl.h>
#include <time.h>

/*
This example project use the Json-C library to decode the objects to C char arrays and 
use the C libcurl library to request the data to the API.
*/


struct string {
	char *ptr;
	size_t len;
};

//Write function to write the payload response in the string structure
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
	size_t new_len = s->len + size*nmemb;
	s->ptr = realloc(s->ptr, new_len + 1);
	if (s->ptr == NULL) {
		fprintf(stderr, "realloc() failed\n");
		exit(EXIT_FAILURE);
	}
	memcpy(s->ptr + s->len, ptr, size*nmemb);
	s->ptr[new_len] = '\0';
	s->len = new_len;

	return size*nmemb;
}

//Initilize the payload string
void init_string(struct string *s) {
	s->len = 0;
	s->ptr = malloc(s->len + 1);
	if (s->ptr == NULL) {
		fprintf(stderr, "malloc() failed\n");
		exit(EXIT_FAILURE);
	}
	s->ptr[0] = '\0';
}

//Update the answer to a specific question
bool update_data(struct json_object *obj_data, char url[])
{
      CURL *hnd = curl_easy_init();

      const char *id = json_object_get_string(json_object_object_get(obj_data, "id"));

      strcat(url,"/");
      strcat(url,id);
      strcat(url,"/attrs?type=answer");

      json_object_object_del(obj_data,"id");
      json_object_object_del(obj_data,"type");

      curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
      curl_easy_setopt(hnd, CURLOPT_URL,url);

      struct curl_slist *headers = NULL;
      headers = curl_slist_append(headers, "Fiware-ServicePath: /");
      headers = curl_slist_append(headers, "Fiware-Service: socialite");
      headers = curl_slist_append(headers, "Content-Type: application/json");
      curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

      curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, json_object_to_json_string(obj_data));


      //Perform the request
      CURLcode ret = curl_easy_perform(hnd);
      if (ret == CURLE_OK){

              long response_code;
              curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE,&response_code);
              printf("\nResponse code:%ld\n",response_code);

              /* always cleanup */
              curl_easy_cleanup(hnd);

              return true;
       }
       else{
              fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
              printf("Error sending Data\n");

              /* always cleanup */
              curl_easy_cleanup(hnd);
              return false;
       }
}

//HTTP Post with the student answer
bool post_data(struct json_object *obj_data, char url[])
{
      CURL *hnd = curl_easy_init();

      curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
      curl_easy_setopt(hnd, CURLOPT_URL,url);

      struct curl_slist *headers = NULL;
      headers = curl_slist_append(headers, "Fiware-ServicePath: /");
      headers = curl_slist_append(headers, "Fiware-Service: socialite");
      headers = curl_slist_append(headers, "Content-Type: application/json");
      curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

      curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, json_object_to_json_string(obj_data));

      //Perform the request
      CURLcode ret = curl_easy_perform(hnd);
      if (ret == CURLE_OK){

	      long response_code;
	      curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE,&response_code);
              printf("\nResponse code:%ld\n",response_code);

	      /* always cleanup */
              curl_easy_cleanup(hnd);

              if(response_code==422)
		 printf("Updating data...\n");
		 return update_data(obj_data,url);

              return true;

       }
       else{
	      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
              printf("Error sending Data\n");

              /* always cleanup */
              curl_easy_cleanup(hnd);
              return false;
       }

}

//Get the Data from the API and return a JSON Object
struct json_object *get_data(char url[])
{
	struct string s;
	struct json_object *jobj;

	//Intialize the CURL request
	CURL *hnd = curl_easy_init();

	//Initilize the char array (string)
	init_string(&s);

	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
	//To run on department network uncomment this request and comment the other
        //To run from outside
	curl_easy_setopt(hnd, CURLOPT_URL, url);

	//Add headers
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "cache-control: no-cache");
	headers = curl_slist_append(headers, "fiware-servicepath: /");
	headers = curl_slist_append(headers, "fiware-service: socialite");

	//Set some options
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, writefunc); //Give the write function here
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &s); //Give the char array address here

	//Perform the request
	CURLcode ret = curl_easy_perform(hnd);
	if (ret != CURLE_OK){
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));

		/*jobj will return empty object*/
		jobj = json_tokener_parse(s.ptr);

		/* always cleanup */
		curl_easy_cleanup(hnd);
		return jobj;

	}
	else if (CURLE_OK == ret) {
		jobj = json_tokener_parse(s.ptr);
		free(s.ptr);

		/* always cleanup */
		curl_easy_cleanup(hnd);
		return jobj;
	}

}

int main(void)
{
	//JSON obect
	struct json_object *jobj_array_questions, *jobj_array_answers, *jobj_obj;
	struct json_object *jobj_object_id, *jobj_object_type, *jobj_object_documents, *jobj_object_options, *jobj_object_solution, *jobj_object_timestamp;
        char url_questions[300] = "http://10.3.4.75:9014/v2/entities/?type=question&limit=100&options=keyValues\0";  //Change the IP to the dns to work otside the department socialiteorion2.dei.uc.pt
        char url_answers[300] = "http://10.3.4.75:9014/v2/entities/?type=answer&options=keyValues&limit=1000\0"; //Change the IP to the dns to work otside the department socialiteorion2.dei.uc.pt
        char url_send_data[300] = "http://10.3.4.75:9014/v2/entities"; //Change the IP to the dns to work otside the department socialiteorion2.dei.uc.pt
	enum json_type type = 0;
	int arraylen = 0;
	int i;
        struct json_object *obj_answer, *obj_timestamp_answer,*obj_solution_answer;
        time_t     now;
        struct tm  ts;
	char current_time[80];

	//Change it, to a specific student, question, and answer!!!!
        char student_id [] = "asjdhsjsdsdadh";
        char question_id []= "999";
        char solution []= "a";
        char request_time[80];

        printf("Retreiving data...\n");
	
        //Get the student data
	jobj_array_questions = get_data(url_questions);
        jobj_array_answers = get_data(url_answers);

	//Get array length
	arraylen = json_object_array_length(jobj_array_questions);

	//Example of howto retrieve the Questions data
	for (i = 0; i < arraylen; i++) {
		//get the i-th object in jobj_array
		jobj_obj = json_object_array_get_idx(jobj_array_questions, i);

		//get the name attribute in the i-th object
		jobj_object_id = json_object_object_get(jobj_obj, "id");
		jobj_object_type = json_object_object_get(jobj_obj, "type");
		jobj_object_documents = json_object_object_get(jobj_obj, "documents");
		jobj_object_options = json_object_object_get(jobj_obj, "options");
		jobj_object_solution = json_object_object_get(jobj_obj, "solution");

		//print out the name attribute
		printf("id=%s\n", json_object_get_string(jobj_object_id));
		printf("type=%s\n", json_object_get_string(jobj_object_type));
		printf("documents=%s\n", json_object_get_string(jobj_object_documents));
		printf("options=%s\n", json_object_get_string(jobj_object_options));
                printf("solution=%s\n", json_object_get_string(jobj_object_solution));
		printf("\n");
	}

        //Get array length
        arraylen = json_object_array_length(jobj_array_answers);

        //Example of howto retrieve the answers data
        for (i = 0; i < arraylen; i++) {
                //get the i-th object in jobj_array
                jobj_obj = json_object_array_get_idx(jobj_array_answers, i);

                //get the name attribute in the i-th object
                jobj_object_id = json_object_object_get(jobj_obj, "id");
                jobj_object_type = json_object_object_get(jobj_obj, "type");
                jobj_object_solution = json_object_object_get(jobj_obj, "solution");
                jobj_object_timestamp = json_object_object_get(jobj_obj, "timestamp");

                //print out the name attribute
                printf("id=%s\n", json_object_get_string(jobj_object_id));
                printf("type=%s\n", json_object_get_string(jobj_object_type));
                printf("solution=%s\n", json_object_get_string(jobj_object_solution));
                printf("timestamp=%s\n", json_object_get_string(jobj_object_timestamp));
                printf("\n");
        }


        // Get current time
        time(&now);
        ts = *localtime(&now);
        strftime(current_time, sizeof(current_time), "%Y-%m-%dT%TZ", &ts);
	strcpy(request_time,current_time);

        //Create an answer
        obj_answer=json_object_new_object();
        obj_solution_answer=json_object_new_object();
        obj_timestamp_answer=json_object_new_object();

        json_object_object_add(obj_answer, "id", json_object_new_string(strcat(strcat(student_id,"_"),question_id)));
        json_object_object_add(obj_answer, "type", json_object_new_string("answer"));
        json_object_object_add(obj_timestamp_answer, "value", json_object_new_string(request_time));
        json_object_object_add(obj_timestamp_answer, "type", json_object_new_string("ISO8601"));
        json_object_object_add(obj_solution_answer, "value", json_object_new_string(solution));
        json_object_object_add(obj_solution_answer, "type", json_object_new_string("text"));
        json_object_object_add(obj_answer, "timestamp", obj_timestamp_answer);
        json_object_object_add(obj_answer, "solution", obj_solution_answer);

	printf("%s",json_object_to_json_string(obj_answer));
	printf("\n");
	post_data(obj_answer,url_send_data);
	return 0;
}
