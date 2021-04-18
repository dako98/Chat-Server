#ifndef __USER_H__
#define __USER_H__

#define debug

#include <string>

class User
{

public:
    User();
    ~User();

    void serialise() const;

private:
    const std::string name;
    std::string password;
};
#endif // __USER_H__
