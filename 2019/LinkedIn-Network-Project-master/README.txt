#LinkedIn-Network-Project

The programs in this project collectively make up a network similar to LinkedIn:

run with precompiled code "./a.out"

1. It is made up of 4 classes - date, contact, connection, and network. 
2. IF you are running this from a server, you can send the network file (.db) in an email format to others!
3. Network is made up of various connections, friends list, and each connection has a date such as birthday and contact info
4. Run showMenu() from a script to have easy access to network, or just use "test_all.cpp"
5. Menu Options are as follows:
	1. Save network database 
    2. Load network database
    3. Add a new connection
    4. Remove a connection
    5. Search
    6. Send an email
    7. Send an iMessage
    8. Print database
    9. Connect friends
6. Make sure to load a network database such as the sample one "student_network.db","sample_network.db" or create your own by adding connections
7. If the connections are friends with each other use option 9 to connect them.







More Detailed Background:
The date class is used store dates with inputs of different forms such as 1/1/1998 from a string, as inputs to a constructor date(1,1,1998) or as inputs from user. 
The connection class stores a person that is supposed to be connected to the main user. A connection object will have attributes such as the connection's first name, last name, and a birthdate of the connection that is stored as a date object. The constructor of this class accepts input for the connection through an input file, inputs to the constructor connection(string fname, string lname, string bdate) or through inputs from the user. This is achieved through function and constructor overloading.
The contact class has similarities to the other classes, however it has no functionality other than being a template irtual class for email and phone classes. 
Both of the previous classes override the equality operator '==' and '!='. It is used to check if the attributes between two objects of the same class are equal and returns a boolean.
The network class's main function is to store connections of the main user. Throughout the lifetime of the program the connections are stored in a double linked list. When the connections are made, the pointer pointing to those connections can be passed to a push_back or push_front function, which adds the connection to the front or back the linked list. There exists a function called remove() which takes a connection with certain attributes out of the network. The remove() function does this with the aid of the search() function which looks for a connection with certain attributes. Connections can also be loaded in from a textfile with a certain format. Or if one wants to save the network, connections are outputted into a textfile.
The network has an added feature which is the menu functionality. It essentially implements loading, saving files, adding, searching, deleting connections, and sending emails in a user friendly way. 

The program works when run through different tests and does not have memory leaks according to ASAN
