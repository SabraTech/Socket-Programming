//
// Created by sabra on 19/11/17.
//

#ifndef SOCKET_PROGRAMMING_UTILITIES_H
#define SOCKET_PROGRAMMING_UTILITIES_H

#include "bits/stdc++.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <vector>
#include <string>


#define PORT "3490" // the port users will be connecting to
#define BACKLOG 10 // how many pending connections queue will hold
#define HTTP_GET 0
#define HTTP_POST 1
#define OK_MSG "HTTP/1.0 200 OK\r\n"
#define ERROR_MSG "HTTP/1.0 404 Not Found\r\n"
#define MAXDATASIZE 100 // max number of bytes we can get at once

using namespace std;

vector <string> split(string s, string delimiter);

vector <string> parse_request(string request);

string get_file_format(vector <string> request);

vector <string> read_file(string file_name);

string make_request(string request);


#endif //SOCKET_PROGRAMMING_UTILITIES_H
