#include <windows.h>
#include <string>
#include <vector>
#include <deque>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>

// Code::Blocks-friendly notes:
// 1) Avoid direct GetStockObject() usage so no extra gdi32 linker setup is needed.
// 2) Cast control IDs through INT_PTR to avoid 64-bit warnings.

using namespace std;

struct Patient
{
    string name;
    int age;
    string gender;
    string bgroup;
    bool emergency;
};

string trim(const string& s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

string toLowerStr(string s)
{
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

bool parseInt(const string& s, int& value)
{
    istringstream iss(s);
    iss >> value;
    return !iss.fail() && iss.eof();
}

vector<string> split(const string& s, char delim)
{
    vector<string> parts;
    string item;
    istringstream iss(s);
    while (getline(iss, item, delim))
    {
        parts.push_back(item);
    }
    return parts;
}

string safeField(string s)
{
    replace(s.begin(), s.end(), '|', '/');
    return s;
}

string tableHeader()
{
    ostringstream oss;
    oss << left
        << setw(25) << "Name"
        << setw(8)  << "Age"
        << setw(12) << "Gender"
        << setw(14) << "Blood Group"
        << setw(12) << "Emergency" << "\r\n";
    oss << string(71, '-') << "\r\n";
    return oss.str();
}

string patientRow(const Patient& p)
{
    ostringstream oss;
    oss << left
        << setw(25) << p.name
        << setw(8)  << p.age
        << setw(12) << p.gender
        << setw(14) << p.bgroup
        << setw(12) << (p.emergency ? "Yes" : "No") << "\r\n";
    return oss.str();
}

string patientsToString(const vector<Patient>& items, const string& title)
{
    ostringstream oss;
    oss << title << "\r\n\r\n";
    if (items.empty())
    {
        oss << "No records found.\r\n";
        return oss.str();
    }

    oss << tableHeader();
    for (size_t i = 0; i < items.size(); ++i)
    {
        oss << patientRow(items[i]);
    }
    return oss.str();
}

string queueToString(const deque<Patient>& items, const string& title)
{
    ostringstream oss;
    oss << title << "\r\n\r\n";
    if (items.empty())
    {
        oss << "Serial is empty.\r\n";
        return oss.str();
    }

    oss << tableHeader();
    for (size_t i = 0; i < items.size(); ++i)
    {
        oss << patientRow(items[i]);
    }
    return oss.str();
}

class Database
{
private:
    vector<Patient> records;

public:
    void add(const Patient& p)
    {
        records.push_back(p);
    }

    int size() const
    {
        return static_cast<int>(records.size());
    }

    const vector<Patient>& all() const
    {
        return records;
    }

    vector<Patient> searchByName(const string& name) const
    {
        vector<Patient> result;
        string target = toLowerStr(trim(name));
        for (size_t i = 0; i < records.size(); ++i)
        {
            if (toLowerStr(records[i].name) == target)
            {
                result.push_back(records[i]);
            }
        }
        return result;
    }

    vector<Patient> searchByBloodGroup(const string& bg) const
    {
        vector<Patient> result;
        string target = toLowerStr(trim(bg));
        for (size_t i = 0; i < records.size(); ++i)
        {
            if (toLowerStr(records[i].bgroup) == target)
            {
                result.push_back(records[i]);
            }
        }
        return result;
    }

    bool deleteByName(const string& name)
    {
        string target = toLowerStr(trim(name));
        for (vector<Patient>::iterator it = records.begin(); it != records.end(); ++it)
        {
            if (toLowerStr(it->name) == target)
            {
                records.erase(it);
                return true;
            }
        }
        return false;
    }

    void sortByName()
    {
        stable_sort(records.begin(), records.end(),
            [](const Patient& a, const Patient& b)
            {
                return toLowerStr(a.name) < toLowerStr(b.name);
            });
    }

    void sortByAge()
    {
        stable_sort(records.begin(), records.end(),
            [](const Patient& a, const Patient& b)
            {
                return a.age < b.age;
            });
    }

    void sortByGender()
    {
        stable_sort(records.begin(), records.end(),
            [](const Patient& a, const Patient& b)
            {
                return toLowerStr(a.gender) < toLowerStr(b.gender);
            });
    }

    void sortByBloodGroup()
    {
        stable_sort(records.begin(), records.end(),
            [](const Patient& a, const Patient& b)
            {
                return toLowerStr(a.bgroup) < toLowerStr(b.bgroup);
            });
    }

    bool exportToFile(const string& fileName) const
    {
        ofstream out(fileName.c_str());
        if (!out.is_open()) return false;

        out << "# Hospital Database\r\n";
        out << "# name|age|gender|blood_group|emergency\r\n";

        for (size_t i = 0; i < records.size(); ++i)
        {
            out << safeField(records[i].name) << "|"
                << records[i].age << "|"
                << safeField(records[i].gender) << "|"
                << safeField(records[i].bgroup) << "|"
                << (records[i].emergency ? 1 : 0) << "\r\n";
        }

        return true;
    }

    void importFromFile(const string& fileName)
    {
        records.clear();

        ifstream in(fileName.c_str());
        if (!in.is_open()) return;

        string line;
        while (getline(in, line))
        {
            line = trim(line);
            if (line.empty()) continue;
            if (line[0] == '#') continue;

            if (line.find('|') != string::npos)
            {
                vector<string> parts = split(line, '|');
                if (parts.size() == 5)
                {
                    Patient p;
                    int age = 0;
                    int emergency = 0;
                    if (parseInt(trim(parts[1]), age) && parseInt(trim(parts[4]), emergency))
                    {
                        p.name = trim(parts[0]);
                        p.age = age;
                        p.gender = trim(parts[2]);
                        p.bgroup = trim(parts[3]);
                        p.emergency = (emergency != 0);
                        records.push_back(p);
                    }
                }
            }
            else
            {
                if (line.find("Name") != string::npos && line.find("Age") != string::npos) continue;

                istringstream iss(line);
                Patient p;
                int emergency = 0;

                if (iss >> p.name >> p.age >> p.gender >> p.bgroup >> emergency)
                {
                    replace(p.name.begin(), p.name.end(), '_', ' ');
                    p.emergency = (emergency != 0);
                    records.push_back(p);
                }
            }
        }
    }
};

class QueueManager
{
private:
    deque<Patient> serial;
    Database database;

public:
    Database& db()
    {
        return database;
    }

    const deque<Patient>& getSerial() const
    {
        return serial;
    }

    int serialCount() const
    {
        return static_cast<int>(serial.size());
    }

    void enqueue(const Patient& p)
    {
        if (p.emergency) serial.push_front(p);
        else serial.push_back(p);
    }

    bool currentPatient(Patient& p) const
    {
        if (serial.empty()) return false;
        p = serial.front();
        return true;
    }

    bool removeOnly(Patient& p)
    {
        if (serial.empty()) return false;
        p = serial.front();
        serial.pop_front();
        return true;
    }

    bool removeAndAdd(Patient& p)
    {
        if (!removeOnly(p)) return false;
        database.add(p);
        return true;
    }

    void importDatabase()
    {
        database.importFromFile("database.txt");
    }

    bool exportDatabase() const
    {
        return database.exportToFile("database.txt");
    }
};

QueueManager g_manager;
HWND g_hMain = NULL;
HWND g_hName = NULL;
HWND g_hAge = NULL;
HWND g_hGender = NULL;
HWND g_hBlood = NULL;
HWND g_hEmergency = NULL;
HWND g_hOutput = NULL;

enum
{
    ID_EDIT_NAME = 101,
    ID_EDIT_AGE,
    ID_EDIT_GENDER,
    ID_EDIT_BLOOD,
    ID_CHECK_EMERGENCY,
    ID_BTN_ADD,
    ID_BTN_REMOVE_ADD,
    ID_BTN_REMOVE_ONLY,
    ID_BTN_CURRENT,
    ID_BTN_VIEW_SERIAL,
    ID_BTN_VIEW_DB,
    ID_BTN_SEARCH_NAME,
    ID_BTN_SEARCH_BLOOD,
    ID_BTN_DELETE,
    ID_BTN_SORT_NAME,
    ID_BTN_SORT_AGE,
    ID_BTN_SORT_GENDER,
    ID_BTN_SORT_BLOOD,
    ID_BTN_EXPORT,
    ID_BTN_CLEAR
};

void setCtrlFont(HWND)
{
    // No-op to avoid requiring extra GDI linker settings in Code::Blocks.
}

HWND addLabel(HWND parent, const char* text, int x, int y, int w, int h)
{
    HWND hWnd = CreateWindowA("STATIC", text, WS_CHILD | WS_VISIBLE,
                              x, y, w, h, parent, NULL, GetModuleHandle(NULL), NULL);
    setCtrlFont(hWnd);
    return hWnd;
}

HWND addEdit(HWND parent, int id, int x, int y, int w, int h)
{
    HWND hWnd = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "",
                                WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
                                x, y, w, h, parent, (HMENU)(INT_PTR)id, GetModuleHandle(NULL), NULL);
    setCtrlFont(hWnd);
    return hWnd;
}

HWND addButton(HWND parent, const char* text, int id, int x, int y, int w, int h)
{
    HWND hWnd = CreateWindowA("BUTTON", text,
                              WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                              x, y, w, h, parent, (HMENU)(INT_PTR)id, GetModuleHandle(NULL), NULL);
    setCtrlFont(hWnd);
    return hWnd;
}

string getTextFrom(HWND hEdit)
{
    int len = GetWindowTextLengthA(hEdit);
    vector<char> buf(len + 1, 0);
    GetWindowTextA(hEdit, &buf[0], len + 1);
    return string(&buf[0]);
}

void setOutput(const string& text)
{
    SetWindowTextA(g_hOutput, text.c_str());
}

void clearInputs()
{
    SetWindowTextA(g_hName, "");
    SetWindowTextA(g_hAge, "");
    SetWindowTextA(g_hGender, "");
    SetWindowTextA(g_hBlood, "");
    SendMessage(g_hEmergency, BM_SETCHECK, BST_UNCHECKED, 0);
    SetFocus(g_hName);
}

void updateTitle()
{
    ostringstream oss;
    oss << "Hospital Management System  |  Serial: "
        << g_manager.serialCount()
        << "  |  Database: "
        << g_manager.db().size();
    SetWindowTextA(g_hMain, oss.str().c_str());
}

bool readPatientFromInputs(Patient& p)
{
    string name = trim(getTextFrom(g_hName));
    string ageText = trim(getTextFrom(g_hAge));
    string gender = trim(getTextFrom(g_hGender));
    string blood = trim(getTextFrom(g_hBlood));

    if (name.empty() || ageText.empty() || gender.empty() || blood.empty())
    {
        MessageBoxA(g_hMain, "Please fill in Name, Age, Gender and Blood Group.",
                    "Input Error", MB_ICONWARNING);
        return false;
    }

    int age = 0;
    if (!parseInt(ageText, age) || age <= 0)
    {
        MessageBoxA(g_hMain, "Please enter a valid positive number for Age.",
                    "Input Error", MB_ICONWARNING);
        return false;
    }

    p.name = name;
    p.age = age;
    p.gender = gender;
    p.bgroup = blood;
    p.emergency = (SendMessage(g_hEmergency, BM_GETCHECK, 0, 0) == BST_CHECKED);
    return true;
}

void showSinglePatient(const Patient& p, const string& title)
{
    vector<Patient> temp;
    temp.push_back(p);
    setOutput(patientsToString(temp, title));
}

void handleAddPatient()
{
    Patient p;
    if (!readPatientFromInputs(p)) return;

    g_manager.enqueue(p);
    updateTitle();
    setOutput(queueToString(g_manager.getSerial(), "Patient added to serial successfully."));
    clearInputs();
}

void handleRemoveOnly()
{
    Patient p;
    if (!g_manager.removeOnly(p))
    {
        MessageBoxA(g_hMain, "Serial is empty.", "Info", MB_OK | MB_ICONINFORMATION);
        return;
    }

    updateTitle();
    showSinglePatient(p, "Patient removed from serial (not added to database).");
}

void handleRemoveAndAdd()
{
    Patient p;
    if (!g_manager.removeAndAdd(p))
    {
        MessageBoxA(g_hMain, "Serial is empty.", "Info", MB_OK | MB_ICONINFORMATION);
        return;
    }

    updateTitle();
    showSinglePatient(p, "Patient removed from serial and added to database.");
}

void handleCurrentPatient()
{
    Patient p;
    if (!g_manager.currentPatient(p))
    {
        MessageBoxA(g_hMain, "Serial is empty.", "Info", MB_OK | MB_ICONINFORMATION);
        return;
    }

    showSinglePatient(p, "Current patient in serial");
}

void handleViewSerial()
{
    setOutput(queueToString(g_manager.getSerial(), "Current serial list"));
}

void handleViewDatabase()
{
    setOutput(patientsToString(g_manager.db().all(), "Database records"));
}

void handleSearchByName()
{
    string name = trim(getTextFrom(g_hName));
    if (name.empty())
    {
        MessageBoxA(g_hMain, "Enter a patient name in the Name field first.",
                    "Input Needed", MB_ICONWARNING);
        return;
    }

    vector<Patient> result = g_manager.db().searchByName(name);
    setOutput(patientsToString(result, "Search result by name"));
}

void handleSearchByBlood()
{
    string blood = trim(getTextFrom(g_hBlood));
    if (blood.empty())
    {
        MessageBoxA(g_hMain, "Enter a blood group in the Blood Group field first.",
                    "Input Needed", MB_ICONWARNING);
        return;
    }

    vector<Patient> result = g_manager.db().searchByBloodGroup(blood);
    setOutput(patientsToString(result, "Search result by blood group"));
}

void handleDeleteByName()
{
    string name = trim(getTextFrom(g_hName));
    if (name.empty())
    {
        MessageBoxA(g_hMain, "Enter a patient name in the Name field first.",
                    "Input Needed", MB_ICONWARNING);
        return;
    }

    bool deleted = g_manager.db().deleteByName(name);
    updateTitle();

    if (deleted)
    {
        setOutput("Record deleted successfully.\r\n");
    }
    else
    {
        setOutput("Patient not found in database.\r\n");
    }
}

void handleSortName()
{
    g_manager.db().sortByName();
    setOutput(patientsToString(g_manager.db().all(), "Database sorted by name"));
}

void handleSortAge()
{
    g_manager.db().sortByAge();
    setOutput(patientsToString(g_manager.db().all(), "Database sorted by age"));
}

void handleSortGender()
{
    g_manager.db().sortByGender();
    setOutput(patientsToString(g_manager.db().all(), "Database sorted by gender"));
}

void handleSortBlood()
{
    g_manager.db().sortByBloodGroup();
    setOutput(patientsToString(g_manager.db().all(), "Database sorted by blood group"));
}

void handleExport()
{
    if (g_manager.exportDatabase())
    {
        MessageBoxA(g_hMain, "Database exported to database.txt",
                    "Export Successful", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBoxA(g_hMain, "Could not write database.txt",
                    "Export Error", MB_OK | MB_ICONERROR);
    }
}

void createControls(HWND hwnd)
{
    addLabel(hwnd, "Patient Information", 20, 15, 200, 20);

    addLabel(hwnd, "Name", 20, 50, 80, 20);
    g_hName = addEdit(hwnd, ID_EDIT_NAME, 110, 47, 190, 24);

    addLabel(hwnd, "Age", 20, 85, 80, 20);
    g_hAge = addEdit(hwnd, ID_EDIT_AGE, 110, 82, 80, 24);

    addLabel(hwnd, "Gender", 20, 120, 80, 20);
    g_hGender = addEdit(hwnd, ID_EDIT_GENDER, 110, 117, 120, 24);

    addLabel(hwnd, "Blood Group", 20, 155, 80, 20);
    g_hBlood = addEdit(hwnd, ID_EDIT_BLOOD, 110, 152, 120, 24);

    g_hEmergency = CreateWindowA("BUTTON", "Emergency Patient",
                                 WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                                 20, 188, 150, 24, hwnd, (HMENU)(INT_PTR)ID_CHECK_EMERGENCY,
                                 GetModuleHandle(NULL), NULL);
    setCtrlFont(g_hEmergency);

    addButton(hwnd, "Add to Serial", ID_BTN_ADD, 20, 225, 135, 32);
    addButton(hwnd, "Remove + Add DB", ID_BTN_REMOVE_ADD, 165, 225, 135, 32);

    addButton(hwnd, "Remove Only", ID_BTN_REMOVE_ONLY, 20, 267, 135, 32);
    addButton(hwnd, "Current Patient", ID_BTN_CURRENT, 165, 267, 135, 32);

    addButton(hwnd, "View Serial", ID_BTN_VIEW_SERIAL, 20, 309, 135, 32);
    addButton(hwnd, "View Database", ID_BTN_VIEW_DB, 165, 309, 135, 32);

    addButton(hwnd, "Search Name", ID_BTN_SEARCH_NAME, 20, 351, 135, 32);
    addButton(hwnd, "Search Blood", ID_BTN_SEARCH_BLOOD, 165, 351, 135, 32);

    addButton(hwnd, "Delete Name", ID_BTN_DELETE, 20, 393, 135, 32);
    addButton(hwnd, "Export DB", ID_BTN_EXPORT, 165, 393, 135, 32);

    addButton(hwnd, "Sort Name", ID_BTN_SORT_NAME, 20, 435, 135, 32);
    addButton(hwnd, "Sort Age", ID_BTN_SORT_AGE, 165, 435, 135, 32);

    addButton(hwnd, "Sort Gender", ID_BTN_SORT_GENDER, 20, 477, 135, 32);
    addButton(hwnd, "Sort Blood", ID_BTN_SORT_BLOOD, 165, 477, 135, 32);

    addButton(hwnd, "Clear Fields", ID_BTN_CLEAR, 20, 519, 280, 32);

    addLabel(hwnd, "Output", 330, 15, 100, 20);
    g_hOutput = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL |
        ES_AUTOHSCROLL | ES_READONLY | WS_VSCROLL | WS_HSCROLL,
        330, 40, 620, 520,
        hwnd,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );
    setCtrlFont(g_hOutput);

    addLabel(hwnd,
             "Use Name and Blood Group fields for search/delete operations.",
             330, 568, 500, 20);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        g_hMain = hwnd;
        createControls(hwnd);
        g_manager.importDatabase();
        updateTitle();

        ostringstream oss;
        oss << "Welcome to Hospital Management System\r\n\r\n"
            << "Loaded " << g_manager.db().size()
            << " record(s) from database.txt (if the file exists).\r\n\r\n"
            << "Fill in patient details on the left and use the buttons to manage serial and database.";
        setOutput(oss.str());
        break;
    }

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_BTN_ADD:           handleAddPatient(); break;
        case ID_BTN_REMOVE_ADD:    handleRemoveAndAdd(); break;
        case ID_BTN_REMOVE_ONLY:   handleRemoveOnly(); break;
        case ID_BTN_CURRENT:       handleCurrentPatient(); break;
        case ID_BTN_VIEW_SERIAL:   handleViewSerial(); break;
        case ID_BTN_VIEW_DB:       handleViewDatabase(); break;
        case ID_BTN_SEARCH_NAME:   handleSearchByName(); break;
        case ID_BTN_SEARCH_BLOOD:  handleSearchByBlood(); break;
        case ID_BTN_DELETE:        handleDeleteByName(); break;
        case ID_BTN_SORT_NAME:     handleSortName(); break;
        case ID_BTN_SORT_AGE:      handleSortAge(); break;
        case ID_BTN_SORT_GENDER:   handleSortGender(); break;
        case ID_BTN_SORT_BLOOD:    handleSortBlood(); break;
        case ID_BTN_EXPORT:        handleExport(); break;
        case ID_BTN_CLEAR:         clearInputs(); break;
        }
        break;
    }

    case WM_DESTROY:
        g_manager.exportDatabase(); // Auto-save on exit
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

int runApp(HINSTANCE hInstance, int nCmdShow)
{
    const char CLASS_NAME[] = "HospitalGUIWindow";

    WNDCLASSA wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowA(
        CLASS_NAME,
        "Hospital Management System",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 990, 650,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Works in a Windows GUI project:
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    return runApp(hInstance, nCmdShow);
}

// Also works if you paste it into a normal Code::Blocks console project:
int main()
{
    return runApp(GetModuleHandle(NULL), SW_SHOWDEFAULT);
}
