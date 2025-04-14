# 🏥 Hospital Management System in C++

A console-based Hospital Management System implemented in **C++**, designed to manage patient records, queue handling based on emergency status, and patient database operations. The system allows enqueuing patients into a waiting serial list, managing priority based on emergency conditions, and maintaining a patient database with features like sorting, searching, importing, and exporting.

---

## 🚀 Features

### 🧾 Patient Serial Management (Queue)
- **Add Patient to Serial**  
  Input patient name, age, gender, blood group, and emergency status. Patients in emergency are added with higher priority (at the front of the queue).

- **Remove Patient from Serial**
  - Remove **and** add the patient to the main database.
  - Remove **without** adding to the database.

- **View Current Patient**  
  Peek at the patient currently at the front of the queue.

- **Display Full Serial**  
  Shows the entire waiting list of patients.

---

### 🗃️ Patient Database Management (Linked List)
- **View Patient Database**  
  Displays all patients that were treated and added to the database.

- **Search Operations**
  - **By Name**
  - **By Blood Group**

- **Delete Patient Record**
  - Removes a patient from the database by name.

- **Sort Operations**
  - Sort by **Name**
  - Sort by **Age**
  - Sort by **Gender**
  - Sort by **Blood Group**

- **Data Persistence**
  - Export database to a `database.txt` file.
  - Import database from `database.txt` file (auto-loaded on startup).

---

