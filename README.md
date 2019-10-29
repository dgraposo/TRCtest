# IRC Privacy Server
This example gives you a sample code to interact with ISABELA API. The sample code was built to Visual Studio Community. If you want to run the code, with all the libraries already installed and pre-compiled, you need to install the [Visual Studio Community 2013](https://docs.microsoft.com/en-us/visualstudio/releasenotes/vs2013-community-vs) (V120 Build tools). 

If you want to create the example from scracth in another IDE, the following libraries need to be installed:
* The [Libcurl C library](https://curl.haxx.se/libcurl/);
* The [JSON-C library](https://github.com/json-c/json-c);

The data is retrieved by invoking an HTTP GET, and a JSON array object is retrieved by the API (with the data from the students). 
The privacy server will act as an anonymisation server, protecting the privacy of the students. The clients that interact with the privacy server will be able to retrieve their own data (the student ID will act as a token that gives access to individual data). When retrieved other student's data, the proxy will anonymize the data to the clients.

## Code

1. Create a JSON object to store the JSON data 
 ```c
	struct json_object *jobj_array, *jobj_obj;
 ```
 2. Get the student data from the ISABELA API 
 ```c
	jobj_array = get_student_data();
 ```
 3. Get individual fields from the array of data (e.g., location)
 ```c
	arraylen = json_object_array_length(jobj_array);
	for (i = 0; i < arraylen; i++) {
		jobj_obj = json_object_array_get_idx(jobj_array, i);
		jobj_object_location = json_object_object_get(jobj_obj, "location");
		printf("location=%s\n", json_object_get_string(jobj_object_location));
	}
 ```
 
The fields available in the ISABELA API are:
* id: student ID code (check your ID in the ISABELA Android settings);
* activity: student activity (Exercise,Sleeping,Classes,Tilting,Walking,In vehicle,Unknown,Still);
* location: student location (University, House, Other);
* callsduration: duration of calls made in the last 5 seconds;
* callsmade: number of calls made in the last 5 seconds;
* callsmissed: number of calls missed in the last 5 seconds;
* callsreceived: number of calls received in the last 5 seconds;
* department: name of the department (DEI);
* smsreceived: number of sms received in the last 5 seconds;
* smssent: number of sms sent in the last 5 seconds;

To use this example from outside, the URL of the request need to be changed from the interal IP address to the DNS address (socialiteorion2.dei.uc.pt)
 ```c
 //From this:
	curl_easy_setopt(hnd, CURLOPT_URL, "http://10.3.4.75:9014/v2/entities?options=keyValues&type=student&attrs=activity,calls_duration,calls_made,calls_missed,calls_received,department,location,sms_received,sms_sent");
	
//To this:
	curl_easy_setopt(hnd, CURLOPT_URL, "http://socialiteorion2.dei.uc.pt:9014/v2/entities?options=keyValues&type=student&attrs=activity,calls_duration,calls_made,calls_missed,calls_received,department,location,sms_received,sms_sent");
 ```
