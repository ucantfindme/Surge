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
        string courseID;                                //CourseID 
        int requiredNumberOfTAs;                        //Number of TAs required
        vector<string> preferredStudentList;            //Vector containing preferred students from most preferred to least preferred
        int lenpreferredStudentList;                    //Length of preferred students
        vector<string> preferredDegreeList;             //Vector containing preferred degree from most preferred to least preferred
        int lengthPreferredDegreeList;                  //Length of preferred degrees
        int assignedTAs;                                //Number of students assigned to this course as TAs(initialised to 0)
        int closingRank;                                //closing-rank of the course
        //Constructor
        Course(string id="",int maxTAs=0,vector<string> preferredStudentListList={},vector<string> preferredDegreeListList={},int n=0,int m=0){
            courseID=id;
            requiredNumberOfTAs=maxTAs;
            preferredStudentList=preferredStudentListList;
            preferredDegreeList=preferredDegreeListList;
            lenpreferredStudentList=n;
            lengthPreferredDegreeList=m;
            assignedTAs=0;
            closingRank=0;
        }
};

//Student Class
class Student{
    public:
        string name;                    //Student's Name
        string email;                   //Student's Email
        string studentRollNo;           //Student's Rollno
        string deg;                     //Student's Degree
        vector<string> prefCourses;     //Student's preference list orderded from most preferred to less
        int lenprefCourse;              //Length of his preferred courses list        
        string assigned;                //Student's Assigned Course (initially it's none)
        //Constructor
        Student(string na="",string em="",string id="",string degree="",vector<string> prefCou={},int n=0){
            studentRollNo=id;
            name=na;
            email=em;
            deg=degree;
            prefCourses=prefCou;
            lenprefCourse=n;
            assigned="-";
        }
};

//Validate module
void validate(int n,int m,map<string,Course>&coursemap,map<string,map<string,int> >&courseStudentrank,map<string,vector<tuple<int,int,string> > >&courseStudentpriority,map<string,vector<tuple<int,string> > >&studentCoursePriority,map<string,string>&assigned){
    //This variable will be counter for number of students who are part of unstable assignment
    int unstableAssignedStudents=0;
    for(auto stu:studentCoursePriority){
        string student=stu.first;
        //we reverse the priority list to get highest priority at the front
        reverse(studentCoursePriority[student].begin(),studentCoursePriority[student].end());
        //we run a loop till the student's assigned course
        for(auto j: studentCoursePriority[student]){
            string course=get<1>(j);
            if(course!=assigned[student]){
                //Here we check the priority rank of the student in the preference list of that course and the closing rank of that course
                if(coursemap[course].closingRank>courseStudentrank[course][student] || coursemap[course].assignedTAs<coursemap[course].requiredNumberOfTAs){
                    unstableAssignedStudents++;
                    break;
                }
            }
            //we dont need to check for the courses of equal or less prior than the present assigned course, hence we break out of the loop
            else break;
        }
    }
    //If t=0 that shows there's no unstable assignement
    if(unstableAssignedStudents==0)cout<<"Output Validated Successfully!\n";
    else cout<<"Number of unstable assignments: "<<unstableAssignedStudents<<"\n";
}

//Course-Optimal Assignment
void assignCourseOptimal(int n,int m,map<string,Course>&coursemap,map<string,Student>&studentmap,map<string,vector<tuple<int,int,string> > >&courseStudentpriority,map<string,vector<tuple<int,string> > >&studentCoursePriority,map<string,map<string,tuple<int,int,string> > >&courseStudentprioritymap,map<string,map<string,tuple<int,string> > >&studentCoursePrioritymap){
    //creating a course queue, initially containing all courses
    queue<string>coursequeue;
    for(auto course:courseStudentpriority)coursequeue.push(course.first);
    //Until there are no courses left
    while(!coursequeue.empty()){
        string course=coursequeue.front();
        //if course has filled all the seats or it has offered to all the students, we can skip it
        if(coursemap[course].assignedTAs==coursemap[course].requiredNumberOfTAs || courseStudentpriority[course].empty()){
            coursequeue.pop();
            continue;
        }
        //get the highest preferred student on the course-student priority list 
        string student=get<2>(*courseStudentpriority[course].rbegin());
        courseStudentpriority[course].pop_back();
        coursemap[course].closingRank++;
        //if the student is not assigned to any course
        if(studentmap[student].assigned=="-"){
            coursemap[course].assignedTAs++;
            studentmap[student].assigned=course;
        }
        else{
            string presentcourse=studentmap[student].assigned;
            //Here we check if the assigned course of this student is less prior to the course making offer
            if(studentCoursePrioritymap[student][course]>studentCoursePrioritymap[student][presentcourse]){
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

//Compare Outputs
void compare(map<string,string>&assigned1,map<string,string>&assigned2){
    //Writing differences into a CSV file
    fstream fout1;
    fout1.open("differences.csv",ios::out);
    fout1<<"S.No.,Roll No,Course Allocated in given output,Course Allocated in myoutput\n";
    int sno=1;
    for(auto i: assigned1){
        if(assigned1[i.first]!=assigned2[i.first])fout1<<sno++<<","+i.first+","+assigned1[i.first]+","+assigned2[i.first]+"\n";
    }
    fout1.close();
}

int main(){
    //load input 
    string coursefile = "input2_new.csv";
    string studentfile = "input1_new.csv";
    vector<vector<string> >coursedata,studentdata;
    coursedata=getfilecontents(coursefile);
    studentdata=getfilecontents(studentfile);

    //creating course objects from course records read from csv and storing them into a map with courseid as the key
    //All the numbers in the indexing depicts the position of it in vector we obtained after loading csv
    map<string,Course> coursemap;
    for(int i=1;i<coursedata.size();i++){
        string id=coursedata[i][2];
        if(id=="-")continue;
        int maxTAs=atoi(&coursedata[i][4][0]);
        if(maxTAs<1)continue;
        vector<string> preferredStudentList;
        for(int j=5;j<15;j++){
            if(coursedata[i][j]!="-")preferredStudentList.push_back(coursedata[i][j]);
            else break;
        }
        vector<string> preferredDegreeList;
        for(int j=15;j<19;j++){
            if(coursedata[i][j]!="-")preferredDegreeList.push_back(coursedata[i][j]);
            else break;
        }
        int n=preferredStudentList.size();
        int m=preferredDegreeList.size();
        coursemap[id] = * new Course(id,maxTAs,preferredStudentList,preferredDegreeList,n,m);
    }

    //creating student objects from student records read from csv and storing them into a map with student rollno as the key
    //All the numbers in the indexing depicts the position of it in vector we obtained after loading csv
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
    map<string,vector<tuple<int,int,string> > >courseStudentpriority;

    //A map with studentid as key and value a vector containing the priorities  of each course of that student
    map<string,vector<tuple<int,string> > >studentCoursePriority;

    //this variable contains course's priority of the students(all) and storing them into nested map
    map<string,map<string,tuple<int,int,string> > >courseStudentprioritymap;

    //this variable contains student's priority of the courses(all) and storing them into nested map
    map<string,map<string,tuple<int,string> > >studentCoursePrioritymap;

    //A nested map to get the rank of a student in the preferrence of a course
    map<string,map<string,int> >courseStudentrank;

    //Assigning priorities to each possible pair of student and course and storing them
    for(auto student:studentmap){
        for(auto course:coursemap){
            //Here we set a lower-bound(here -1000(since the number of students is less than 200)), if the student is not preferred, we assign this reference priority and use the roll number as tie-breaker
            int p1=-1000,p2=-1000;
            //Now we look for position of student in course's preferrence list 
            for(int x=0;x<course.second.lenpreferredStudentList;x++){
                //if we find it we update p1 with the position and break out of the loop
                if(course.second.preferredStudentList[x]==student.first){
                    p1=-x;
                    break;
                }
            }
            //Now we look for student's degree in course's preferred degree list
            for(int x=0;x<course.second.lengthPreferredDegreeList;x++){
                //if we find it we update p2 with the position and break out of the loop
                if(course.second.preferredDegreeList[x]==student.second.deg){
                    p2=-x;
                    break;
                }
            }
            //If the student is not in the course's preferred list or the student's degree is not preferred by the course, we dont need to include student in creating priority list of the course
            if(p1==-1000 && p2==-1000)continue;
            //we create a tuple of p1, p2 and student's rollno
            courseStudentpriority[course.first].push_back(make_tuple(p1,p2,student.first));
            courseStudentprioritymap[course.first][student.first]=make_tuple(p1,p2,student.first);
            //now we need to create a tuple to insert in student's preferred course, we again set a reference point(p1=-1000)
            p1=-1000;
            //We check if the course if preferred by the student
            for(int x=0;x<student.second.lenprefCourse;x++){
                //if yes we updaste p1 with the position of the course in the preferred list and break out of the loop
                if(student.second.prefCourses[x]==course.first){
                    p1=-x;
                    break;
                }
            }
            //we create a tuple with p1 and courseid and insert it into priority list of student's courses
            studentCoursePriority[student.first].push_back(make_tuple(p1,course.first));
            studentCoursePrioritymap[student.first][course.first]=make_tuple(p1,course.first);
        }
    }
    
    //sorting them to get a ordered priorities
    for(auto student:studentCoursePriority)sort(studentCoursePriority[student.first].begin(),studentCoursePriority[student.first].end());
    for(auto course:courseStudentpriority)sort(courseStudentpriority[course.first].begin(),courseStudentpriority[course.first].end());
    
    //Assigning ranks
    for(auto course:coursemap){
        //initialise the rank=0
        int rk=0;
        for(auto student:courseStudentpriority[course.first])courseStudentrank[course.first][get<2>(student)]=n-rk++;
    }

    //Calling Assignment Module
    assignCourseOptimal(n,m,coursemap,studentmap,courseStudentpriority,studentCoursePriority,courseStudentprioritymap,studentCoursePrioritymap);

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
    validate(n,m,coursemap,courseStudentrank,courseStudentpriority,studentCoursePriority,mymap);
    //validate(n,m,coursemap,courseStudentrank,courseStudentpriority,studentCoursePriority,testmap);

    //Difference in outputs
    compare(testmap,mymap);

    //Write the requirement and assigned number of TAs to each course onto a CSV file
    fstream fout1;
    fout1.open("mycount.csv",ios::out);
    fout1<<"S.No.,Course,#TAs Requirement,#TAs Assigned\n";
    int sno=1;
    for(auto i: coursemap)fout1<<sno++<<","+i.first+","<<i.second.requiredNumberOfTAs<<","<<i.second.assignedTAs<<"\n";
    fout1.close();

    return 0;
}
