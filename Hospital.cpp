#include <iostream>
#include<fstream>
#include <iomanip>
#include<bits/stdc++.h>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <conio.h>
using namespace std;
int size=0;
int sizeDatabase=0;

class Node
{
public:
    string name,gender,bgroup;
    int age;
    int emergency;
    Node* next;

    Node(string name1,int age1,string gender1,string bgroup1,int emergency1)
    {
        name=name1;
        age=age1;
        gender=gender1;
        bgroup=bgroup1;
        emergency=emergency1;
        next=nullptr;
    }
};


class Database
{
public:
    Node* head;

    Database()
    {
        head=nullptr;
    }
    void insertAtEnd(string name1,int age1,string gender1,string bgroup1,int emergency1)
    {
        Node* newNode = new Node(name1,age1,gender1,bgroup1,emergency1);
        if (head == nullptr)
        {
            head = newNode;
        }
        else
        {
            Node* current = head;
            while (current->next != nullptr)
            {
                current = current->next;
            }
            current->next = newNode;
        }
        sizeDatabase++;
    }

    void displayDatabase()
    {
        Node* current = head;
        cout<<setw(25)<<"Name"<<setw(5)<<"Age"<<setw(8)<<"Gender"<<setw(13)<<"Blood Group"<<setw(18)<<"Emergency Status"<<endl;
        while (current != nullptr)
        {

            cout<<setw(25)<<current->name<<setw(5)<<current->age<<setw(8)<<current->gender<<setw(13)<<current->bgroup<<setw(18)<<current->emergency<<endl;


            current = current->next;
        }
    }

    string replaceSpacesWithUnderscores(const string& input)
    {
        string result = input;
        for (char& ch : result)
        {
            if (ch == ' ')
            {
                ch = '_';
            }
        }
        return result;
    }

    void replaceUnderscoresWithSpaces(string &str)
    {
        for (char &c : str)
        {
            if (c == '_')
            {
                c = ' ';
            }
        }
    }

    void exportDatabase()
    {
        ofstream outFile("database.txt");
        Node* current = head;
        outFile<<setw(25)<<"Name"<<setw(5)<<"Age"<<setw(8)<<"Gender"<<setw(13)<<"Blood Group"<<setw(18)<<"Emergency Status"<<endl;
        while (current != nullptr)
        {
            string modifiedName=replaceSpacesWithUnderscores(current->name);
            outFile<<setw(25)<<modifiedName<<setw(5)<<current->age<<setw(8)<<current->gender<<setw(13)<<current->bgroup<<setw(18)<<current->emergency<<endl;
            current = current->next;
        }
        outFile.close();
        cout << "Data exported to 'database.txt'." << endl;
    }


    void importDatabase()
    {
        ifstream inFile("database.txt");
        if (!inFile.is_open())
        {
            cout << "Error opening file 'database.txt'." << endl;
            return;
        }

        string header;
        getline(inFile, header);

        while (true)
        {
            string name, gender, bgroup;
            int age, emergency;

            if (!(inFile >> setw(25) >> name >> age >> setw(8) >> gender >> setw(13) >> bgroup >> emergency))
            {

                break;
            }

            replaceUnderscoresWithSpaces(name);
            insertAtEnd(name, age, gender, bgroup, emergency);
        }

        inFile.close();
        //cout << "Data imported from 'database.txt'." << endl;
    }

    Node* searchByName(string nameToSearch)
    {
        Node* current = head;

        while (current != nullptr)
        {
            if (current->name == nameToSearch)
            {

                return current;
            }

            current = current->next;
        }
        return nullptr;
    }


    Node* searchByBgroup(string BgroupToSearch)
    {
        Node* current = head;

        while (current != nullptr)
        {
            if (current->bgroup == BgroupToSearch)
            {

                return current;
            }

            current = current->next;
        }
        return nullptr;
    }


    void deleteByName(string nameToDelete)
    {
        Node* current = head;
        Node* prev = nullptr;


        if (current != nullptr && current->name == nameToDelete)
        {
            head = current->next;
            delete current;
            return;
        }


        while (current != nullptr && current->name != nameToDelete)
        {
            prev = current;
            current = current->next;
        }


        if (current == nullptr)
        {
            cout << "Patient with name '" << nameToDelete << "' not found." << endl;
            return;
        }


        prev->next = current->next;


        delete current;
        cout << "Data of Patient with name '" << nameToDelete << "' is Deleted" << endl;
    }

    void swapData(Node* node1, Node* node2)
    {
        string tempName = node1->name;
        int tempAge = node1->age;
        string tempGender = node1->gender;
        string tempBgroup = node1->bgroup;
        int tempEmergency = node1->emergency;

        node1->name = node2->name;
        node1->age = node2->age;
        node1->gender = node2->gender;
        node1->bgroup = node2->bgroup;
        node1->emergency = node2->emergency;

        node2->name = tempName;
        node2->age = tempAge;
        node2->gender = tempGender;
        node2->bgroup = tempBgroup;
        node2->emergency = tempEmergency;
    }


    void sortByName()
    {
        if (head == nullptr || head->next == nullptr)
        {

            return;
        }

        Node* current = head;

        while (current != nullptr)
        {
            Node* minNode = current;
            Node* temp = current->next;

            while (temp != nullptr)
            {

                if (temp->name < minNode->name)
                {
                    minNode = temp;
                }
                temp = temp->next;
            }


            swapData(current, minNode);
            current = current->next;
        }
    }


    void sortByAge()
    {
        Node* current = head;
        while (current != nullptr)
        {
            Node* minNode = current;
            Node* temp = current->next;

            while (temp != nullptr)
            {
                if (temp->age < minNode->age)
                {
                    minNode = temp;
                }
                temp = temp->next;
            }

            swapData(current, minNode);
            current = current->next;
        }
    }

    void sortByGender()
    {
        Node* current = head;
        while (current != nullptr)
        {
            Node* minNode = current;
            Node* nextNode = current->next;

            while (nextNode != nullptr)
            {
                if (nextNode->gender < minNode->gender)
                {
                    minNode = nextNode;
                }
                nextNode = nextNode->next;
            }

            if (minNode != current)
            {
                swapData(current, minNode);
            }
            current = current->next;
        }
    }

    void sortByBGroup()
    {
        Node* current = head;

        while (current != nullptr)
        {
            Node* minNode = current;
            Node* nextNode = current->next;

            while (nextNode != nullptr)
            {
                if (nextNode->bgroup < minNode->bgroup)
                {
                    minNode = nextNode;
                }
                nextNode = nextNode->next;
            }

            if (minNode != current)
            {
                swapData(current, minNode);
            }

            current = current->next;
        }
    }


};


class Queue
{
public:
    Node* front;
    Node* rear;
    Database database;
    Queue()
    {
        front=nullptr;
        rear=nullptr;
    }

    void enqueue(string name1,int age1,string gender1,string bgroup1,int emergency1)
    {
        Node* newNode = new Node(name1,age1,gender1,bgroup1,emergency1);
        //if (size==0)
        {
            front = rear = newNode;
        }
        //else
        {

            if(emergency1!=0)
            {
                newNode->next = front;
                front=newNode;
            }

            else
            {
                rear->next = newNode;
                rear = newNode;
            }
        }
        //size++;
    }

    void dequeue()
    {
//        if (size==0)
        {
            cout << "Serial is empty" << endl;
            return;
        }
        Node* temp = front;
        front = front->next;
        delete temp;
        //size--;
        cout << "Patient successfully removed without being added to database" << endl;
    }

    void dequeueAdd()
    {
        if (size==0)
        {
            cout << "Serial is empty" << endl;
            return;
        }
        Node* temp = front;
        front = front->next;
        database.insertAtEnd(temp->name,temp->age,temp->gender,temp->bgroup,temp->emergency);
        delete temp;
        size--;
        cout << "Patient successfully removed and added to database" << endl;
    }


    void peek()
    {
        if (size==0)
        {
            cout << "Serial is empty" << endl;

        }
        else
        {
            cout<<setw(25)<<"Name"<<setw(5)<<"Age"<<setw(8)<<"Gender"<<setw(13)<<"Blood Group"<<setw(18)<<"Emergency Status"<<endl;
            cout<<setw(25)<<front->name<<setw(5)<<front->age<<setw(8)<<front->gender<<setw(13)<<front->bgroup<<setw(18)<<front->emergency<<endl;
        }
    }


    void displayQueue()
    {
        if (size==0)
        {
            cout << "Serial is empty" << endl;

        }
        else
        {
            Node* current = front;
            cout<<setw(25)<<"Name"<<setw(5)<<"Age"<<setw(8)<<"Gender"<<setw(13)<<"Blood Group"<<setw(18)<<"Emergency Status"<<endl;
            while (current != nullptr)
            {
                cout<<setw(25)<<current->name<<setw(5)<<current->age<<setw(8)<<current->gender<<setw(13)<<current->bgroup<<setw(18)<<current->emergency<<endl;
                current = current->next;
            }
        }
    }

    void displayDatabase()
    {
        database.displayDatabase();
    }
    void exportDatabase()
    {
        database.exportDatabase();
    }

    void importDatabase()
    {
        database.importDatabase();
    }

    void searchByName(string nameToSearch)
    {


        Node* foundNode = database.searchByName(nameToSearch);

        if (foundNode != nullptr)
        {

            cout << "Patient found: " << endl;
            cout<<setw(25)<<"Name"<<setw(5)<<"Age"<<setw(8)<<"Gender"<<setw(13)<<"Blood Group"<<setw(18)<<"Emergency Status"<<endl;
            cout<<setw(25)<<foundNode->name<<setw(5)<<foundNode->age<<setw(8)<<foundNode->gender<<setw(13)<<foundNode->bgroup<<setw(18)<<foundNode->emergency<<endl;

        }
        else
        {

            cout << "Patient not found." << endl;
        }

    }

    void searchByBgroup(string BgroupToSearch)
    {

        Node* foundNode = database.searchByBgroup(BgroupToSearch);

        if (foundNode != nullptr)
        {

            cout << "Patients with searched Blood Group are: " << endl;
            cout<<setw(25)<<"Name"<<setw(5)<<"Age"<<setw(8)<<"Gender"<<setw(13)<<"Blood Group"<<setw(18)<<"Emergency Status"<<endl;
            cout<<setw(25)<<foundNode->name<<setw(5)<<foundNode->age<<setw(8)<<foundNode->gender<<setw(13)<<foundNode->bgroup<<setw(18)<<foundNode->emergency<<endl;

        }
        else
        {

            cout << "Patients with searched Blood Group not found." << endl;
        }

    }


    void deleteByName(string nameToDelete)
    {
        database.deleteByName(nameToDelete);

    }

    void sortByName()
    {

        database.sortByName();
    }

    void sortByAge()
    {

        database.sortByAge();
    }

    void sortByGender()
    {

        database.sortByGender();
    }
    void sortByBgroup()
    {

        database.sortByBGroup();

    }

};

void clearConsole()
{

    system("CLS");

}

void subMenu2()
{
    this_thread::sleep_for(chrono::seconds(1));
    clearConsole();
    cout << "===== Remove Patient from Serial =====" << endl;
    cout << "1. Remove and Add to Database" << endl;
    cout << "2. Remove without Adding to Database" << endl;
    cout << "3. Go Back to Main Menu" << endl;
    cout << "====================" << endl;
}

void subMenu5()
{
    this_thread::sleep_for(chrono::seconds(1));
    clearConsole();
    cout << "===== Database Options =====" << endl;
    cout << "1.  View Database" << endl;
    cout << "2.  Search Database by Name" << endl;
    cout << "3.  Search Database by Blood Group" << endl;
    cout << "4.  Delete Entry" << endl;
    cout << "5.  Sort by Name" << endl;
    cout << "6.  Sort by Age" << endl;
    cout << "7.  Sort by Gender" << endl;
    cout << "8.  Sort by Blood Group" << endl;
    cout << "9.  Export to text file" << endl;
    cout << "10. Go Back to Main Menu" << endl;
    cout << "====================" << endl;
}

void displayMenu()
{
    this_thread::sleep_for(chrono::seconds(1));
    clearConsole();
    cout << "===== Options =====" << endl;
    cout << "1. Add Patient to Serial" << endl;
    cout << "2. Remove Patient from Serial" << endl;
    cout << "3. View Current Patient" << endl;
    cout << "4. View Whole Serial list" << endl;
    cout << "5. Database Options" << endl;
    cout << "6. Exit system" << endl;
    cout << "====================" << endl;
}


int main()
{
    Queue queue;
    queue.importDatabase();
    int a;
    int choice = 0;
    int subChoice2 = 0;
    int subChoice5 = 0;
    string name1,gender1,bgroup1;
    int age1;
    int emergency1;
    string search1,search2,delete1;
    cout << " Welcome to Hospital Management System " <<endl;
    this_thread::sleep_for(chrono::seconds(1));
    clearConsole();
    do
    {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        cout << endl;
        switch (choice)
        {
        case 1:
            cout << "Enter patient name: "<<endl;
            getline(cin >> ws, name1);
            cout << endl;
            cout << "Enter patient Age, Gender and Blood Group"<<endl;
            cin >> age1 >> gender1 >>bgroup1 ;
            cout << endl;
            cout << "Is the patient in emergency condition ? (Press '1'for Yes and '0' for No)"<<endl;
            cin >> emergency1;
            cout << endl;
            queue.enqueue(name1,age1,gender1,bgroup1,emergency1);
            cout << "Patient successfully enqueued"  << endl;

            break;
        case 2:
            do
            {
                subMenu2();
                cout << "Enter your choice: ";
                cin >> subChoice2;
                cout << endl;
                switch (subChoice2)
                {
                case 1:
                    queue.dequeueAdd();
                    break;

                case 2:
                    queue.dequeue();
                    break;

                case 3:
                    cout << "Going back to the main menu." << endl;
                    break;

                default:
                    cout << "Invalid choice. Please enter a valid option." << endl;
                }
            }
            while (subChoice2 != 3);
            break;


        case 3:
            cout << "Current Patient is : " << endl;
            queue.peek();
            cout <<endl<<"Press any key to go back.......";
            a=_getch();
            if (a!=0)
            {
                break;
                a=0;
            }

        case 4:
            cout << "Current List is : " << endl;
            queue.displayQueue();
            cout <<endl<<"Press any key to go back.......";
            a=_getch();
            if (a!=0)
            {
                break;
                a=0;
            }

        case 5:
            do
            {
                subMenu5();
                cout << "Enter your choice: ";
                cin >> subChoice5;
                cout << endl;
                switch (subChoice5)
                {
                case 1:
                    cout << "Showing Database" << endl;
                    queue.displayDatabase();
                    cout <<endl<<"Press any key to go back.......";
                    a=_getch();
                    if (a!=0)
                    {
                        break;
                        a=0;
                    }

                case 2:
                    cout <<"Enter name to search"<<endl;
                    getline(cin >> ws, search1);
                    queue.searchByName(search1);
                    cout <<endl<<"Press any key to go back.......";
                    a=_getch();
                    if (a!=0)
                    {
                        break;
                        a=0;
                    }

                case 3:
                    cout <<"Enter Blood Group to search"<<endl;
                    cin >> search2 ;
                    queue.searchByBgroup(search2);
                    cout <<endl<<"Press any key to go back.......";
                    a=_getch();
                    if (a!=0)
                    {
                        break;
                        a=0;
                    }

                case 4:
                    cout <<"Enter name to delete"<<endl;
                    getline(cin >> ws, delete1);
                    queue.deleteByName(delete1);
                    break;

                case 5:
                    queue.sortByName();
                    cout << "Database successfully sorted by Name" << endl;
                    break;

                case 6:
                    queue.sortByAge();
                    cout << "Database successfully sorted by Age" << endl;
                    break;

                case 7:
                    queue.sortByGender();
                    cout << "Database successfully sorted by Gender" << endl;
                    break;

                case 8:
                    queue.sortByBgroup();
                    cout << "Database successfully sorted by Blood Group" << endl;
                    break;

                case 9:
                    queue.exportDatabase();
                    break;

                case 10:
                    cout << "Going back to the main menu." << endl;
                    break;

                default:
                    cout << "Invalid choice. Please enter a valid option." << endl;
                }
            }
            while (subChoice5 != 10);
            break;
        case 6:
            cout << "Exiting the program. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please enter a valid option." << endl;
        }
    }
    while (choice != 6);

    return 0;
}
