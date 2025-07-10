#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

struct Account {
    int accountNumber;
    char name[100];
    float balance;
};

static map<int, string> passwordMap;

void loadPasswords() {
    ifstream passwordFile("passwords.dat", ios::binary);
    if (passwordFile) {
        int accountNumber;
        string password;
        while (passwordFile.read(reinterpret_cast<char*>(&accountNumber), sizeof(accountNumber))) {
            getline(passwordFile, password);
            passwordMap[accountNumber] = password;
        }
        passwordFile.close();
    }
}

void savePasswords() {

    ofstream passwordFile("passwords.dat", ios::binary);

    for (const auto& entry : passwordMap) {

        passwordFile.write((char*)&entry.first, sizeof(entry.first));

        passwordFile << entry.second << endl;

    }

    passwordFile.close();

}

void createAccount() {
    struct Account a;
    cout << "Enter Account Number: ";
    cin  >> a.accountNumber;

    
    FILE *listFile = fopen("accounts_list.txt", "r");
    if (!listFile) {
        printf("Error opening accounts list!\n");
        return;
    }
    char line[200];
    bool exists = false;
    while (fgets(line, sizeof(line), listFile)) {
        int num;
        char name[100], balanceFile[100];
        if (sscanf(line, "%d %99s %99s", &num, name, balanceFile) == 3) {
            if (num == a.accountNumber) {
                exists = true;
                break;
            }
        }
    }
    fclose(listFile);

    if (exists) {
        printf("Account already exists! Try again!!!\n");
        return;
    }

    
    printf("Enter Your Name within 100 characters: ");
    getchar(); 
    fgets(a.name, sizeof(a.name), stdin);
    size_t len = strlen(a.name);
    if (len > 0 && a.name[len - 1] == '\n')
        a.name[len - 1] = '\0';

    
    a.balance = 0.0f;
    printf("Enter the initial deposit amount: ");
    scanf("%f", &a.balance);

    
    string pwd;
    cout << "Set your account password (no spaces): ";
    cin  >> pwd;
    passwordMap[a.accountNumber] = pwd;

    
    char filename[100];

    snprintf(filename, sizeof(filename), "account_%d.txt", a.accountNumber);

    FILE *fptr = fopen(filename, "w");

    if (!fptr) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(fptr, "Account Number: %d\n", a.accountNumber);
    fprintf(fptr, "Name: %s\n", a.name);
    fclose(fptr);

    
    char filenameBalance[100];

    snprintf(filenameBalance, sizeof(filenameBalance), "balance_%d.bin", a.accountNumber);

    FILE *balanceFile = fopen(filenameBalance, "wb");

    if (!balanceFile) {

        printf("Error opening balance file!\n");

        return;

    }
    fwrite(&a.balance, sizeof(a.balance), 1, balanceFile);

    fclose(balanceFile);

    
    listFile = fopen("accounts_list.txt", "a");
    if (!listFile) {
        printf("Error opening accounts list!\n");
        return;
    }
    fprintf(listFile, "%d %s %s\n", a.accountNumber, a.name, filenameBalance);

    fclose(listFile);

    
    savePasswords();

    printf("Account created successfully!\n");
}

void viewAccount(){
    struct Account a;
    cout << "Enter Account Number to view: ";
    cin  >> a.accountNumber;

    
    string pass;
    cout << "Enter password: ";
    cin >>pass;

    
    if (passwordMap[a.accountNumber] != pass) {
        printf("Incorrect password!\n");
        return;
    }

    
    char filename[100];

    snprintf(filename, sizeof(filename), "account_%d.txt", a.accountNumber);

    FILE* fptr = fopen(filename, "r");

    if (fptr==NULL) {

        printf("Error opening account file!\n");

        return;
    }

    int ch;

    while ((ch = fgetc(fptr)) != EOF) putchar(ch);

    cout<<"\n";

    fclose(fptr);

    
    char balanceFilename[100];

    snprintf(balanceFilename, sizeof(balanceFilename), "balance_%d.bin", a.accountNumber);

    FILE* balanceFile = fopen(balanceFilename, "rb");

    if (!balanceFile) {

        printf("Error opening balance file!\n");

        return;
    }
    float balance;
    fread(&balance, sizeof(balance), 1, balanceFile);
    fclose(balanceFile);

    printf("Balance: %.2f\n", balance);
    printf("Account details displayed successfully!\n");
}


void deposit(){
    int acctNo;
    cout << "Enter Account Number to deposit into: ";
    cin  >> acctNo;

    
    string pass;
    cout << "Enter password: ";
    cin  >> pass;
    if (passwordMap[acctNo] != pass) {
        printf("Incorrect password!\n");
        return;
    }

    
    char balanceFilename[100];
    snprintf(balanceFilename, sizeof(balanceFilename), "balance_%d.bin", acctNo);
    FILE* balanceFile = fopen(balanceFilename, "rb+");
    if (!balanceFile) {
        printf("Error opening balance file!\n");
        return;
    }
    float currentBalance;
    fread(&currentBalance, sizeof(currentBalance), 1, balanceFile);

    
    float depositAmount;
    printf("Enter deposit amount: ");

    scanf("%f", &depositAmount);

    currentBalance += depositAmount;

    
    rewind(balanceFile);
    fwrite(&currentBalance, sizeof(currentBalance), 1, balanceFile);
    fclose(balanceFile);

    printf("Deposit successful! New balance: %.2f\n", currentBalance);
}

void withdraw(){
    int acctNo;
    cout << "Enter Account Number to withdraw from: ";
    cin  >> acctNo;

    
    string pass;
    cout<<"Enter password: ";
    cin>>pass;
    if (passwordMap[acctNo]!=pass) {
        printf("Incorrect password!\n");
        return;
    }

    
    char balanceFilename[100];
    snprintf(balanceFilename, sizeof(balanceFilename), "balance_%d.bin", acctNo);
    FILE* balanceFile = fopen(balanceFilename, "rb+");
    if (!balanceFile) {
        printf("Error opening balance file!\n");
        return;
    }
    float currentBalance;
    
    fread(&currentBalance, sizeof(currentBalance), 1, balanceFile);

    
    float withdrawAmount;
    printf("Enter withdrawal amount: ");
    scanf("%f", &withdrawAmount);
    if (withdrawAmount > currentBalance) {
        printf("Insufficient Amount!!!\n");
        fclose(balanceFile);
        return;
    }
    currentBalance -= withdrawAmount;

    
    rewind(balanceFile);

    fwrite(&currentBalance, sizeof(currentBalance), 1, balanceFile);

    fclose(balanceFile);

    printf("Withdrawal successful! New balance: %.2f\n", currentBalance);
}

void listAccounts(){
    FILE* listFile = fopen("accounts_list.txt", "r");
    if (!listFile) {
        printf("Error opening accounts list file!\n");
        return;
    }
    char filename[200];
    printf("\n=== List of Accounts ===\n");

    while (fgets(filename, sizeof(filename), listFile)) {

        filename[strcspn(filename, "\n")] = '\0';

        printf("%s\n", filename);

    }

    fclose(listFile);

    printf("Accounts listed successfully!\n");
}

void deleteAccount(){
    int acctNo;
    cout << "Enter Account Number to delete: ";
    cin  >> acctNo;

    
    string pwd;
    cout << "Enter password: ";
    cin  >> pwd;
    if (passwordMap[acctNo] != pwd) {
        printf("Incorrect password! Account deletion aborted.\n");
        return;
    }

    
    char acctFile[100], balFile[100];

    snprintf(acctFile, sizeof(acctFile), "account_%d.txt", acctNo);

    snprintf(balFile, sizeof(balFile), "balance_%d.bin", acctNo);

    remove(acctFile);

    remove(balFile);

    passwordMap.erase(acctNo);

    
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
            if (num == acctNo) continue;
        }
        fputs(line, tempFile);
    }

    fclose(listFile);

    fclose(tempFile);

    remove("accounts_list.txt");

    rename("temp_accounts_list.txt", "accounts_list.txt");

   
    savePasswords();

    printf("Account removed from the Bank!\n");
}

int main() {

    loadPasswords();

    int choice;
    do {
        printf("\n=== Banking System Menu ===\n"
               "1. Create Account\n"
               "2. View Account\n"
               "3. Deposit\n"
               "4. Withdraw\n"
               "5. List All Accounts\n"
               "6. Delete Account\n"
               "7. Exit\n"
               "Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount();   break;
            case 2: viewAccount();     break;
            case 3: deposit();         break;
            case 4: withdraw();        break;
            case 5: listAccounts();    break;
            case 6: deleteAccount();   break;
            case 7: printf("Exiting...\n");     break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 7);

    return 0;
}
