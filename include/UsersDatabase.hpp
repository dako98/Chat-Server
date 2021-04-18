#ifndef __USERSDATABASE_H__
#define __USERSDATABASE_H__


class UsersDatabase {

public:
    static UsersDatabase& getInstance();
    
    UsersDatabase(UsersDatabase const&) = delete;
    void operator=(UsersDatabase const&) = delete;

private:
    UsersDatabase();
};

#endif // __USERSDATABASE_H__
