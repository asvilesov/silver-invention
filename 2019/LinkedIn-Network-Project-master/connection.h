#ifndef CONNECTION_H
#define CONNECTION_H

#include "date.h"
#include "contact.h"
#include "misc.h"
#include "fstream"
#include "iostream"
#include <ostream>
#include <string>
#include <vector>

using namespace std;

class Connection{
    friend class Network;

private:
	string f_name;
	string l_name;
	Date *birthdate;
    Email *email;
    Phone *phone;
    Connection* next;
    Connection* prev;
    string id;
    vector<Connection*> myfriends;

public: 
    Connection();
    ~Connection();
    Connection(string filename);
    Connection(string f_name, string l_name, string bdate, string email, string phone);
	void print_connection();
    void write_connection(ostream& filename);
	void set_connection();
	void set_connection(string filename);
    bool operator==(const Connection& rhs);
    bool operator!=(const Connection& rhs);
};


#endif

