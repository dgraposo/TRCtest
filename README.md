# TRC Server
This git repo gives you a sample code to interact with ISABELA API. There is a sample code built to the linux and gcc compiler and available on the linux-gcc branch.

## Linux-GCC

Install on Ubuntu
1. Download the code:
```console
git clone https://github.com/dgraposo/TRCtest.git
```
2. Install all the missing libraries with apt-get (gcc,clang,libtool,autoconf,automake), see the [instructions](https://github.com/json-c/json-c)

3. Run the following commands in json-c path:
```console
   sh autogen.sh
   ./configure
   make
   make install (need sudo)
   make check
```

4. Install libcurl: 
```console
   sudo apt-get install libcurl4-gnutls-dev
```
5. Add the library to the environment variables:
```console
   export LD_RUN_PATH=/usr/local/lib
   export LD_LIBRARY_PATH=/usr/local/lib/
```

6. Compile the application using gcc:
```console
   gcc main.c -o main -lcurl -ljson-c
```

The examples available in the main() allow the retreive of all questions, answers, and also to submit new answers or to update existing ones.
