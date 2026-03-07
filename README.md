# 🏥 Hospital Management System (GUI) in C++

A **Windows GUI-based Hospital Management System** built in **C++** using the **Win32 API**.  
This version is designed as a **single standalone `.cpp` file** that can be compiled and run in **Code::Blocks on Windows**.

The system manages patient serial handling with emergency priority, treated-patient database operations, and file-based data persistence through `database.txt`.

---

## 🚀 Features

### 🖥️ Graphical User Interface
- Built with the **Win32 API**
- Single-window desktop interface
- Input fields for:
  - Patient Name
  - Age
  - Gender
  - Blood Group
  - Emergency status
- Buttons for all major operations
- Output panel to display serial and database records

---

## 🧾 Patient Serial Management

The patient serial is handled using a queue-like system.

### Add Patient to Serial
Enter:
- Name
- Age
- Gender
- Blood Group
- Emergency status

Behavior:
- **Emergency patients** are added to the **front**
- **Normal patients** are added to the **back**

### Remove Patient from Serial
Two options are available:
- **Remove Only**  
  Removes the patient from the serial without saving to the database
- **Remove + Add DB**  
  Removes the patient from the serial and adds them to the treated patient database

### View Current Patient
- Displays the patient currently at the front of the serial

### View Full Serial
- Shows the complete waiting list of patients

---

## 🗃️ Patient Database Management

The database stores patients removed from the serial and marked as treated.

### View Database
- Displays all stored patient records

### Search Operations
- **Search by Name**
- **Search by Blood Group**

### Delete Operation
- Delete a patient record by name

### Sort Operations
- Sort database by:
  - **Name**
  - **Age**
  - **Gender**
  - **Blood Group**

---

## ✅ Input Validation
The program validates patient input before adding to the serial.

Checks include:
- Empty fields are not allowed
- Age must be a valid positive number

---

## 💾 Data Persistence

### Import on Startup
- The program automatically loads records from `database.txt` when it starts

### Export on Demand
- The database can be exported manually using the **Export DB** button

### Auto Save on Exit
- The database is also saved automatically when the application closes

---

## 📄 Database File Format

Patient data is stored in `database.txt`.

Example format:

```txt
# Hospital Database
# name|age|gender|blood_group|emergency
John Doe|25|Male|A+|1
Jane Smith|30|Female|O-|0
````

---

## 🛠️ Technologies Used

* **C++**
* **Win32 API**
* Standard C++ containers:

  * `deque` for patient serial
  * `vector` for patient database

---

## ▶️ How to Run in Code::Blocks

1. Open **Code::Blocks**
2. Create a new C++ project or open the provided `.cpp` file
3. Paste the full GUI source code into a single `.cpp` file
4. Build and run on **Windows**

> This project is intended for **Windows only**, because it uses `windows.h` and Win32 GUI functions.

---

## 📌 Main Functional Buttons

* Add to Serial
* Remove + Add DB
* Remove Only
* Current Patient
* View Serial
* View Database
* Search Name
* Search Blood
* Delete Name
* Sort Name
* Sort Age
* Sort Gender
* Sort Blood
* Export DB
* Clear Fields

---

## 📷 Project Overview

This version upgrades the old console-based hospital system into a **desktop GUI application** while keeping the same core hospital workflow:

* patient serial handling
* emergency priority
* treated-patient database
* search/sort/delete operations
* import/export support

---

## 👨‍💻 Notes

* Designed as a **single-file standalone C++ program**
* Compatible with **Code::Blocks**
* Uses a simple and lightweight native Windows GUI
* Best suited for small academic or practice hospital management projects
```
