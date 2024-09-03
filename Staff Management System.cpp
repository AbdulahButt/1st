#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cctype>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

class Person
{
public:
    string name;
    int id;
    string phone;
    string address;

    Person(string name, int id, string phone, string address)
        : name(name), id(id), phone(phone), address(address) {}
};

class Staff : public Person
{
public:
    Staff(string name, int id, string phone, string address)
        : Person(name, id, phone, address) {}
};

class Visitor : public Person
{
public:
    Visitor(string name, int id, string phone, string address)
        : Person(name, id, phone, address) {}
};

class Attendance
{
public:
    int id;
    string name;
    string time;
    bool isPresent;

    Attendance(int id, string name, string time, bool isPresent)
        : id(id), name(name), time(time), isPresent(isPresent) {}
};

class ManagementSystem
{
private:
    vector<Staff> staffList;
    vector<Visitor> visitorList;
    vector<Attendance> attendanceList;

    int getIdInput()
    {
        int id;
        while (true)
        {
            cout << "Enter 4 digit ID : ";
            cin >> id;
            if (cin.fail() || id < 1000 || id > 9999)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input.\n";
            }
            else
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return id;
            }
        }
    }

    string getStringInput(const string &prompt, bool allowOnlyAlpha = false)
    {
        string input;
        while (true)
        {
            cout << prompt;
            getline(cin, input);
            bool isValid = true;
            if (allowOnlyAlpha)
            {
                for (char c : input)
                {
                    if (!isalpha(c) && !isspace(c))
                    {
                        isValid = false;
                        break;
                    }
                }
            }
            if (isValid)
            {
                return input;
            }
            else
            {
                cout << "Invalid input. Please enter only alphabetic characters.\n";
            }
        }
    }

    string getPhoneInput()
    {
        string phone;
        while (true)
        {
            cout << "Enter 11 digits number: ";
            getline(cin, phone);
            bool isValid = true;
            if (phone.length() != 11)
            {
                isValid = false;
            }
            else
            {
                for (char c : phone)
                {
                    if (!isdigit(c))
                    {
                        isValid = false;
                        break;
                    }
                }
            }
            if (isValid)
            {
                return phone;
            }
            else
            {
                cout << "Invalid input. Please enter exactly 11 digits.\n";
            }
        }
    }

    bool findStaffById(int id, Staff &staff)
    {
        for (const auto &s : staffList)
        {
            if (s.id == id)
            {
                staff = s;
                return true;
            }
        }
        return false;
    }

    bool findVisitorById(int id, Visitor &visitor)
    {
        for (const auto &v : visitorList)
        {
            if (v.id == id)
            {
                visitor = v;
                return true;
            }
        }
        return false;
    }

    void saveStaffToFile()
    {
        ofstream outFile("staff.txt");
        for (const auto &staff : staffList)
        {
            outFile << staff.name << ',' << staff.id << ',' << staff.phone << ',' << staff.address << endl;
        }
        outFile.close();
    }

    void loadStaffFromFile()
    {
        ifstream inFile("staff.txt");
        if (inFile.is_open())
        {
            string line;
            while (getline(inFile, line))
            {
                stringstream ss(line);
                string name, idStr, phone, address;
                getline(ss, name, ',');
                getline(ss, idStr, ',');
                getline(ss, phone, ',');
                getline(ss, address, ',');
                int id = stoi(idStr);
                staffList.push_back(Staff(name, id, phone, address));
            }
            inFile.close();
        }
    }

    void saveVisitorsToFile()
    {
        ofstream outFile("visitors.txt");
        for (const auto &visitor : visitorList)
        {
            outFile << visitor.name << ',' << visitor.id << ',' << visitor.phone << ',' << visitor.address << endl;
        }
        outFile.close();
    }

    void loadVisitorsFromFile()
    {
        ifstream inFile("visitors.txt");
        if (inFile.is_open())
        {
            string line;
            while (getline(inFile, line))
            {
                stringstream ss(line);
                string name, idStr, phone, address;
                getline(ss, name, ',');
                getline(ss, idStr, ',');
                getline(ss, phone, ',');
                getline(ss, address, ',');
                int id = stoi(idStr);
                visitorList.push_back(Visitor(name, id, phone, address));
            }
            inFile.close();
        }
    }

    void saveAttendanceToFile()
    {
        ofstream outFile("attendance.txt");
        for (const auto &record : attendanceList)
        {
            outFile << record.id << ',' << record.name << ',' << record.time << ',' << (record.isPresent ? "Present" : "Absent") << endl;
        }
        outFile.close();
    }

    void loadAttendanceFromFile()
    {
        ifstream inFile("attendance.txt");
        if (inFile.is_open())
        {
            string line;
            while (getline(inFile, line))
            {
                stringstream ss(line);
                string idStr, name, time, status;
                getline(ss, idStr, ',');
                getline(ss, name, ',');
                getline(ss, time, ',');
                getline(ss, status, ',');
                int id = stoi(idStr);
                bool isPresent = (status == "Present");
                attendanceList.push_back(Attendance(id, name, time, isPresent));
            }
            inFile.close();
        }
    }

    string getCurrentTime()
    {
        time_t now = time(0);
        char *dt = ctime(&now);
        string timeStr(dt);
        timeStr.pop_back(); // Remove the newline character
        return timeStr;
    }

public:
    ManagementSystem()
    {
        loadStaffFromFile();
        loadVisitorsFromFile();
        loadAttendanceFromFile();
    }

    ~ManagementSystem()
    {
        saveStaffToFile();
        saveVisitorsToFile();
        saveAttendanceToFile();
    }

    void addStaff()
    {
        cin.ignore();
        string name = getStringInput("Enter staff name: ", true);
        int id = getIdInput();
        string phone = getPhoneInput();
        string address = getStringInput("Enter staff address: ");
        staffList.push_back(Staff(name, id, phone, address));
        cout << "Staff added successfully!\n";
    }

    void viewStaff()
    {
        cout << "\nStaff List:\n";
        cout << setw(20) << left << "Name"
             << setw(10) << left << "ID"
             << setw(15) << left << "Phone"
             << setw(30) << left << "Address" << endl;
        cout << string(75, '-') << endl;
        for (const auto &staff : staffList)
        {
            cout << setw(20) << left << staff.name
                 << setw(10) << left << staff.id
                 << setw(15) << left << staff.phone
                 << setw(30) << left << staff.address << endl;
        }
    }

    void removeStaff()
    {
        int id = getIdInput();
        auto it = find_if(staffList.begin(), staffList.end(), [id](const Staff &s)
                          { return s.id == id; });
        if (it != staffList.end())
        {
            staffList.erase(it);
            cout << "Staff removed successfully!\n";
        }
        else
        {
            cout << "Staff with ID " << id << " not found.\n";
        }
    }

    void searchStaffById()
    {
        int id = getIdInput();
        Staff staff("", 0, "", "");
        if (findStaffById(id, staff))
        {
            cout << "Name: " << staff.name << ", ID: " << staff.id
                 << ", Phone: " << staff.phone << ", Address: " << staff.address << endl;
        }
        else
        {
            cout << "Staff with ID " << id << " not found.\n";
        }
    }

    void addVisitor()
    {
        cin.ignore();
        string name = getStringInput("Enter visitor name: ", true);
        int id = getIdInput();
        string phone = getPhoneInput();
        string address = getStringInput("Enter visitor address: ");
        visitorList.push_back(Visitor(name, id, phone, address));
        cout << "Visitor added successfully!\n";
    }

    void viewVisitors()
    {
        cout << "\nVisitor List:\n";
        cout << setw(20) << left << "Name"
             << setw(10) << left << "ID"
             << setw(15) << left << "Phone"
             << setw(30) << left << "Address" << endl;
        cout << string(75, '-') << endl;
        for (const auto &visitor : visitorList)
        {
            cout << setw(20) << left << visitor.name
                 << setw(10) << left << visitor.id
                 << setw(15) << left << visitor.phone
                 << setw(30) << left << visitor.address << endl;
        }
    }

    void markAttendance()
    {
        cout << "\nMark Attendance:\n";
        for (auto &staff : staffList)
        {
            cout << "Mark attendance for " << staff.name << " (ID: " << staff.id << "): ";
            string input;
            cin >> input;
            bool isPresent = (input == "P" || input == "p");
            string time = getCurrentTime();
            attendanceList.push_back(Attendance(staff.id, staff.name, time, isPresent));
        }
        cout << "Attendance marked successfully!\n";
    }

    void viewAttendanceStatus()
    {
        cout << "\nAttendance Status:\n";
        for (const auto &staff : staffList)
        {
            auto it = find_if(attendanceList.begin(), attendanceList.end(), [&staff](const Attendance &a)
                              { return a.id == staff.id; });
            if (it != attendanceList.end())
            {
                cout << "Name: " << staff.name << "\n ID: " << staff.id << "\n Status: "
                     << (it->isPresent ? "Present" : "Absent") << "\n Time: " << it->time << endl;
            }
            else
            {
                cout << "Name: " << staff.name << " \n ID: " << staff.id << "\n Status: Absent" << endl;
            }
        }
    }
};

int main()
{
    ManagementSystem system;
    int choice;

    do
    {
        cout << "\nManagement System Menu\n";
        cout << "1. Add Staff\n";
        cout << "2. View Staff\n";
        cout << "3. Remove Staff\n";
        cout << "4. Search Staff by ID\n";
        cout << "5. Add Visitor\n";
        cout << "6. View Visitors\n";
        cout << "7. Mark Attendance\n";
        cout << "8. View Attendance Status\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            system.addStaff();
            break;
        case 2:
            system.viewStaff();
            break;
        case 3:
            system.removeStaff();
            break;
        case 4:
            system.searchStaffById();
            break;
        case 5:
            system.addVisitor();
            break;
        case 6:
            system.viewVisitors();
            break;
        case 7:
            system.markAttendance();
            break;
        case 8:
            system.viewAttendanceStatus();
            break;
        case 9:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 9);

    return 0;
}
