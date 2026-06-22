# Pharmacy Management System: Code Documentation & Viva Prep

This document breaks down the `ai.c` file section by section to help you understand the logic, prepare for your viva, and be able to explain how your code works.

## 1. Core Concepts & Overview
Your program is a **Console-Based Pharmacy Management System**. It acts as a State Machine.
*   **State Machine:** A global variable `state` keeps track of where the user is in the program. `state = 0` means login, `state = 1` means the main menu, and other states (like 2, 3, 4, 10) are used to handle multi-step inputs safely.
*   **Persistent Storage (File I/O):** Data is saved permanently onto the hard drive so it doesn't get lost when the program closes.
    *   `Cred.bin`: Stores the login password in a binary format.
    *   `medicine.txt`: Stores the inventory data in a structured text format separated by pipes (`|`).

---

## 2. Header Files & Global Variables

```c
#include <stdio.h>   // Standard Input/Output: allows printf, scanf, fgets, file operations
#include <string.h>  // String manipulation: allows strcmp (compare), strtok (split), strcspn
#include <stdlib.h>  // Standard Library: allows system("cls"), atoi (string to int), atof (string to float)

int state = 0, check = 0; 
char temp[50];
```
*   `state`: This is the most important variable. Changing its value moves the user through different menus and prevents them from moving forward if they input something wrong.
*   `temp[50]`: A temporary array used for catching random inputs when pausing the screen.

---

## 3. Data Structures

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
*   **Why use a struct?** A `struct` bundles related variables together. Instead of having separate arrays for IDs, names, and prices, one `Medicine` variable holds all the attributes of a single drug, making the code organized.

---

## 4. Key Functions Explained

### A. `clearscreen()`
```c
void clearscreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
```
*   **What it does:** Clears the console text to keep the UI clean.
*   **How it works:** It uses Preprocessor Directives (`#ifdef`). Before the code even compiles, it checks if the OS is Windows (`_WIN32`). If yes, it compiles `system("cls")`. If Mac/Linux, it compiles `system("clear")`.

### B. `login()`
*   **What it does:** Acts as a gatekeeper. If no password exists, it registers one. If it does, it asks for it.
*   **How it works:**
    *   `fopen("Cred.bin", "rb+")`: Opens the binary file in read/update mode.
    *   If the file doesn't exist (`pass == NULL`), it creates it using `"wb"` (write binary) mode, asks the user for a new password using `fgets`, writes it to the file using `fwrite`, and closes it.
    *   If it exists, it reads the saved password into `stored_pass` using `fread`. It then asks the user to input a password.
    *   `strcmp(stored_pass, temp_pass) == 0`: Compares the two strings. If they match exactly, it evaluates to `0`, granting access by setting `state = 1` (the main menu).

### C. `create()` (Add Medicine)
*   **What it does:** Asks the user for medicine details step-by-step and saves them to `medicine.txt`.
*   **How it works:**
    *   Opens file in `"a+"` (append) mode. This means new text will be added to the bottom of the file without deleting existing data.
    *   Uses a series of `while (state == X)` loops. If the user enters a letter instead of a number for the ID, `scanf` fails and returns 0 instead of 1. The code catches this, prints an error, and the loop repeats because the state didn't change!
    *   **Input Buffer Clearing:** `while (getchar() != '\n');` is crucial. When `scanf` fails on a letter, that letter stays stuck in the input buffer. This loop eats the bad input until it hits the Enter key, preventing an infinite loop.
    *   **Saving:** `fprintf(Pill, "%d|%s|%.2f|%d|%d/%d\n", ...)` formats the data with pipe symbols `|` as delimiters and saves it.

### D. `view()` (Read File)
*   **What it does:** Reads all medicines from the file and prints them nicely.
*   **How it works:**
    *   Opens file in `"r"` (read-only) mode.
    *   `while (fgets(line, sizeof(line), Pill) != NULL)`: Reads the file line by line until it hits the end of the file.
    *   **`strtok` (String Tokenizer):** This is the core logic. `strtok(line, "|")` takes a string like `1|Aspirin|5.00` and cuts it at the `|` symbol. Calling `strtok(NULL, "|")` fetches the next piece. A `switch(field)` block prints the right label (ID, Name, Price) based on which piece we are looking at.

### E. `search()`
*   **What it does:** Finds a specific medicine by ID.
*   **How it works:** Very similar to `view()`. It reads line by line, uses `strtok` to grab the very first piece of data (the ID), and uses `strcmp(token, search) == 0` to see if it matches the user's input. If it matches, it prints the rest of the pieces.

### F. `update()` & `delete()`
*   **What they do:** Modify or remove a specific record from the text file.
*   **The File Rewriting Concept (Crucial for Viva):** You cannot easily delete or modify data in the middle of a sequential text file. The standard C approach is:
    1.  Open original file `medicine.txt` in Read mode (`"r"`).
    2.  Open a temporary file `temp.txt` in Write mode (`"w"`).
    3.  Read the original file line by line.
    4.  **For Delete:** If the line's ID matches the target, DO NOT write it to `temp.txt`. Write everything else.
    5.  **For Update:** If it matches, extract the old values into variables, ask the user for the new value, merge them into the `struct`, and write the *new* line to `temp.txt`.
    6.  Close both files.
    7.  Use `remove("medicine.txt")` to delete the old database.
    8.  Use `rename("temp.txt", "medicine.txt")` to make the new file the official database.

### G. `main()`
*   **What it does:** The main control hub of the program.
*   **How it works:**
    *   An infinite loop `while(1)` runs the entire program.
    *   It checks `state`. If `0`, it routes to `login()`.
    *   If `1`, it prints the dashboard and waits for a `choice`.
    *   A `switch(choice)` acts as a router, calling `create()`, `view()`, `delete()`, etc.
    *   If the user selects 7 (Quit), it uses `return 0;` which cleanly kills the program.

---

## Viva Tips & Common Questions to Prepare For
1.  **"Why did you use `fgets` instead of `scanf` for strings?"**
    *   *Answer:* `scanf` stops reading as soon as it hits a space. If a medicine is named "Cough Syrup", `scanf` only reads "Cough". `fgets` reads the entire line including spaces, making it safer for names.
2.  **"What is the purpose of `strtok`?"**
    *   *Answer:* Since we save data in one big string like `1|Panadol|10|100|5/2028`, `strtok` splits that string into smaller, usable tokens based on the delimiter `|`.
3.  **"How are you handling incorrect user inputs (e.g., typing letters instead of numbers)?"**
    *   *Answer:* By checking the return value of `scanf` (e.g., `scanf("%d") != 1`). If it doesn't return 1, the input failed. I then use a `while(getchar() != '\n')` loop to clear the bad input out of the buffer so the program doesn't crash or get stuck in an infinite loop.
4.  **"Why do you use a temporary file for updating and deleting?"**
    *   *Answer:* Text files are sequential. If you change "Aspirin" to "Paracetamol", the new text is longer and would overwrite parts of the next record. By copying everything to a new temp file and swapping them, we ensure the data formatting is perfectly preserved without corrupting the file.
