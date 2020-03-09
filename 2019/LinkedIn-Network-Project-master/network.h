
#ifndef NETWORK_H
#define NETWORK_H

#include "connection.h"
#include "date.h"
#include <stdlib.h>
#include <fstream>
#include <dirent.h>
#include <limits>
#include <cctype>
#include <algorithm>

class Network{

    private:
        Connection* head = NULL;
        Connection* tail = NULL;
        int count; 
        Connection* search(Connection* searchEntry);
        Connection* search(string fname, string lname);
        Connection* search(string id_test);

    public:
        Network();
        Network(string fileName);
        ~Network();
        void push_front(Connection* newEntry);
        void push_back(Connection* newEntry);
        bool remove(string fname, string lname);
        void saveDB(string filename);
        void loadDB(string filename);
        void printDB();
        void showMenu();
        void sendEmail(string addr, string subject, string content, string attachment);
        void sendMsg(string number, string content);
};

#endif
