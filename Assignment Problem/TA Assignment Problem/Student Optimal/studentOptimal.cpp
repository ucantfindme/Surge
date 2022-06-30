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
        priority_queue<tuple<int,int,string> > assignedTAs;
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

void assignStudentOptimal(map<string,Course>coursemap,map<string,Student>studentmap,map<string,string>outputmap){
    int m=coursemap.size(),n=studentmap.size();
    queue<string>priorStudents;
    map<string,queue<string> >nonpriorStudents;
    int np=0;
    for(auto i: studentmap){
        if(!i.second.prefCourses.empty())priorStudents.push(i.first);
        else{
            nonpriorStudents[i.second.deg].push(i.first);
            np++;
        }
    }
    while(!priorStudents.empty()){
        string student=priorStudents.front();
        priorStudents.pop();
        string course=studentmap[student].prefCourses[studentmap[student].top++];
        //cout<<student<<" "<<course<<"\n";
        int n=coursemap[course].lenprefTA;
        int p1=-INT32_MAX,p2=-INT32_MAX;
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
                //if(student=="190902")cout<<"Here i am "<<studentmap[student].deg<<"\n";
                nonpriorStudents[studentmap[student].deg].push(student);
                //cout<<student<<"\n";
                np++;
            }
        }
    }
    for(auto course : coursemap){
        //cout<<course.first<<" "<<np<<" "<<course.second.tasReq-course.second.assignedTAs.size()<<"\n";
        while(np!=0 && course.second.assignedTAs.size()<course.second.tasReq){
            bool added=false;
            for(int i=0;i<course.second.lenprefProg;i++){
                if(!nonpriorStudents[course.second.prefProg[i]].empty()){
                    added=true;
                    string student=nonpriorStudents[course.second.prefProg[i]].front();
                    nonpriorStudents[course.second.prefProg[i]].pop();
                    //cout<<student<<" "<<np<<"\n";
                    np--;
                    tuple<int,int,string>pri=make_tuple(INT32_MAX,-i,student);
                    coursemap[course.first].assignedTAs.push(pri);
                    studentmap[student].assigned=course.first;
                    break;
                }
            }
            if(!added){
                for(auto i: nonpriorStudents){
                    if(!i.second.empty()){
                        string student=i.second.front();
                        nonpriorStudents[i.first].pop();
                        //cout<<student<<" "<<np<<"\n";
                        np--;
                        tuple<int,int,string>pri=make_tuple(INT32_MAX,INT32_MAX,student);
                        coursemap[course.first].assignedTAs.push(pri);
                        studentmap[student].assigned=course.first;
                        break;
                    }
                }
            }
        }
    }
    fstream fout;
    fout.open("ouputCourseAssignment.csv",ios::out);
    fout<<"CourseID,AssignedTA-1,AssignedTA-2,AssignedTA-3,AssignedTA-4,AssignedTA-5,AssignedTA-6,AssignedTA-7,AssignedTA-8,AssignedTA-9,AssignedTA-10,AssignedTA-11,AssignedTA-12,AssignedTA-13,AssignedTA-14,AssignedTA-15,AssignedTA-16\n";
    int req=0;
    for(auto i: coursemap){
        req+=i.second.tasReq;
        //cout<<i.first<<" "<<i.second.tasReq<<" ";
        string s=i.first+",";
        vector<string>temp;
        while(!i.second.assignedTAs.empty()){
            temp.push_back(get<2>(i.second.assignedTAs.top()));
            i.second.assignedTAs.pop();
        }
        reverse(temp.begin(),temp.end());
        for(int j=0;j<16;j++){
            if(j>=temp.size()){
                if(j==15)s+="\n";
                else s+=",";
            }
            else{
                studentmap[temp[j]].assigned=i.first;
                if(j==15)s+=temp[j]+"\n";
                else s+=temp[j]+",";
            }
        }
        fout<<s;
    }
    fstream fout1;
    fout1.open("ouputStudentAssignmentViaCourseOptimal.csv",ios::out);
    fout1<<"S.No.,Name,Roll No,Email,Degree,Course Allocated\n";
    int sno=1;
    for(auto i: studentmap){
        fout1<<sno++<<","+i.second.name+","+i.first+","+i.second.email+","+i.second.deg+","+i.second.assigned+"\n";
    }
    //cout<<req<<" "<<studentmap.size()<<"\n";
    map<string,string>obtainedoutputmap;
    int right=0;
    int total=studentmap.size();
    vector<string>studentlist;
    for(auto i:studentmap){
        string student=i.first;
        studentlist.push_back(student);
        string course=i.second.assigned;
        obtainedoutputmap[student]=course;
        if(outputmap[student]==course)right++;
    }
    cout<<right<<" "<<total<<"\n";
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
    assignStudentOptimal(coursemap,studentmap,outputmap);
    //for(auto i:studentmap)cout<<i.first<<" "<<i.second.deg<<"\n";
    return 0;
}
