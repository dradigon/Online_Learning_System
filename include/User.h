#ifndef USER_H
#define USER_H

#include <string>

using namespace std;

class User {
protected:
    string username;
    string password;
    string email;

public:
    string getUsername() const {
        return username;
    }
    User(string username, string password, string email)
        : username(username), password(password), email(email) {}

    virtual void saveToFile() const = 0;

    bool verifyCredentials(const string& enteredPassword) const {
        return password == enteredPassword;
    }

    virtual ~User() = default;
};

#endif // USER_H
