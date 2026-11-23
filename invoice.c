
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "invoice.h"
#include "services.h"
#include "input.h"


/* Illustrative GST rate; actual treatment depends on applicable GST rules. */

#define GST_RATE 0.18


/* Find a customer using customer ID */

static const Customer *findCustomerById(
    const Customer customers[],
    int customerCount,
    int customerId
)
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


/* Find a vehicle using vehicle ID */

static const Vehicle *findVehicleById(
    const Vehicle vehicles[],
    int vehicleCount,
    int vehicleId
)
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


/* Find a service order using order ID */

static const ServiceOrder *findOrderById(
    const ServiceOrder orders[],
    int orderCount,
    int orderId
)
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


/* Check whether an order already has an invoice */

static int isOrderAlreadyInvoiced(
    const Invoice invoices[],
    int invoiceCount,
    int orderId
)
{
    for (int i = 0; i < invoiceCount; i++)
    {
        if (invoices[i].orderId == orderId)
        {
            return 1;
        }
    }

    return 0;
}


/* Print invoice information to a file or terminal */

static int printInvoice(
    FILE *output,
    const Invoice *invoice,
    const Customer *customer,
    const Vehicle *vehicle,
    const ServiceOrder *order
)
{
    double totalPartsCost = 0.0;
    double totalLabourCost = 0.0;

    fprintf(output, "\n");
    fprintf(output, "============================================================\n");
    fprintf(output, "                  VEHICLE SERVICE CENTRE\n");
    fprintf(output, "                     SERVICE INVOICE\n");
    fprintf(output, "============================================================\n");

    fprintf(output, "\nInvoice Number : INV-%04d\n", invoice->invoiceId);
    fprintf(output, "Invoice Date   : %s\n", invoice->invoiceDateTime);
    fprintf(output, "Service Order  : %d\n", invoice->orderId);

    fprintf(output, "\n---------------- CUSTOMER DETAILS ----------------\n");

    fprintf(output, "Customer ID    : %d\n", customer->customerId);
    fprintf(output, "Customer Name  : %s\n", customer->name);
    fprintf(output, "Mobile Number  : %s\n", customer->phone);
    fprintf(output, "Address        : %s\n", customer->address);

    fprintf(output, "\n---------------- VEHICLE DETAILS -----------------\n");

    fprintf(output, "Vehicle ID     : %d\n", vehicle->vehicleId);
    fprintf(output, "Registration   : %s\n", vehicle->registrationNumber);
    fprintf(output, "Manufacturer   : %s\n", vehicle->manufacturer);
    fprintf(output, "Model          : %s\n", vehicle->model);
    fprintf(output, "Category       : %s\n", vehicle->category);
    fprintf(output, "Fuel Type      : %s\n", vehicle->fuelType);
    fprintf(output, "Manufactured   : %d\n", vehicle->manufacturingYear);

    fprintf(output, "\n---------------- SERVICE DETAILS -----------------\n");

    fprintf(output, "--------------------------------------------------------------------------\n");

    fprintf(output, "%-25s %5s %11s %11s %12s\n",
            "SERVICE",
            "QTY",
            "PARTS",
            "LABOUR",
            "TOTAL");

    fprintf(output, "--------------------------------------------------------------------------\n");

    for (int i = 0; i < order->selectedCount; i++)
    {
        int serviceId = order->selectedServices[i].serviceId;
        int quantity = order->selectedServices[i].quantity;

        const Service *service = getServiceById(serviceId);

        if (service == NULL)
        {
            fprintf(output, "ERROR: Invalid service ID %d\n", serviceId);
            return 0;
        }

        double partsCost = service->partsCost * quantity;
        double labourCost = service->labourCost * quantity;
        double lineTotal = partsCost + labourCost;

        totalPartsCost += partsCost;
        totalLabourCost += labourCost;

        fprintf(output, "%-25s %5d %11.2f %11.2f %12.2f\n",
                service->name,
                quantity,
                partsCost,
                labourCost,
                lineTotal);
    }

    double subtotal = totalPartsCost + totalLabourCost;
    double gstAmount = subtotal * GST_RATE;
    double grandTotal = subtotal + gstAmount;

    fprintf(output, "--------------------------------------------------------------------------\n");

    fprintf(output, "\nParts Cost       : Rs. %.2f\n", totalPartsCost);
    fprintf(output, "Labour Charges   : Rs. %.2f\n", totalLabourCost);

    fprintf(output, "----------------------------------------\n");

    fprintf(output, "Subtotal         : Rs. %.2f\n", subtotal);
    fprintf(output, "GST Rate         : %.0f%% (illustrative)\n", GST_RATE * 100);
    fprintf(output, "GST Amount       : Rs. %.2f\n", gstAmount);

    fprintf(output, "----------------------------------------\n");

    fprintf(output, "GRAND TOTAL      : Rs. %.2f\n", grandTotal);

    fprintf(output, "\n============================================================\n");
    fprintf(output, "             Thank you for choosing our service!\n");
    fprintf(output, "============================================================\n");

    fprintf(output, "\nNote: This is a computer-generated estimated service invoice.\n");
    fprintf(output, "GST treatment and final charges must be verified before billing.\n");

    if (ferror(output))
    {
        return 0;
    }

    return 1;
}


/* Generate and save a new invoice */

void generateInvoice(
    const Customer customers[],
    int customerCount,
    const Vehicle vehicles[],
    int vehicleCount,
    const ServiceOrder orders[],
    int orderCount,
    Invoice invoices[],
    int *invoiceCount
)
{
    if (*invoiceCount >= MAX_INVOICES)
    {
        printf("\nMaximum invoice limit reached!\n");
        return;
    }

    if (orderCount == 0)
    {
        printf("\nNo service orders available.\n");
        printf("Create a service order before generating an invoice.\n");
        return;
    }

    printf("\n========== INVOICE GENERATION ==========\n");

    printf("\nAvailable Service Orders:\n");

    for (int i = 0; i < orderCount; i++)
    {
        printf("Order ID: %d | Vehicle ID: %d\n",
               orders[i].orderId,
               orders[i].vehicleId);
    }

    int orderId = readInteger(
        "\nEnter Service Order ID: ",
        1,
        MAX_ORDERS
    );

    const ServiceOrder *order = findOrderById(
        orders,
        orderCount,
        orderId
    );

    if (order == NULL)
    {
        printf("\nInvalid Service Order ID!\n");
        return;
    }

    if (order->selectedCount <= 0 ||
        order->selectedCount > MAX_SELECTED_SERVICES)
    {
        printf("\nInvalid service order. Invoice cannot be generated.\n");
        return;
    }

    if (isOrderAlreadyInvoiced(invoices, *invoiceCount, orderId))
    {
        printf("\nAn invoice has already been generated for this order.\n");
        return;
    }

    const Vehicle *vehicle = findVehicleById(
        vehicles,
        vehicleCount,
        order->vehicleId
    );

    if (vehicle == NULL)
    {
        printf("\nVehicle record not found!\n");
        return;
    }

    const Customer *customer = findCustomerById(
        customers,
        customerCount,
        vehicle->customerId
    );

    if (customer == NULL)
    {
        printf("\nCustomer record not found!\n");
        return;
    }

    /* Validate all selected services before creating the file */

    for (int i = 0; i < order->selectedCount; i++)
    {
        int serviceId = order->selectedServices[i].serviceId;
        int quantity = order->selectedServices[i].quantity;

        if (getServiceById(serviceId) == NULL ||
            quantity < 1 ||
            quantity > 10)
        {
            printf("\nInvalid service details in this order.\n");
            printf("Invoice generation cancelled.\n");
            return;
        }
    }

    Invoice newInvoice;

    newInvoice.invoiceId = *invoiceCount + 1;
    newInvoice.orderId = orderId;

    /* Generate current date and time */

    time_t currentTime = time(NULL);

    if (currentTime == (time_t)-1)
    {
        printf("\nUnable to retrieve system date and time.\n");
        return;
    }

    struct tm *timeInfo = localtime(&currentTime);

    if (timeInfo == NULL)
    {
        printf("\nUnable to convert system date and time.\n");
        return;
    }

    if (strftime(
            newInvoice.invoiceDateTime,
            sizeof(newInvoice.invoiceDateTime),
            "%d-%m-%Y %H:%M:%S",
            timeInfo
        ) == 0)
    {
        printf("\nUnable to format invoice date and time.\n");
        return;
    }

    /* Generate invoice filename */

    snprintf(
        newInvoice.fileName,
        sizeof(newInvoice.fileName),
        "invoice_%04d.txt",
        newInvoice.invoiceId
    );

    /* Open invoice file */

    FILE *invoiceFile = fopen(newInvoice.fileName, "w");

    if (invoiceFile == NULL)
    {
        printf("\nUnable to create invoice file.\n");
        printf("Check folder permissions and available disk space.\n");
        return;
    }

    /* Write invoice into the file */

    int writeSuccess = printInvoice(
        invoiceFile,
        &newInvoice,
        customer,
        vehicle,
        order
    );

    int closeResult = fclose(invoiceFile);

    if (!writeSuccess || closeResult != 0)
    {
        printf("\nInvoice file could not be saved completely.\n");
        printf("Check the file before using it.\n");
        return;
    }

    /* Update invoice records only after successful file creation */

    invoices[*invoiceCount] = newInvoice;

    (*invoiceCount)++;

    /* Display the generated invoice in the terminal */

    printf("\n");

    printInvoice(
        stdout,
        &newInvoice,
        customer,
        vehicle,
        order
    );

    printf("\nInvoice generated successfully!\n");
    printf("Invoice Number: INV-%04d\n", newInvoice.invoiceId);
    printf("Saved File: %s\n", newInvoice.fileName);
}


/* Display invoices generated during the current session */

void displayInvoices(
    const Invoice invoices[],
    int invoiceCount
)
{
    if (invoiceCount == 0)
    {
        printf("\nNo invoices generated during this session.\n");
        return;
    }

    printf("\n================ GENERATED INVOICES ================\n");

    printf("%-15s %-15s %-25s %-25s\n",
           "INVOICE ID",
           "ORDER ID",
           "DATE AND TIME",
           "FILE NAME");

    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < invoiceCount; i++)
    {
        printf("%-15d %-15d %-25s %-25s\n",
               invoices[i].invoiceId,
               invoices[i].orderId,
               invoices[i].invoiceDateTime,
               invoices[i].fileName);
    }

    printf("===============================================================\n");
}
