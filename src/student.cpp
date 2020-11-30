#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <strings.h>
#include <cstring>
#include <fstream>
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

    printf("%s\n", buffer );

    close(sock);
}

void getGrade(void *sockfd, string assignmentname, string studentname)
{
    int sock = *(int*)sockfd;
    char buffer[4096];
    bzero(buffer, 4096);

    string str = "GET /server/" + assignmentname + " " + "?name=" + studentname + " HTTP/1.1";

    char strbuffer[(str.length() + 1)];
    strcpy(strbuffer, str.c_str());
    send(sock, strbuffer, sizeof(strbuffer) , 0); 
    read(sock, buffer, sizeof(buffer));

    printf("%s\n", buffer);

    close(sock);
}

void postAssignment(void *sockfd, string assignmentname, string studentname, string filetext)
{
    int sock = *(int*)sockfd;
    char buffer[4096];
    bzero(buffer, 4096);

    string str = "POST /server/" + assignmentname + " " + "?name=" + studentname + " HTTP/1.1\n" + filetext;

    char strbuffer[(str.length() + 1)];
    strcpy(strbuffer, str.c_str());
    send(sock, strbuffer, sizeof(strbuffer) , 0); 
    read(sock, buffer, sizeof(buffer));

    printf("%s\n", buffer);

    close(sock);
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
    
    getAssignments(&sockfd);

    
    //getGrade(&sockfd, assignmentname, studentname);
    //postAssignment(&sockfd, assignmentname, studentname, filetext);
    
    return 0; 
} 