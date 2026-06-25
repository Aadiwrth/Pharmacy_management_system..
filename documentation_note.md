# Pharmacy Management System: Complete Code Documentation & Viva Prep

This document provides a comprehensive, step-by-step tutorial of the `main.c` file. It breaks down each function, logic structure, preprocessor directive, and standard library function to help you confidently answer any question your teacher asks during the viva.

---

## 1. Header Files & Standard Library Functions
*(Crucial for Viva: Teachers love asking "Which header file does X function belong to?")*

```c
#include <stdio.h>   
```
*   **Stands for:** Standard Input/Output.
*   **Functions used from this header:**
    *   `printf()`, `scanf()`: Standard console printing and reading.
    *   `fopen()`, `fclose()`: Opening and closing files.
    *   `fprintf()`, `fgets()`: Writing formatted text to files and reading strings from files.
    *   `fread()`, `fwrite()`: Reading and writing binary data (used in `login()`).
    *   `rewind()`: **Important!** This resets the file pointer back to the very beginning of the file. Used in `create()` to scan from the top of the file to check for duplicate IDs.
    *   `remove()`, `rename()`: Used in the update/delete functions to delete the old `medicine.txt` and rename `temp.txt`.
    *   `sprintf()`: String print. Instead of printing to the console, it "prints" a formatted string into a character array. Used in `sell()` to dynamically generate the bill filename (e.g., `bills/123.txt`).

```c
#include <string.h>  
```
*   **Stands for:** String Manipulation Library.
*   **Functions used from this header:**
    *   `strcmp(str1, str2)`: String Compare. Returns `0` if both strings are exactly identical. Used in `login()` to verify passwords and `search()` to match IDs.
    *   `strcpy(dest, src)`: String Copy. Copies the contents of one string into another.
    *   `strcspn(str, "\n")`: String Complement Span. Used in `view()` to find the hidden `\n` (newline) character at the end of a string so we can remove it.
    *   `strtok(str, "|")`: String Tokenize. Used to split a long string into smaller pieces wherever the `|` symbol appears.

```c
#include <stdlib.h>  
```
*   **Stands for:** Standard Library.
*   **Functions used from this header:**
    *   `system()`: Passes a command to the host operating system's terminal (e.g., `system("cls")` to clear the screen, or `system("mkdir bills")` to create a folder).
    *   `atoi()` / `atof()`: ASCII to Integer / ASCII to Float. Converts a string variable into an actual math number. (e.g., converting "150.50" into `150.50`).

```c
#include <conio.h>
```
*   **Stands for:** Console Input/Output (Windows specific).
*   **Functions used from this header:**
    *   `getch()`: Get Character. Reads a single keypress instantly **without** displaying it on the screen. Vital for our `*****` password masking.

---

## 2. Preprocessor Directives & Cross-Platform Logic

```c
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
```
*   **What is it?** This is a **Preprocessor Directive**. Before the code is even compiled, the preprocessor reads these lines.
*   **Why did we use it?** Different Operating Systems use different terminal commands to clear the screen (`cls` for Windows, `clear` for Linux/Mac). `_WIN32` is a macro automatically defined by the compiler when you are on Windows. This block makes the code **cross-platform**.

---

## 3. Global Variables & Data Structures

```c
int state = 0, check = 0; 
```
*   **`state` (State Machine Logic):** We use this global variable to control the entire flow of the program. Instead of nesting endless loops, `main()` checks `state` to decide which menu to show.

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
*   **Why a `struct`?** A `struct` bundles related data types together. Instead of having 6 separate arrays, we create a single "Blueprint" (`Medicine`) that holds everything about one specific pill together.

---

## 4. Function-By-Function Tutorial (Line-by-Line Breakdown)

### A. `login()`
*   **How it works:** Secures the app using a binary file (`Cred.bin`).
*   **Line-by-Line Logic:**
    *   `fopen("Cred.bin","rb+")`: Opens in "read/update binary" mode.
    *   `if(pass == NULL)`: If file doesn't exist (first launch), switch to `"wb"` (write binary) mode, prompt for a new password, and save it using `fwrite()`.
    *   `fread(...)`: Reads the binary password into `stored_pass`.
    *   `strcmp(stored_pass, temp_pass) == 0`: If identical, change `state = 1` to grant access.

### B. `create()` (Add Medicine)
*   **How it works:** Appends new structs to `medicine.txt`.
*   **Key Implementations:**
    *   **Duplicate ID Check:** `rewind(Pill)` moves the file cursor to the absolute beginning. We loop `fgets` to read every line, extracting the ID. If it matches the user's new ID, we reject it.
    *   **Buffer Clearing:** `while (getchar() != '\n');` "eats" invalid characters stuck in the input buffer if `scanf` fails, preventing infinite crash loops.

### C. `view()` & `search()`
*   **Newline Stripping:** `line[strcspn(line, "\n")] = 0;` finds the `\n` character attached by `fgets` and replaces it with `\0` (null terminator). This prevents table formatting from breaking.
*   **Tokenization:** `strtok(line, "|")` cuts the string at the first pipe symbol. Calling `strtok(NULL, "|")` continues cutting to extract Name, Price, etc.
*   **Table Formatting:** `printf("%-10s", text);` The `%s` means string. `10` reserves exactly 10 spaces. `-` forces the text to align to the **left**.

### D. `sell()` (Point of Sale System)
*   `system("mkdir bills 2>nul");` creates a directory. `2>nul` hides any "Folder already exists" errors.
*   **Dynamic File Names:** `sprintf(filename, "bills/%s.txt", bill_id);` pieces together a string to name the file dynamically.
*   If `med.stock >= qty`, it updates the stock in memory, writes the updated data to `temp.txt`, and generates the receipt file.

### E. `update()` & `delete()`
*   **The Temp File Rewrite Logic:**
    1.  Open `medicine.txt` (`"r"`) and `temp.txt` (`"w"`).
    2.  Read the original file line by line.
    3.  **Delete:** If ID matches, *skip writing it*.
    4.  **Update:** If ID matches, ask user for new data and write the *new* formatted string.
    5.  For all non-matching lines, copy them exactly as is.
    6.  Close files. `remove("medicine.txt")` deletes the old DB. `rename("temp.txt", "medicine.txt")` promotes the temp file.

### F. Password Masking Logic (`inputPassword()`)
*   `ch = getch();`: Reads the keypress silently.
*   `if (ch == '\r' || ch == '\n')`: If Enter is pressed, append `\0` to finish the string and `break`.
*   `else if (ch == '\b' || ch == 127)`: If Backspace is pressed:
    *   `i--;` reduces the array index.
    *   `printf("\b \b");` moves cursor back, prints a blank space over the `*`, and moves cursor back again.
*   `pass[i++] = ch;`: Normal character? Store it in memory.
*   `printf("*");`: Print a star to the screen.

---

## 5. Confident Viva Answers for Your Teacher

**Teacher:** *"Where does the `rewind()` function come from, and what exactly does it do in your `create()` function?"*
**Your Answer:** "`rewind()` is a standard library function from `<stdio.h>`. In the `create()` function, before I save a new medicine, I use `rewind()` to force the file pointer back to the very first line of `medicine.txt`. This allows me to scan the entire file from the top to ensure the new Medicine ID doesn't already exist, preventing duplicates."

**Teacher:** *"Which library provides `strcmp` and `strcpy`, and what is the difference?"*
**Your Answer:** "They both come from `<string.h>`. `strcmp` compares two strings and returns 0 if they are an exact match, which I used for the login password check. `strcpy` copies the contents of one string variable into another."

**Teacher:** *"I see you used `remove()` and `rename()`. Where do these come from, and why use them?"*
**Your Answer:** "They belong to `<stdio.h>`. Because C cannot easily delete or insert a single word in the middle of a text file, my `update()` and `delete()` functions write all the data to a new temporary file. I then use `remove()` to delete the old database and `rename()` to change the temporary file's name to the official database name."

**Teacher:** *"How does your program dynamically create files like `bills/123.txt`?"*
**Your Answer:** "I used the `sprintf()` function from `<stdio.h>`. Unlike `printf` which outputs to the console screen, `sprintf` outputs a formatted string directly into a character array variable. I pass the user's Bill ID into `sprintf` to generate the exact file path, and then use `fopen` on that newly generated path."

**Teacher:** *"Why did you use `fgets` instead of `scanf` for the Medicine Name?"*
**Your Answer:** "`scanf` with `%s` stops reading as soon as it hits a space. If a user types 'Cough Syrup', `scanf` will only store 'Cough'. `fgets` from `<stdio.h>` safely reads the entire string including spaces until the user presses Enter."

**Teacher:** *"How do you handle invalid inputs to prevent the console from crashing?"*
**Your Answer:** "Whenever I use `scanf` for numbers, if the user types a letter, `scanf` fails and leaves the letter stuck in the buffer. I immediately run a `while(getchar() != '\n');` loop to flush the input buffer, essentially 'eating' the bad characters so the program remains stable."

**Teacher:** *"Where does `getch()` come from and why did you use it?"*
**Your Answer:** "It comes from the `<conio.h>` header. I used it for the password masking system because `getch()` captures a user's keystroke instantly without printing it to the terminal screen. This allowed me to secretly store the password characters in memory while manually printing `*` symbols to the screen."
