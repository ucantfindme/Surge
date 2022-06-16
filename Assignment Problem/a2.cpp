#include<iostream>
#include<queue>
#include<tuple>
using namespace std;

class Course{
    public:
        string cid;
        int tasReq;
        string* prefTA;
        string* prefProg;
        priority_queue<tuple<int,int,string>> assignedTAs;
        Course(string id,int maxTAs,string* prefTAList,string* prefProgList){
            cid=id;
            tasReq=maxTAs;
            prefTA=prefTAList;
            prefProg=prefProgList;
        }
};

class Student{
    public:
        string sid;
        string deg;
        string* prefCourses;
        string assigned = "-";
        Student(string id,string degree,string* prefCou){
            sid=id;
            deg=degree;
            prefCourses=prefCou;
        }
};

int main(){
    
}