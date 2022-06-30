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
    return content;
}

int main(){
    string testoutputfile = "output.csv";
    string myoutputfile = "myoutput.csv";
    vector<vector<string> >testout,myout;
    testout=getfilecontents(testoutputfile);
    myout=getfilecontents(myoutputfile);
    map<string,string>testmap,mymap;
    int tot=testout.size();
    int right=0;
    cout<<testout.size()<<" "<<myout.size()<<"\n";
    /*
    for(int i=1;i<tot;i++){
        testmap[testout[i][2]]=testout[i][5];
        mymap[myout[i][2]]=myout[i][5];
    }
    for(auto i:testmap){
        if(i.second==mymap[i.first])right++;
    }*/
    cout<<"Number of right mappings: "<<right<<"\n"<<"Total Number of Students: "<<tot-1<<"\n";
}