#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Student.h"
#include "Instructor.h"
#include "Course.h"

using namespace std;


#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"

void printBorderedText(const string& text, const string& borderColor = BLUE) {
    const int borderWidth = 50;
    cout << borderColor << string(borderWidth, '*') << RESET << endl;
    cout << borderColor << "* " << text << string(borderWidth - text.length() - 3, ' ') << "*" << RESET << endl;
    cout << borderColor << string(borderWidth, '*') << RESET << endl;
}

// Function to display titles with decoration
void displayTitle(const string& title) {
    cout << YELLOW << string(60, '=') << RESET << endl;
    cout << BLUE << "      " << title << RESET << endl;
    cout << YELLOW << string(60, '=') << RESET << endl;
}

void displayMessage(const string& message, const string& color = GREEN) {
    if (color == "green") {
        cout << GREEN << message << RESET << endl;
    } else if (color == "red") {
        cout << RED << message << RESET << endl;
    } else if (color == "yellow") {
        cout << YELLOW << message << RESET << endl;
    } else {
        cout << BLUE << message << RESET << endl;
    }
}




// Helper functions to manage directories and files without filesystem library
bool directoryExists(const string& dirName) {
    ifstream dirCheck(dirName);
    return dirCheck.good();
}

void createDirectory(const string& dirName) {
    system(("mkdir " + dirName).c_str());
}

void initializeDirectories() {
    if (!directoryExists("students")) createDirectory("students");
    if (!directoryExists("instructors")) createDirectory("instructors");
    if (!directoryExists("courses")) createDirectory("courses");

    // Ensure the courses.txt file exists
    ofstream courseFile("courses/courses.txt", ios::app);
    courseFile.close();
}

void displayAvailableCourses() {
    vector<Course> courses = Course::loadAllCourses();
    if (courses.empty()) {
        cout << "No courses available." << endl;
    } else {
        cout << "Available Courses:" << endl;
        for (size_t i = 0; i < courses.size(); ++i) {
            cout << i + 1 << ". " << courses[i].getCourseName() << endl;
        }
    }
}

void searchCourses(const string& searchTerm) {
    vector<Course> courses = Course::loadAllCourses();
    bool found = false;

    displayTitle("Search Results");
    for (size_t i = 0; i < courses.size(); ++i) {
        if (courses[i].getCourseName().find(searchTerm) != string::npos) {
            cout << GREEN << i + 1 << ". " << courses[i].getCourseName() << RESET << endl;
            found = true;
        }
    }

    if (!found) {
        cout << RED << "No courses found for the search term '" << searchTerm << "'." << RESET << endl;
    }
}

// Function to display URLs and track progress
void displayURLsAndProgress(vector<string>& urls, Student* student) {
    int completedURLs = 0;
    ifstream progressFile("students/" + student->getUsername() + "_progress.txt");
    if (progressFile) {
        progressFile >> completedURLs;
        progressFile.close();
    }

    for (size_t i = completedURLs; i < urls.size(); ++i) {
        cout << "URL " << i + 1 << ": " << urls[i] << endl;
        cout << "Do you want to go to the next URL? (y/n): ";
        char next;
        cin >> next;

        if (next == 'y') {
            completedURLs = i + 1;
        } else {
            break;
        }
    }

    // Save the updated progress
    ofstream progressOut("students/" + student->getUsername() + "_progress.txt");
    progressOut << completedURLs;
    progressOut.close();
}

void displayEnrolledCoursesAndOptions(Student* student) {
    vector<Course*> enrolledCourses = student->getEnrolledCourses();
    if (enrolledCourses.empty()) {
        cout << "You are not enrolled in any courses yet." << endl;
        cout << "0 courses enrolled. Do you want to start a new course? (y/n): ";
        char option;
        cin >> option;

        if (option == 'y') {
            cout << "Do you want to search for a course? (y/n): ";
            char searchOption;
            cin >> searchOption;

            if (searchOption == 'y') {
                string searchTerm;
                cout << "Enter the course name to search: ";
                cin.ignore();  // Clear the input buffer
                getline(cin, searchTerm);

                searchCourses(searchTerm);  // Call the search function
                cout << "Enter the course number to enroll: ";
                int courseIndex;
                cin >> courseIndex;

                vector<Course> courses = Course::loadAllCourses();
                if (courseIndex > 0 && courseIndex <= static_cast<int>(courses.size())) {
                    student->enrollInCourse(new Course(courses[courseIndex - 1].getCourseName(), courses[courseIndex - 1].getCourseURLs()));
                    student->saveToFile();
                    cout << "Enrolled in course successfully!" << endl;

                    // Display URLs and track progress
                    vector<string> urls = courses[courseIndex - 1].getCourseURLs();
                    displayURLsAndProgress(urls, student);
                } else {
                    cout << "Invalid course number." << endl;
                }
            } else {
                displayAvailableCourses();
                cout << "Enter the course number to enroll: ";
                int courseIndex;
                cin >> courseIndex;

                vector<Course> courses = Course::loadAllCourses();
                if (courseIndex > 0 && courseIndex <= static_cast<int>(courses.size())) {
                    student->enrollInCourse(new Course(courses[courseIndex - 1].getCourseName(), courses[courseIndex - 1].getCourseURLs()));
                    student->saveToFile();
                    cout << "Enrolled in course successfully!" << endl;

                    vector<string> urls = courses[courseIndex - 1].getCourseURLs();
                    displayURLsAndProgress(urls, student);
                } else {
                    cout << "Invalid course number." << endl;
                }
            }
        }
    } else {
        cout << "You are enrolled in the following courses:" << endl;
        for (size_t i = 0; i < enrolledCourses.size(); ++i) {
            cout << i + 1 << ". " << enrolledCourses[i]->getCourseName() << endl;
            int completedURLs = 0;
            ifstream progressFile("students/" + student->getUsername() + "_progress_" + enrolledCourses[i]->getCourseName() + ".txt");
            if (progressFile) {
                progressFile >> completedURLs;
                progressFile.close();
            }
            cout << "Progress: " << completedURLs << "/" << enrolledCourses[i]->getCourseURLs().size() << " URLs completed." << endl;
        }

        cout << "Do you want to continue any of these courses or join a new one? (Enter 'c' for continue, 'n' for new course): ";
        char option;
        cin >> option;

        if (option == 'c') {
            cout << "Enter the number of the course you want to continue: ";
            int courseIndex;
            cin >> courseIndex;

            if (courseIndex > 0 && courseIndex <= static_cast<int>(enrolledCourses.size())) {
                vector<string> urls = enrolledCourses[courseIndex - 1]->getCourseURLs();
                displayURLsAndProgress(urls, student);
            } else {
                cout << "Invalid course number." << endl;
            }
        } else if (option == 'n') {
            displayAvailableCourses();
            cout << "Enter the course number to enroll: ";
            int courseIndex;
            cin >> courseIndex;

            vector<Course> courses = Course::loadAllCourses();
            if (courseIndex > 0 && courseIndex <= static_cast<int>(courses.size())) {
                student->enrollInCourse(new Course(courses[courseIndex - 1].getCourseName(), courses[courseIndex - 1].getCourseURLs()));
                student->saveToFile();
                cout << "Enrolled in course successfully!" << endl;

                vector<string> urls = courses[courseIndex - 1].getCourseURLs();
                displayURLsAndProgress(urls, student);
            } else {
                cout << "Invalid course number." << endl;
            }
        }
    }
}



int main() {
    initializeDirectories();
    char c = 'n';
    do {
        displayTitle("User Authentication");

        cout << "Are you a student or instructor? (Enter '1 - Student ' or '2 - Instructor'): ";
        string userType;
        cin >> userType;

        if (userType == "1") {
            displayMessage("Student Login", "yellow");
            cout << "Are you a new or existing student? (Enter '1 - new' or '2 - existing'): ";
            string status;
            cin >> status;

            if (status == "1") {
                string username, password, email;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                cout << "Enter email: ";
                cin >> email;

                Student newStudent(username, password, email);
                newStudent.saveToFile();
                displayMessage("Student account created!", "green");
                displayMessage("Please Relogin as existing student ... For Learning a Course", "yellow");
            } else if (status == "2") {
                char ch = 'n';
                do {
                    string username, password;
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;

                    Student* student = Student::loadFromFile(username, password);
                    if (student) {
                        displayMessage("Login successful!", "green");
                        displayEnrolledCoursesAndOptions(student);
                        delete student;
                    } else {
                        displayMessage("Invalid credentials.", "red");
                        displayMessage("Do you want to Relogin?(y/n)", "yellow");
                        cin >> ch;
                    }
                } while (ch != 'n');
            }
        } else if (userType == "2") {
            displayMessage("Instructor Login", "yellow");
            cout << "Are you a new or existing instructor? (Enter '1 - new' or '2 - existing'): ";
            string status;
            cin >> status;

            if (status == "1") {
                string username, password, email;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                cout << "Enter email: ";
                cin >> email;

                Instructor newInstructor(username, password, email);
                newInstructor.saveToFile();
                displayMessage("Instructor account created!", "green");
                cout << "Please Relogin as existing instructor .... To add Courses " << endl;
            } else if (status == "2") {
                char ch = 'n';
                do {
                    string username, password;
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;

                    Instructor* instructor = Instructor::loadFromFile(username, password);
                    if (instructor) {
                        displayMessage("Login successful!", "green");
                        cout << "Would you like to add a new course? (y/n): ";
                        char add;
                        cin >> add;

                        if (add == 'y') {
                            string courseName;
                            cout << "Enter course name: ";
                            cin.ignore(); // To clear the buffer
                            getline(cin, courseName);

                            vector<string> courseURLs;
                            string url;
                            cout << "Enter at least 3 URLs for the course (enter 'done' when finished):" << endl;
                            for (int i = 0; i < 3; ++i) {
                                cout << "Enter URL " << i + 1 << ": ";
                                cin >> url;
                                courseURLs.push_back(url);
                            }

                            instructor->createCourseWithURLs(courseName, courseURLs);
                            displayMessage("Course added successfully!", "green");
                        }
                        instructor->listCourses();
                        delete instructor;
                    } else {
                        displayMessage("Invalid credentials.", "red");
                        displayMessage("Do you want to Relogin? (y/n)", "yellow");
                        cin >> ch;
                    }
                }while (ch != 'n');
            }
        } else {
            displayMessage("Invalid user type entered.", "red");
            displayMessage("Do you want to Relogin?(y/n)", "yellow");
            cin >> c;
        }
    }while (c != 'n');
    cout << " Thank you for Logging " << endl;
    return 0;
}
