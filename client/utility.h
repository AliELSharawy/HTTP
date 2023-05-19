#include <netinet/in.h>
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <unistd.h>

using namespace std;
#ifndef CLIENT_UTILITY_H
#define CLIENT_UTILITY_H

pair<string, int> extract_inf(char buffer[], int start){
    int index = start;
    stringstream name;
    while(buffer[index] != '?'){
        name << buffer[index];
        index++;
    }
    return {name.str(),index+1};
}

//// write file at path location
void write_file(string& path, const string& data){
    std::ofstream file(path, std::ios_base::binary);
    std::string fileString;

    for(char i : data){
        fileString += i;
    }
    file << fileString;
}

//// read file from path location
string read_file(char* path){
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)){
        std::string s(buffer.begin(), buffer.end());
        return s;
    }
    return "";
}

// extract file name from path
string get_file_name(char* path){
    size_t index = strlen(path)-1;
    while (index > 0 && path[index] != '/'){
        index--;
    }
    if (index ==0 && path[index] != '/')
        index--;
    size_t len = strlen(path) - index;
    char buff[len];
    if (index ==0 && path[index] != '/')
        index--;
    memcpy( buff, &path[index+1], len-1 );
    buff[len-1] = '\0';
    return buff;
}
#endif //CLIENT_UTILITY_H
