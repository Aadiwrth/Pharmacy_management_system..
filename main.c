#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

struct Medicine {
    int id;
    char name[50];
    float price;
    int stock;
    int expireMonth;
    int expireYear;

};

int state = 0,check = 0; 
char temp[50];
int main();
void inputPassword(char *pass, int max_length) {
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == '\r' || ch == '\n') {
            pass[i] = '\0';
            printf("\n");
            break;
        } else if (ch == '\b' || ch == 127) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (i < max_length - 1) {
            pass[i++] = ch;
            printf("*");
        }
    }
}

void clearscreen() {
#ifdef _WIN32 //* this is basically a preprocessor for if statement kinda like that and the compiler automatically defines _WIN32 when compiling on window cause window uses "cls" and linux/mac uses "clear" 
    system("cls"); //* system is basically a 
#else
    system("clear");
#endif
}


void login(){
    FILE *pass = fopen("Cred.bin","rb+");
    char temp_pass[100],temp_file_pass[100],stored_pass[100];
    if(pass == NULL)
    {
        pass = fopen("Cred.bin","wb");
        printf("=============================\n");
        printf("Register System\n");
        printf("=============================\n");
        printf("Enter Your New Password: ");
        inputPassword(temp_file_pass, sizeof(temp_file_pass));
        fwrite(temp_file_pass,sizeof(temp_file_pass),1,pass);
        fclose(pass);
        return;
    }else {
        if (fread(stored_pass, sizeof(stored_pass), 1, pass) != 1){
                printf("Password  is empty!\n");
                fclose(pass);
                return;
            }
        fclose(pass);
        printf("=============================\n");
        printf("Login System\n");
        printf("=============================\n");
        printf("Enter your Password: ");
        inputPassword(temp_pass, sizeof(temp_pass));
    }
    if(strcmp(stored_pass,temp_pass) == 0){
            state = 1;
            clearscreen();
            return;
    }else {
            printf("Invalid password\n");
            printf("Press Any key to Continue");
            getch();
            clearscreen();
            return;
   }
}

void create() {
    struct Medicine med;
    int temp_bal, temp_id;
    FILE *Pill = fopen("medicine.txt", "a+");

    if (Pill == NULL) {
        printf("The File can't be opened!!!\n");
        return;
    }

    printf("=============================\n");
    printf("Pharmacy Management\n");
    printf("=============================\n");

    while (state == 1) {
        printf("1. Enter ID number: ");
        if (scanf("%d", &med.id) != 1) {
            printf("Invalid ID number!!\n");
            while (getchar() != '\n');
        } else {
            while (getchar() != '\n');
            int exists = 0;
            rewind(Pill);
            char line[256];
            while (fgets(line, sizeof(line), Pill) != NULL) {
                char line_copy[256];
                strcpy(line_copy, line);
                char *token = strtok(line_copy, "|");
                if (token != NULL && atoi(token) == med.id) {
                    exists = 1;
                    break;
                }
            }
            if (exists) {
                printf("ID %d already exists! Please choose a different Id.\n", med.id);
            } else {
                state = 2;
            }
        }
    }
    while (state == 2) {
        printf("2. Enter Medicine Name: ");
        fgets(med.name, sizeof(med.name), stdin);

        if (strlen(med.name) > 2) {
            state = 3;
        } else {
            printf("Invalid a name should contain minimum character of 2 letter\n");
        }
    }
    while (state == 3) {
        printf("3. Enter your Medicine price: ");

        if (scanf("%f", &med.price) != 1 || med.price <= 0) {
            printf("Invalid!!\n");
            while (getchar() != '\n');
        } else {
            state = 4;
        }
    }
    while (state == 4) {
        printf("4. Enter Number of Stocks: ");

        if (scanf("%d", &med.stock) != 1 || med.stock <= 0) {
            printf("Invalid Please enter a valid stock number.\n");
            while (getchar() != '\n');
        } else {
            state = 5;
        }
    }
    while (state == 5) {
        printf("Enter Expire Date(M/Y): ");
        if (scanf("%d/%d", &med.expireMonth, &med.expireYear) != 2) {
            printf("Invalid Please enter a valid expire Date (ie: 5/2027).\n");
            while (getchar() != '\n');
        } else {
            if (med.expireMonth < 1 || med.expireMonth > 12) {
                printf("Invalid Month\n");
                continue;
            }
            if (med.expireYear < 2026) {
                printf("Invalid Year\n");
                continue;
            } else {
                state = 1;
                continue;
            }
        }
    }

    med.name[strcspn(med.name, "\n")] = '\0';
    fprintf(Pill, "%d|%s|%.2f|%d|%d/%d\n", med.id, med.name, med.price, med.stock, med.expireMonth, med.expireYear);

    clearscreen();
    printf("===YOUR INPUT DETAILS===\n");
    printf("Id: %d\n", med.id);
    printf("Medicine Name: %s\n", med.name);
    printf("Price: Rs. %.2f\n", med.price);
    printf("Stocks: %d\n", med.stock);
    printf("Expire Date: %d/%d\n\n", med.expireMonth, med.expireYear);
    printf("\nPress Any Key to Continue\n");
    while (getchar() != '\n');
    getch();
    clearscreen();
    fclose(Pill);
    return;
}

void view() {
    FILE *Pill = fopen("medicine.txt", "r");
    if (Pill == NULL) {
        printf("The File can't be opened!!!\n");
        return;
    }
    char line[256];
    int has_records = 0;

    printf("=================================================================================\n");
    printf("%-10s | %-20s | %-10s | %-15s | %-15s\n", "ID", "Name", "Price", "Stock Left", "Expire Date");
    printf("---------------------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), Pill) != NULL) {
        has_records = 1;
        line[strcspn(line, "\n")] = 0; // Remove newline character
        
        char *id = strtok(line, "|");
        char *name = strtok(NULL, "|");
        char *price = strtok(NULL, "|");
        char *stock = strtok(NULL, "|");
        char *expire = strtok(NULL, "|");

        if (id && name && price && stock && expire) {
            printf("%-10s | %-20s | %-10s | %-15s | %-15s\n", id, name, price, stock, expire);
        }
    }
    
    if (!has_records) {
        printf("                              Empty! No medicines available.                     \n");
    }
    printf("=================================================================================\n");
    
    while (getchar() != '\n');
    fclose(Pill);
    printf("Press Any key to Continue.");
    getch();
    clearscreen();
    return;

}// the strtok() function in C is basically a std function used to split a string into a smaller peice
void search() {
    FILE *Pill = fopen("medicine.txt", "r");
    if(Pill == NULL){
         printf("The File can't be opened!!!\n");
         return;
    }
    int search_state = 1;
    char search[256];
    char line[256];
    printf("Enter the ID: ");
    scanf("%255s", search);
    while (getchar() != '\n'); // THis part is used to Clear buffer
    
    printf("\n=================================================================================\n");
    printf("%-10s | %-20s | %-10s | %-15s | %-15s\n", "ID", "Name", "Price", "Stock Left", "Expire Date");
    printf("---------------------------------------------------------------------------------\n");

    while(fgets(line,sizeof(line),Pill) != NULL){
        line[strcspn(line, "\n")] = 0; 
        char line_copy[256];
        strcpy(line_copy, line);
        
        char *id = strtok(line_copy, "|");
        if(id != NULL && strcmp(id,search) == 0){
            search_state = 0;
            char *name = strtok(NULL, "|");
            char *price = strtok(NULL, "|");
            char *stock = strtok(NULL, "|");
            char *expire = strtok(NULL, "|");

            if (name && price && stock && expire) {
                printf("%-10s | %-20s | %-10s | %-15s | %-15s\n", id, name, price, stock, expire);
            }
            break;
        }
    }
    
    if(search_state == 1){
        printf("                          The ID couldn't be found!!                             \n");
    }
    printf("=================================================================================\n");

    printf("Press Any key to Continue.");
    getch();
    clearscreen();
    fclose(Pill);
    return;
}

void sell(){
    state = 1;
    char bill_id[50];
    int num_items;
    
#ifdef _WIN32
    system("mkdir bills 2>nul");
#else
    system("mkdir bills 2>/dev/null");
#endif

    printf("=============================\n");
    printf("Sell Medicine\n");
    printf("=============================\n");
    printf("Enter new Bill ID: ");
    scanf("%s", bill_id);
    
    char filename[100];
    sprintf(filename, "bills/%s.txt", bill_id);
    FILE *bill_file = fopen(filename, "w");
    if(bill_file == NULL){
        printf("Failed to create bill file.\n");
        return;
    }
    
    printf("How many different medicines to sell :  ");
    while(scanf("%d", &num_items) != 1 || num_items <= 0) {
        printf("Invalid input. Enter a valid number: ");
        while(getchar() != '\n');
    }
    
    float total_amt = 0.0;
    
    fprintf(bill_file, "=================================\n");
    fprintf(bill_file, "       PHARMACY BILL\n");
    fprintf(bill_file, "Bill ID: %s\n", bill_id);
    fprintf(bill_file, "=================================\n");
    fprintf(bill_file, "%-10s %-20s %-10s %-10s %-10s\n", "ID", "Name", "Quantity", "Price", "Total");
    fprintf(bill_file, "---------------------------------------------------------------\n");

    for(int i = 0; i < num_items; i++) {
        char search_id[256];
        int qty;
        printf("\n--- Item %d ---\n", i + 1);
        printf("Enter Medicine ID: ");
        scanf("%s", search_id);
        printf("Quantity: ");
        while(scanf("%d", &qty) != 1 || qty <= 0) {
            printf("Invalid input. Quantity: ");
            while(getchar() != '\n');
        }
        
        FILE *Pill = fopen("medicine.txt", "r");
        FILE *Temp = fopen("temp.txt", "w");
        if(Pill == NULL || Temp == NULL){
            printf("Database error. Cannot open medicine.txt\n");
            if (Pill) fclose(Pill);
            if (Temp) fclose(Temp);
            break;
        }
        
        char line[256], line_copy[256];
        int found = 0;
        
        while(fgets(line, sizeof(line), Pill) != NULL){
            strcpy(line_copy, line);
            char *token = strtok(line, "|");
            if(token != NULL && strcmp(token, search_id) == 0){
                found = 1;
                struct Medicine med;
                med.id = atoi(token);
                token = strtok(NULL, "|"); if(token) strcpy(med.name, token);
                token = strtok(NULL, "|"); if(token) med.price = atof(token);
                token = strtok(NULL, "|"); if(token) med.stock = atoi(token);
                token = strtok(NULL, "/"); if(token) med.expireMonth = atoi(token);
                token = strtok(NULL, "\n"); if(token) med.expireYear = atoi(token);
                
                if(med.stock >= qty) {
                    int old_stock = med.stock;
                    med.stock -= qty;
                    float item_total = med.price * qty;
                    total_amt += item_total;
                    
                    fprintf(Temp, "%d|%s|%.2f|%d|%d/%d\n", med.id, med.name, med.price, med.stock, med.expireMonth, med.expireYear);
                    fprintf(bill_file, "%-10d %-20s %-10d %-10.2f %-10.2f\n", med.id, med.name, qty, med.price, item_total);
        

                } else {
                    printf("\nInsufficient stock! Available: %d\n", med.stock);
                    fputs(line_copy, Temp);
                }
            } else {
                fputs(line_copy, Temp);
            }
        }
        fclose(Pill);
        fclose(Temp);
        
        remove("medicine.txt");
        rename("temp.txt", "medicine.txt");
        
        if(!found) {
            printf("\nMedicine ID %s not found!\n", search_id);
        }
    }
    
    fprintf(bill_file, "---------------------------------------------------------------\n");
    fprintf(bill_file, "TOTAL: Rs.%.2f\n", total_amt);
    fprintf(bill_file, "=================================\n");
    fclose(bill_file);
    
    printf("\n=================================\n");
    printf("Sell process complete!\n");
    printf("Total Bill: Rs.%.2f\n", total_amt);
    printf("Bill saved to %s\n", filename);
    printf("Press Any key to Continue...");
    while(getchar() != '\n'); 
    getch(); 
    clearscreen();
}

void update(){
    state = 10;
    struct Medicine med;
    FILE *Pill = fopen("medicine.txt", "r");
    FILE *Temp = fopen("temp.txt", "w");
    if(Pill == NULL || Temp == NULL){
        printf("The File can't be opened!!!\n");
        if (Pill) fclose(Pill);
        if (Temp) fclose(Temp);
        return;
    }
    int search_state = 1,choice;
    char search[256];
    char line[256];
    char line_copy[256];
    printf("What Do you Want to Update: \n\n1.ID\n2.Medicine Name\n3.Medicine Price\n4.Medicine Stock\n5.Expire Date\n\n");
    while (1) {
        printf("Enter your Choice: ");
        if (scanf("%d", &choice) == 1 && choice >= 1 && choice <= 5) {
            break;
        }
        printf("Invalid choice! Please enter a number between 1 and 5.\n");
        while (getchar() != '\n');
    }
    state = 1;
    
    clearscreen();
    printf("Enter the ID: ");
    scanf("%s", search);
    while (getchar() != '\n'); 

    while(fgets(line,sizeof(line),Pill) != NULL){
        strcpy(line_copy, line);
        char *token = strtok(line,"|");
        if(token != NULL && strcmp(token,search) == 0){
            search_state = 0;
            med.id = atoi(token);
            token = strtok(NULL, "|"); if(token) strcpy(med.name, token);
            token = strtok(NULL, "|"); if(token) med.price = atof(token);
            token = strtok(NULL, "|"); if(token) med.stock = atoi(token);
            token = strtok(NULL, "/"); if(token) med.expireMonth = atoi(token);
            token = strtok(NULL, "\n"); if(token) med.expireYear = atoi(token);

            switch (choice) {
                case 1:
                    while (1) {
                        printf("1. Enter the new ID number: ");
                        if (scanf("%d", &med.id) != 1) {
                            printf("Invalid ID number!!\n");
                            while (getchar() != '\n');
                        } else {
                            while (getchar() != '\n');
                            break;
                        }
                    }
                    state = 1;
                    break;
                case 2:
                    while (1) {
                        printf("2. Enter the new Medicine Name: ");
                        fgets(med.name, sizeof(med.name), stdin);

                        if (strlen(med.name) > 2) {
                            med.name[strcspn(med.name, "\n")] = '\0';
                            break;
                        } else {
                            printf("Invalid a name should contain minimum character of 2 letter\n");
                        }
                    }
                    state = 1;
                    break;
                case 3:
                    while (1) {
                        printf("3. Enter the new Medicine price: ");

                        if (scanf("%f", &med.price) != 1 || med.price <= 0) {
                            printf("Invalid!!\n");
                            while (getchar() != '\n');
                        } else {
                            break;
                        }
                    }
                    state = 1;
                    break;
                case 4:
                    while (1) {
                        printf("4. Enter the new Number of Stocks: ");

                        if (scanf("%d", &med.stock) != 1 || med.stock <= 0) {
                            printf("Invalid Please enter a valid stock number.\n");
                            while (getchar() != '\n');
                        } else {
                            break;
                        }
                    }
                    state = 1;
                    break;
                case 5:
                    while (1) {
                        printf("Enter the new Expire Date(M/Y): ");
                        if (scanf("%d/%d", &med.expireMonth, &med.expireYear) != 2) {
                            printf("Invalid Please enter a valid Expire Date.\n");
                            while (getchar() != '\n');
                        } else {
                            if (med.expireMonth < 1 || med.expireMonth > 12) {
                                printf("Invalid Month\n");
                                continue;
                            }
                            if (med.expireYear < 2026) {
                                printf("Invalid Year\n");
                                continue;
                            } else {
                                break;
                            }
                        }
                    }
                    break;
            }
            fprintf(Temp, "%d|%s|%.2f|%d|%d/%d\n", med.id, med.name, med.price, med.stock, med.expireMonth, med.expireYear);
        }else{
            fputs(line_copy, Temp);
        }
    }
    
    fclose(Pill);
    fclose(Temp);

    remove("medicine.txt");
    rename("temp.txt", "medicine.txt");

    if(search_state == 1){
        printf("\nThe Id couldn't be found!\n");
        printf("Press Any Key to Continue.\n");
        getch();
    }else{
        printf("\nSuccessfully updated!\n");
        printf("Press Any Key to Continue.\n");
        getch();
    }
    return;
}
void delete(){
    state = 1;
    FILE *Pill = fopen("medicine.txt", "r");
    FILE *Temp = fopen("temp.txt", "w");
    if(Pill == NULL || Temp == NULL){
        printf("The File can't be opened!!!\n");
        if (Pill){
            fclose(Pill);  
        } 
        if (Temp) {
            fclose(Temp);
        }
        return;
    }
    int search_state = 1;
    char search[256];
    char line[256];
    char line_copy[256];
    printf("Enter the ID: ");
    scanf("%255s", search);
    while (getchar() != '\n'); 
    while(fgets(line,sizeof(line),Pill) != NULL){
        strcpy(line_copy, line);
        char *token = strtok(line,"|");
        if(token != NULL && strcmp(token,search) == 0){
            search_state = 0; 
        }else{
            fputs(line_copy, Temp);
        }
    }
    
    fclose(Pill);
    fclose(Temp);

    remove("medicine.txt");
    rename("temp.txt", "medicine.txt");

    if(search_state == 1){
        printf("\nThe Id couldn't be found!\n");
        printf("Press Any key to Continue.\n");
        getch();
    }else{
        printf("\nSuccessfully deleted!\n");
        printf("Press Any key to Continue.\n");
        getch();
    }
    return;
}


int main() {
    while (1)
    {
        if (state == 0){
            login();
        }else if(state == 1) {
            int choice;
            printf("=============================\n");
            printf("Pharmacy Management\n");
            printf("=============================\n");
            printf("1. Add Medicine\n");
            printf("2. View all available medicine\n");
            printf("3. Search specific Medicine\n");
            printf("4. Sell your Medicine\n");
            printf("5. Update Medicine\n");
            printf("6. Delete Medicine\n");
            printf("7. Quit\n");
            printf("Enter your choice: ");
            if ((scanf("%d", &choice)) != 1) {
                puts("\n\nInvalid Input \n\"Termination Session\"");
                continue;

            } else {

                switch (choice) {
                case 1:
                    clearscreen();
                    create();
                    break;
                case 2:
                    clearscreen();
                    view();
                    break;
                case 3:
                    clearscreen();
                    search();
                    break;
                case 4:
                    clearscreen();
                    sell();
                    break;
                case 5:
                    clearscreen();
                    update(); 
                    break;
                case 6:
                    clearscreen();
                    delete();
                    break;
                case 7:
                    puts("Thanks for visiting our Dashboard <3!!");
                    return 0;
                default:
                printf("Invalid Input please try again\n");
                break;
                }
            }
        }
        else {
        return 0;
        }
        }    
}


