#include "network.h"
// #include <limits>
#include "misc.h"
// #include <fstream>
// #include <dirent.h>

Network::Network(){
    head = NULL;
    tail = NULL;
    count = 0;
}


Network::Network(string fileName){
    loadDB(fileName);
}

Network::~Network(){
    Connection *ptr = head;
    while(ptr != NULL){
        head = ptr->next;
        delete ptr;
        ptr = head;
    }
}


void Network::push_front(Connection* newEntry){
    // Adds a new Connection (newEntry) to the front of LL 
    newEntry->prev = NULL;
    newEntry->next = head;

    if (head != NULL)
        head->prev = newEntry;
    else
        tail = newEntry;
    
    head = newEntry;
    count++;
}


void Network::push_back(Connection* newEntry){
    // Adds a new Connection (newEntry) to the back of LL
    newEntry->next = NULL;
    newEntry->prev = tail;

    if(tail != NULL){
        tail->next = newEntry;
    }
    else{
        head = newEntry;
    }
    tail = newEntry;
    count++;
}


Connection* Network::search(Connection* searchEntry){
    // Searches the Network for searchEntry
    // if found, returns a pointer to it, else returns NULL
    Connection *ptr = head;
    //cout << "start" << endl;
    while(ptr != NULL){
        //cout << "searching";
        if(*ptr == *searchEntry){
            //cout << "FOUND";
            return ptr;
            break;
        }
        ptr = ptr->next;
    }
    //cout << "return Null" << endl;
    return NULL;
}


Connection* Network::search(string fname, string lname){

    Connection *ptr = head;
    while(ptr != NULL){
        if(ptr->f_name == fname && ptr-> l_name ==lname){
            return ptr;
            break;
        }
        ptr = ptr->next;
    }
    return NULL;

    // New == for Connection, only based on fname and lname
    // if found, returns a pointer to it, else returns NULL
}

Connection* Network::search(string id_test){
    
    Connection *ptr = head;
    while(ptr != NULL){
        if(ptr->id == id_test){
            return ptr;
            break;
        }
        ptr = ptr->next;
    }
    return NULL;
}


void Network::printDB(){
    cout << "Number of connections: " << count << endl;
    cout << "------------------------------" << endl;
    Connection* ptr = head;
    while(ptr != NULL){
        ptr->print_connection();
        cout << "------------------------------" << endl;
        ptr = ptr->next;
    }
}


void Network::saveDB(string filename){
    // Saves the netwrok in file <filename>
    // Note: The format of the output file is very abstract, just raw data
    ofstream outfile;
    outfile.open(filename.c_str());
    string line;
    Connection* ptr = head;
    while(ptr!= NULL){
        //outfile << ptr->l_name << ", " << ptr->f_name << endl << ptr->birthdate->get_date() << endl << ptr->email->get_contact("full") <<  endl << ptr->phone->get_contact("full") << endl << "---------------------------" << endl;
        ptr->write_connection(outfile);
        ptr = ptr->next;
    }
}


void Network::loadDB(string filename){
    // Loads the network from file <filename>

    Connection* ptr = head;
    while(head != NULL){
        ptr = head->next;
        delete head;
        head = ptr;
        count--;
    }
    head = NULL;
    tail = NULL;
    ifstream infile;
    infile.open(filename.c_str());
    string fname, lname, bdate, type, email, phone, line;
    while(getline(infile, line)){
        lname = line.substr(0, line.find(','));
        line.erase(0, line.find(',') + 2);
        fname = line;
        //cout << lname;

        getline(infile, line);
        string bdate = line;
        
        getline(infile, line);
        // line.erase(0,1);
        // type = line.substr(0, line.find(')'));
        // line.erase(0, line.find(')') + 2);
        email = line;

        getline(infile, line);
        // line.erase(0,1);
        // type = line.substr(0, line.find(')'));
        // line.erase(0, line.find(')') + 2);
        phone = line;
        getline(infile, line);
        while(line[0] != '-' && line[1] != '-' && line[2] != '-' && line[3] != '-' && line[4] != '-'){
            getline(infile, line);
        }

        Connection* newEntry = new Connection(fname, lname, bdate, email, phone);
        this->push_back(newEntry);
    }
    infile.close();
    infile.open(filename.c_str());
    while(getline(infile, line)){
        lname = line.substr(0, line.find(','));
        line.erase(0, line.find(',') + 2);
        fname = line;
        getline(infile, line);
        getline(infile, line);
        getline(infile, line);
        getline(infile, line);
        //cout << debug << "\n";
        while(line[0] != '-' && line[1] != '-' && line[2] != '-' && line[3] != '-' && line[4] != '-'){
            search(fname, lname)->myfriends.push_back(search(line));
            getline(infile, line);
        }
    }

}


bool Network::remove(string fname, string lname){
    string random = "4/4/1234";
    Connection item(fname, lname, random, random, random);
    Connection *delete_connection = &item;
    Connection *trying = NULL;
    trying = search(delete_connection);
    if(trying!= NULL){
        if(trying->next ==NULL && trying -> prev == NULL){
        head = NULL; //edit for correction of lab 5
        }
        else if(trying->next == NULL){
            trying->prev->next = NULL;
            tail = trying->prev;
        }
        else if(trying->prev == NULL){
            trying->next->prev = NULL;
            head = trying->next;
        }
        else{
            trying->prev->next = trying->next;
            trying->next->prev = trying->prev;
        }
        vector<Connection*>::iterator itera;
        vector<Connection*>::iterator itera_sec;
        for(itera = trying->myfriends.begin(); itera != trying->myfriends.end(); itera++){
            //cout << "check";
            int delete_friend;
            for(vector<Connection*>::size_type i = 0; i < (*itera)->myfriends.size(); i++){
                //cout << (*itera)->myfriends[i]->id << " ";
                if((*itera)->myfriends[i]->id == trying->id ){
                    //cout << "found";
                    (*itera)->myfriends.erase((*itera)->myfriends.begin()+i);
                }
            }
        }
        delete trying;
        count--;
        return true;
        //delete all traces of the connection in other friend lists
    }
    else{
            return false;
        }
}


void Network::sendEmail(string addr, string subject, string content, string attachment=""){
    // Sending a command looks like:
    //echo "content" | mailx -s "subject" -a "attachment" addr;
    // string new_str = '?' + old_str + '?'

    string vars = "\"";
    string cmd;
    if( attachment == ""){
        cmd = "echo " + vars  + content + vars + " | mailx -s " + vars + subject + vars + " " + vars + addr + vars;
        system(cmd.c_str());
    }
    else{
        cmd = "echo " + vars + content + vars + " | mailx -s " + vars + subject + vars + " -a " + vars + attachment + vars + " " + vars + addr + vars;
        system(cmd.c_str());
    }


}


void Network::showMenu(){

    int opt;
    while(1){
        cout << "\033[2J\033[1;1H";
        printMe("banner"); // from misc library 

        cout << "Select from below (Type Number): \n";
        cout << "1. Save network database \n";
        cout << "2. Load network database \n";
        cout << "3. Add a new connection \n";
        cout << "4. Remove a connection \n";
        cout << "5. Search \n";
        cout << "6. Send an email \n";
        cout << "7. Send an iMessage \n";
        cout << "8. Print database \n";
        cout << "9. Connect \n";
        cout << "\nSelect an option ... ";
        
        if (cin >> opt) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            cin.clear();codeName
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Wrong option! " << endl;
            return;
        }
        
        string fname, lname, fileName, bdate;
        bool exist;
        //vars for dirent.h
        DIR *dir;
        string find_db;
        struct dirent *ent;
        char targetFolderAddr[] = "./";
        cout << "\033[2J\033[1;1H";

        if (opt==1){
            cout << "Saving network database \n";
            cout << "Enter the name of the save file: ";
            cin >> fileName;
            this->saveDB(fileName);
            cout << "Network saved in " << fileName << endl;

        }
        else if (opt==2){
            cout << "Loading network database \n";

            if ((dir = opendir ("./")) != NULL) {
                  while ((ent = readdir (dir)) != NULL) {
                      find_db = ent->d_name;
                      //cout << find_db.substr(find_db.find('.'), 4) << endl;
                      if(find_db.find('.') != -1){
                          if(find_db.substr(find_db.find('.'), 4) == ".db"){
                            cout << ent->d_name << endl;
                          }
                      }
                  }
                  closedir (dir);
            } else {
                  /* could not open directory */
                  perror ("No directory!");
            }
            cout << "Enter the name of the load file: "; 
            cin >> fileName;
            if ((dir = opendir ("./")) != NULL) {
                  while ((ent = readdir (dir)) != NULL) {
                      find_db = ent->d_name;
                      if(find_db == fileName){
                        exist = true;
                        this->loadDB(fileName);
                        // If file is loaded successfully, also print the count of connections in it: 
                        cout << "Network loaded from " << fileName << " with " << count << " connections \n";

                      }
                  }
                  closedir (dir);
            } else {
                  /* could not open directory */
                  perror ("No directory!");
            }
            // If file with name FILENAME does not exist: 
            if (exist == false){
                cout << "File FILENAME does not exist!" << endl;
            }
        }
        else if (opt == 3){
            // Add a new Connection ONLY if it does not exist
            Connection *newcon = new Connection;
            if(this->search(newcon) == NULL){
                this->push_front(newcon);
                cout << "Adding a new connection \n";
            }
            else{
                cout << "Sorry, this connection already exists ;(." << endl;
            }
        }
        else if (opt == 4){
            cout << "Removing a connection \n";
            cout << "First name: ";
            getline(cin, fname);
            cout << "Last name: ";
            getline(cin, lname);
            if(remove(fname,lname)){
                cout << "Remove Successful! \n";
            }
            else{
                cout << "Connection not found! \n";
            }
            // if found, cout << "Remove Successful! \n";
            // if not found: cout << "Connection not found! \n";
        }
        else if (opt==5){
            cout << "Searching: \n";
            cout << "First Name: ";
            getline(cin, fname);
            cout << "Last Name: ";
            getline(cin, lname);

            if(this->search(fname, lname) != NULL){
                this->search(fname, lname)->print_connection();
            }
            else{
                cout << "Not found! \n";
            }
            // if found: print connection
            // if not, cout << "Not found! \n";
        }
        else if (opt==6){
            cout << "Sending email to: \n";
            cout << "First Name: ";
            getline (std::cin, fname);
            cout << "Last Name: ";
            getline (std::cin, lname);
            Connection* ptr = search(fname, lname);
            if (ptr){
                string subject, content, attachment;
                cout << "Sending email to: \n";
                ptr->print_connection();
                cout << "Subject: ";
                std::getline (std::cin, subject);
                cout << "Content: ";
                std::getline (std::cin, content);
                cout << "Attachment: ";
                std::getline (std::cin, attachment);
                string addr = ptr->email->get_contact("1");
                sendEmail(addr, subject, content, attachment);
            }
            else 
                cout << "Contact not found! \n";
        }
        else if (opt==7){
            cout << "Option currently not available\n";
        }
        else if (opt==8){
            cout << "Network Database: \n";
            this->printDB();
        }
        else if (opt==9){
            cout << "Make Friends \n";
            string fname2;
            string lname2;
            cout << "Connection 1 \n";
            cout << "First name: ";
            getline(cin, fname);
            cout << "Last name: ";
            getline(cin, lname);
            if(search(fname,lname)){
                cout << "Connection 2 \n";
                cout << "First name: ";
                getline(cin, fname2);
                cout << "Last name: ";
                getline(cin, lname2);
                if(search(fname2,lname2)){
                    search(fname, lname)->myfriends.push_back(search(fname2, lname2));
                    search(fname2, lname2)->myfriends.push_back(search(fname, lname));
                }
                else{
                    cout << "Connection not found! \n";
                }
            }
            else{
                cout << "Connection not found! \n";
            }
        }
        else
            cout << "Nothing matched!\n";
        
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "\n\nPress Enter key to go back to main menu ... ";
        string temp;
        std::getline (std::cin, temp);
        cout << "\033[2J\033[1;1H";
    }
}
