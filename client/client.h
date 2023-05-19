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
#include "utility.h"

using namespace std;
#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

/*
     * index : value
     * 0     : request type(get, post)
     * 1     : file path( get -> file path in server directory, post -> file path in client directory)
     * 2     : server ip (host name)
     * 3     : port number (optional)
*/

//// parse request
vector<string>request_parser(const char* request){
    vector<string> parsed_request;
    stringstream str_stream(request);
    string tmp;
    while(getline(str_stream, tmp, ' ')){
        parsed_request.push_back(tmp);
    }
    return parsed_request;
}

void request_runner( const char* server_ip, int server_port,  char* request_type,  char* file_path){
    //// create client socket
    int client_sock;
    struct  sockaddr_in server_addr{};
    client_sock = socket(AF_INET, SOCK_STREAM,0);
    if(client_sock < 0){
        printf("Socket Error");
        return;
    }
    printf("client socket created\n");

    memset(&server_addr, '\0' , sizeof(client_sock));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = server_port;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    //// connect to server, server is listening for client connection until acceptance of connection
    int n = connect(client_sock, (struct sockaddr*) & server_addr, sizeof (server_addr));
    if (n < 0){
        printf("Connection Failed \n");
        return;
    }
    printf("connection accepted ...\n");

    /* format of data to send
        * 0/1 file_name ? length_of_data ? Data
        * 0/1 -> to indicate request type 0:post  1:get
        * file name to write in server
        * ? -> stop point
        * length of data send
        * ? stop point
        * Data
    */
    if (strcmp(request_type, "client_get")  == 0){
        //// get request
        stringstream to_send;
        to_send << '1';
        to_send << get_file_name(file_path);
        to_send <<'?';

        char buff[strlen(file_path)+2];
        strcpy(buff,to_send.str().c_str());
        send(client_sock,buff, sizeof (buff),0);
        printf("client send successfully\n");

        char buffer[1048576];
        bzero(buffer,1048576);
        recv(client_sock, buffer, sizeof(buffer), 0);
        string response ;
        response += buffer[0] ;
        response += buffer[1];
        response += buffer[2];

        if (strcmp(response.c_str(),"200") == 0){
            pair<string ,int> data_length_pair = extract_inf(buffer,3);
            int len = stoi(data_length_pair.first);
            stringstream data;
            int truth_data = data_length_pair.second + len;
            for (int i = data_length_pair.second; i < truth_data; ++i) {
                data << buffer[i];
            }
            string client_path = "../client_file/";
            string str1 = client_path+get_file_name(file_path);
            write_file(str1, data.str());
            cout << "response : " << "HTTP/1.1 200 OK\\r\\n"  <<endl  <<data.str();
        }
        else{
            cout << "response : " << "HTTP/1.1 404 Not Found\r\n";
        }
        printf("received successfully\n");

    }
    else if (strcmp(request_type, "client_post") == 0){
        //// post request
        string name = get_file_name(file_path);
        string data = read_file( file_path);
        stringstream to_send;
        size_t len1 = data.length();

        stringstream s;
        s<<len1;
        string data_length;
        s >>data_length;
        to_send << '0';
        to_send << name;
        to_send << '?';
        to_send << data_length;
        to_send << '?';
        to_send << data;
        size_t len = (to_send.str()).length();
        char buffer[len];
        string str1 = to_send.str();
        for(int i=0;i<str1.length();i++){
            buffer[i] = str1[i];
        }
        //strcpy(buffer,str1.c_str());
        send(client_sock, buffer, strlen(buffer), 0);

        bzero(buffer,3);
        recv(client_sock, buffer, sizeof(buffer), 0);
        cout << "response code:  " << "HTTP/1.1 200 OK\\r\\n";
    }
    close(n);
}
#endif //CLIENT_CLIENT_H
