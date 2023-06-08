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

//Compare Outputs
void compare(map<string,string>&assigned1,map<string,string>&assigned2){
    //Writing differences into a CSV file
    fstream fout1;
    fout1.open("differences.csv",ios::out);
    fout1<<"S.No.,Roll No,Course Allocated in old Allocation,Course Allocated in new Allocation\n";
    int sno=1;
    for(auto i: assigned1){
        if(assigned1[i.first]!=assigned2[i.first])fout1<<sno++<<","+i.first+","+assigned1[i.first]+","+assigned2[i.first]+"\n";
    }
    fout1.close();
}

int main(){
    string newoutputfile = "Student_allocation_new.csv";
    string oldoutputfile = "Student_allocation.csv";
    vector<vector<string> >newout;
    vector<vector<string> >oldout;
    newout=getfilecontents(newoutputfile);
    oldout=getfilecontents(oldoutputfile);
    map<string,string>newmap,oldmap;
    int tot=newout.size();
    for(int i=1;i<tot;i++){
        newmap[newout[i][2]]=newout[i][5];
        oldmap[oldout[i][2]]=oldout[i][5];
    }
    compare(oldmap,newmap);
}