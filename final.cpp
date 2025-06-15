#include<iostream>
#include<windows.h> //windows.h is a windows specific header file that gives access to system level functions like changing the colour of console.
#include<fstream> //for file handling.
using namespace std;

// view_Flight is a global function here it can be accessed from all the classes.
void view_Flight(); //Function Declaration

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
// GetStdHandle(STD_OUTPUT_HANDLE) gets the console's output handle.
// SetConsoleTextAttribute() sets the text color.
// colorCode is a number (like 1 for blue, 4 for red, etc.)

class UI 
{    
public:
    void show_main_menu() {
        setColor(1);  // Light Cyan for airplane
        cout << R"(
                                               
                      --@--@--@--@--@--@--@--@--(_)--@--@--@--@--@--@--@--@--
        )";

        setColor(11);  // Blue
        cout << "                     ====================================\n";
        cout << "                                   FLIGHT RESERVATION SYSTEM  \n";
        cout << "                             ====================================\n";

        setColor(10);  // Green 
        //MENU
        cout << "1. Passenger Registration\n";
        cout << "2. View Flights\n";
        cout << "3. Book Flight\n";
        cout << "4. Admin Access (Add Flights)\n";
        cout << "0. Exit\n";
        cout << "====================================\n";

        setColor(7);  // Reset colour to default
    }
};

class Person //Base Class 
{
protected:
    string name;
    int age;
    char gender;
    int adhar;
    string passwordp;

public:
    virtual void set_info() //Taking input from user.
    {
        cout << "Enter name: ";
        cin >> name;
        cout << "Enter age: ";
        cin >> age;
        cout << "Enter Aadhar number: ";
        cin >> adhar;
        cout << "Enter Gender (M/F): ";
        cin >> gender;
        cout << "Set your password: ";
        cin >> passwordp;
    }

    bool verifyPassword(string pass) //verify password (will return either zero or one)
    {
        return pass == passwordp;
    }

    virtual void display_info() = 0; //pure virtual function
};

class Passenger : public Person  //Derived Class
{
private:
    int passenger_ID;
    int nextp_ID = 1000;
    bool isRegistered = false;  //flag to check registration

public:
    Passenger() {
        passenger_ID = nextp_ID++; 
        nextp_ID ++;                   //To generate a unique passenger ID for every passenger.
    }

    void set_info() override
    {
        Person::set_info();   //Calling function from base class to get passenger information!!
        cout << "Passenger registered successfully!\nYour ID: " << passenger_ID << endl;
        isRegistered = true;
    }

    void display_info() override  //Displaying passengers registered information based on password 
    {
        if (!isRegistered) {
            cout << "You need to register first.\n";
            return;
        }

        string pass;
        cout << "Enter Password: ";
        cin >> pass;
        if (verifyPassword(pass))   //verifypassword() function of base class is called.
        {   
            cout << "Passenger Details:\n";
            cout << "Name: " << name << "\nAge: " << age << "\nAadhar: " << adhar << "\nGender: " << gender << "\nID: " << passenger_ID << endl;
        } else {
            cout << "Incorrect password.\n";
        }
    }

    void BookFlight() //Booking Flight after registration. 
    {
        if (!isRegistered) {
            cout << "Please register before booking a flight.\n";
            return;
        }

        string pass;
        cout << "Enter Password to Book Flight: ";
        cin >> pass;
        if (!verifyPassword(pass)) {
            cout << "Incorrect password.\n";
            return;
        }

        view_Flight();    //To display the available flights.
        int n;
        cout << "Enter Flight number to book: ";
        cin >> n;

        // Reading the selected flight's details
        ifstream finf("Flights.txt");
        string forigin, fdestination, fdate, ftime;
        int flight_no = 1;
        bool found = false;

        while (finf >> forigin >> fdestination >> fdate >> ftime)
        {
            if (flight_no == n)
            {
                found = true;
                break;
            }
            flight_no++;
        }
        finf.close();

        if (found)
        {
            cout << "Flight booked successfully!\n";
            cout << "\n--- TICKET ---\n";
            cout << "Passenger Name: " << name << endl;
            cout << "Flight Number: " << n << endl;
            cout << "From: " << forigin << "  To: " << fdestination << endl;
            cout << "Date: " << fdate << "  Time: " << ftime << endl;
            cout << "Thank you for booking with us!\n";
        }
        else
        {
            cout << "Invalid flight number selected.\n";
        }
    }
};



class Admin //Admin class to manage flights 
{
private:
    string origin, destination;
    string date, time;

public:
    void add_Flight() //Add new Flights
    {
        const string adminPassword = "53616373";   //fixed password
        string inputPassword;
        cout << "Enter Admin Password: ";
        cin >> inputPassword;

        if (inputPassword != adminPassword) // Check password
        {
            cout << "Access Denied: Incorrect password.\n";
            return;
        }
        //FILE HANDLING
        ofstream foutf("Flights.txt", ios::app);  //Creating a file Flights.txt and opening it in append mode.
        int n;
        cout << "How many flights do you want to add? ";
        cin >> n;

        for (int i = 0; i < n; ++i)   //for loop to add n number of flights.
        {
            cout << "\nFlight " << i + 1 << ":\n";
            cout << "Origin: ";
            cin >> origin;
            cout << "Destination: ";
            cin >> destination;
            cout << "Date (DD-MM-YYYY): ";
            cin >> date;
            cout << "Time (HH.MM): ";
            cin >> time;
            foutf << origin << " " << destination << " " << date << " " << time << endl; //printing the data in file.
        }

        foutf.close(); //closing the file.
        cout << "Flights added successfully!\n";
    }

    friend void view_Flight(); 
};

void view_Flight() //view available flights
{
    int flight_no = 1; 
    string forigin, fdestination;
    string fdate, ftime;

    cout << "\nAvailable Flights:\n";
    ifstream finf("Flights.txt");         //Reading the data from Flights.txt

    while (finf >> forigin >> fdestination >> fdate >> ftime)
    {
        cout << flight_no++ << ". Origin: " << forigin
             << ", Destination: " << fdestination            //Print all available flights
             << ", Date: " << fdate
             << ", Time: " << ftime << "\n";
    }

    finf.close();
    if (flight_no == 1) {
        cout << "No flights available.\n";
    }
}

int main() {
    UI ui;          //object of UI class is created
    ui.show_main_menu();

    Passenger p;
    Admin admin;
    int choice;

    while (true) 
    {
        cout << "\nEnter choice (0 to exit): ";
        cin >> choice;

        switch (choice)   //Switch case to perform function based on choice
        {
            case 0:                            
                cout << "Exiting system. Thank you!\n";
                return 0;
            case 1:
                p.set_info();
                break;
            case 2:
                view_Flight();
                break;
            case 3:
                p.BookFlight();
                break;
            case 4:
                admin.add_Flight();
                break;
            default:
                cout << "Invalid choice. Try again.\n";
                break;
        }
    }

    return 0;
}