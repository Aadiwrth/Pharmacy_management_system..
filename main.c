#include <stdio.h>
#include <string.h>
//Initializing state variable
int state = 0,check = 0; 
char temp[50];
int main();
void clearscreen(){
    //printf("\033[2J\033[H"); // "\033 or \x18 is suppose to represent ESC character and 2 selects all the character in the terminal then J erases the screen and another part \033 or \x18 is again represent ESC character and [H represent HOME destination/positon so it moves the cursor to row and colum 1 also the ESC start's the ANSI command "[" is the control sequence introducer(CSI)  "
    printf("TEST");
}

struct Medicine {
    int id;
    char name[50];
    float price;
    int stock;
    int expireMonth;
    int expireYear;

};
void login(){
    FILE *pass = fopen("Cred.bin","rb+");
    char temp_pass[100],temp_file_pass[100];
    if(pass == NULL)
    {
        pass = fopen("Cred.bin", "wb");
    }

    if(fgets(temp_file_pass,sizeof(temp_file_pass),pass) == NULL){
            printf("=============================\n");
            printf("Register System\n");
            printf("=============================\n");
            printf("Enter Your New Password: ");
            fgets(temp_file_pass,sizeof(temp_file_pass),stdin);
            fwrite(temp_file_pass,sizeof(char),sizeof(temp_file_pass),pass);
            fclose(pass);
            main();
            return;
    }else {
    printf("=============================\n");
    printf("Login System\n");
    printf("=============================\n");
    printf("Enter your Password: ");
    fgets(temp_pass,sizeof(temp_pass),stdin);
    }
    if(strcmp(temp_file_pass,temp_pass) == 0){
            state = 1;
            clearscreen();
            fclose(pass);
            main();
            return;
    }else {
            printf("Invalid password\n");
            printf("Press Enter to Continue");
            getchar();
            clearscreen();
            login();
            fclose(pass);
   }
}

// Function for the first option menu where you can Add medicine
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
            state = 2;
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
            printf("Invalid Please enter a valid stock number.\n");
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

    fprintf(Pill, "%d|%s|%.2f|%d|%d/%d\n",
            med.id,
            med.name,
            med.price,
            med.stock,
            med.expireMonth,
            med.expireYear);

    clearscreen();

    printf("===YOUR INPUT DETAILS===\n");
    printf("Id: %d\n", med.id);
    printf("Medicine Name: %s\n", med.name);
    printf("Price: %.2f\n", med.price);
    printf("Stocks: %d\n", med.stock);
    printf("Expire Date: %d/%d\n\n", med.expireMonth, med.expireYear);

    printf("\nPress Enter to Continue\n");

    while (getchar() != '\n');
    fgets(temp, sizeof(temp), stdin);

    clearscreen();
    fclose(Pill);
    main();
}void view() {
    FILE *Pill = fopen("medicine.txt", "r");
    if (Pill == NULL) {
        printf("The File can't be opened!!!\n");
        return;
    }
    char line[256];

    printf("======= Your Medicine Details =======\n");
    while (fgets(line, sizeof(line), Pill) != NULL) {
        int field = 0;
        char *token = strtok(line, "|");
        while (token != NULL) {
            switch (field) {
                case 0:
                    printf("ID: %s\n", token);
                    break;
                case 1:
                    printf("Name: %s\n", token);
                    break;
                case 2:
                    printf("Price: %s\n", token);
                    break;
                case 3:
                    printf("Stock Left: %s\n", token);
                    break;
                case 4:
                    printf("Expire Date: %s\n", token);
                    break;
            }
            token = strtok(NULL, "|");
            field++;
        }
        printf("=================================\n");
    }
    gets(temp);
    fclose(Pill);
    printf("Press Enter to Continue.");
    gets(temp);
    clearscreen();
    main();

}// the strtok() function in C is basically a std function used to split a string into a smaller peice
void search() {
    FILE *Pill = fopen("medicine.txt", "r");
    if(Pill == NULL){
         printf("The File can't be opened!!!\n");
         return;
    }
    int search_state;
    char search[256];
    char line[256];
    printf("Enter the ID: ");
    scanf("%s",search);
    while(fgets(line,sizeof(line),Pill) != NULL){
        char *token = strtok(line,"|");
        search_state = 0;
        int field = 0;
        if(strcmp(token,search) == 0){
            while (token != NULL) {
            switch (field) {
                case 0:
                    printf("ID: %s\n", token);
                    break;
                case 1:
                    printf("Name: %s\n", token);
                    break;
                case 2:
                    printf("Price: %s\n", token);
                    break;
                case 3:
                    printf("Stock Left: %s\n", token);
                    break;
                case 4:
                    printf("Expire Date: %s\n", token);
                    break;
            }
            token = strtok(NULL, "|");
            field++;
        }
            gets(temp);
            fclose(Pill);
            printf("Press Enter to Continue.");
            gets(temp);
            clearscreen();
            break;
        }else{
            search_state = 1;

        }
    }
    if(search_state == 1){
        printf("The Id couldn;t be found!!\n");
    }
        fclose(Pill);
        main();
}
void sell(){}
void update(){
    state = 10;
    struct Medicine med;
    FILE *Pill = fopen("medicine.txt", "a+");
    if(Pill == NULL){
        printf("The File can't be opened!!!\n");
        return;
    }
    int search_state,choice;
    char search[256];
    char line[256];
    printf("What Do you Want to Update: \n\n1.ID\n2.Medicine Name\n3.Medicine Price\n4.Medicine Stock\n5.Expire Date\n\n");
    while (state == 10) {
    printf("Enter your Choice: ");
    scanf("%d",&choice);
    state = 1;
    }
    
    clearscreen();
    printf("Enter the ID: ");
    scanf("%s", search);
    while(fgets(line,sizeof(line),Pill) != NULL){
        char *token = strtok(line,"|");
        search_state = 0;
        if(strcmp(token,search) == 0){
            switch (choice) {
                case 1:
                    state = 1;
                    while (state == 1) {
                        printf("1. Enter ID number: ");
                        if (scanf("%d", &med.id) != 1) {
                            printf("Invalid ID number!!\n");
                            while (getchar() != '\n');
                        } else {
                            while (getchar() != '\n');
                        }
                    }
                case 2:
                    state = 2;
                    while (state == 2) {
                        printf("2. Enter Medicine Name: ");
                        fgets(med.name, sizeof(med.name), stdin);

                        if (strlen(med.name) > 2) {
                            state = 3;
                        } else {
                            printf("Invalid a name should contain minimum character of 2 letter\n");
                        }
                    }
                case 3:
                    state = 3;
                    while (state == 3) {
                        printf("3. Enter your Medicine price: ");

                        if (scanf("%f", &med.price) != 1 || med.price <= 0) {
                            printf("Invalid!!\n");
                            while (getchar() != '\n');
                        } else {
                            state = 4;
                        }
                    }
                case 4:
                    state = 4;
                    while (state == 4) {
                        printf("4. Enter Number of Stocks: ");

                        if (scanf("%d", &med.stock) != 1 || med.stock <= 0) {
                            printf("Invalid Please enter a valid stock number.\n");
                            while (getchar() != '\n');
                        } else {
                            state = 5;
                        }
                    }
                case 5:
                    state = 5;
                    while (state == 5) {
                        printf("Enter Expire Date(M/Y): ");
                        if (scanf("%d/%d", &med.expireMonth, &med.expireYear) != 2) {
                            printf("Invalid Please enter a valid stock number.\n");
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
                default:
                    printf("Invalid Choice Try again\n");
                    state = 10;
            }
           
                break;
        }else{
            search_state  = 1;
        }
    }
    if(search_state = 1){
        printf("The Id couldn't be found!\n");
    }
    fclose(Pill);
    main();

}
void delete(){}
int quit() {
  puts("Thank you for visiting our Dashboard <3");
  return 0;
}
void menu(){

}
int main() {
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
            quit();
            break;
        default:
          printf("Invalid Input please try again\n");
          main();
        }
      }
      return 0;
    }
    else {
      return 0;
    }
    
}


