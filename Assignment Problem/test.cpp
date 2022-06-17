#include<iostream>
#include<queue>
#include<tuple>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<map>
using namespace std;

vector<vector<string> > getfilecontents(string fname){
    vector<vector<string> > content;
    vector<string> row;
    string line, word;
    fstream file (fname, ios::in);
    if(file.is_open()){
        while(getline(file, line)){
            row.clear();
            stringstream str(line);
            while(getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    }
    else cout<<"Could not open the file\n";
    for(int i=0;i<content.size();i++){
        for(int j=0;j<content[i].size();j++){
            cout<<content[i][j]<<" ";
        }
        cout<<"\n";
    }
    return content;
}
int main(){
    string coursefile = "2022-23-I-FacultyChoices.csv";
    string studentfile = "2022-23-I-StudentChoices.csv";
    vector<vector<string> >coursedata,studentdata;
    coursedata=getfilecontents(coursefile);
    studentdata=getfilecontents(studentfile);
    return 0;
}