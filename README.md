# IRC Privacy Server
This git repo gives you a sample code to interact with ISABELA API. There are two sample codes:

- Built to Visual Studio Community and available in the master branch. 
- Built to the linux and gcc compiler and available on the linux-gcc branch.

## Linux-GCC

Install on Ubuntu
1. Download the code:
```console
git clone https://github.com/dgraposo/IRCtest.git
```
2. Checkout to the linux branch:
```console
git checkout linux-gcc
```
3. Install all the missing libraries with apt-get (gcc,clang,libtool,autoconf,automake), see the [instructions](https://github.com/json-c/json-c)

4. Run the following commands in json-c path:
```console
   sh autogen.sh
   ./configure
   make
   make install (need sudo)
   make check
```

5. Install libcurl: 
```console
   sudo apt-get install libcurl4-gnutls-dev
```
6. Add the library to the environment variables:
```console
   export LD_RUN_PATH=/usr/local/lib
   export LD_LIBRARY_PATH=/usr/local/lib/
```

7. Compile the application using gcc:
```console
   gcc main.c -o main -lcurl -ljson-c
```
