#include <iostream>
#include "contact.h"
#include "date.h"
#include "connection.h"
#include "network.h"

int main(){




	//cout << "hello";
	Email one("home", "a@lol.com");
	//one.print();
	//cout << one.get_contact("full");

	Phone two("home", "6263907245");
	//cout << two.get_contact("full");
	//two.print();
	
	//cout << "hello3";	
	Connection gotcha("ale", "vil", "11/10/1998","e@lol.com", "6263908241");
	Connection test_equiv("alex", "vil", "2/3/1234", "asdf", "2344452624");
	if (gotcha == test_equiv){
		cout << "true" << endl;
	}
	else{
		cout << "false" << endl;
	}
	//gotcha.print_connection();
	//Connection testing("networkDB.db");
	//Connection testing;

	//cout << "hello3";

	Network usc;
	//usc.remove("Van Nostrand", "Martin");
	//usc.printDB();
	//usc.saveDB("lol.txt");
	usc.showMenu();
}