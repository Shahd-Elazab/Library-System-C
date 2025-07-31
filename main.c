#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME_LEN 100
#define MAX_LINE_LEN 256
#define MAX_BOOKS 50
#define MAX_MEMBERS 30
void addMemberToFile(const char *membersFile, int memberID) {
    FILE *file = fopen(membersFile, "a");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", membersFile);
        return;
    }
    int memberBorrows =0;

    if (memberID > 999999) {
        printf("Error: Member ID must be no longer than 6 digits.\n");
        fclose(file);
        return;
    }

    fprintf(file, "%d,%d\n", memberID, memberBorrows);
    fclose(file);
}
void addBookToFile(const char *bookfile, int id, int copies) {
    FILE *file = fopen(bookfile, "a");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", bookfile);
        return;
    }
    int bookBorrows = 0; // Default borrow count

    if (id > 999){
        printf("Error: Book ID must be no longer than 3 digits.\n");
        fclose(file);
        return;
    }

    // Write the new book data to the file
    fprintf(file, "%d,%d,%d\n", id, copies, bookBorrows);
    fclose(file);
}
int processloan(const char *membersFile, const char *booksFile, int bookID, int memberID,const char *date) {
    // Open the members file to validate and update the borrow count
    FILE *members = fopen(membersFile, "r+");
    if (!members) {
        printf("Error: Unable to open file %s\n", membersFile);
        return;
        }
    char line[MAX_LINE_LEN];
    int currentMemberID, borrowCount = 0;
    int memberFound = 0;

    FILE *tempMembers = fopen("temp_members.csv", "w");
    if (!tempMembers) {
        printf("Error: Unable to create temporary file.\n");
        fclose(members);
        return;
        }
    while (fgets(line, sizeof(line), members)) {
        if (sscanf(line, "%d,%d", &currentMemberID, &borrowCount) == 2) {
            if (currentMemberID == memberID) {
                memberFound = 1;
                if (borrowCount >= 5) {
                    printf("Error: Member ID %d has already borrowed the maximum number of books (5).\n", memberID);
                    fclose(members);
                    fclose(tempMembers);
                    remove("temp_members.csv");
                    return 1; // Indicate failure
                }
                // Update the borrow count for the member
                fprintf(tempMembers, "%d,%d\n", currentMemberID, borrowCount + 1);
            }
             else {
                fprintf(tempMembers, "%s", line);
            }
        } else {
            fprintf(tempMembers, "%s", line); // Handle malformed lines
        }
    }

    if (!memberFound) {
        printf("Error: Member ID %d not found.\n", memberID);
        fclose(members);
        fclose(tempMembers);
        remove("temp_members.csv");
        return 1; // Indicate failure
    }

    fclose(members);
    fclose(tempMembers);
    remove(membersFile);
    rename("temp_members.csv", membersFile);

    // Open the books file to validate and update the borrow count
    FILE *books = fopen(booksFile, "r+");
    if (!books) {
        printf("Error: Unable to open file %s\n", booksFile);
        return;
    }

    int currentBookID, totalCopies, currentBorrowCount = 0;
    int bookFound = 0;

    FILE *tempBooks = fopen("temp_books.csv", "w");
    if (!tempBooks) {
        printf("Error: Unable to create temporary file for books.\n");
        fclose(books);
        return;
    }

    while (fgets(line, sizeof(line), books)) {
        if (sscanf(line, "%d,%d,%d", &currentBookID, &totalCopies, &currentBorrowCount) == 3) {
            if (currentBookID == bookID) {
                bookFound = 1;
                if (currentBorrowCount >= totalCopies) {
                    printf("Error: No copies available for Book ID %d.\n", bookID);
                    fclose(books);
                    fclose(tempBooks);
                    remove("temp_books.csv");
                     return 1; // Indicate failure
                }
                // Update the borrow count for the book
                fprintf(tempBooks, "%d,%d,%d\n", currentBookID, totalCopies, currentBorrowCount + 1);
            } else {
                fprintf(tempBooks, "%s", line);
            }
        } else {
            fprintf(tempBooks, "%s", line); // Handle malformed lines
        }
    }

    if (!bookFound) {
        printf("Error: Book ID %d not found.\n", bookID);
        fclose(books);
        fclose(tempBooks);
        remove("temp_books.csv");
        return 1; // Indicate failure
    }
    int day, month, year;
   if (sscanf(date, "%d/%d/%d", &day, &month, &year) != 3 || day < 1 || day > 31 || month < 1 || month > 12 || year < 1) {
    printf("Error: Invalid date format. Please use dd/mm/yyyy.\n");
    return 1;
}

    fclose(books);
    fclose(tempBooks);
    remove(booksFile);
    rename("temp_books.csv", booksFile);

    return 0; // Indicate success

}
void addLoan(const char *loansFile, const char *membersFile, const char *booksFile, int bookID, int memberID, const char *date) {
    int result = processloan(membersFile, booksFile, bookID, memberID,date);
    if (result != 0) {
        return;
    }
    // If processLoan succeeds, add the loan to the loans file
    FILE *file = fopen(loansFile, "a");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", loansFile);
        return;
    }

    fprintf(file, "%d,%d,%s\n", bookID, memberID, date);
    fclose(file);
}
//function 1 Number_Books
int Number_Books(const char *bookfile){
    FILE *file = fopen(bookfile, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return -1;  // Indicate error
    }
    int count = 0;
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file)){  // Read each line
        char *token = strtok(line, ",");      // Get the first column
        if (token != NULL) {                 // If the first column exists
            count++;
        }
    }

    fclose(file);
    if (count == 0) {
        printf("none\n");
    }
    return count-1;  // Return the count
}
//fuction 2 Number_Members
int Number_members(const char *membersfile){
    FILE *file= fopen(membersfile,"r");
    if (file==NULL){
        printf("not found\n");
        return-1;
}
    int counter=0;
    char line[MAX_LINE_LEN];

   while (fgets(line, sizeof(line), file)){  // Read each line
        char *token = strtok(line, ",");      // Get the first column
        if (token != NULL) {                 // If the first column exists
            counter++;
        }
    }
     if (counter == 0) {
        printf("none\n");
    }

    fclose(file);
    return counter-1;

}
//function 3 Book_ID_Min
void Book_ID_Min(const char *booksFile) {
    FILE *file = fopen(booksFile, "r"); // Open the books file in read mode
    if (!file) {
        printf("Error: Unable to open file %s\n", booksFile);
        return;}
    char line[MAX_LINE_LEN]; // Buffer to hold each line from the file
    int bookID, copies;
    int minBookID = -1; // Variable to track the minimum book ID (-1 indicates no books)

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        // Parse the line to extract Book ID and Copies
        if (sscanf(line, "%d,%d", &bookID, &copies) == 2){
            if (minBookID == -1 || bookID < minBookID) {
                minBookID = bookID; // Update the minimum book ID
                }}}
    fclose(file); // Close the file
    // Print the minimum Book ID or "none" if no books were found
    if (minBookID == -1) {
        printf("none\n");
    } else {
        printf("%d\n", minBookID);
    }
}
//function 4 Books_Available
void Books_Available(const char *booksFile) {
    FILE *file = fopen(booksFile, "r");
    if (!file) {
        printf("Error: Unable to open file %s\n", booksFile);
        return;
    }

    char line[MAX_LINE_LEN];
    int found = 0;
    int id, copies, borrowCount;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d,%d,%d", &id, &copies, &borrowCount) == 3) {
            if (copies > borrowCount) { // available copies exist
                printf("%d\n", id);
                found = 1;
            }
        }
    }

    if (!found) {
        printf("none\n");
    }

    fclose(file);
}
// function 5 List_Book_Borrowers
void List_Book_Borrowers(const char *loansFile, int bookID) {
    FILE *file = fopen(loansFile, "r");
    if (!file) {
        printf("Error: Unable to open file %s\n", loansFile);
        return;
    }

    char line[MAX_LINE_LEN];
    int bID, mID;
    char date[11];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d,%d,%10s", &bID, &mID, date) == 3 && bID == bookID) {
            printf("%d\n", mID);
            found = 1;
        }
    }

    if (!found) {
        printf("none\n");
    }

    fclose(file);
}
//function 6 List_Member_Books
void List_Member_Books(const char *loansFile, int memberID) {
    FILE *file = fopen(loansFile, "r");
    if (!file) {
        printf("Error: Unable to open file %s\n", loansFile);
        return;
    }

    char line[MAX_LINE_LEN];
    int bID, mID;
    char date[11];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d,%d,%10s", &bID, &mID, date) == 3 && mID == memberID) {
            printf("%d\n", bID);
            found = 1;
        }
    }

    if (!found) {
        printf("none\n");
    }

    fclose(file);
}
//function 7 Most_Borrowed
void Most_Borrowed(const char *booksFile) {
    FILE *file = fopen(booksFile, "r"); // Open the books file in read mode
    if (!file) {
        printf("Error: Unable to open file %s\n", booksFile);
        return;
    }

    char line[MAX_LINE_LEN]; // Buffer to hold each line from the file
    int bookID, totalCopies, borrowCount;
    int maxBorrowCount = -1; // To track the maximum borrow count
    int found = 0;           // Flag to check if at least one book exists

    // Step 1: Find the maximum borrow count
    while (fgets(line, sizeof(line), file)) {
        // Parse each line to extract Book ID, Total Copies, and Borrow Count
        if (sscanf(line, "%d,%d,%d", &bookID, &totalCopies, &borrowCount) == 3) {
            if (borrowCount > maxBorrowCount) {
                maxBorrowCount = borrowCount; // Update maximum borrow count
            }
        }
    }

    rewind(file); // Reset the file pointer to start for the second pass

    // Step 2: Find all books with the maximum borrow count
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d,%d,%d", &bookID, &totalCopies, &borrowCount) == 3) {
            if (borrowCount == maxBorrowCount) {
                printf("%d\n", bookID); // Print the Book ID
                found = 1;
            }
        }
    }

    fclose(file); // Close the books file

    // If no books are found, print "none"
    if (!found) {
        printf("none\n");
    }
}
//function 8 Members_Less n
void Members_Less(const char *membersFile, int n) {
    FILE *file = fopen(membersFile, "r");
    if (!file) {
        printf("Error: Unable to open file %s\n", membersFile);
        return;
    }

    char line[MAX_LINE_LEN];
    int id, count;
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d,%d", &id, &count) == 2 && count < n) {
            printf("%d\n", id);
            found = 1;
        }
    }

    if (!found) {
        printf("none\n");
    }

    fclose(file);
}
// function 9 Books_Unborrowed
void Books_Unborrowed(const char *booksFile) {
    FILE *file = fopen(booksFile, "r");
    if (!file) {
        printf("Error: Unable to open file %s\n", booksFile);
        return;
    }

    char line[MAX_LINE_LEN];
    int id, copies, borrowCount;
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d,%d,%d", &id, &copies, &borrowCount) == 3 && borrowCount == 0) {
            printf("%d\n", id);
            found = 1;
        }
    }

    if (!found) {
        printf("none\n");
    }

    fclose(file);
}
//function 10 Books_Borrowed_Days
int Books_Borrowed_Days(const char *loansFile) {
    FILE *file = fopen(loansFile, "r");
    if (!file) {
        printf("Error: Unable to open file %s\n", loansFile);
        return 0;
    }

    char line[MAX_LINE_LEN];
    char dates[100][11]; // store up to 100 unique dates
    int dateCount = 0;

    while (fgets(line, sizeof(line), file)) {
        int bookID, memberID;
        char date[11];

        if (sscanf(line, "%d,%d,%10s", &bookID, &memberID, date) == 3) {
            int exists = 0;
            for (int i = 0; i < dateCount; i++) {
                if (strcmp(dates[i], date) == 0) {
                    exists = 1;
                    break;
                }
            }
            if (!exists) {
                strcpy(dates[dateCount], date);
                dateCount++;
            }
        }
    }

    fclose(file);
    return dateCount;
}
//function 11 Books_Per_Member
void Books_Per_Member(const char *membersFile) {
    FILE *file = fopen(membersFile, "r"); // Open the members file in read mode
    if (!file) {
        printf("Error: Unable to open file %s\n", membersFile);
        return;
    }

    char line[MAX_LINE_LEN]; // Buffer to hold each line from the file
    int memberID, borrowCount;
    int found = 0; // Flag to check if there are any members

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        // Parse each line to extract Member ID and Borrow Count
        if (sscanf(line, "%d,%d", &memberID, &borrowCount) == 2) {
            printf("%d %d\n", memberID, borrowCount); // Print Member ID and Borrow Count
            found = 1;
        }
    }

    fclose(file);

    // If no members are found, print "none"
    if (!found) {
        printf("none\n");
    }
}
//function 12 Overlapping_Borrowers
void Overlapping_Borrowers(const char *loansFile, int bookID) {
    FILE *file = fopen(loansFile, "r");
    if (!file) {
        printf("Error: Unable to open file %s\n", loansFile);
        return;
    }

    char line[MAX_LINE_LEN];
    int bID, mID;
    char date[11];

    int found = 0;

    // Compare each record with others to find overlaps
    int memberIDs[100];
    int memberCount = 0;

    // Read all entries into arrays
    int bookIDs[100];
    char dates[100][11];
    int total = 0;

    rewind(file);
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d,%d,%10s", &bID, &mID, date) == 3 && bID == bookID) {
            bookIDs[total] = bID;
            memberIDs[total] = mID;
            strcpy(dates[total], date);
            total++;
        }
    }

    // Compare each pair for overlapping dates
    for (int i = 0; i < total; i++) {
        for (int j = i + 1; j < total; j++) {
            if (strcmp(dates[i], dates[j]) == 0) {
                printf("%d\n", memberIDs[i]);
                printf("%d\n", memberIDs[j]);
                found = 1;
            }
        }
    }

    if (!found) {
        printf("none\n");
    }

    fclose(file);
}
int main() {
    const char *booksFile = "Books.csv";
    const char *membersFile = "members.csv";
    const char *loansFile = "loans.csv";
    char header[MAX_LINE_LEN];
     char line[MAX_LINE_LEN];
    // Process Books
    int bookCount = Number_Books(booksFile);
    int memberCount = Number_members(membersFile);
    // Read the first header
    if (fgets(header, sizeof(header), stdin) && strncmp(header, "Books:", 6) == 0) {

        // Read the first line after the header
        while (fgets(line, sizeof(line), stdin)) {
            // Check if the current line is "Members:"
            if (strncmp(line, "Members:", 8) == 0) {
                break;
            }
             if (bookCount >= 50) {
                printf("Error: Maximum number of books (50) reached.\n");
                continue; // Skip further input
            }

            // Process the current book entry
            int id, copies;
            if (sscanf(line, "%d %d", &id, &copies) == 2) {
                addBookToFile(booksFile, id, copies);
                  bookCount++;

            } else {
                printf("Invalid book entry. Please try again.\n");
            }
        }
    }

     fflush(stdin);
     // Process Members
    if (strncmp(line, "Members:", 8) == 0) {
        while (fgets(line, sizeof(line), stdin)) {
            if (strncmp(line, "Borrowed_Books:", 15) == 0) {
                break;
            }
             if (memberCount >= 30) {
                printf("Error: Maximum number of members (30) reached.\n");
                continue; // Skip further input
            }


            int memberID;
            if (sscanf(line, "%d", &memberID) == 1) {
                addMemberToFile(membersFile, memberID);
               memberCount++;

            } else {
                printf("Invalid member entry. Please try again.\n");
            }
        }
    }

   //process loans
    if (strncmp(line, "Borrowed_Books:", 15) == 0) {
    while (fgets(line, sizeof(line), stdin)) {
        // If user types Done, exit to commands section
        if (strncmp(line, "Done", 4) == 0) {
            break;
        }

        int bookID, memberID;
        char date[11];
        if (sscanf(line, "%d %d %10s", &bookID, &memberID, date) == 3) {
            addLoan(loansFile, membersFile, booksFile, bookID, memberID, date);
        } else {
            printf("Invalid borrowed book entry. Please try again.\n");
        }
    }
}
char operation[MAX_LINE_LEN];
 while (1)
    {scanf("%s", operation);
       if (strcmp(operation, "Number_Books") == 0) {
            printf("%d\n", Number_Books(booksFile));
        } else if (strcmp(operation, "Number_Members") == 0) {
            printf("%d\n", Number_members(membersFile));
        } else if (strcmp(operation, "Book_ID_Min") == 0) {
            Book_ID_Min(booksFile);
        } else if (strcmp(operation, "Books_Available") == 0) {
            Books_Available(booksFile);
        } else if (strcmp(operation, "List_Book_Borrowers") == 0) {
            int bookID;
            scanf("%d", &bookID);
            List_Book_Borrowers(loansFile, bookID);
        } else if (strcmp(operation, "List_Member_Books") == 0) {
            int memberID;
            scanf("%d", &memberID);
            List_Member_Books(loansFile, memberID);
        } else if (strcmp(operation, "Most_Borrowed") == 0) {
            Most_Borrowed(booksFile);
        } else if (strcmp(operation, "Members_Less") == 0) {
            int n;
            scanf("%d", &n);
            Members_Less(membersFile, n);
        } else if (strcmp(operation, "Books_Unborrowed") == 0) {
            Books_Unborrowed(booksFile);
        } else if (strcmp(operation, "Books_Borrowed_Days") == 0) {
            printf("%d\n", Books_Borrowed_Days(loansFile));
        }
        else if (strcmp(operation, "Books_Per_Member") == 0) {
            Books_Per_Member(membersFile);
        }
        else if (strcmp(operation, "Overlapping_Borrowers") == 0){
            int bookID;
            scanf("%d", &bookID);
            Overlapping_Borrowers(loansFile, bookID);
        }
        else if (strcmp(operation, "Quit") == 0) {
                printf("Thanks!");
            break;
        }
         else {
            printf("Invalid operation name. Please try again.\n");
        }
    }

}
