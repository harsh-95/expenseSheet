#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <fstream>
#include <string>

#include "Menu.h"
#include "Account.h"
#include "User.h"

using namespace std;

void listUsers() {
    Account user;
    int noOfAccounts = user.getNoOfAccounts();
    printf("INDEX   NAME                           SPENT    NO OF ITEMS\n");
    printf("----- x ------------------------------x------x-----------\n");
    user.printAll();
    printf("%5d : BACK TO MAIN MENU\n", noOfAccounts + 1);
    printf("CHOOSE AN OPTION : ");
}

void createAccount() {
    Account user;
    char name[30], password[15], check[15], choice;
    while(1) {
        system("cls");
        printf("Enter your name : ");
        fflush(stdin);
        gets(name);
        user.getAccountData()->setName(name);
        if(user.isAvailable() == 0) {
            printf("THIS USERNAME ALREADY TAKEN. DO WANT TRY ANOTHER ONE (Yes/No) ? : ");
            fflush(stdin);
            scanf("%c", &choice);
            if(choice == 'n' || choice == 'N') {
                printf("\nACCOUNT NOT CREATED!\n");
                return;
            }
        } else {
            break;
        }
    }
    while(1) {
        system("cls");
        printf("USERNAME : %s\n", name);
        printf("CHOOSE PASSWORD : ");
        fflush(stdin);
        gets(password);
        printf("CONFIRM PASSWORD : ");
        fflush(stdin);
        gets(check);
        if(strcmp(password, check) != 0) {
            printf("\nPASSWORD NOT MATCHCING. DO YOU WANT TO TRY AGAIN (Yes/No)? : ");
            fflush(stdin);
            scanf("%c", &choice);
            if(choice == 'n' || choice == 'N') {
                printf("\nACCOUNT NOT CREATED.\n");
                return;
            }
        } else {
            break;
        }

    }
    user.getAccountData()->setName(name);
    user.getAccountData()->setPassword(password);
    user.getAccountData()->setNoOfItems(0);
    user.getAccountData()->setSpent(0);
    user.write();
    User::createDataSource(user.getAccountData()->getName());
    printf("\nACCOUNT CREATED SUCCESSFULLY!\n");
}

int logIn() {
    char password[16], retryChoice;
    int isLoggedIn = 0, noOfAccounts, position;
    Account user;
    noOfAccounts = user.getNoOfAccounts();
    while(1) {
        system("cls");
        listUsers();
        fflush(stdin);
        scanf("%d", &position);
        if(position > noOfAccounts) {
            return 0;
        }
        user.setIndex(position - 1);
        printf("ENTER PASSWORD : ");
        fflush(stdin);
        gets(password);
        if(user.authenticate(password) == 1) {
            printf("Log in Successful\n");
            getch();
            return position;
        } else {
            printf("INCORRECT PASSWORD. DO YOU WANT TO TRY AGAIN (Yes/No) ? : ");
            fflush(stdin);
            scanf("%c", &retryChoice);
            if(retryChoice == 'N' || retryChoice == 'n') {
                return 0;
            }
        }
    }
    getch();
}

void deleteAccount() {
    char password[16], retryChoice;
    Account user;
    int position, noOfAccounts = user.getNoOfAccounts();
    while(1) {
        system("cls");
        listUsers();
        fflush(stdin);
        scanf("%d", &position);

        if (position > noOfAccounts) {
            return;
        }

        user.setIndex(position - 1);
        printf("ENTER PASSWORD : ");
        fflush(stdin);
        gets(password);

        if (user.authenticate(password)) {
            user.erase();
            User::deleteDataSource(user.getAccountData()->getName());
            printf("ACCOUNT DELETED SUCCESSFULLY");
            return;
        } else {
            printf("INCORRECT PASSWORD. DO YOU WANT TO TRY AGAIN (Yes/No) ? : ");
            fflush(stdin);
            scanf("%c", &retryChoice);
            if(retryChoice == 'N' || retryChoice == 'n') {
                return;
            }
        }
    }
}

void addItem(User* user) {
    system("cls");
    user->newItem();
    user->writeItem();
    printf("ITEM RECORDED");
}

void updateAccountDetails(Account* userAccount, User* user) {
    userAccount->getAccountData()->setSpent(user->getSpent());
    userAccount->getAccountData()->setNoOfItems(user->getNoOfItems());
    userAccount->save();
}

void displayYears(vector<int> years) {
    for(int index = 0; index < years.size(); ++index) {
        printf("%d : %d\n", (index + 1), years.at(index));
    }
}

void monhtlyDisplayMenu(User* user, int year) {
    int monthChoice;
    do {
        system("cls");
        displayMonths();
        printf("13 : DISPLAY ALL EXPENSE THIS YEAR\n14 : BACK\n");
        printf("\nCHOOSE AN OPTION : ");
        scanf("%d", &monthChoice);

        if(monthChoice >= 1 && monthChoice <= 12) {
            user->printItemsForYearAndMonth(year, monthChoice);
            getch();
        } else if (monthChoice == 13) {
            user->printItemsForYear(year);
            getch();
        } else {
            monthChoice = 14;
        }
    } while(monthChoice != 14);
}

void yearlyDisplayMenu(User* user) {
    vector<int> years = user->getYears();
    int yearChoice, lastYearPosition, lastYearIndex;
    do {
        system("cls");
        displayYears(years);
        lastYearPosition = years.size();
        printf("%d : DISPLAY ALL RECORDS\n", (lastYearPosition + 1));
        printf("%d : BACK\n", (lastYearPosition + 2));
        printf("\nCHOOSE AN OPTION : ");
        scanf("%d", &yearChoice);
        if(yearChoice >= 1 && yearChoice <= lastYearPosition) {
            monhtlyDisplayMenu(user, years.at(yearChoice - 1));
        } else if (yearChoice == (lastYearPosition + 1)) {
            user->printItemsForAllYears();
            getch();
        } else {
            // year choice can be (lastYearPosition + 2) or greater than that
            // So if it greater than that then...
            yearChoice = lastYearPosition + 2;
        }
        
    } while (yearChoice != (lastYearPosition + 2));
}

void deleteItem(User* user) {
    char deleteChoice;
    system("cls");
    printf("ARE YOUR SURE YOU WANT TO DELETE THE LAST RECORED ITEM?(y/n) : ");
    fflush(stdin);
    scanf("%c", &deleteChoice);
    if(deleteChoice == 'y' || deleteChoice == 'Y') {
        Item item = user->deleteLastRecord();
        printf("LAST RECORD DELETED\n");
        item.print();
    } else {
        printf("NO ACTION TAKEN");
    }
}

void displayMenu(User* user) {
    yearlyDisplayMenu(user);
}