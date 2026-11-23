
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "search.h"
#include "services.h"

/* Read a line of text safely. */
static void readSearchText(const char *prompt, char text[], int size)
{
    int ch;

    printf("%s", prompt);

    if (fgets(text, size, stdin) == NULL)
    {
        text[0] = '\0';
        return;
    }

    text[strcspn(text, "\n")] = '\0';

    /* Clear remaining input if the entered text was too long. */
    if (strchr(text, '\n') == NULL)
    {
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
            /* Discard extra characters. */
        }
    }
}

/* Read an integer safely. */
static int readSearchInteger(const char *prompt)
{
    char input[30];
    char *endPointer;
    long value;

    while (1)
    {
        readSearchText(prompt, input, sizeof(input));

        if (input[0] == '\0')
        {
            printf("Input cannot be empty. Try again.\n");
            continue;
        }

        value = strtol(input, &endPointer, 10);

        while (isspace((unsigned char)*endPointer))
        {
            endPointer++;
        }

        if (*endPointer == '\0' && endPointer != input &&
            value >= 1 && value <= 2147483647L)
        {
            return (int)value;
        }

        printf("Invalid input. Enter a valid positive integer.\n");
    }
}

/* Compare two strings without considering letter case. */
static int equalsIgnoreCase(const char first[], const char second[])
{
    while (*first != '\0' && *second != '\0')
    {
        if (tolower((unsigned char)*first) !=
            tolower((unsigned char)*second))
        {
            return 0;
        }

        first++;
        second++;
    }

    return *first == '\0' && *second == '\0';
}

/* Check whether a string contains another string, ignoring case. */
static int containsIgnoreCase(const char text[], const char search[])
{
    size_t textLength = strlen(text);
    size_t searchLength = strlen(search);

    if (searchLength == 0 || searchLength > textLength)
    {
        return 0;
    }

    for (size_t i = 0; i <= textLength - searchLength; i++)
    {
        size_t j = 0;

        while (j < searchLength &&
               tolower((unsigned char)text[i + j]) ==
               tolower((unsigned char)search[j]))
        {
            j++;
        }

        if (j == searchLength)
        {
            return 1;
        }
    }

    return 0;
}

static const Customer *findCustomerById(
    const Customer customers[],
    int customerCount,
    int customerId)
{
    for (int i = 0; i < customerCount; i++)
    {
        if (customers[i].customerId == customerId)
        {
            return &customers[i];
        }
    }

    return NULL;
}

static const Vehicle *findVehicleById(
    const Vehicle vehicles[],
    int vehicleCount,
    int vehicleId)
{
    for (int i = 0; i < vehicleCount; i++)
    {
        if (vehicles[i].vehicleId == vehicleId)
        {
            return &vehicles[i];
        }
    }

    return NULL;
}

static const ServiceOrder *findOrderById(
    const ServiceOrder orders[],
    int orderCount,
    int orderId)
{
    for (int i = 0; i < orderCount; i++)
    {
        if (orders[i].orderId == orderId)
        {
            return &orders[i];
        }
    }

    return NULL;
}

/* Display customer details. */
static void displayCustomerDetails(const Customer *customer)
{
    printf("\n----------------------------------------\n");
    printf("CUSTOMER DETAILS\n");
    printf("----------------------------------------\n");
    printf("Customer ID : %d\n", customer->customerId);
    printf("Name        : %s\n", customer->name);
    printf("Mobile      : %s\n", customer->phone);
    printf("Address     : %s\n", customer->address);
    printf("----------------------------------------\n");
}

/* Display vehicle details and its registered owner. */
static void displayVehicleDetails(
    const Vehicle *vehicle,
    const Customer customers[],
    int customerCount)
{
    const Customer *owner;

    printf("\n----------------------------------------\n");
    printf("VEHICLE DETAILS\n");
    printf("----------------------------------------\n");

    printf("Vehicle ID          : %d\n", vehicle->vehicleId);
    printf("Registration Number : %s\n", vehicle->registrationNumber);
    printf("Category            : %s\n", vehicle->category);
    printf("Manufacturer        : %s\n", vehicle->manufacturer);
    printf("Model               : %s\n", vehicle->model);
    printf("Fuel Type           : %s\n", vehicle->fuelType);
    printf("Manufacturing Year  : %d\n", vehicle->manufacturingYear);

    owner = findCustomerById(
        customers, customerCount, vehicle->customerId);

    if (owner != NULL)
    {
        printf("\nREGISTERED OWNER\n");
        printf("Customer ID : %d\n", owner->customerId);
        printf("Name        : %s\n", owner->name);
        printf("Mobile      : %s\n", owner->phone);
    }
    else
    {
        printf("\nRegistered customer record was not found.\n");
    }

    printf("----------------------------------------\n");
}

/* Search customers by ID, partial name, or exact mobile number. */
static void searchCustomer(
    const Customer customers[],
    int customerCount)
{
    int choice;
    int found = 0;
    int customerId;
    char searchText[100];

    printf("\n--- SEARCH CUSTOMER ---\n");
    printf("1. Search by Customer ID\n");
    printf("2. Search by Name\n");
    printf("3. Search by Mobile Number\n");

    choice = readSearchInteger("Enter your choice: ");

    if (choice == 1)
    {
        customerId = readSearchInteger("Enter Customer ID: ");

        for (int i = 0; i < customerCount; i++)
        {
            if (customers[i].customerId == customerId)
            {
                displayCustomerDetails(&customers[i]);
                found = 1;
                break;
            }
        }
    }
    else if (choice == 2)
    {
        readSearchText("Enter customer name or part of the name: ",
                       searchText, sizeof(searchText));

        if (searchText[0] == '\0')
        {
            printf("Search text cannot be empty.\n");
            return;
        }

        for (int i = 0; i < customerCount; i++)
        {
            if (containsIgnoreCase(customers[i].name, searchText))
            {
                displayCustomerDetails(&customers[i]);
                found = 1;
            }
        }
    }
    else if (choice == 3)
    {
        readSearchText("Enter mobile number: ",
                       searchText, sizeof(searchText));

        for (int i = 0; i < customerCount; i++)
        {
            if (strcmp(customers[i].phone, searchText) == 0)
            {
                displayCustomerDetails(&customers[i]);
                found = 1;
            }
        }
    }
    else
    {
        printf("Invalid search option.\n");
        return;
    }

    if (!found)
    {
        printf("\nNo matching customer record was found.\n");
    }
}

/* Search vehicles by ID or registration number. */
static void searchVehicle(
    const Customer customers[],
    int customerCount,
    const Vehicle vehicles[],
    int vehicleCount)
{
    int choice;
    int found = 0;
    int vehicleId;
    char registrationNumber[30];

    printf("\n--- SEARCH VEHICLE ---\n");
    printf("1. Search by Vehicle ID\n");
    printf("2. Search by Registration Number\n");

    choice = readSearchInteger("Enter your choice: ");

    if (choice == 1)
    {
        vehicleId = readSearchInteger("Enter Vehicle ID: ");

        for (int i = 0; i < vehicleCount; i++)
        {
            if (vehicles[i].vehicleId == vehicleId)
            {
                displayVehicleDetails(
                    &vehicles[i], customers, customerCount);

                found = 1;
                break;
            }
        }
    }
    else if (choice == 2)
    {
        readSearchText("Enter Registration Number: ",
                       registrationNumber,
                       sizeof(registrationNumber));

        if (registrationNumber[0] == '\0')
        {
            printf("Registration number cannot be empty.\n");
            return;
        }

        for (int i = 0; i < vehicleCount; i++)
        {
            if (equalsIgnoreCase(
                    vehicles[i].registrationNumber,
                    registrationNumber))
            {
                displayVehicleDetails(
                    &vehicles[i], customers, customerCount);

                found = 1;
            }
        }
    }
    else
    {
        printf("Invalid search option.\n");
        return;
    }

    if (!found)
    {
        printf("\nNo matching vehicle record was found.\n");
    }
}

/* Search a service order and display its related information. */
static void searchServiceOrder(
    const Customer customers[],
    int customerCount,
    const Vehicle vehicles[],
    int vehicleCount,
    const ServiceOrder orders[],
    int orderCount)
{
    int orderId;
    const ServiceOrder *order;
    const Vehicle *vehicle;
    const Customer *customer;

    orderId = readSearchInteger("Enter Service Order ID: ");

    order = findOrderById(orders, orderCount, orderId);

    if (order == NULL)
    {
        printf("\nNo service order with that ID was found.\n");
        return;
    }

    printf("\n========================================\n");
    printf("SERVICE ORDER DETAILS\n");
    printf("========================================\n");

    printf("Order ID : %d\n", order->orderId);

    vehicle = findVehicleById(
        vehicles, vehicleCount, order->vehicleId);

    if (vehicle != NULL)
    {
        displayVehicleDetails(vehicle, customers, customerCount);

        customer = findCustomerById(
            customers, customerCount, vehicle->customerId);

        if (customer != NULL)
        {
            printf("Customer: %s\n", customer->name);
        }
    }
    else
    {
        printf("Linked vehicle record was not found.\n");
    }

    printf("\nSELECTED SERVICES\n");
    printf("----------------------------------------\n");

    if (order->selectedCount <= 0 ||
        order->selectedCount > MAX_SELECTED_SERVICES)
    {
        printf("No valid selected service entries found.\n");
    }
    else
    {
        for (int i = 0; i < order->selectedCount; i++)
        {
            const Service *service =
                getServiceById(order->selectedServices[i].serviceId);

            if (service != NULL)
            {
                printf("%d. %s | Quantity: %d\n",
                       i + 1,
                       service->name,
                       order->selectedServices[i].quantity);
            }
            else
            {
                printf("%d. Unknown service (ID: %d) | Quantity: %d\n",
                       i + 1,
                       order->selectedServices[i].serviceId,
                       order->selectedServices[i].quantity);
            }
        }
    }

    printf("========================================\n");
}

/* Display the saved invoice text file. */
static void displayInvoiceFile(const char fileName[])
{
    FILE *file;
    char line[256];

    file = fopen(fileName, "r");

    if (file == NULL)
    {
        printf("\nInvoice file could not be opened.\n");
        printf("Expected file: %s\n", fileName);
        return;
    }

    printf("\n========== SAVED INVOICE ==========\n");

    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }

    printf("\n===================================\n");

    fclose(file);
}

/* Search invoices by invoice ID. */
static void searchInvoice(
    const Invoice invoices[],
    int invoiceCount)
{
    int invoiceId;
    int found = 0;

    invoiceId = readSearchInteger("Enter Invoice ID: ");

    for (int i = 0; i < invoiceCount; i++)
    {
        if (invoices[i].invoiceId == invoiceId)
        {
            printf("\n----------------------------------------\n");
            printf("INVOICE RECORD\n");
            printf("----------------------------------------\n");
            printf("Invoice ID : %d\n", invoices[i].invoiceId);
            printf("Order ID   : %d\n", invoices[i].orderId);
            printf("Date/Time  : %s\n", invoices[i].invoiceDateTime);
            printf("File Name  : %s\n", invoices[i].fileName);
            printf("----------------------------------------\n");

            displayInvoiceFile(invoices[i].fileName);

            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("\nNo invoice with that ID was found.\n");
    }
}

/* Main search and record management menu. */
void searchMenu(
    const Customer customers[],
    int customerCount,
    const Vehicle vehicles[],
    int vehicleCount,
    const ServiceOrder orders[],
    int orderCount,
    const Invoice invoices[],
    int invoiceCount)
{
    int choice;

    do
    {
        printf("\n========================================\n");
        printf("       SEARCH AND RECORD MANAGEMENT\n");
        printf("========================================\n");
        printf("1. Search Customer\n");
        printf("2. Search Vehicle\n");
        printf("3. Search Service Order\n");
        printf("4. Search Invoice\n");
        printf("5. Return to Main Menu\n");
        printf("========================================\n");

        choice = readSearchInteger("Enter your choice: ");

        switch (choice)
        {
            case 1:
                searchCustomer(customers, customerCount);
                break;

            case 2:
                searchVehicle(
                    customers, customerCount,
                    vehicles, vehicleCount);
                break;

            case 3:
                searchServiceOrder(
                    customers, customerCount,
                    vehicles, vehicleCount,
                    orders, orderCount);
                break;

            case 4:
                searchInvoice(invoices, invoiceCount);
                break;

            case 5:
                printf("Returning to the main menu...\n");
                break;

            default:
                printf("Invalid choice. Select an option from 1 to 5.\n");
        }

    } while (choice != 5);
}
