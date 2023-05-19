#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include "iostream"
#include "utility.h"
#include "functions.h"
#include <pthread.h>
#include <mutex>

const int MAX_ALLOWED_CLIENTS = 20;
using namespace std;
int clients = 0;
mutex mtx;

void* handle_client(void *client_sock_p){
    printf("Client connected.\n");
    int client_sock = *((int *)client_sock_p);
    char buffer[1048576];
    recv(client_sock, buffer, sizeof(buffer), 0);
    cout<< "received client message: " << buffer << endl;
    if (buffer[0] == '0'){
        //// post request
        cout << "handling post request\n";
        string response_code = post_request_runner(buffer);
        cout << "response code: " + response_code << endl;
        send(client_sock, &response_code, response_code.length(), 0);
    }
    else if(buffer[0] == '1'){
        //// get request
        string name = extract_inf(buffer,1).first;
        string server_abs_path = "../server_file/";
        string path = server_abs_path+name;
        string data = read_file(path.c_str());
        string response_code = "404";
        if (!data.empty())
            response_code = "200";
        cout << "response code: " + response_code << endl;
        stringstream to_send;
        to_send << response_code ;
        to_send << data.length();
        to_send << '?';
        to_send << data;
        string s = to_send.str();
        cout<<s<<endl;
        char buffer_to_send[s.length()];
        strcpy(buffer_to_send,s.c_str());
        send(client_sock, buffer_to_send, sizeof (buffer_to_send), 0);
        printf("server send successfully\n");
    }
    mtx.lock();
    cout << "number of clients : " << clients << endl;
    clients--;
    mtx.unlock();
    return nullptr;
}
int main(int argc, char** argv) {
    const char *ip = "127.0.0.1";
    int port = 5571;
    int server_sock , client_sock;
    struct  sockaddr_in server_addr{} , client_addr{};
    socklen_t addr_size;
    int bind_identifier, numberOfClients = 20;
    server_sock = socket(AF_INET, SOCK_STREAM,0);
    if(server_sock < 0){
        perror("Socket Error");
        exit(1);
    }
    printf("Server Socket Created\n");
    memset(&server_addr, '\0' , sizeof(server_sock));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    bind_identifier = bind(server_sock,(struct sockaddr*)& server_addr,sizeof (server_addr));
    if(bind_identifier < 0){
        perror("Bind error");
        exit(1);
    }
    printf("Bind to port number %d\n",port);
    listen(server_sock,numberOfClients);
    while (true){
        printf("listening ..... \n");
        addr_size = sizeof (client_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, & addr_size);

        if(clients == MAX_ALLOWED_CLIENTS){
            printf("Reached the max limit number of connections\n");
            continue;
        }
        cout << "creat new thread for connection : " << client_sock << endl;
        pthread_t request_thread;
        mtx.lock();
        clients++;
        mtx.unlock();
        pthread_create(&request_thread, nullptr, &handle_client, &client_sock);
        pthread_detach(request_thread);
        //// this will allow the thread run on its own see join function in docs for more information
    }
}