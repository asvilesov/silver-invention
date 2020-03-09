// #include <string>
#include "connection.h"

Connection::Connection(){
    set_connection();
}


Connection::~Connection(){
    delete phone;
    delete email;
    delete birthdate;
}


Connection::Connection(string fname, string lname, string bdate, string email, string phone){
    string type;
    email.erase(0,1);
    type = email.substr(0, email.find(')'));
    email.erase(0, email.find(')') + 2);
    this->email = new Email(type, email);   
    
    phone.erase(0,1);
    type = phone.substr(0, phone.find(')'));
    phone.erase(0, phone.find(')') + 2);
    this->phone = new Phone(type, phone);
    
    this->birthdate = new Date(bdate);
    this->f_name = fname;
    this->l_name = lname;
    this->id = codeName(fname, lname);
}


Connection::Connection(string filename){
    set_connection(filename);
}


void Connection::set_connection(){
    // prompts for the information of the user
    // first/last name can have spaces!
    // date format must be "M/D/YYYY"
    // We are sure user enters info in correct format.
    string temp;
    string type;

    cout << "First Name: ";
    getline(cin, f_name);
    cout << "Last Name: ";
    getline(cin, l_name);

    cout << "Birthdate (M/D/YYYY): ";
    getline(cin, temp);
    birthdate = new Date(temp);
    
    cout << "Type of email address: ";
    getline(cin, type);
    cout << "Email address: ";
    getline(cin, temp);
    email = new Email(type, temp);

    cout << "Type of phone number: ";
    getline(cin, type);
    cout << "Phone number: ";
    getline(cin, temp);
    phone = new Phone(type, temp);

    id = codeName(f_name, l_name);
}


void Connection::set_connection(string filename){
    // Phone number in files can have '-' or not.
    string temp;
    string type;

    ifstream input_textfile(filename.c_str());
    string line;
    
    getline(input_textfile, line);
    f_name = line.substr(0, line.find(','));
    line.erase(0, line.find(',') + 2);
    l_name = line;

    getline(input_textfile, line);
    birthdate = new Date(line);
    
    getline(input_textfile, line);
    line.erase(0,1);
    type = line.substr(0, line.find(')'));
    line.erase(0, line.find(')') + 2);
    temp = line;
    email = new Email(type, temp);

    getline(input_textfile, line);
    line.erase(0,1);
    type = line.substr(0, line.find(')'));
    line.erase(0, line.find(')') + 2);
    temp = line;
    phone = new Phone(type, temp);

    id = codeName(f_name, l_name);
}


bool Connection::operator==(const Connection& rhs){
    if( (f_name == rhs.f_name) && 
        (l_name == rhs.l_name)
        ) {
        return true;
    }
    else{
        return false;
    }
}


bool Connection::operator!=(const Connection& rhs){ 
    return !(*this == rhs);
}


void Connection::print_connection(){
    cout << l_name <<", " << f_name << endl;
    birthdate->print_date("Month D, YYYY");
    email->print();
    phone->print();
    vector<Connection*>::iterator itera;
    for(itera = myfriends.begin(); itera != myfriends.end(); itera++){
        cout << codeName((*itera)->f_name, (*itera)->l_name) << endl;
    }
}

void Connection::write_connection(ostream& filename){

    filename << l_name << ", " << f_name << endl 
            << birthdate->get_date() << endl << email->get_contact("full") <<  endl 
            << phone->get_contact("full") << endl;
    vector<Connection*>::iterator itera;
    for(itera = myfriends.begin(); itera != myfriends.end(); itera++){
        filename << codeName((*itera)->f_name, (*itera)->l_name) << endl;
    }
    filename << "---------------------------" << endl;

}

