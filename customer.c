
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "customer.h"
#include "input.h"


void registerCustomer(Customer customers[], int *customerCount)
{
    if (*customerCount >= MAX_CUSTOMERS)
    {
        printf("\nCustomer storage is full!\n");
        return;
    }

    Customer *newCustomer = &customers[*customerCount];

    newCustomer->customerId = *customerCount + 1;

    printf("\n========== CUSTOMER REGISTRATION ==========\n");

    readString("Enter customer name: ",
               newCustomer->name,
               sizeof(newCustomer->name));

    while (1)
    {
        readString("Enter 10-digit mobile number: ",
                   newCustomer->phone,
                   sizeof(newCustomer->phone));

        int valid = strlen(newCustomer->phone) == 10;

        for (int i = 0; newCustomer->phone[i] != '\0'; i++)
        {
            if (!isdigit((unsigned char)newCustomer->phone[i]))
            {
                valid = 0;
                break;
            }
        }

        if (valid)
        {
            break;
        }

        printf("Invalid mobile number! Enter exactly 10 digits.\n");
    }

    readString("Enter customer address: ",
               newCustomer->address,
               sizeof(newCustomer->address));

    (*customerCount)++;

    printf("\nCustomer registered successfully!\n");
    printf("Customer ID: %d\n", newCustomer->customerId);
}


void displayCustomers(const Customer customers[], int customerCount)
{
    if (customerCount == 0)
    {
        printf("\nNo customer records available.\n");
        return;
    }

    printf("\n================ CUSTOMER RECORDS ================\n");

    for (int i = 0; i < customerCount; i++)
    {
        printf("\nCustomer ID : %d\n", customers[i].customerId);
        printf("Name        : %s\n", customers[i].name);
        printf("Mobile      : %s\n", customers[i].phone);
        printf("Address     : %s\n", customers[i].address);

        printf("--------------------------------------------------\n");
    }
}


Customer *findCustomerById(Customer customers[], int customerCount, int id)
{
    for (int i = 0; i < customerCount; i++)
    {
        if (customers[i].customerId == id)
        {
            return &customers[i];
        }
    }

    return NULL;
}
