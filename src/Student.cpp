#include "Student.h"
#include "Course.h"
#include <fstream>

Student::Student(string username, string password, string email)
    : User(username, password, email) {}

void Student::enrollInCourse(Course* course) {
    enrolledCourses.push_back(course);
    cout << "You have successfully enrolled in " << course->getCourseName() << ". Do you want to start learning now? (y/n): ";
    char choice;
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        string progressFile = "students/" + username + "_" + course->getCourseName() + "_progress.txt";
        ifstream progressInFile(progressFile);
        size_t lastProgressIndex = 0;

        // If progress file exists, read the last visited URL index
        if (progressInFile) {
            progressInFile >> lastProgressIndex;
            progressInFile.close();
        }

        // Load the course content starting from where the student left off
        vector<string> courseURLs = course->getCourseURLs();
        for (size_t i = lastProgressIndex; i < courseURLs.size(); ++i) {
            cout << "Course content for " << course->getCourseName() << " - " << i + 1 << ": " << courseURLs[i] << endl;
            cout << "Have you finished reading this section? (y/n): ";
            char sectionChoice;
            cin >> sectionChoice;
            if (sectionChoice == 'n' || sectionChoice == 'N') {
                // Save progress
                ofstream progressOutFile(progressFile);
                progressOutFile << i;
                progressOutFile.close();
                return;  // Exit so the student can continue later
            }
        }

        // If the student finished all content, reset progress
        ofstream progressOutFile(progressFile);
        progressOutFile << courseURLs.size();
        progressOutFile.close();
        cout << "Congratulations! You have completed the course!" << endl;
    }
}

void Student::listCourses() const {
    if (enrolledCourses.empty()) {
        cout << "Not enrolled in any courses." << endl;
    } else {
        cout << "Courses enrolled by " << username << ":" << endl;
        for (size_t i = 0; i < enrolledCourses.size(); ++i) {
            cout << i + 1 << ". " << *enrolledCourses[i] << endl;
        }
    }
}

void Student::saveToFile() const {
    ofstream outFile("students/" + username + ".txt");
    if (outFile) {
        outFile << username << endl;
        outFile << password << endl;
        outFile << email << endl;
        outFile.close();
    } else {
        cerr << "Failed to open file for saving student data." << endl;
    }
}

Student* Student::loadFromFile(const string& username, const string& password) {
    ifstream inFile("students/" + username + ".txt");
    if (inFile) {
        string fileUsername, filePassword, fileEmail;
        inFile >> fileUsername >> filePassword >> fileEmail;
        if (fileUsername == username && filePassword == password) {
            return new Student(fileUsername, filePassword, fileEmail);
        }
    }
    return nullptr;
}

bool Student::operator==(const Student& other) const {
    return this->username == other.username;
}

ostream& operator<<(ostream& os, const Student& student) {
    os << student.username << " (" << student.email << ")";
    return os;
}
