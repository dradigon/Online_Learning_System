#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "User.h"
#include "Course.h"

using namespace std;

class Instructor : public User {
private:
    vector<Course*> taughtCourses;

public:
    Instructor(string username, string password, string email);

    void addCourse(Course* course);
    void listCourses() const;
    void saveToFile() const override;

    static Instructor* loadFromFile(const string& username, const string& password);

    // New method to add a course with URLs
    void createCourseWithURLs(const string& courseName, const vector<string>& courseURLs);

    // Overload equality operator to compare instructors
    bool operator==(const Instructor& other) const;

    // Overload stream insertion operator to print instructor details
    friend ostream& operator<<(ostream& os, const Instructor& instructor);
};

#endif // INSTRUCTOR_H
