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
        string cid;                     //CourseID 
        int tasReq;                     //Number of TAs required
        vector<string> prefTA;          //Vector containing preferred students from most preferred to least preferred
        int lenprefTA;                  //Length of preferred students
        vector<string> prefProg;        //Vector containing preferred degree from most preferred to least preferred
        int lenprefProg;                //Length of preferred degrees
        int assignedTAs;                //Number of students assigned to this course as TAs(initialised to 0)
        //Constructor
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
    //This variable will be counter for number of students who are part of unstable assignment
    int t=0;
    for(auto stu:studentcoursepriority){
        string student=stu.first;
        //we reverse the priority list to get highest priority at the front
        reverse(studentcoursepriority[student].begin(),studentcoursepriority[student].end());
        //we run a loop till the student's assigned course
        for(auto j: studentcoursepriority[student]){
            string course=get<1>(j);
            if(course!=assigned[student]){
                //Here we check the priority rank of the student in the preference list of that course and the closing rank of that course
                if(n-coursestudentpriority[course].size()>courseStudentrank[course][student]){
                    t++;
                    break;
                }
            }
            else break;
        }
    }
    //If t=0 that shows there's no unstable assignement
    if(t==0)cout<<"Output Validated Successfully!\n";
    else cout<<"Number of unstable assignments: "<<t<<"\n";
}

//Course-Optimal Assignment
void assignCourseOptimal(int n,int m,map<string,Course>&coursemap,map<string,Student>&studentmap,map<string,vector<tuple<int,int,string> > >&coursestudentpriority,map<string,vector<tuple<int,string> > >&studentcoursepriority,map<string,map<string,tuple<int,int,string> > >&coursestudentprioritymap,map<string,map<string,tuple<int,string> > >&studentcourseprioritymap){
    //creating a course queue, initially containing all courses
    queue<string>coursequeue;
    for(auto course:coursestudentpriority)coursequeue.push(course.first);
    //Until there are no courses left
    while(!coursequeue.empty()){
        string course=coursequeue.front();
        //if course has filled all the seats or it has offered to all the students, we can skip it
        if(coursemap[course].assignedTAs==coursemap[course].tasReq || coursestudentpriority[course].empty()){
            coursequeue.pop();
            continue;
        }
        //get the highest preferred student on the course-student priority list 
        string student=get<2>(*coursestudentpriority[course].rbegin());
        coursestudentpriority[course].pop_back();
        //if the student is not assigned to any course
        if(studentmap[student].assigned=="-"){
            coursemap[course].assignedTAs++;
            studentmap[student].assigned=course;
        }
        else{
            string presentcourse=studentmap[student].assigned;
            //Here we check if the assigned course of this student is less prior to the course making offer
            if(studentcourseprioritymap[student][course]>studentcourseprioritymap[student][presentcourse]){
                //if yes we assign the course to the student and take back previously assigned course into our course queue 
                coursemap[course].assignedTAs++;
                coursemap[presentcourse].assignedTAs--;
                studentmap[student].assigned=course;
                coursequeue.push(presentcourse);
            }
        }
    }
    //Writing our assignment into a CSV file
    fstream fout1;
    fout1.open("myoutput.csv",ios::out);
    fout1<<"S.No.,Name,Roll No,Email,Degree,Course Allocated\n";
    int sno=1;
    for(auto i: studentmap)fout1<<sno++<<","+i.second.name+","+i.first+","+i.second.email+","+i.second.deg+","+i.second.assigned+"\n";
    fout1.close();
}

//Given a csv filename this module returns a 2D vector of csv contents
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
    //load input 
    string coursefile = "input2_new.csv";
    string studentfile = "input1_new.csv";
    vector<vector<string> >coursedata,studentdata;
    coursedata=getfilecontents(coursefile);
    studentdata=getfilecontents(studentfile);

    //creating course objects from course records read from csv and storing them into a map with courseid as the key
    map<string,Course> coursemap;
    for(int i=1;i<coursedata.size();i++){
        string id=coursedata[i][2];
        if(id=="-")continue;
        int maxTAs=atoi(&coursedata[i][4][0]);
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

    //creating student objects from student records read from csv and storing them into a map with student rollno as the key
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
    }
    
    int n=studentmap.size(),m=coursemap.size();

    //A map with courseid as key and value a vector containing the priorities of each student of that course
    map<string,vector<tuple<int,int,string> > >coursestudentpriority;

    //A map with studentid as key and value a vector containing the priorities  of each course of that student
    map<string,vector<tuple<int,string> > >studentcoursepriority;

    //this variable contains course's priority of the students(all) and storing them into nested map
    map<string,map<string,tuple<int,int,string> > >coursestudentprioritymap;

    //this variable contains student's priority of the courses(all) and storing them into nested map
    map<string,map<string,tuple<int,string> > >studentcourseprioritymap;

    //A nested map to get the rank of a student in the preferrence of a course
    map<string,map<string,int> >courseStudentrank;

    //Assigning priorities to each possible pair of student and course and storing them
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
    
    //sorting them to get a ordered priorities
    for(auto student:studentcoursepriority)sort(studentcoursepriority[student.first].begin(),studentcoursepriority[student.first].end());
    for(auto course:coursestudentpriority)sort(coursestudentpriority[course.first].begin(),coursestudentpriority[course.first].end());
    
    //Assigning ranks
    for(auto course:coursemap){
        int rk=0;
        for(auto student:coursestudentpriority[course.first])courseStudentrank[course.first][get<2>(student)]=n-rk++;
    }

    //Calling Assignment Module
    assignCourseOptimal(n,m,coursemap,studentmap,coursestudentpriority,studentcoursepriority,coursestudentprioritymap,studentcourseprioritymap);

    //Validation
    //Load ouputs from csv
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
    
    //validate by checking if there are any unstable assignment
    validate(n,m,coursemap,courseStudentrank,coursestudentpriority,studentcoursepriority,mymap);
    //validate(n,m,coursemap,courseStudentrank,coursestudentpriority,studentcoursepriority,testmap);

    //Write the requirement and assigned number of TAs to each course onto a CSV file
    fstream fout1;
    fout1.open("mycount.csv",ios::out);
    fout1<<"S.No.,Course,#TAs Requirement,#TAs Assigned\n";
    int sno=1;
    for(auto i: coursemap)fout1<<sno++<<","+i.first+","<<i.second.tasReq<<","<<i.second.assignedTAs<<"\n";
    fout1.close();

    return 0;
}
