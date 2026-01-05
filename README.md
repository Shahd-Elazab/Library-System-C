# Library Management System (C Project)

This project is a simple **library management system** written in C. It stores data in CSV files and supports operations to manage books, members, and borrowing activities.

---

## Features

- Add books and members (prevents duplicates).
- Record borrowing transactions and track available copies.
- Query library data using commands like:
  - Number_Books, Number_Members
  - Books_Available, Most_Borrowed, Books_Unborrowed
  - List_Book_Borrowers, List_Member_Books
  - Members_Less, Books_Borrowed_Days, Overlapping_Borrowers
  - Books_Per_Member, Book_ID_Min

---

## Operations Explained:
1. Number_Books
What it does:
Counts and displays the total number of books stored in Books.csv.

2. Number_Members
What it does:
Counts and displays the total number of members stored in members.csv.

3. Book_ID_Min
What it does:
Finds and prints the smallest (minimum) Book ID from Books.csv.

4. Books_Available
What it does:
Lists all books that still have available copies to borrow (copies > borrow count).

5. Most_Borrowed
What it does:
Displays the book(s) that have been borrowed the most times (highest borrow count).

6. Books_Unborrowed
What it does:
Lists all books that have never been borrowed (borrow count = 0).
Prints none if every book has been borrowed.

7. Members_Less
What it does:
After entering this command, the program asks you for a number N.
It then lists all members who borrowed less than N books.

8. Books_Per_Member
What it does:
Lists each member alongside how many books they have borrowed in total.

9. List_Book_Borrowers
What it does:
After entering this command, the program asks for a Book ID.
It then lists all members who borrowed that specific book.

10. List_Member_Books
What it does:
After entering this command, the program asks for a Member ID.
It then lists all books that this member has borrowed.

11. Books_Borrowed_Days
What it does:
Counts how many unique borrowing dates exist in loans.csv (e.g., if two books were borrowed on the same day, it counts that date once).

12. Overlapping_Borrowers
What it does:
After entering this command, the program asks for a Book ID.
It then finds and prints pairs of members who borrowed that book on the same date (overlapping borrowing).

13. Quit
What it does:
Ends the program with a “Thanks!” message.


---

## Files in this Repository

- main.c → The source code for the program.  
- Books.csv → Stores books (Book ID, Copies, Borrow Count).  
- members.csv → Stores members (Member ID, Borrow Count).  
- loans.csv → Stores borrowing records (Book ID, Member ID, Date).  
- README.md → Project overview, usage guide, and example test case.  

---

## How to Compile and Run

1.Compile the program:
   ```bash
   gcc main.c -o library

2.Run the program:
./library

3.Input data sections in this order:
Books:
101 3
102 2
Members:
1001
1002
Borrowed_Books:
101 1001 01/07/2025
102 1002 02/07/2025
Done

4.Run operations:
Number_Books
Books_Available
Members_Less
Quit

---

## Example Test Case:
Books:
101 3
102 2
103 1
104 5
Members:
1001
1002
1003
1004
Borrowed_Books:
101 1001 01/07/2025
101 1002 01/07/2025
102 1001 02/07/2025
103 1003 03/07/2025
104 1004 03/07/2025
104 1001 03/07/2025
Done

---

## Expected Key Outputs:

Number_Books → 4
Books_Available → 101, 102, 104
Most_Borrowed → 101, 104
Books_Borrowed_Days → 3
Members_Less (2) → 1002, 1003, 1004
List_Book_Borrowers (101) → 1001, 1002
List_Member_Books (1001) → 101, 102, 104
Overlapping_Borrowers (101) → 1001, 1002

---

## How to Reset Test Data
If you want to start fresh:

1.Clear contents of Books.csv, members.csv, and loans.csv.

2.Keep only headers like this:

BookID,Copies,BorrowCount
MemberID,BorrowCount
BookID,MemberID,Date

