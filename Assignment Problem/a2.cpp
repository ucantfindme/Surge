#include<iostream>
#include<queue>
#include<tuple>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<map>
using namespace std;

class Course{
    public:
        string cid;
        int tasReq;
        string* prefTA;
        int lenprefTA;
        string* prefProg;
        int lenprefProg;
        priority_queue<tuple<int,int,string>> assignedTAs;
        Course(string id,int maxTAs,string* prefTAList,string* prefProgList,int n,int m){
            cid=id;
            tasReq=maxTAs;
            prefTA=prefTAList;
            prefProg=prefProgList;
            lenprefTA=n;
            lenprefProg=m;
        }
};

class Student{
    public:
        string sid;
        string deg;
        string* prefCourses;
        int lenprefCourse;
        int top;
        string assigned = "-";
        Student(string id,string degree,string* prefCou,int n){
            sid=id;
            deg=degree;
            prefCourses=prefCou;
            lenprefCourse=n;
            top=0;
        }
};

void assignStudentOptimal(map<string,Course>coursemap,map<string,Student>studentmap){
    int m=coursemap.size(),n=studentmap.size();
    queue<string>priorStudents;
    queue<string>nonpriorStudents;
    for(auto i: studentmap){
        if(i.second.prefCourses)priorStudents.push(i.first);
        else nonpriorStudents.push(i.first);
    }
    while(!priorStudents.empty()){
        string student=priorStudents.front();
        priorStudents.pop();
        string course=studentmap[student].prefCourses[studentmap[student].top++];
        int n=coursemap[course].lenprefTA;
        int p1=-1,p2=-1;
        for(int i=0;i<n;i++){
            if(coursemap[course].prefTA[i]==student){
                p1=i;
                break;
            }
        }
        n=coursemap[course].lenprefProg;
        for(int i=0;i<n;i++){
            if(coursemap[course].prefProg[i]==studentmap[student].deg){
                p2=i;
                break;
            }
        }
        tuple<int,int,string>pri=make_tuple(-p1,-p2,student);
        if(coursemap[course].assignedTAs.size()<coursemap[course].tasReq){
            coursemap[course].assignedTAs.push(pri);
        }
        else{
            if(coursemap[course].assignedTAs.top()<pri){
                tuple<int,int,string>popped=coursemap[course].assignedTAs.top();
                coursemap[course].assignedTAs.pop();
                coursemap[course].assignedTAs.push(pri);
                studentmap[student].assigned=course;
                student=get<2>(popped);
                studentmap[student].assigned="-";
            }
            if(studentmap[student].top!=studentmap[student].lenprefCourse){
                priorStudents.push(student);
            }
            else{
                nonpriorStudents.push(student);
            }
        }
    }
}

int main(){
    
}