#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <regex>
#include <thread>
#include <vector>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

bool validName(string name)
{
    regex txt_regex("[A-Z]|\\.|\\\\|\\/");
    return !regex_search(name, txt_regex);
}

bool updateKey(string name, string key)
{
    fstream file;
    file.open("server/" + name + "/key.txt", ios::out);
    file << key;
    file.close();
    return true;
}

bool compareFiles(string p1, string p2) {

    ifstream file1(p1);
    ifstream file2(p2);

    string line1, line2;

    bool out = true;

    while(getline(file1,line1)&&getline(file2,line2)){
        if(line1.compare(line2)!=0){
            out = false;
            break;
        }
    }
    file1.close();
    file2.close();
    
    return out;
}

bool generateKey(string name){
    string cmdS = "g++ server/"+name+"/key.cpp -std=c++2a -o server/" + name + "/key.out && ./server/" + name + "/key.out < server/" + name + "/key.txt > server/" + name + "/key.txt";
    char cmd[cmdS.length() + 1];
    strcpy(cmd, cmdS.c_str());
    system(cmd);
    return true;
}

bool gradeAssignment(string name)
{
    map<string, string> grades;
    fstream gradeFile("server/" + name + "/grades.txt");
    string line;
    string personName, score;
    while (getline(gradeFile, line))
    {
        personName = line.substr(0, line.find(':'));
        score = line.substr(line.find(':') + 1);
        grades.insert({personName, score});
    }

    for (auto &p : filesystem::directory_iterator("server/" + name))
    {
        if (!p.path().extension().string().compare(".cpp"))
        {
            string cmdS = "g++ " + p.path().string() + " -std=c++2a -o server/" + name + "/student.out && ./server/" + name + "/student.out < server/" + name + "/input.txt > server/" + name + "/" + (p.path().stem().string()) + ".txt";
            char cmd[cmdS.length() + 1];
            strcpy(cmd, cmdS.c_str());
            system(cmd);
            if(compareFiles("server/"+name+"/"+p.path().stem().string() + ".txt", "server/" + name + "/key.txt")){
                grades.insert_or_assign(p.path().stem().string(), "100");
            } else if (!grades.contains(p.path().stem().string())){
                grades.insert({p.path().stem().string(),"--"});
            }
        }
    }
    gradeFile.close();

    ofstream file("server/" + name + "/grades.txt");
    for (auto entry = grades.begin(); entry != grades.end(); entry++)
    {
        file << entry->first<<":"<<entry->second<<"\n";
    }
    file.close();

    return true;
}

void toFile(string fname, string content){
    ofstream file(fname);
    file << content;
    file.close();
}

string getFile(string fname){
    try{
        ifstream file("server/" + fname + "/grades.txt");
        ostringstream ss;
        ss << file.rdbuf(); // reading data
        return ss.str();
    } catch(exception e){
        cout << "ERROR OPENING FILE"<<endl;
        return "";
    }
}


bool updateInput(string name, string input)
{
    fstream file;
    file.open("server/" + name + "/input.txt", ios::out);
    file << input;
    file.close();
    return true;
}

string createAssignment(string name, string key, string input = "")
{
    string out;
    if (!validName(name))
        return "Invalid assignment name, make sure the name is all lowercase, does not contain any /, \\, or .";
    if (filesystem::exists("server/" + name))
        return "Assignment with this name already exists";
    filesystem::create_directory("server/" + name);

    if (!updateKey(name, key))
        return "Error creating key";
    if (!updateInput(name, key))
        return "Error creating input file";

    return "Successfuly created assignment";
}

string getAssignments(){
    string out = "Assignments: ";
    for (auto &p : filesystem::directory_iterator("server/")){
        if(p.is_directory()){
            out += p.path().filename().string()+" ";
        }
    }

    return out;

}

void clientHandler(void *sockfd)
{
    char buffer[4096];
    bzero(buffer, 4096);
    int sock = *(int*)sockfd;
    
    read(sock, buffer, sizeof(buffer));
    
    string str(buffer);
    int spacepos = str.find(' ');
    if (str.compare(0, spacepos, "GET") == 0)
    {
        string path, query, name, assignmentname;
        int assignmentnamepos = 8;
        
        if (str.find('?') != string::npos)
        {
            string grade;
            int questpos = str.find('?');
            path = str.substr((spacepos + 1), (questpos - (spacepos + 1)));
            spacepos = str.find(' ', questpos);
            query = str.substr((questpos + 1), (spacepos - (questpos + 1)));
            int equalpos = query.find('=');
            name = query.substr(equalpos + 1);
            assignmentname = path.substr(assignmentnamepos);

            if (name.compare("grades") == 0)
            {
                grade = getFile(assignmentname);
                //need function that takes assignment name and returns grades.txt as a string
            }
            else
            {
                // DUE TO TIME, DROP THE STUDENT VIEWS THEIR GRADE FUNCTIONALITY
                //need function that takes assignment name and student name and returns grade string
            }
            char strbuffer[(grade.length() + 1)];
            strcpy(strbuffer, grade.c_str());
            send(sock, strbuffer, sizeof(strbuffer), 0);
        }
        else
        {
            string assignments;
            cout << "Getting assignments..." << endl;
            assignments = getAssignments();

            char strbuffer[(assignments.length() + 1)];
            strcpy(strbuffer, assignments.c_str());
            send(sock, strbuffer, sizeof(strbuffer), 0);
        }
        
    }
    else if (str.compare(0, spacepos, "POST") == 0)
    {
        string path, query, name, assignmentname, grade, filetext;
        int assignmentnamepos = 8;
        int questpos = str.find('?');
        path = str.substr((spacepos + 1), (questpos - (spacepos + 1)));

        if (path.length() == 7)
        {
            spacepos = str.find(' ', questpos);
            query = str.substr((questpos + 1), (spacepos - (questpos + 1)));
            int equalpos = query.find('=');
            name = query.substr(equalpos + 1);
            int newlinepos = str.find('\n');
            filetext = str.substr(newlinepos + 1);
            string message = createAssignment(name, filetext);

            char strbuffer[(message.length() + 1)];
            strcpy(strbuffer, message.c_str());
            send(sock, strbuffer, sizeof(strbuffer), 0);
        }
        else
        {
            spacepos = str.find(' ', questpos);
            query = str.substr((questpos + 1), (spacepos - (questpos + 1)));
            int equalpos = query.find('=');
            name = query.substr(equalpos + 1);
            int newlinepos = str.find('\n');
            filetext = str.substr(newlinepos + 1);
            assignmentname = path.substr(assignmentnamepos);

            //need updateGrades function and submitAssignment function
            // TO UPDATE GRADES, JUST CALL THE GRADEASSIGNMENT() FUNCTION
            // FOR SUBMITTING, USE TOFILE() TO WRITE A STRING TO A FILE, MAKE SURE THE FILENAME IS server/assignmentName/studentName.cpp
            
            string message;
            if (name.compare("grades") == 0)
            {
                string filename = "server/" + assignmentname + "/" + name + ".txt";
                toFile(filename, filetext);
                message = "Grades file has been submitted";
            }
            else
            {
                if (validName(assignmentname))
                {
                    string filename = "server/" + assignmentname + "/" + name + ".cpp";
                    toFile(filename, filetext);
                    message = "Assignment has been submitted successfully";
                }
                else
                {
                    message = "Not a valid assignment name";
                }
                
            }
            char strbuffer[(message.length() + 1)];
            strcpy(strbuffer, message.c_str());
            send(sock, strbuffer, sizeof(strbuffer), 0);
        } 
    }

    close(sock);
}

int main()
{
    cout << "Starting server..." << endl;


    // cout << getAssignments() << endl;
    //cout << createAssignment("test2","asdf\nasdf");

    int port = 5000;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int csockfd;
    socklen_t clientlength;
    struct sockaddr_in saddr, caddr;
    bzero((char *) &saddr, sizeof(saddr));

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);

    bind(sockfd, (struct sockaddr *) &saddr, sizeof(saddr));
   
    listen(sockfd, 5);

    clientlength = sizeof(caddr);
    vector<thread> threads;

    while(true)
    {
        csockfd = accept(sockfd, (struct sockaddr *) &caddr, &clientlength);
        threads.emplace_back([&](){clientHandler(&csockfd);});
    }




    //gradeAssignment("test");
    //cout << compareFiles("server/test/evitolo.txt","server/test/key.txt");

    //system("./test.out < vals > ");

    return 1;
}