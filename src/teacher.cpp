#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <strings.h>
#include <sstream>
#include <cstring>
#include <string>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

void toFile(string fname, string content){
    ofstream file(fname);
    file << content;
    file.close();
}

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

void getGrades(void *sockfd, string assignmentname)
{
    int sock = *(int*)sockfd;
    char buffer[4096];
    bzero(buffer, 4096);

    string str = "GET /server/" + assignmentname + "?name=grades HTTP/1.1";

    char strbuffer[(str.length() + 1)];
    strcpy(strbuffer, str.c_str());
    send(sock, strbuffer, sizeof(strbuffer) , 0); 
    read(sock, buffer, sizeof(buffer));

    cout << buffer << endl;
    toFile("grades.txt", buffer);
    cout << "The grades have also been printed to a file called grades.txt" << endl;

    close(sock);
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

    close(sock);
}

void updateGrades(void *sockfd, string assignmentname, string filetext)
{
    int sock = *(int*)sockfd;
    char buffer[4096];
    bzero(buffer, 4096);

    string str = "POST /server/" + assignmentname + "?name=grades HTTP/1.1\n" + filetext;

    char strbuffer[(str.length() + 1)];
    strcpy(strbuffer, str.c_str());
    send(sock, strbuffer, sizeof(strbuffer) , 0); 
    read(sock, buffer, sizeof(buffer));

    printf("%s\n", buffer);
}

void updateKey(void *sockfd, string assignmentname, string filetext)
{
    int sock = *(int*)sockfd;
    char buffer[4096];
    bzero(buffer, 4096);

    string str = "POST /server/" + assignmentname + "?name=key HTTP/1.1\n" + filetext;

    char strbuffer[(str.length() + 1)];
    strcpy(strbuffer, str.c_str());
    send(sock, strbuffer, sizeof(strbuffer) , 0); 
    read(sock, buffer, sizeof(buffer));

    printf("%s\n", buffer);
}

void updateInput(void *sockfd, string assignmentname, string filetext)
{
    int sock = *(int*)sockfd;
    char buffer[4096];
    bzero(buffer, 4096);

    string str = "POST /server/" + assignmentname + "?name=input HTTP/1.1\n" + filetext;

    char strbuffer[(str.length() + 1)];
    strcpy(strbuffer, str.c_str());
    send(sock, strbuffer, sizeof(strbuffer) , 0); 
    read(sock, buffer, sizeof(buffer));

    printf("%s\n", buffer);
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
    cout << "Commands for teacher:\n\tls - list current assignments\n\tget <name> - gets the grade for the assignment of name\n\tcreate <name> <filepath> - creates an assignment of the specified name and uploads the specified file as the solution.cpp (use updateI to add input values)\n\tupdate <name> <filepath> - updates the answer key's c++\n\tupdateI <name> <filepath> - updates the input file for the given assignment\n\tupdateG <name> <filepath> - updates the grades for an assignment" << endl;
}
   
int main() 
{ 
    
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
            getGrades(&sockfd, aname);
        } else if(input.compare("create") == 0){
            cin >> aname;
            cin >> fname;
            createAssignment(&sockfd, aname, getFile(fname));
        } else if(input.compare("update") == 0){
            cin >> aname;
            cin >> fname;
            updateKey(&sockfd, aname, getFile(fname));
        } else if(input.compare("updateI") == 0){
            cin >> aname;
            cin >> fname;
            updateInput(&sockfd, aname, getFile(fname));
        } else if(input.compare("updateG") == 0){
            cin >> aname;
            cin >> fname;
            updateGrades(&sockfd, aname, getFile(fname));
        } else if(input.compare("help") == 0){
            help();
        } else {
            cout << "UNKNOWN COMMAND, TYPE 'help' FOR HELP" << endl;
        }

        cin >> input;

    } 
    
    return 0; 
} 