#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class Course {
private:
    string courseName;
    vector<string> courseURLs; // To store URLs for the course

public:
    // Constructor to initialize course with a name and URLs
    Course(string name, const vector<string>& urls);

    // Getter for course name
    string getCourseName() const;

    // Getter for course URLs
    vector<string> getCourseURLs() const;

    // Static method to load all courses from the file
    static vector<Course> loadAllCourses();
    static Course loadCourseByName(const string& courseName);

    // Static method to add a new course to the file
    static void addCourse(const string& courseName, const vector<string>& urls);

    // Overload equality operator to compare two courses
    bool operator==(const Course& other) const;

    // Overload stream insertion operator to print course
    friend ostream& operator<<(ostream& os, const Course& course);
};

#endif // COURSE_H
