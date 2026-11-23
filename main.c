
#include <stdio.h>

#include "structures.h"
#include "input.h"
#include "customer.h"
#include "vehicle.h"
#include "services.h"
#include "estimation.h"
#include "invoice.h"
#include "records.h"
#include "search.h"
#include "history.h"


/* Save all application data */

static void saveApplicationData(
    const Customer customers[],
    int customerCount,
    const Vehicle vehicles[],
    int vehicleCount,
    const ServiceOrder orders[],
    int orderCount,
    const Invoice invoices[],
    int invoiceCount
)
{
    if (saveAllRecords(
            customers,
            customerCount,
            vehicles,
            vehicleCount,
            orders,
            orderCount,
            invoices,
            invoiceCount
        ))
    {
        printf("\nApplication data saved successfully.\n");
    }
    else
    {
        printf("\nWARNING: Application data could not be saved!\n");
    }
}


/* Customer Management Menu */

void customerMenu(
    Customer customers[],
    int *customerCount,
    const Vehicle vehicles[],
    int vehicleCount,
    const ServiceOrder orders[],
    int orderCount,
    const Invoice invoices[],
    int invoiceCount
)
{
    int choice;

    do
    {
        printf("\n========== CUSTOMER MANAGEMENT ==========\n");
        printf("1. Register New Customer\n");
        printf("2. View All Customers\n");
        printf("3. Back to Main Menu\n");

        choice = readInteger("Enter your choice: ", 1, 3);

        switch (choice)
        {
            case 1:
                registerCustomer(customers, customerCount);

                saveApplicationData(
                    customers, *customerCount,
                    vehicles, vehicleCount,
                    orders, orderCount,
                    invoices, invoiceCount
                );
                break;

            case 2:
                displayCustomers(customers, *customerCount);
                break;

            case 3:
                printf("\nReturning to main menu...\n");
                break;
        }

    } while (choice != 3);
}


/* Vehicle Management Menu */

void vehicleMenu(
    Vehicle vehicles[],
    int *vehicleCount,
    Customer customers[],
    int customerCount,
    const ServiceOrder orders[],
    int orderCount,
    const Invoice invoices[],
    int invoiceCount
)
{
    int choice;

    do
    {
        printf("\n========== VEHICLE MANAGEMENT ==========\n");
        printf("1. Register New Vehicle\n");
        printf("2. View All Vehicles\n");
        printf("3. Back to Main Menu\n");

        choice = readInteger("Enter your choice: ", 1, 3);

        switch (choice)
        {
            case 1:
                registerVehicle(
                    vehicles,
                    vehicleCount,
                    customers,
                    customerCount
                );

                saveApplicationData(
                    customers, customerCount,
                    vehicles, *vehicleCount,
                    orders, orderCount,
                    invoices, invoiceCount
                );
                break;

            case 2:
                displayVehicles(vehicles, *vehicleCount);
                break;

            case 3:
                printf("\nReturning to main menu...\n");
                break;
        }

    } while (choice != 3);
}


/* Service Management Menu */

void serviceMenu(
    Vehicle vehicles[],   //sizeof(Vehicle)
    int vehicleCount,
    ServiceOrder orders[],
    int *orderCount,
    const Customer customers[],
    int customerCount,
    const Invoice invoices[],
    int invoiceCount
)
{
    int choice;

    do
    {
        printf("\n========== SERVICE MANAGEMENT ==========\n");
        printf("1. View Service Catalogue\n");
        printf("2. Create Service Order\n");
        printf("3. View Service Orders\n");
        printf("4. Back to Main Menu\n");

        choice = readInteger("Enter your choice: ", 1, 4);

        switch (choice)
        {
            case 1:
                displayServiceCatalogue();
                break;

            case 2:
                createServiceOrder(
                    vehicles,
                    vehicleCount,
                    orders,
                    orderCount
                );

                saveApplicationData(
                    customers, customerCount,
                    vehicles, vehicleCount,
                    orders, *orderCount,
                    invoices, invoiceCount
                );
                break;

            case 3:
                displayServiceOrders(
                    vehicles,
                    vehicleCount,
                    orders,
                    *orderCount
                );
                break;

            case 4:
                printf("\nReturning to main menu...\n");
                break;
        }

    } while (choice != 4);
}


/* Main Application */

int main(void)
{
    Customer customers[MAX_CUSTOMERS] = {0};
    Vehicle vehicles[MAX_VEHICLES] = {0};
    ServiceOrder orders[MAX_ORDERS] = {0};
    Invoice invoices[MAX_INVOICES] = {0};

    int customerCount = 0;
    int vehicleCount = 0;
    int orderCount = 0;
    int invoiceCount = 0;

    int choice;

    /* Load previously saved application data */

    if (!loadAllRecords(
            customers,
            &customerCount,
            vehicles,
            &vehicleCount,
            orders,
            &orderCount,
            invoices,
            &invoiceCount
        ))
    {
        printf("\nApplication cannot start because database loading failed.\n");
        printf("Do not delete or overwrite the existing database file.\n");

        return 1;
    }

    printf("\nVehicle Service Cost Estimator started successfully.\n");

    do
    {
        printf("\n");
        printf("================================================\n");
        printf("          VEHICLE SERVICE COST ESTIMATOR\n");
        printf("================================================\n");

        printf("\n");
        printf("1. Customer Management\n");
        printf("2. Vehicle Management\n");
        printf("3. Service Management\n");
        printf("4. Estimate Service Cost\n");
        printf("5. Generate Invoice\n");
        printf("6. View Generated Invoices\n");
        printf("7. Search and Record Management\n");
        printf("8. Exit\n");
        printf("9. Vehicle Service History\n");

        printf("\n================================================\n");

        choice = readInteger("Enter your choice: ", 1, 9);

        switch (choice)
        {
            case 1:
                customerMenu(
                    customers,
                    &customerCount,
                    vehicles,
                    vehicleCount,
                    orders,
                    orderCount,
                    invoices,
                    invoiceCount
                );
                break;

            case 2:
                vehicleMenu(
                    vehicles,
                    &vehicleCount,
                    customers,
                    customerCount,
                    orders,
                    orderCount,
                    invoices,
                    invoiceCount
                );
                break;

            case 3:
                serviceMenu(
                    vehicles,
                    vehicleCount,
                    orders,
                    &orderCount,
                    customers,
                    customerCount,
                    invoices,
                    invoiceCount
                );
                break;

            case 4:
                generateCostEstimate(
                    vehicles,
                    vehicleCount,
                    orders,
                    orderCount
                );
                break;

            case 5:
            {
                int previousInvoiceCount = invoiceCount;

                generateInvoice(
                    customers,
                    customerCount,
                    vehicles,
                    vehicleCount,
                    orders,
                    orderCount,
                    invoices,
                    &invoiceCount
                );

                /* Save only when a new invoice was generated */

                if (invoiceCount > previousInvoiceCount)
                {
                    saveApplicationData(
                        customers, customerCount,
                        vehicles, vehicleCount,
                        orders, orderCount,
                        invoices, invoiceCount
                    );
                }

                break;
            }

            case 6:
                displayInvoices(invoices, invoiceCount);
                break;

            case 7:
                searchMenu(
                    customers,
                    customerCount,
                    vehicles,
                    vehicleCount,
                    orders,
                    orderCount,
                    invoices,
                    invoiceCount
                );
                break;

            case 8:
                saveApplicationData(
                    customers, customerCount,
                    vehicles, vehicleCount,
                    orders, orderCount,
                    invoices, invoiceCount
                );

                printf("\nThank you for using Vehicle Service Cost Estimator!\n");
                break;

            case 9:
                displayVehicleServiceHistory(
                    customers,
                    customerCount,
                    vehicles,
                    vehicleCount,
                    orders,
                    orderCount,
                    invoices,
                    invoiceCount
                );
                break;
        }

    } while (choice != 8);

    return 0;
}
