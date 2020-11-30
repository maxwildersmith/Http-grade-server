#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <strings.h>
#include <cstring>
#include <string>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

void getAssignments(void *sockfd)
{
    int sock = *(int*)sockfd;
    char buffer[4096];
    bzero(buffer, 4096);

    send(sock, "GET /server HTTP/1.1", sizeof("GET /server HTTP/1.1") , 0); 
    read(sock, buffer, sizeof(buffer));

    cout << buffer << "  ASDF"<<endl;

    printf("%s\n", buffer );
}

void getGrades(void *sockfd, string assignmentname)
{
    int sock = *(int*)sockfd;
    char buffer[4096];
    bzero(buffer, 4096);

    string str = "GET /server/" + assignmentname + " " + "?name=grades HTTP/1.1";

    char strbuffer[(str.length() + 1)];
    strcpy(strbuffer, str.c_str());
    send(sock, strbuffer, sizeof(strbuffer) , 0); 
    read(sock, buffer, sizeof(buffer));

    printf("%s\n", buffer);
}

void createAssignment(void *sockfd, string assignmentname, string filetext)
{
    int sock = *(int*)sockfd;
    char buffer[4096];
    bzero(buffer, 4096);

    string str = "POST /server?name=" + assignmentname + " HTTP/1.1\n" + filetext;

    char strbuffer[(str.length() + 1)];
    strcpy(strbuffer, str.c_str());
    send(sock, strbuffer, sizeof(strbuffer) , 0); 
    read(sock, buffer, sizeof(buffer));

    printf("%s\n", buffer);
}

void updateGrades(void *sockfd, string assignmentname, string filetext)
{
    int sock = *(int*)sockfd;
    char buffer[4096];
    bzero(buffer, 4096);

    string str = "POST /server/" + assignmentname + " " + "?name=grades HTTP/1.1\n" + filetext;

    char strbuffer[(str.length() + 1)];
    strcpy(strbuffer, str.c_str());
    send(sock, strbuffer, sizeof(strbuffer) , 0); 
    read(sock, buffer, sizeof(buffer));

    printf("%s\n", buffer);
}
   
int main() 
{ 
    int sockfd; 
    struct sockaddr_in saddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    saddr.sin_family = AF_INET; 
    saddr.sin_port = htons(5000);   
    inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);


    //get user input command

    
    connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
    cout << "Connected to the server..." << endl; 
    getAssignments(&sockfd);

    string input;
    cin >> input;
    
    // do{

    //     input << cin;
    // } while (input.compare("q") != 0);

    //getAssignments(&socskfd);
    //getGrades(&sockfd, assignmentname);
    //createAssignment(&sockfd, assignmentname, filetext);
    //updateGrades(&sockfd, assignmentname, filetext);
     
    
    return 0; 
} 