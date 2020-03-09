#include "date.h"

Date::Date(){
    day = 1;
    month = 1;
    year = 1970;
}


Date::Date(int day, int month, int year){
    set_date(day, month, year);
}


Date::Date(string str){
    // input format is M/D/YYYY
    set_date(str); 
}


bool Date::operator==(const Date& rhs){
    if ( (day == rhs.day) && 
         (month == rhs.month) && 
         (year == rhs.year))
        return true;
    else
        return false;
}


bool Date::operator!=(const Date& rhs){
    return !(*this == rhs);
}


bool Date::check_date(){
    // Leap years are those years divisible by 4, 
    // except for century years whose number is not divisible by 400. 

    if (day<1)
        return false;

    // 31: Jan March May July August October December
    // 30: April, June, September, November, 
    // 29: Feb

    switch (month){
        case Jan:
        case Mar:
        case May:
        case Jul:
        case Aug:
        case Oct:
        case Dec:
        if (day>31)
            return false;
        break;

        case Apr:
        case Jun:
        case Sep:
        case Nov:
        if (day > 30)
            return false;
        break;

        case Feb:
        if (day > 29)
            return false;
        else if (day == 28 & year%4 == 0){
            if (year%100 == 0 & year%400 == 0)
                return false;
            else
                return true;
        }
        else 
            return true;
        break;

        default:
        return false;
    }
    return true;
}

bool Date::set_date(string str){
    // input format is M/D/YYYY

    month = atoi(str.substr(0, str.find('/')).c_str());

    str = str.substr(str.find('/')+1).c_str();
    day = atoi(str.substr(0, str.find('/')).c_str());

    str = str.substr(str.find('/')+1).c_str();
    year = atoi(str.c_str());

    if (check_date()==false){
        cout << "Error! Invalid date!" << endl;
        cout << "Date set to default!" << endl;
        day = 1;
        month = 1;
        year = 1970;
        return false;
    }
    return true;
}

bool Date::set_date(int day, int month, int year){
    this->month = month; 
    this->day = day;
    this->year = year;
    if (check_date()==false){
        cout << "Error! Invalid date!" << endl;
        cout << "Date set to default!" << endl;
        this->day = 1;
        this->month = 1;
        this->year = 1970;
        return false;
    }
    return true;
}

void Date::print_date(string type){
    string month_str;
    switch( month ) {
        case Jan: month_str = "January"; break;
        case Feb: month_str = "February"; break;
        case Mar: month_str = "March"; break;
        case Apr: month_str = "April"; break;
        case May: month_str = "May"; break;
        case Jun: month_str = "June"; break;
        case Jul: month_str = "July"; break;
        case Aug: month_str = "August"; break;
        case Sep: month_str = "September"; break;
        case Oct: month_str = "October"; break;
        case Nov: month_str = "November"; break;
        case Dec: month_str = "December"; break;
    }
    if (type == "M/D/YYYY")
            cout << month <<'/' << day << '/' << year << endl;
    else if (type == "Month D, YYYY")
            cout << month_str <<' ' << day << ", " << year << endl;
    else if (type == "D-Month-YYYY")
            cout << day << '-' << month_str << '-' << year << endl;
    else
        cout << "Wrong print type!" << endl;
}


string Date::get_date(){
    ostringstream ss;
    ss << month << "/" << day << "/" << year; 
    string str = ss.str();
    return str;
}

