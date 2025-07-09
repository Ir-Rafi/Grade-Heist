#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Account {
    int accountNumber;
    char name[100];
    float balance;
};
void createAccount(){
    struct Account a;
    printf("Enter Account Number: ");
    scanf("%d", &a.accountNumber);
    printf("Enter Name: ");
    scanf("%99s", a.name);
    a.balance = 0.0;
    printf("Enter the initial deposit amount: ");
    scanf("%f", &a.balance);
    char filename[100];
    snprintf(filename, sizeof(filename),"account_%d.txt",a.accountNumber);
    FILE *fptr=fopen(filename,"w");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        return;
    }
    else{
        fprintf(fptr, "Account Number: %d\n", a.accountNumber);
        fprintf(fptr, "Name: %s\n", a.name);
        char filenameBalance[100];
        snprintf(filenameBalance, sizeof(filenameBalance), "balance_%d.txt", a.accountNumber);
        FILE* balanceFile = fopen(filenameBalance, "w");
        if (balanceFile == NULL) {
            printf("Error opening balance file!\n");
            fclose(fptr);
            return;
        }
        fprintf(balanceFile, "Balance: %.2f\n", a.balance);
        //fclose(balanceFile);
    FILE *listFile = fopen("accounts_list.txt", "a");
    if (!listFile) {
        printf("Error opening  list");
        return;
    }
    fprintf(listFile,"%d %s %s\n",a.accountNumber,a.name,filenameBalance);
    fclose(listFile);
    fclose(balanceFile);
        fclose(fptr);
        printf("Account created successfully!\n");
    }
};
void viewAccount(){
    struct Account a;
    printf("Enter Account Number to view: ");
    scanf("%d", &a.accountNumber);
    char filename[100];
    snprintf(filename, sizeof(filename), "account_%d.txt", a.accountNumber);
    FILE* fptr=fopen(filename, "r");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        return;
    }
    char ch;
    while((ch = fgetc(fptr)) != EOF) {
        printf("%c", ch);
    }
    printf("\n");
    fclose(fptr);
    char balanceFilename[100];
    snprintf(balanceFilename, sizeof(balanceFilename), "balance_%d.txt", a.accountNumber);
    FILE* balanceFile = fopen(balanceFilename, "r");
    if (balanceFile == NULL){
        printf("Error opening balance file!\n");
        return;
    }
    float balance;
    fscanf(balanceFile, "Balance: %f", &balance);
    fclose(balanceFile);
    printf("Balance: %.2f\n", balance);
    printf("Account details displayed successfully!\n");
};
void deposit(){
    struct Account a;
    printf("Enter Account Number to deposit into: ");
    scanf("%d", &a.accountNumber);
    char filename[100];
    snprintf(filename, sizeof(filename), "account_%d.txt", a.accountNumber);
    FILE* balanceFile = fopen(filename, "r+");
    if (balanceFile == NULL) {
        printf("Error opening balance file!\n");
        return;
    }
    float depositAmount;
    printf("Enter deposit amount: ");
    scanf("%f", &depositAmount);
    
    float currentBalance;
    fscanf(balanceFile, "Balance: %f", &currentBalance);
    
    currentBalance += depositAmount;
    
    rewind(balanceFile);
    fprintf(balanceFile, "Balance: %.2f\n", currentBalance);
    fclose(balanceFile);
    
    printf("Deposit successful! New balance: %.2f\n", currentBalance);
};
void withdraw(){
    struct Account a;
    printf("Enter Account Number to withdraw from: ");
    scanf("%d", &a.accountNumber);
    char filename[100];
    snprintf(filename, sizeof(filename), "account_%d.txt", a.accountNumber);
    FILE* balanceFile = fopen(filename, "r+");
    if (balanceFile == NULL) {
        printf("Error opening balance file!\n");
        return;
    }
    float withdrawAmount;
    printf("Enter withdrawal amount: ");
    scanf("%f", &withdrawAmount);
    
    float currentBalance;
    fscanf(balanceFile, "Balance: %f", &currentBalance);
    
    if (withdrawAmount > currentBalance) {
        printf("Insufficient Amount!!!\n");
        fclose(balanceFile);
        return;
    }
    
    currentBalance -= withdrawAmount;
    
    rewind(balanceFile);
    fprintf(balanceFile, "Balance: %.2f\n", currentBalance);
    fclose(balanceFile);
    
    printf("Withdrawal successful! New balance: %.2f\n", currentBalance);
};
void listAccounts(){
    FILE* listFile = fopen("accounts_list.txt", "r");
    if (listFile == NULL) {
        printf("Error opening accounts list file!\n");
        return;
    }
    char filename[100];
    printf("\n=== List of Accounts ===\n");
    while (fgets(filename, sizeof(filename), listFile)) {
        filename[strcspn(filename, "\n")] = 0; 
        printf("%s\n", filename);
    }
    fclose(listFile);
    printf("Accounts listed successfully!\n");
};
void deleteAccount(){
    struct Account a;
    printf("Enter Account Number to delete: ");
    scanf("%d", &a.accountNumber);
    char acctFile[100], balFile[100];
    snprintf(acctFile, sizeof(acctFile), "account_%d.txt", a.accountNumber);
    snprintf(balFile, sizeof(balFile), "balance_%d.txt", a.accountNumber);

    /*if (remove(acctFile) == 0)
        printf("Account file deleted successfully!\n");
    else
        printf("Error deleting account file!\n");

    if (remove(balFile) == 0)
        printf("Balance file deleted successfully!\n");
    else
        printf("Error deleting balance file!\n");*/
        remove(acctFile);
        remove(balFile);
    FILE *listFile = fopen("accounts_list.txt", "r");
    if (!listFile) {
        printf("Error opening accounts list file!\n");
        return;
    }
    FILE *tempFile = fopen("temp_accounts_list.txt", "w");
    if (!tempFile) {
        printf("Error creating temporary file!\n");
        fclose(listFile);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), listFile)){
        int num;
        char name[100], balName[100];
        if (sscanf(line, "%d %99s %99s", &num, name, balName) == 3) {
            if (num == a.accountNumber) {
                continue;
            }
        }
        fputs(line, tempFile);
    }

    fclose(listFile);
    fclose(tempFile);
    remove("accounts_list.txt");
    rename("temp_accounts_list.txt", "accounts_list.txt");
    printf("Account removed from the Bank!\n");
}

int main() {
    int choice;
    do {
        printf("\n=== Banking System Menu ===\n");
        printf("1. Create Account\n");
        printf("2. View Account\n");
        printf("3. Deposit\n");
        printf("4. Withdraw\n");
        printf("5. List All Accounts\n");
        printf("6. Delete Account\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: viewAccount(); break;
            case 3: deposit(); break;
            case 4: withdraw(); break;
            case 5: listAccounts(); break;
            case 6: deleteAccount(); break;
            case 7: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 7);

    return 0;
}
// Function Definitions
