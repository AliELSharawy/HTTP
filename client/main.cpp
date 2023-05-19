#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include "client.h"

using namespace std;

void request_initialization(vector<string> parsed_request, int default_server_port){
    //// initialize request and extract parameters then run request
    if(parsed_request.empty()){
        return;
    }
    char *server_ip;
    server_ip = strcpy(new char[parsed_request[2].length() + 1] , parsed_request[2].c_str());
    int server_port = default_server_port;
    if(parsed_request.size() == 4)
        server_port = stoi(parsed_request[3]);
    char *type;
    type = strcpy(new char[parsed_request[0].length() + 1], parsed_request[0].c_str());
    char *file_path ;
    file_path = strcpy(new char[parsed_request[1].length() + 1], parsed_request[1].c_str());
    request_runner(server_ip, server_port, type, file_path);
    free(server_ip);
    free(type);
    free(file_path);
}

int main(int argc, char** argv) {
    int default_server_port = 5571;
    string path_command ="../request.txt";
    fstream request_stream(path_command);
    vector<string> parsed_request;
    string request;
    cout << endl ;
    while (! request_stream.eof()){
        getline(request_stream,request);
        char * arr;
        arr = strcpy(new char [request.length()+1], request.c_str());
        parsed_request = request_parser(arr);
        free(arr);
        request_initialization(parsed_request, default_server_port);
    }
    cout << "\nDone XD";
    return 0;
}