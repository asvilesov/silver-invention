
#ifndef CONTACT_H
#define CONTACT_H

#include <iostream>
#include <string>
#include "contact.h"
#include<stdlib.h>
#include <sstream>
using namespace std;


class Contact {
protected:	
	string type;
public:
	virtual void print() = 0;
    virtual string get_contact(string style="full") = 0;
	virtual void set_contact() = 0;
};


class Email: public Contact{
private:
    string email_addr;
public:
    Email(string type, string email_addr);
    void set_contact();
    string get_contact(string style);
    void print();
};


class Phone: public Contact{
private:
	long int phone_num; 
public:
    Phone(string type, string phone_number);
    void set_contact();
    string get_contact(string style);
    void print();
};

#endif

