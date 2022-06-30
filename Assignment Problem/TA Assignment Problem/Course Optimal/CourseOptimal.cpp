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
        vector<string> prefTA;
        int lenprefTA;
        vector<string> prefProg;
        int lenprefProg;
        int assignedTAs;
        Course(string id="",int maxTAs=0,vector<string> prefTAList={},vector<string> prefProgList={},int n=0,int m=0){
            cid=id;
            tasReq=maxTAs;
            prefTA=prefTAList;
            prefProg=prefProgList;
            lenprefTA=n;
            lenprefProg=m;
            assignedTAs=0;
        }
};

class Student{
    public:
        string name;
        string email;
        string sid;
        string deg;
        vector<string> prefCourses;
        int lenprefCourse;
        int top;
        string assigned;
        Student(string na="",string em="",string id="",string degree="",vector<string> prefCou={},int n=0){
            sid=id;
            name=na;
            email=em;
            deg=degree;
            prefCourses=prefCou;
            lenprefCourse=n;
            top=0;
            assigned="-";
        }
};

void assignCourseOptimal(map<string,Course>coursemap,map<string,Student>studentmap){
    int m=coursemap.size(),n=studentmap.size();
    map<string,vector<tuple<int,int,string> > >coursestudentpriority;
    map<string,vector<tuple<int,string> > >studentcoursepriority;
    map<string,map<string,tuple<int,int,string> > >coursestudentprioritymap;
    map<string,map<string,tuple<int,string> > >studentcourseprioritymap;
    for(auto student:studentmap){
        for(auto course:coursemap){
            int p1=-1000,p2=-1000;
            for(int x=0;x<course.second.lenprefTA;x++){
                if(course.second.prefTA[x]==student.first){
                    p1=-x;
                    break;
                }
            }
            for(int x=0;x<course.second.lenprefProg;x++){
                if(course.second.prefProg[x]==student.second.deg){
                    p2=-x;
                    break;
                }
            }
            coursestudentpriority[course.first].push_back(make_tuple(p1,p2,student.first));
            coursestudentprioritymap[course.first][student.first]=make_tuple(p1,p2,student.first);
            p1=-1000;
            for(int x=0;x<student.second.lenprefCourse;x++){
                if(student.second.prefCourses[x]==course.first){
                    p1=-x;
                    break;
                }
            }
            studentcoursepriority[student.first].push_back(make_tuple(p1,course.first));
            studentcourseprioritymap[student.first][course.first]=make_tuple(p1,course.first);
        }
    }
    for(auto student:studentcoursepriority){
        sort(studentcoursepriority[student.first].begin(),studentcoursepriority[student.first].end());
    }
    queue<string>coursequeue;
    for(auto course:coursestudentpriority){
        coursequeue.push(course.first);
        sort(coursestudentpriority[course.first].begin(),coursestudentpriority[course.first].end());
    }
    while(!coursequeue.empty()){
        string course=coursequeue.front();
        if(coursemap[course].assignedTAs==coursemap[course].tasReq || coursestudentpriority[course].empty()){
            coursequeue.pop();
            continue;
        }
        string student=get<2>(*coursestudentpriority[course].rbegin());
        coursestudentpriority[course].pop_back();
        if(studentmap[student].assigned=="-"){
            coursemap[course].assignedTAs++;
            studentmap[student].assigned=course;
        }
        else{
            string presentcourse=studentmap[student].assigned;
            if(studentcourseprioritymap[student][course]>studentcourseprioritymap[student][presentcourse]){
                studentmap[student].assigned=course;
                coursequeue.push(presentcourse);
            }
        }
    }
    int k=0;
    for(auto course:coursemap){
        k+=course.second.tasReq;
        cout<<"CourseID: "<<course.first<<"\n";
        cout<<"required TAs: "<<course.second.tasReq<<"\n";
        cout<<"Number of TAs Assigned: "<<course.second.assignedTAs<<"\n";
        cout<<"Size of priorityqueue: "<<coursestudentpriority[course.first].size()<<"\n\n";

    }
    cout<<k<<" "<<studentcoursepriority.size()<<"\n";
    fstream fout1;
    fout1.open("myouput.csv",ios::out);
    fout1<<"S.No,Name,Roll No,Email,Degree,Course Allocated\n";
    int sno=1;
    for(auto i: studentmap){
        fout1<<sno++<<","+i.second.name+","+i.first+","+i.second.email+","+i.second.deg+","+i.second.assigned+"\n";
    }
    //cout<<req<<" "<<studentmap.size()<<"\n";
    map<string,string>obtainedoutputmap;
    vector<string>studentlist;
    for(auto i:studentmap){
        string student=i.first;
        studentlist.push_back(student);
        string course=i.second.assigned;
        obtainedoutputmap[student]=course;
    }
}

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
    string coursefile = "input2_new.csv";
    string studentfile = "input1_new.csv";
    vector<vector<string> >coursedata,studentdata,out;
    coursedata=getfilecontents(coursefile);
    studentdata=getfilecontents(studentfile);
    out=getfilecontents("output.csv");

    map<string,Course> coursemap;
    for(int i=1;i<coursedata.size();i++){
        string id=coursedata[i][2];
        if(id=="-")continue;
        int maxTAs=atoi(&coursedata[i][4][0]);
        //cout<<id<<" "<<maxTAs<<"\n";
        if(maxTAs<1)continue;
        vector<string> prefTAList;
        for(int j=5;j<15;j++){
            if(coursedata[i][j]!="-")prefTAList.push_back(coursedata[i][j]);
            else break;
        }
        vector<string> prefProgList;
        for(int j=15;j<19;j++){
            if(coursedata[i][j]!="-")prefProgList.push_back(coursedata[i][j]);
            else break;
        }
        int n=prefTAList.size();
        int m=prefProgList.size();
        coursemap[id] = * new Course(id,maxTAs,prefTAList,prefProgList,n,m);
    }

    map<string,Student>studentmap;
    for(int i=1;i<studentdata.size();i++){
        string id=studentdata[i][1];
        string deg=studentdata[i][4];
        string name=studentdata[i][3];
        string email=studentdata[i][2];
        vector<string> prefCou;
        for(int j=8;j<38;j++){
            if(coursemap.find(studentdata[i][j])==coursemap.end())continue;
            prefCou.push_back(studentdata[i][j]);
        }
        int n=prefCou.size();
        studentmap[id]=*new Student(name,email,id,deg,prefCou,n);
        //cout<<id<<"\n";
    }

    map<string,string>outputmap;
    for(int i=0;i<out.size();i++){
        string student=out[i][2];
        string course=out[i][5];
        outputmap[student]=course;
    }
    //cout<<"ok1"<<"\n";
    assignCourseOptimal(coursemap,studentmap);
    //for(auto i:studentmap)cout<<i.first<<" "<<i.second.deg<<"\n";
    return 0;
}
