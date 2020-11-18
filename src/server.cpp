#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <regex>

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

int main()
{
    cout << "Starting server..." << endl;

    cout << createAssignment("test2","asdf\nasdf");

    //gradeAssignment("test");
    //cout << compareFiles("server/test/evitolo.txt","server/test/key.txt");

    //system("./test.out < vals > ");

    return 1;
}