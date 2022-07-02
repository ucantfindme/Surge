#include<iostream>
#include<queue>
#include<tuple>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<map>
using namespace std;

//Course Class
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

//Student Class
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

//Validate module
void validate(int n,int m,map<string,Course>&coursemap,map<string,map<string,int> >&courseStudentrank,map<string,vector<tuple<int,int,string> > >&coursestudentpriority,map<string,vector<tuple<int,string> > >&studentcoursepriority,map<string,string>&assigned){
    int t=0;
    for(auto stu:studentcoursepriority){
        string student=stu.first;
        reverse(studentcoursepriority[student].begin(),studentcoursepriority[student].end());
        for(auto j: studentcoursepriority[student]){
            string course=get<1>(j);
            if(course!=assigned[student]){
                if(n-coursestudentpriority[course].size()>courseStudentrank[course][student]){
                    t++;
                    break;
                }
            }
            else break;
        }
    }
    if(t==0)cout<<"Output Validated Successfully!\n";
    else cout<<"Number of unstable assignments: "<<t<<"\n";
}

void assignCourseOptimal(int n,int m,map<string,Course>&coursemap,map<string,Student>&studentmap,map<string,vector<tuple<int,int,string> > >&coursestudentpriority,map<string,vector<tuple<int,string> > >&studentcoursepriority,map<string,map<string,tuple<int,int,string> > >&coursestudentprioritymap,map<string,map<string,tuple<int,string> > >&studentcourseprioritymap){
    queue<string>coursequeue;
    for(auto course:coursestudentpriority)coursequeue.push(course.first);
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
                coursemap[course].assignedTAs++;
                coursemap[presentcourse].assignedTAs--;
                studentmap[student].assigned=course;
                coursequeue.push(presentcourse);
            }
        }
    }
    fstream fout1;
    fout1.open("myoutput.csv",ios::out);
    fout1<<"S.No.,Name,Roll No,Email,Degree,Course Allocated\n";
    int sno=1;
    for(auto i: studentmap)fout1<<sno++<<","+i.second.name+","+i.first+","+i.second.email+","+i.second.deg+","+i.second.assigned+"\n";
    fout1.close();
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
    vector<vector<string> >coursedata,studentdata;
    coursedata=getfilecontents(coursefile);
    studentdata=getfilecontents(studentfile);

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
    
    int n=studentmap.size(),m=coursemap.size();
    map<string,vector<tuple<int,int,string> > >coursestudentpriority;
    map<string,vector<tuple<int,string> > >studentcoursepriority;
    map<string,map<string,tuple<int,int,string> > >coursestudentprioritymap;
    map<string,map<string,tuple<int,string> > >studentcourseprioritymap;
    map<string,map<string,int> >courseStudentrank;
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
    for(auto course:coursestudentpriority){
        sort(coursestudentpriority[course.first].begin(),coursestudentpriority[course.first].end());
    }
    for(auto course:coursemap){
        int rk=0;
        for(auto student:coursestudentpriority[course.first]){
            courseStudentrank[course.first][get<2>(student)]=n-rk++;
        }
    }

    assignCourseOptimal(n,m,coursemap,studentmap,coursestudentpriority,studentcoursepriority,coursestudentprioritymap,studentcourseprioritymap);

    string testoutputfile = "output.csv";
    string myoutputfile = "myoutput.csv";
    vector<vector<string> >testout,myout;
    testout=getfilecontents(testoutputfile);
    myout=getfilecontents(myoutputfile);
    map<string,string>testmap,mymap;
    int tot=testout.size();
    for(int i=1;i<tot;i++){
        testmap[testout[i][2]]=testout[i][5];
        mymap[myout[i][2]]=myout[i][5];
    }
    
    validate(n,m,coursemap,courseStudentrank,coursestudentpriority,studentcoursepriority,mymap);
    //validate(n,m,coursemap,courseStudentrank,coursestudentpriority,studentcoursepriority,testmap);

    fstream fout1;
    fout1.open("mycount.csv",ios::out);
    fout1<<"S.No.,Course,#TAs Requirement,#TAs Assigned\n";
    int sno=1;
    for(auto i: coursemap)fout1<<sno++<<","+i.first+","<<i.second.tasReq<<","<<i.second.assignedTAs<<"\n";
    fout1.close();

    return 0;
}
