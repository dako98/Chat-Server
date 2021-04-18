#include "UsersDatabase.hpp"

UsersDatabase::UsersDatabase() {

}

UsersDatabase& UsersDatabase::getInstance() {
    static UsersDatabase instance;
    return instance;
}