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
        vector<string> assignedTAs;
        Course(string id="",int maxTAs=0,vector<string> prefTAList={},vector<string> prefProgList={},int n=0,int m=0){
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
        vector<string> prefCourses;
        int lenprefCourse;
        int top;
        string assigned;
        Student(string id="",string degree="",vector<string> prefCou={},int n=0){
            sid=id;
            deg=degree;
            prefCourses=prefCou;
            lenprefCourse=n;
            top=0;
            assigned="-";
        }
};

void assignCourseOptimal(map<string,Course>coursemap,map<string,Student>studentmap){
    map<string,priority_queue<tuple<int,int,string> > >studentprioritycoursemap;
    map<string,queue<string> >degreemap;
    for(auto i:studentmap){
        string deg = studentmap[i.first].deg;
        degreemap[deg].push(i.first);
    }
    //cout<<"ok1"<<"\n";
    queue<string>unassignedcourses;
    for(auto i: coursemap){
        string course=i.first;
        for(auto j: studentmap){
            string student=j.first;
            auto it=find(i.second.prefTA.begin(),i.second.prefTA.end(),student);
            int p1=INT32_MAX,p2=INT32_MAX;
            if(it!=i.second.prefTA.end())p1=it-i.second.prefTA.begin();
            it=find(i.second.prefProg.begin(),i.second.prefProg.end(),j.second.deg);
            if(it!=i.second.prefProg.end())p2=it-i.second.prefProg.begin();
            studentprioritycoursemap[course].push(make_tuple(-p1,-p2,student));
        }
        cout<<course<<" "<<get<0>(studentprioritycoursemap[course].top())<<" "<<get<1>(studentprioritycoursemap[course].top())<<" "<<get<2>(studentprioritycoursemap[course].top())<<"\n";
        unassignedcourses.push(course);
    }
    while(!unassignedcourses.empty()){
        string course=unassignedcourses.front();
        unassignedcourses.pop();
        if(coursemap[course].assignedTAs.size()==coursemap[course].tasReq)continue;
        if(studentprioritycoursemap[course].empty())continue;
        string student=get<2>(studentprioritycoursemap[course].top());
        studentprioritycoursemap[course].pop();
        if(studentmap[student].assigned=="-"){
            studentmap[student].assigned=course;
            coursemap[course].assignedTAs.push_back(student);
        }
        else{
            string previouslyassignedcourse=studentmap[student].assigned;
            auto it1=find(studentmap[student].prefCourses.begin(),studentmap[student].prefCourses.end(),course);
            auto it2=find(studentmap[student].prefCourses.begin(),studentmap[student].prefCourses.end(),previouslyassignedcourse);
            if(it1<it2){
                studentmap[student].assigned=course;
                //cout<<course<<"\n";
                auto it=find(coursemap[previouslyassignedcourse].assignedTAs.begin(),coursemap[previouslyassignedcourse].assignedTAs.end(),student);
                //cout<<previouslyassignedcourse<<"\n";
                //cout<<it-coursemap[previouslyassignedcourse].assignedTAs.begin()<<" "<<coursemap[previouslyassignedcourse].assignedTAs.size()<<"\n";
                coursemap[previouslyassignedcourse].assignedTAs.erase(it);
                coursemap[course].assignedTAs.push_back(student);
                unassignedcourses.push(previouslyassignedcourse);
            }
        }
        unassignedcourses.push(course);
    }
    fstream fout;
    fout.open("ouputCourseAssignmentViaCourseOptimal.csv",ios::out);
    fout<<"CourseID,AssignedTA-1,AssignedTA-2,AssignedTA-3,AssignedTA-4,AssignedTA-5,AssignedTA-6,AssignedTA-7,AssignedTA-8,AssignedTA-9,AssignedTA-10,AssignedTA-11,AssignedTA-12,AssignedTA-13,AssignedTA-14,AssignedTA-15,AssignedTA-16\n";
    int req=0;
    for(auto i: coursemap){
        req+=i.second.tasReq;
        //cout<<i.first<<" "<<i.second.tasReq<<" "<<i.second.assignedTAs.size()<<"\n";
        string s=i.first+",";
        vector<string>temp=i.second.assignedTAs;
        for(int j=0;j<16;j++){
            if(j>=temp.size()){
                if(j==15)s+="\n";
                else s+=",";
            }
            else{
                if(j==15)s+=temp[j]+"\n";
                else s+=temp[j]+",";
            }
        }
        fout<<s;
    }
    fstream fout1;
    fout1.open("ouputStudentAssignmentViaCourseOptimal.csv",ios::out);
    fout1<<"StudentID,Assigned Course\n";
    for(auto i: studentmap)fout1<<i.first+","+i.second.assigned+"\n";
    //cout<<req<<" "<<studentmap.size()<<"\n";
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
    string coursefile = "2022-23-I-FacultyChoices.csv";
    string studentfile = "2022-23-I-StudentChoices.csv";
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
        vector<string> prefCou;
        for(int j=8;j<38;j++){
            if(coursemap.find(studentdata[i][j])==coursemap.end())continue;
            prefCou.push_back(studentdata[i][j]);
        }
        int n=prefCou.size();
        studentmap[id]=*new Student(id,deg,prefCou,n);
        //cout<<id<<"\n";
    }
    //cout<<"ok1"<<"\n";
    assignCourseOptimal(coursemap,studentmap);
    //for(auto i:studentmap)cout<<i.first<<" "<<i.second.deg<<"\n";
    return 0;
}