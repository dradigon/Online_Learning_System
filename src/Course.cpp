#include "Course.h"

Course::Course(string name, const vector<string>& urls) 
    : courseName(name), courseURLs(urls) {}

string Course::getCourseName() const {
    return courseName;
}

vector<string> Course::getCourseURLs() const {
    return courseURLs;
}

vector<Course> Course::loadAllCourses() {
    vector<Course> courses;
    ifstream inFile("courses/courses.txt");

    if (inFile) {
        string line;
        string courseName;
        vector<string> courseURLs;
        while (getline(inFile, line)) {
            if (!line.empty()) {
                if (courseName.empty()) {
                    courseName = line;  // First line is the course name
                } else {
                    courseURLs.push_back(line);  // Remaining lines are URLs
                }
            }
            if (line.empty() || inFile.peek() == EOF) {
                if (!courseName.empty()) {
                    courses.emplace_back(courseName, courseURLs);
                    courseName.clear();
                    courseURLs.clear();
                }
            }
        }
        inFile.close();
    }
    return courses;
}

// Load a specific course by name
Course Course::loadCourseByName(const string& courseName) {
    ifstream inFile("courses/" + courseName + ".txt");
    vector<string> courseURLs;

    if (inFile) {
        string url;
        while (getline(inFile, url)) {
            courseURLs.push_back(url);
        }
        inFile.close();
    } else {
        cerr << "Failed to load course content for " << courseName << endl;
    }

    return Course(courseName, courseURLs);
}

bool Course::operator==(const Course& other) const {
    return this->courseName == other.courseName;
}

ostream& operator<<(ostream& os, const Course& course) {
    os << course.courseName;
    return os;
}

void Course::addCourse(const string& courseName, const vector<string>& urls) {
    ofstream outFile("courses/courses.txt", ios::app);
    if (outFile) {
        outFile << courseName << endl;
        for (const auto& url : urls) {
            outFile << url << endl;  // Save each URL in the file
        }
        outFile << endl;  // Separate courses with an empty line
        outFile.close();

        // Create a course-specific file and add URLs
        ofstream courseFile("courses/" + courseName + ".txt");
        if (courseFile) {
            for (const auto& url : urls) {
                courseFile << url << endl;
            }
            courseFile.close();
        } else {
            cerr << "Failed to create course-specific file for " << courseName << endl;
        }
    } else {
        cerr << "Failed to open courses.txt for writing." << endl;
    }
}
