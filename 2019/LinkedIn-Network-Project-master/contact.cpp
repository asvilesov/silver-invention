#include "contact.h"
// #include<string>
// #include<iostream> 
// #include<stdlib.h>
// #include <sstream>
//using namespace std;


Email::Email(string type, string email_addr){
    this->type = type;
    this->email_addr = email_addr;
}

void Email::set_contact(){
    cout << "Enter the type of email address: ";
    cin >> type;
    cout << "Enter email address: ";
    cin >> email_addr;
}


string Email::get_contact(string style = "full"){
    // Note: We have default argument in declaration and not in definition!
    if (style=="full")
        return "(" + type + ") " + email_addr;
    else 
        return email_addr;
}


void Email::print(){
    // Note: get_contact is called with default argument
    cout << "Email " << get_contact() << endl;
}


Phone::Phone(string type, string num){
    long int area_code = atoi(num.substr(0, 3).c_str());
    if(num[3] == '-'){
        num.erase(0,4);
    }
    else{
        num.erase(0,3);
    }
    long int  number_one = atoi(num.substr(0, 3).c_str());
    if(num[3] == '-'){
        num.erase(0,4);
    }
    else{
        num.erase(0,3);
    }
    long int number_two = atoi(num.substr(0,4).c_str());
    //cout << number_one <<  " "  << number_two << " ";
    //cout << num.substr(0, 3).c_str() << endl;
    //cout << number << endl;
    this->type = type;
    this->phone_num = area_code*10000000 + number_one*10000 + number_two;
    //cout << phone_num; 
    
}

void Phone::set_contact(){
    cout <<"Enter the type of phone number: ";
    cin  >> type;
    cout << "Enter the phone number: ";
    cin >> phone_num;
}


string Phone::get_contact(string style = "full"){
    // long int x1 = (phone_num/10000000);
    // long int x2 = ((phone_num)/10000)%1000;
    // long int x3 = phone_num%10000;
    // //cout << endl << x3 << " " << x2 << " " << x1;
    // int i = 0;
    // string addZero;
    
    // ostringstream s1;
    // s1 << x1;
    // string s1s = s1.str();
    // if(s1s.length() < 3){
    //     for(i = 0; i < 3 - s1s.length(); i++){
    //         addZero += "0";
    //     }
    // }
    // addZero = "";
    // s1s = addZero + s1s;

    // ostringstream s2;
    // s2 << x2;
    // string s2s = s2.str();
    // if(s1s.length() < 3){
    //     for(i = 0; i < 3 - s2s.length(); i++){
    //         addZero += "0";
    //     }
    // }
    // s2s = addZero + s2s;
    // addZero = "";

    // ostringstream s3;
    // s3 << x3;
    // string s3s = s3.str();
    // if(s1s.length() < 4){
    //     for(i = 0; i < 4 - s3s.length(); i++){
    //         addZero += "0";
    //         cout << addZero;
    //     }
    // }
    // s3s = addZero + s3s;


    
    ostringstream s4;
    s4 << phone_num;

    string why = s4.str();

    if (style=="full")
        return "(" + type + ") " + why.substr(0,3) + "-" + why.substr(3,3) + "-" + why.substr(6,4);
    else 
        return s4.str();  
}


void Phone::print(){
    // Note: get_contact is called with default argument
    cout << "Phone " << get_contact() << endl;
}
