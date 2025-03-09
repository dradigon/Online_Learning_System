#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "User.h"
#include "Course.h"

using namespace std;

class Student : public User {
private:
    vector<Course*> enrolledCourses;

public:

    Student(string username, string password, string email);
    vector<Course*> getEnrolledCourses() const {
        return enrolledCourses;
    }
    void enrollInCourse(Course* course);
    void listCourses() const;
    void saveToFile() const override;

    static Student* loadFromFile(const string& username, const string& password);

    // Overload equality operator to compare students
    bool operator==(const Student& other) const;

    // Overload stream insertion operator to print student details
    friend ostream& operator<<(ostream& os, const Student& student);
};

#endif // STUDENT_H
