#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <strings.h>
#include <sstream>
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

    string str = "GET /server/" + assignmentname + "?name=" + studentname + " HTTP/1.1";

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

    string str = "POST /server/" + assignmentname + "?name=" + studentname + " HTTP/1.1\n" + filetext;

    char strbuffer[(str.length() + 1)];
    strcpy(strbuffer, str.c_str());
    send(sock, strbuffer, sizeof(strbuffer) , 0); 
    read(sock, buffer, sizeof(buffer));

    printf("%s\n", buffer);

    close(sock);
}

string getFile(string fname){
    try{
        ifstream file(fname);
        ostringstream ss;
        ss << file.rdbuf(); // reading data
        return ss.str();
    } catch(exception e){
        cout << "ERROR OPENING FILE"<<endl;
        return "";
    }
}

void help(){
    cout << "Commands for student:\n\tls - list current assignments\n\tget <name> - get your grade for the specific assignment\n\tsubmit <name> <filepath> - submits an assignment to the specified assignment name from the specified file" << endl;
}
   
int main() 
{ 
    string sname;
    cout << "Enter your name in the format john_doe with an underscore between first and last name, all in lowercase" << endl;
    cin >> sname;

    help();

    string input;
    string aname;
    string fname;
    cin >> input;
    
    while (input.compare("q") != 0){
        int sockfd; 
        struct sockaddr_in saddr;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        saddr.sin_family = AF_INET; 
        saddr.sin_port = htons(5000);   
        inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);

        connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
        
        cout << "    - "<<input<<" -"<<endl;
        if(input.compare("ls") == 0){
            getAssignments(&sockfd);
        } else if(input.compare("get") == 0){
            cin >> aname;
            getGrade(&sockfd, aname, sname);
        } else if(input.compare("submit") == 0){
            cin >> aname;
            cin >> fname;
            postAssignment(&sockfd, aname, sname, getFile(fname));
        } else if(input.compare("help") == 0){
            help();
        } else {
            cout << "UNKNOWN COMMAND, TYPE 'help' FOR HELP" << endl;
        }

        cin >> input;

    } 

    
    //getGrade(&sockfd, assignmentname, studentname);
    //postAssignment(&sockfd, assignmentname, studentname, filetext);
    
    return 0; 
} 