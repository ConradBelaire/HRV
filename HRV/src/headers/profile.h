#ifndef PROFILE_H
#define PROFILE_H

class DBManager;

class Profile {

public:
    Profile(int);
    int getId();

private:
    int id;

};

#endif // PROFILE_H
