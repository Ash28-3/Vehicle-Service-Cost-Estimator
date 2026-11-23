
#include <stdio.h>

#include "history.h"
#include "input.h"
#include "services.h"

/* Find a vehicle using its ID. */
static const Vehicle *getVehicleById(
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

/* Find a customer using their ID. */
static const Customer *getCustomerById(
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

/* Display all invoices associated with a service order. */
static void displayOrderInvoices(
    const Invoice invoices[],
    int invoiceCount,
    int orderId)
{
    int invoiceFound = 0;

    for (int i = 0; i < invoiceCount; i++)
    {
        if (invoices[i].orderId == orderId)
        {
            printf("\nInvoice ID   : %d\n", invoices[i].invoiceId);
            printf("Invoice Date : %s\n", invoices[i].invoiceDateTime);
            printf("Invoice File : %s\n", invoices[i].fileName);

            invoiceFound = 1;
        }
    }

    if (!invoiceFound)
    {
        printf("Invoice       : No invoice generated for this order.\n");
    }
}

/* Display the services included in a particular order. */
static void displayOrderServices(const ServiceOrder *order)
{
    if (order->selectedCount <= 0 ||
        order->selectedCount > MAX_SELECTED_SERVICES)
    {
        printf("No valid service entries are available.\n");
        return;
    }

    for (int i = 0; i < order->selectedCount; i++)
    {
        const Service *service =
            getServiceById(order->selectedServices[i].serviceId);

        if (service != NULL)
        {
            printf("  %d. %-30s Quantity: %d\n",
                   i + 1,
                   service->name,
                   order->selectedServices[i].quantity);
        }
        else
        {
            printf("  %d. Unknown service (ID: %d) Quantity: %d\n",
                   i + 1,
                   order->selectedServices[i].serviceId,
                   order->selectedServices[i].quantity);
        }
    }
}

/* Display the complete service history of a vehicle. */
void displayVehicleServiceHistory(
    const Customer customers[],
    int customerCount,
    const Vehicle vehicles[],
    int vehicleCount,
    const ServiceOrder orders[],
    int orderCount,
    const Invoice invoices[],
    int invoiceCount)
{
    int vehicleId;
    int historyFound = 0;
    int historyCount = 0;

    const Vehicle *vehicle;
    const Customer *customer;

    printf("\n========== VEHICLE SERVICE HISTORY ==========\n");

    if (vehicleCount == 0)
    {
        printf("\nNo vehicles are registered in the system.\n");
        return;
    }

    vehicleId = readInteger("Enter Vehicle ID: ", 1, 2147483647);

    vehicle = getVehicleById(
        vehicles,
        vehicleCount,
        vehicleId
    );

    if (vehicle == NULL)
    {
        printf("\nNo vehicle found with ID %d.\n", vehicleId);
        return;
    }

    customer = getCustomerById(
        customers,
        customerCount,
        vehicle->customerId
    );

    printf("\n============================================\n");
    printf("             VEHICLE INFORMATION\n");
    printf("============================================\n");

    printf("Vehicle ID          : %d\n", vehicle->vehicleId);
    printf("Registration Number : %s\n", vehicle->registrationNumber);
    printf("Category            : %s\n", vehicle->category);
    printf("Manufacturer        : %s\n", vehicle->manufacturer);
    printf("Model               : %s\n", vehicle->model);
    printf("Fuel Type           : %s\n", vehicle->fuelType);
    printf("Manufacturing Year  : %d\n", vehicle->manufacturingYear);

    if (customer != NULL)
    {
        printf("\nOWNER INFORMATION\n");
        printf("Customer ID : %d\n", customer->customerId);
        printf("Name        : %s\n", customer->name);
        printf("Mobile      : %s\n", customer->phone);
    }
    else
    {
        printf("\nOwner information is unavailable.\n");
    }

    printf("\n============================================\n");
    printf("             SERVICE HISTORY\n");
    printf("============================================\n");

    for (int i = 0; i < orderCount; i++)
    {
        if (orders[i].vehicleId == vehicleId)
        {
            historyFound = 1;
            historyCount++;

            printf("\n--------------------------------------------\n");
            printf("SERVICE RECORD %d\n", historyCount);
            printf("--------------------------------------------\n");

            printf("Service Order ID : %d\n", orders[i].orderId);

            printf("\nServices:\n");
            displayOrderServices(&orders[i]);

            printf("\nINVOICE INFORMATION\n");
            displayOrderInvoices(
                invoices,
                invoiceCount,
                orders[i].orderId
            );
        }
    }

    if (!historyFound)
    {
        printf("\nNo service history is available for this vehicle.\n");
    }
    else
    {
        printf("\n============================================\n");
        printf("Total Service Orders: %d\n", historyCount);
        printf("============================================\n");
    }
}
