#include "Instructor.h"
#include "Course.h"

Instructor::Instructor(string username, string password, string email)
    : User(username, password, email) {}

void Instructor::addCourse(Course* course) {
    taughtCourses.push_back(course);
}

void Instructor::listCourses() const {
    if (taughtCourses.empty()) {
        cout << "No courses taught yet." << endl;
    } else {
        cout << "Courses taught by " << username << ":" << endl;
        for (size_t i = 0; i < taughtCourses.size(); ++i) {
            cout << i + 1 << ". " << *taughtCourses[i] << endl;
        }
    }
}

void Instructor::saveToFile() const {
    ofstream outFile("instructors/" + username + ".txt");
    if (outFile) {
        outFile << username << endl;
        outFile << password << endl;
        outFile << email << endl;
        outFile.close();
    } else {
        cerr << "Failed to open file for saving instructor data." << endl;
    }
}

Instructor* Instructor::loadFromFile(const string& username, const string& password) {
    ifstream inFile("instructors/" + username + ".txt");
    if (inFile) {
        string fileUsername, filePassword, fileEmail;
        inFile >> fileUsername >> filePassword >> fileEmail;
        if (fileUsername == username && filePassword == password) {
            return new Instructor(fileUsername, filePassword, fileEmail);
        }
    }
    return nullptr;
}

void Instructor::createCourseWithURLs(const string& courseName, const vector<string>& courseURLs) {
    Course* newCourse = new Course(courseName, courseURLs);  // Create a new course with URLs
    addCourse(newCourse);  // Add it to the instructor's list of courses
    Course::addCourse(courseName, courseURLs);  // Also add the course to the file
}

bool Instructor::operator==(const Instructor& other) const {
    return this->username == other.username;
}

ostream& operator<<(ostream& os, const Instructor& instructor) {
    os << instructor.username << " (" << instructor.email << ")";
    return os;
}
