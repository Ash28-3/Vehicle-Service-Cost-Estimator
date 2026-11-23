
#include <stdio.h>

#include "estimation.h"
#include "services.h"
#include "input.h"


/*
   Illustrative GST rate.
   Actual GST treatment must be verified for the relevant service.
*/

#define GST_RATE 0.18


/* Find a vehicle using its ID */

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


/* Generate the cost estimate for a service order */

void generateCostEstimate(
    const Vehicle vehicles[],
    int vehicleCount,
    const ServiceOrder orders[],
    int orderCount
)
{
    if (orderCount == 0)
    {
        printf("\nNo service orders available.\n");
        printf("Create a service order first.\n");
        return;
    }

    printf("\n========== COST ESTIMATION ==========\n");

    printf("\nAvailable Service Orders:\n");

    for (int i = 0; i < orderCount; i++)
    {
        printf("Order ID: %d | Vehicle ID: %d\n",
               orders[i].orderId,
               orders[i].vehicleId);
    }

    int orderId = readInteger(
        "\nEnter Order ID: ", 1, MAX_ORDERS
    );

    const ServiceOrder *selectedOrder = NULL;

    for (int i = 0; i < orderCount; i++)
    {
        if (orders[i].orderId == orderId)
        {
            selectedOrder = &orders[i];
            break;
        }
    }

    if (selectedOrder == NULL)
    {
        printf("\nInvalid Order ID!\n");
        return;
    }

    const Vehicle *vehicle = findVehicleById(
        vehicles,
        vehicleCount,
        selectedOrder->vehicleId
    );

    if (vehicle == NULL)
    {
        printf("\nVehicle record not found for this order.\n");
        return;
    }

    double totalPartsCost = 0.0;
    double totalLabourCost = 0.0;
    double subtotal = 0.0;

    printf("\n");
    printf("===============================================================\n");
    printf("                    SERVICE COST ESTIMATE\n");
    printf("===============================================================\n");

    printf("\nOrder ID           : %d\n", selectedOrder->orderId);
    printf("Vehicle            : %s %s\n",
           vehicle->manufacturer,
           vehicle->model);
    printf("Registration       : %s\n",
           vehicle->registrationNumber);
    printf("Fuel Type          : %s\n", vehicle->fuelType);

    printf("\n");
    printf("--------------------------------------------------------------------------\n");

    printf("%-25s %5s %11s %11s %12s\n",
           "SERVICE",
           "QTY",
           "PARTS",
           "LABOUR",
           "TOTAL");

    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < selectedOrder->selectedCount; i++)
    {
        int serviceId =
            selectedOrder->selectedServices[i].serviceId;

        int quantity =
            selectedOrder->selectedServices[i].quantity;

        const Service *service = getServiceById(serviceId);

        if (service == NULL)
        {
            printf("Warning: Service ID %d not found. Skipping.\n",
                   serviceId);
            continue;
        }

        double partsCost = service->partsCost * quantity;

        double labourCost = service->labourCost * quantity;

        double lineTotal = partsCost + labourCost;

        totalPartsCost += partsCost;

        totalLabourCost += labourCost;

        subtotal += lineTotal;

        printf("%-25s %5d %11.2f %11.2f %12.2f\n",
               service->name,
               quantity,
               partsCost,
               labourCost,
               lineTotal);
    }

    double gstAmount = subtotal * GST_RATE;

    double grandTotal = subtotal + gstAmount;

    printf("--------------------------------------------------------------------------\n");

    printf("\nParts Cost       : Rs. %.2f\n", totalPartsCost);

    printf("Labour Charges   : Rs. %.2f\n", totalLabourCost);

    printf("----------------------------------------\n");

    printf("Subtotal         : Rs. %.2f\n", subtotal);

    printf("GST Rate         : %.0f%% (illustrative)\n",
           GST_RATE * 100);

    printf("GST Amount       : Rs. %.2f\n", gstAmount);

    printf("----------------------------------------\n");

    printf("ESTIMATED TOTAL  : Rs. %.2f\n", grandTotal);

    printf("========================================\n");

    printf("\nNote: This is an estimated cost, not a final tax invoice.\n");
}
