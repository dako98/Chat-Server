#ifndef __USER_H__
#define __USER_H__

#define debug

#include <string>
#include <iostream>

class User
{

public:
    User();
    User(const std::string &name,
         const std::string &password);

    User(const User &other);

    User &operator=(User &&other);

    std::string getName() const;
    
    ~User();

    void serialise() const;

    friend std::ostream &operator<<(std::ostream &out, const User &obj);

private:
    const std::string name;
    std::string password;
};
#endif // __USER_H__
