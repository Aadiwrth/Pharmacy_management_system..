# Pharmacy Management System: Complete Code Documentation & Viva Prep

This document provides a comprehensive, step-by-step tutorial of the `main.c` file. It breaks down each function, logic structure, and preprocessor directive to help you confidently answer any question your teacher asks during the viva.

---

## 1. Header Files & Preprocessor Directives

```c
#include <stdio.h>   
#include <string.h>  
#include <stdlib.h>  
```
*   **Why we used them:** 
    *   `stdio.h`: Standard Input/Output. Required for `printf`, `scanf`, and all file operations (`fopen`, `fprintf`, `fgets`).
    *   `string.h`: Required for string manipulation functions like `strcmp` (comparing strings), `strtok` (splitting strings), `strcpy` (copying strings), and `strcspn` (finding characters).
    *   `stdlib.h`: Standard Library. Required for `system()` calls, and conversions like `atoi` (string to integer) and `atof` (string to float).

```c
#include <conio.h>
```
*   **What is it?** We include `<conio.h>` to use the `getch()` function.
*   **Why did we use it?** `getch()` reads a keypress instantly without displaying it on the screen, which is required for our `*****` password masking.

```c
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
```
*   **What is it?** This is a **Preprocessor Directive**. Before the code is even compiled into an executable, the preprocessor reads these lines.
*   **Why did we use it?** Different Operating Systems use different terminal commands to clear the screen (`cls` for Windows, `clear` for Linux/Mac). `_WIN32` is a macro automatically defined by the compiler when you are on Windows. This block tells the compiler: *"If compiling on Windows, use `system("cls")`. If compiling anywhere else, use `system("clear")`."* This makes the code **cross-platform**.
*   **Alternatives:** We could have used a third-party library (like `ncurses`) or ANSI escape codes (e.g., `printf("\e[1;1H\e[2J");`) to clear the screen. However, using standard `system()` calls combined with preprocessor directives is much simpler and highly effective for standard C projects.

---

## 2. Global Variables & Data Structures

```c
int state = 0, check = 0; 
```
*   **`state` (State Machine Logic):** We use this global variable to control the entire flow of the program. Instead of nesting endless loops and `if` statements, the `main()` function checks `state` to decide which menu to show. If the user makes an invalid input, the `state` remains unchanged, trapping them in the current menu until they provide correct data.

```c
struct Medicine {
    int id;
    char name[50];
    float price;
    int stock;
    int expireMonth;
    int expireYear;
};
```
*   **Why a `struct`?** A `struct` bundles related data types together. Instead of having 6 separate arrays for IDs, names, and prices (which is hard to manage), we create a single "Blueprint" (`Medicine`) that holds everything about one specific pill together.

---

## 3. Function-By-Function Tutorial

### A. `login()`
*   **How it works:** It uses a binary file (`Cred.bin`) to store the password securely.
*   **Line-by-Line Logic:**
    *   `fopen("Cred.bin","rb+")`: Opens the file in "read/update binary" mode.
    *   `if(pass == NULL)`: If the file doesn't exist, it means it's the user's first time running the program. It switches to `"wb"` (write binary) mode, asks the user to create a new password, and saves it.
    *   `fread(...)`: Reads the binary password into a local array.
    *   `strcmp(stored_pass, temp_pass) == 0`: If the typed password exactly matches the saved one, the user is authenticated by changing `state = 1`.

### B. `create()` (Add Medicine)
*   **How it works:** Gathers data from the user and appends it to `medicine.txt`.
*   **Key Implementations:**
    *   **Duplicate ID Check:** Before saving, we use `rewind(Pill)` to move the file pointer back to the top of the file, read every line with `fgets`, and check if the entered `med.id` already exists. If it does, we reject the input and warn the user.
    *   **Buffer Clearing:** `while (getchar() != '\n');` is used heavily here. If `scanf("%d")` fails (e.g., the user types "abc" instead of "12"), the invalid characters get stuck in the input buffer. This `while` loop essentially "eats" those stuck characters until it hits the enter key, preventing the program from getting stuck in an infinite loop.
    *   **File Writing:** `fprintf(Pill, "%d|%s...", ...)` writes the structure data to the text file, separated by `|` delimiters.

### C. `view()` (Display Inventory)
*   **How it works:** Reads `medicine.txt` and prints it as a beautiful, aligned table.
*   **Key Implementations:**
    *   **Empty File Alert:** Uses a `has_records` flag. If `fgets` never reads a line from the file, it cleanly prints "Empty! No medicines available."
    *   **Newline Stripping:** `line[strcspn(line, "\n")] = 0;` finds the hidden newline character `\n` attached to the end of the line by `fgets` and replaces it with a null terminator `\0`. This prevents our table from breaking across multiple lines.
    *   **String Tokenization (`strtok`):** The code reads the whole line as one string (e.g., `1|Panadol|10|100...`). `strtok(line, "|")` cuts the string at the first pipe symbol to grab the ID. Calling `strtok(NULL, "|")` continues cutting from where it left off, extracting the Name, Price, Stock, etc.

### D. `search()`
*   **How it works:** Similar to `view()`, but it compares the first token (the ID) to the user's search query.
*   **Key Implementation:** `if (strcmp(id, search) == 0)` checks if the IDs match. If they do, it extracts the rest of the tokens, prints them in the exact same tabular format as `view()`, and uses a `break;` statement to stop searching the rest of the file (saving processing power).

### E. `sell()` (Point of Sale System)
*   **How it works:** Decreases inventory stock dynamically and generates a receipt.
*   **Key Implementations:**
    *   `system("mkdir bills 2>nul");` (or `2>/dev/null` on Linux) creates a new directory for receipts. The `2>nul` redirects any error output (like "Folder already exists") into the void so the user's screen remains clean.
    *   **Dynamic File Names:** `sprintf(filename, "bills/%s.txt", bill_id);` pieces together a string to name the text file exactly what the user typed for the Bill ID.
    *   **File Rewriting:** If there is enough stock (`med.stock >= qty`), it updates the stock in memory, writes the *updated* string to a `temp.txt` file, and logs the financial transaction into the `bills/` receipt text file.

### F. `update()` & `delete()`
*   **How it works:** You cannot easily delete or modify a string sitting in the middle of a sequential text file. The only safe way in C is the "Temp File Rewrite" method.
*   **The Logic (Crucial for Viva):**
    1.  Open the original `medicine.txt` (`"r"`) and a new `temp.txt` (`"w"`).
    2.  Read the original file line by line.
    3.  **Delete:** If the line's ID matches the target, *do not write it* to the temp file. If it doesn't match, write it exactly as is.
    4.  **Update:** If the line matches, ask the user for the new specific data (like a new price), format it, and write the *newly formatted line* to `temp.txt`. For all other lines, copy them exactly as they are.
    5.  Close both files.
    6.  `remove("medicine.txt")` deletes the old database.
    7.  `rename("temp.txt", "medicine.txt")` promotes the temp file to be the new official database.

### G. Password Masking Logic (`inputPassword()`)
When explaining how you convert passwords to `*` to your professor, you can break it down line-by-line:
*   `ch = getch();`: This reads the key you pressed instantly, **without** echoing (showing) it on the screen.
*   `if (ch == '\r' || ch == '\n')`: `\r` stands for **Carriage Return** (the Enter key in Windows) and `\n` stands for **Newline**. If the user presses Enter, we cap the string with `\0` (the null terminator) to finish the password string, print a newline, and `break` the loop.
*   `else if (ch == '\b' || ch == 127)`: `\b` is the **Backspace** character. If the user presses Backspace and the length `i` is greater than 0:
    *   `i--;`: We reduce the password array index so the next typed character will overwrite the deleted one in memory.
    *   `printf("\b \b");`: This is a clever visual trick on the terminal! We move the cursor back one space (`\b`), print a blank space (` `) to visually erase the star on the screen, and then move the cursor back one space again (`\b`) so it's ready for the next letter.
*   `pass[i++] = ch;`: If it's a normal character, we store the actual typed character inside the `pass` array.
*   `printf("*");`: Right after storing the real character in memory, we manually print a `*` to the screen. The computer knows the real password, but the user only sees stars!

### H. Table Formatting (`%-10s`, `%-20s`)
In `view()`, `search()`, and `sell()`, you use formatting strings like: 
`printf("%-10s | %-20s | %-10s", "ID", "Name", "Price");`
Here is exactly how to explain this logic:
*   **`%s`**: Tells C to expect and print a String.
*   **`10` or `20` (Width Specifier)**: This reserves a fixed number of spaces for that specific column. For example, `20` means "reserve exactly 20 character spaces for the Medicine Name". If the name is "Panadol" (7 letters), C will automatically add 13 blank spaces after it. This guarantees that the next column (`|`) will perfectly align vertically for every single row.
*   **`-` (Minus Sign / Left Justify)**: Without the minus sign, the text would be pushed to the far right side of those 20 spaces (right-aligned). The `-` forces the text to be **left-aligned**. It prints the text first on the left, and pads the remaining blank spaces on the right.

---

## 4. Confident Viva Answers for Your Teacher

**Teacher:** *"Why did you use `fgets` instead of `scanf` for the Medicine Name?"*
**Your Answer:** "`scanf` with `%s` stops reading as soon as it hits a space. If a user types 'Cough Syrup', `scanf` will only store 'Cough'. `fgets` safely reads the entire string including spaces until the user presses Enter."

**Teacher:** *"What is the purpose of `strtok`, and why didn't you just use `fscanf`?"*
**Your Answer:** "I chose to use pipe delimiters (`|`) to store data securely on a single line. `fscanf` struggles if a text field like a Medicine Name contains spaces. By reading the whole line with `fgets` and splitting it using `strtok`, I have precise control over extracting the ID, Name, and Price without formatting errors."

**Teacher:** *"How do you handle invalid inputs to prevent the console from crashing?"*
**Your Answer:** "Whenever I use `scanf` for integers or floats, I check its return value (e.g., `if (scanf("%d") != 1)`). If the user types a letter instead of a number, `scanf` fails and leaves the letter stuck in the buffer. I immediately run a `while(getchar() != '\n');` loop to flush the input buffer, keeping the program stable and asking the user to try again."

**Teacher:** *"Why do you create a temporary file when updating or deleting records?"*
**Your Answer:** "Because text files in C are sequential streams of bytes. If I change the word 'Aspirin' to 'Paracetamol', the new word is longer and would physically overwrite the data of the next medicine in the file. The standard and safest methodology is to read the old file, write the updated information into a new temporary file, delete the old file, and rename the new one."

**Teacher:** *"What does `strcspn` do in your view function?"*
**Your Answer:** "When `fgets` reads a line from a file, it keeps the newline character (`\n`) at the very end of the string. `strcspn(line, "\n")` searches the string and returns the index position of that newline. I then set that index to `\0` (the null terminator) to safely strip the newline away, which prevents my UI tables from breaking across multiple lines."

**Teacher:** *"How does your password masking work?"*
**Your Answer:** "It uses the `getch()` function from `<conio.h>` which reads a key instantly without displaying it on the screen. It then manually prints a `*` for every character typed."
