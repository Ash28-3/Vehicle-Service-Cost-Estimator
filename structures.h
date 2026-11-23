
#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX_CUSTOMERS 100
#define MAX_VEHICLES 100
#define MAX_SERVICES 20
#define MAX_SELECTED_SERVICES 20
#define MAX_ORDERS 100
#define MAX_INVOICES 100

typedef struct
{
    int customerId;
    char name[50];
    char phone[15];
    char address[100];

} Customer;


typedef struct
{
    int vehicleId;
    int customerId;

    char registrationNumber[20];
    char category[20];
    char manufacturer[40];
    char model[40];
    char fuelType[20];

    int manufacturingYear;

} Vehicle;


/* Service catalogue structure */

typedef struct
{
    int serviceId;
    char name[60];
    char category[30];

    double partsCost;
    double labourCost;

} Service;


/* Selected service structure */

typedef struct
{
    int serviceId;
    int quantity;

} SelectedService;


/* Service order structure */

typedef struct
{
    int orderId;
    int vehicleId;

    int selectedCount;

    SelectedService selectedServices[MAX_SELECTED_SERVICES];

} ServiceOrder;


/* Invoice structure */

typedef struct
{
    int invoiceId;
    int orderId;

    char invoiceDateTime[30];
    char fileName[50];

} Invoice;

#endif
