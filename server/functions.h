#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include "iostream"
#include "utility.h"

using namespace std;
#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

struct timeout{
    int time;
};

string post_request_runner(char buffer[]){
    pair<string,int> file_name_pair = extract_inf(buffer, 1);
    int curr_identifier_index = file_name_pair.second;
    stringstream data_stream;
    pair<string,int> data_length_pair = extract_inf(buffer, curr_identifier_index);
    curr_identifier_index = data_length_pair.second;
    int data_index = stoi(data_length_pair.first);
    int data_length = data_index + curr_identifier_index;
    for (int i = curr_identifier_index; i < data_length ; i++){
        data_stream << buffer[i];
    }
    string relative_path = "../server_file/"+file_name_pair.first;
    write_file(relative_path, data_stream.str());
    return "200";
}
#endif //SERVER_FUNCTIONS_H
