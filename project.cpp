#include<iostream>
#include<bits/stdc++.h>
using namespace std;
class Student{
    public:
    string name;
    int rollNumber;
    string course;
    double marks;

    Student() : rollNumber(0), marks(0.0){} //constructor

    void readStudentInfo()
    {
        cout<<"Enter name: "<<endl;
        cin.ignore();
        cin>>name;
        cout<<"Enter roll number: "<<endl;
        cin>>rollNumber;
        cin.ignore();
        cout<<"Enter course: "<<endl;
        cin>>course;
        cout<<"Enter marks: "<<endl;
        cin>>marks;
    }

    void displayStudentInfo() const{
        cout<<"Name: "<<name<<endl;
        cout<<"Roll Number: "<<rollNumber<<endl;
        cout<<"Course: "<<course<<endl;
        cout<<"Marks: "<<marks<<endl;
        cout<<"--------------------------"<<endl;
    }
};

class StudentRecordManager{
    private:
    vector<Student> students;
    map<int, vector<int>> courseIndex;

    public:
    const vector<Student>& getStudents() const{
        return students;
    }

    void addStudent(){
        Student newStudent;
        newStudent.readStudentInfo();
        students.push_back(newStudent);

        courseIndex[newStudent.rollNumber].push_back(static_cast<int>(students.size())-1);
        cout<<"Student added successfully."<<endl;
    }

    void addStudentFromFile(const Student& student){
        students.push_back(student);
    }

    void displayStudents() const{ //Declaring member functions as const tells
    // the compiler that these functions do not modify the internal state of 
    //the object and can be safely called on const instances of the class.
        if(students.empty()){
            cout<<"No students to display."<<endl;
            return;
        }
        cout<<"Student Records: "<<endl;
        for(const auto& student: students){
            student.displayStudentInfo();
        }
    }

    void displayStatistics() const{
        if(students.empty()){
            cout<<"No students to calculate statistics."<<endl;
            return;
        }

        double totalMarks=0.0;
        double highestMarks=students[0].marks;
        double lowestMarks=students[0].marks;

        for(const auto& student: students){
            totalMarks+=student.marks;
            highestMarks=max(highestMarks,student.marks);
            lowestMarks=min(lowestMarks,student.marks);
        }

        double averageMarks=totalMarks/students.size();

        cout<<"Statistics:"<<endl;
        cout<<"Average Marks: "<<averageMarks<<endl;
        cout<<"Highest Marks: "<<highestMarks<<endl;
        cout<<"Lowest Marks: "<<lowestMarks<<endl;
    }

    void sortByRollNumber(){
        sort(students.begin(),students.end(), [](const Student& a, const Student& b){
            return a.rollNumber<b.rollNumber;
        });
        cout<<"Students sorted by roll number."<<endl;
    }

    void sortByMarks(){
        sort(students.begin(),students.end(),
        [](const Student& a,const Student& b){
            return a.marks>b.marks;
        });
        cout<<"Students sorted by marks."<<endl;
    }

    void filterByCourse(const string& course){
        vector<Student> filteredStudents;
        for(const auto& index:courseIndex){
            for(int studentIndex : index.second){
                if (students[studentIndex].course==course){
                    filteredStudents.push_back(students[studentIndex]);
                }
            }
        }
        if(filteredStudents.empty()){
            cout<<"No students for course: "<<course<<endl;
        }
        else{
            cout<<"Students for course: "<<course<<endl;
            for(const auto& student : filteredStudents){
                student.displayStudentInfo();
            }
        }
    }

    void updateStudent(int rollNumber, const string& newName, const string& newCourse, double newMarks) {
    auto it = find_if(students.begin(), students.end(), [rollNumber](Student& student) {
        return student.rollNumber == rollNumber;
    });

    if (it != students.end()) {
        Student& student = *it;
        student.name = newName;
        student.course = newCourse;
        student.marks = newMarks;
        cout << "Student information updated for roll number " << rollNumber << endl;
    } else {
        cout << "Student with roll number " << rollNumber << " not found." << endl;
    }
}

    void deleteStudent(int rollNumber){
        auto it=remove_if(students.begin(),students.end(),[rollNumber](const Student& student){
            return student.rollNumber==rollNumber;
        });
        if(it !=students.end()){
            students.erase(it,students.end());
            cout<<"Student with roll number "<<rollNumber<<" deleted"<<endl;
        }
        else{
            cout<<"Student with roll number "<<rollNumber<<" not found"<<endl;
        }
    }
};

void saveRecordsToFile(const StudentRecordManager& recordManager, const string& record){
    ofstream outFile(record);
    if(outFile.is_open()){
        for(const auto& student : recordManager.getStudents()){
            outFile<<student.name<<","<<student.rollNumber<<","<<student.course<<","<<student.marks<<endl;
        }
        outFile.close();
        cout<<"Records saved to "<<record<<"endl";
    }else{
        cerr<<"Unable to open file for writing."<<endl;
    }
}

void loadRecordsFromFile(StudentRecordManager& recordManager, const string& record) {
    ifstream inFile(record);
    if(inFile.is_open()){
        string line;
        while(getline(inFile,line)){
            istringstream iss(line);
            Student newStudent;
            getline(iss,newStudent.name,',');
            iss>>newStudent.rollNumber; 
            getline(iss,newStudent.course,',');
            iss>>newStudent.marks;
            recordManager.addStudentFromFile(newStudent);
        }
        inFile.close();
            cout<<"Records loaded from "<<record<<endl;
        }
        else{
            cerr<<"Unable to open file for reading."<<endl;
        }
    }

    

int main(){
    StudentRecordManager recordManager;
    while(true){
        cout<<"Student Record Management System"<<endl;
        cout<<"1. Add Student"<<endl;
        cout<<"2. Display Students"<<endl;
        cout<<"3. Display Statistics"<<endl;
        cout<<"4. Sort Students by Roll Number"<<endl;
        cout<<"5. Sort Students by Marks"<<endl;
        cout<<"6. Filter Students by Course"<<endl;
        cout<<"7. Save Records to File"<<endl;
        cout<<"8. Load Records from File"<<endl;
        cout<<"9. Update Student"<<endl;
        cout<<"10. Delete Student"<<endl;
        cout<<"11. Exit"<<endl;

        int choice;
        cin>>choice;
        switch(choice){
            case 1:
            recordManager.addStudent();
            break;

            case 2:
            recordManager.displayStudents();
            break;

            case 3:
            recordManager.displayStatistics();
            break;

            case 4:
            recordManager.sortByRollNumber();
            break;

            case 5:
            recordManager.sortByMarks();
            break;

            case 6:{
                cin.ignore();
                string course;
                cout<<"Enter course to filter by: ";
                getline(cin,course);
            recordManager.filterByCourse(course);
            break;
            }

            case 7:{
                string filename;
                cout<<"Enter filename to save records: ";
                cin.ignore();
                getline(cin,filename);
                saveRecordsToFile(recordManager,filename);
                break;
            }

            case 8:{
                string filename;
                cout<<"Enter filename to load records from: ";
                cin.ignore();
                getline(cin,filename);
                loadRecordsFromFile(recordManager,filename);
                break;
            } 

            case 9:{
                int rollNumber;
                cout<<"Enter roll number to update: ";
                cin>>rollNumber;

                string newName, newCourse;
                double newMarks;

                cout<<"Enter new name: ";
                cin.ignore();
                getline(cin,newName);
                cout<<"Enter new course: ";
                getline(cin,newCourse);
                cout<<"Enter new marks: ";
                cin>>newMarks;
                
                recordManager.updateStudent(rollNumber,newName,newCourse,newMarks);
                break;
            }

            case 10:{
                int rollNumber;
                cout<<"Enter roll number to delete: ";
                cin>>rollNumber;
                recordManager.deleteStudent(rollNumber);
                break;
            }
            case 11:
            cout<<"Exiting program.."<<endl;
            return 0;
            default:
            cout<<"Invalid choice"<<endl;
         }
    }
    return 0;
}