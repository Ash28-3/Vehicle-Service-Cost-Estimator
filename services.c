
#include <stdio.h>
#include <string.h>

#include "services.h"
#include "input.h"


/* Predefined Service Catalogue */

static const Service serviceCatalogue[] =
{
    {1, "Engine Oil Change", "Engine", 1200.0, 250.0},
    {2, "Oil Filter Replacement", "Engine", 350.0, 100.0},
    {3, "Air Filter Replacement", "Engine", 450.0, 100.0},
    {4, "Brake Pad Replacement", "Brakes", 1800.0, 400.0},
    {5, "Coolant Replacement", "Cooling", 700.0, 200.0},
    {6, "Battery Replacement", "Electrical", 4000.0, 250.0},
    {7, "Wheel Alignment", "Tyres", 600.0, 200.0},
    {8, "Wheel Balancing", "Tyres", 400.0, 150.0},
    {9, "General Inspection", "General", 300.0, 200.0},
    {10, "AC Servicing", "AC", 1500.0, 400.0},
    {11, "Vehicle Washing", "Cleaning", 300.0, 100.0},
    {12, "Brake Inspection", "Brakes", 200.0, 150.0}
};


#define SERVICE_COUNT \
    (sizeof(serviceCatalogue) / sizeof(serviceCatalogue[0]))


/* Find a service using its ID */
/* This function is accessible from other C files */

const Service *getServiceById(int serviceId)
{
    for (int i = 0; i < (int)SERVICE_COUNT; i++)
    {
        if (serviceCatalogue[i].serviceId == serviceId)
        {
            return &serviceCatalogue[i];
        }
    }

    return NULL;
}


/* Display all available services */

void displayServiceCatalogue(void)
{
    printf("\n");
    printf("===============================================================\n");
    printf("                    SERVICE CATALOGUE\n");
    printf("===============================================================\n");

    printf("%-5s %-27s %-12s %12s %12s\n",
           "ID", "SERVICE", "CATEGORY", "PARTS (Rs)", "LABOUR (Rs)");

    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < (int)SERVICE_COUNT; i++)
    {
        printf("%-5d %-27s %-12s %12.2f %12.2f\n",
               serviceCatalogue[i].serviceId,
               serviceCatalogue[i].name,
               serviceCatalogue[i].category,
               serviceCatalogue[i].partsCost,
               serviceCatalogue[i].labourCost);
    }

    printf("===============================================================\n");
}


/* Create a new service order */

void createServiceOrder(
    Vehicle vehicles[],
    int vehicleCount,
    ServiceOrder orders[],
    int *orderCount
)
{
    if (*orderCount >= MAX_ORDERS)
    {
        printf("\nMaximum service order limit reached!\n");
        return;
    }

    if (vehicleCount == 0)
    {
        printf("\nNo registered vehicles found!\n");
        printf("Please register a vehicle first.\n");
        return;
    }

    printf("\n========== CREATE SERVICE ORDER ==========\n");

    printf("\nRegistered Vehicles:\n");

    for (int i = 0; i < vehicleCount; i++)
    {
        printf("\nVehicle ID: %d", vehicles[i].vehicleId);
        printf("\nRegistration: %s", vehicles[i].registrationNumber);
        printf("\nManufacturer: %s", vehicles[i].manufacturer);
        printf("\nModel: %s\n", vehicles[i].model);
    }

    int vehicleId = readInteger(
        "\nEnter Vehicle ID: ", 1, MAX_VEHICLES
    );

    int vehicleFound = 0;

    for (int i = 0; i < vehicleCount; i++)
    {
        if (vehicles[i].vehicleId == vehicleId)
        {
            vehicleFound = 1;
            break;
        }
    }

    if (!vehicleFound)
    {
        printf("\nInvalid Vehicle ID!\n");
        return;
    }

    ServiceOrder *newOrder = &orders[*orderCount];

    newOrder->orderId = *orderCount + 1;
    newOrder->vehicleId = vehicleId;
    newOrder->selectedCount = 0;

    printf("\nSelected Vehicle ID: %d\n", vehicleId);

    displayServiceCatalogue();

    printf("\nSelect services using their IDs.\n");
    printf("Enter 0 when you have finished selecting services.\n");

    while (1)
    {
        int serviceId = readInteger(
            "\nEnter Service ID (0 to finish): ",
            0, (int)SERVICE_COUNT
        );

        if (serviceId == 0)
        {
            break;
        }

        const Service *selectedService = getServiceById(serviceId);

        if (selectedService == NULL)
        {
            printf("\nService not found!\n");
            continue;
        }

        int existingIndex = -1;

        for (int i = 0; i < newOrder->selectedCount; i++)
        {
            if (newOrder->selectedServices[i].serviceId == serviceId)
            {
                existingIndex = i;
                break;
            }
        }

        if (existingIndex != -1)
        {
            printf("\nThis service is already selected.\n");

            int currentQuantity =
                newOrder->selectedServices[existingIndex].quantity;

            if (currentQuantity >= 10)
            {
                printf("Maximum quantity of 10 reached.\n");
                continue;
            }

            int additionalQuantity = readInteger(
                "Enter additional quantity (1-10): ",
                1, 10
            );

            if (additionalQuantity > 10 - currentQuantity)
            {
                printf("Maximum total quantity is 10.\n");
                continue;
            }

            newOrder->selectedServices[existingIndex].quantity +=
                additionalQuantity;

            printf("Quantity updated successfully!\n");
        }
        else
        {
            if (newOrder->selectedCount >= MAX_SELECTED_SERVICES)
            {
                printf("\nMaximum selected services reached!\n");
                break;
            }

            int quantity = readInteger(
                "Enter quantity (1-10): ", 1, 10
            );

            int index = newOrder->selectedCount;

            newOrder->selectedServices[index].serviceId = serviceId;
            newOrder->selectedServices[index].quantity = quantity;

            newOrder->selectedCount++;

            printf("\nService added successfully: %s\n",
                   selectedService->name);
        }
    }

    if (newOrder->selectedCount == 0)
    {
        printf("\nNo services selected. Order cancelled.\n");
        return;
    }

    (*orderCount)++;

    printf("\n============================================\n");
    printf("SERVICE ORDER CREATED SUCCESSFULLY!\n");
    printf("Order ID: %d\n", newOrder->orderId);
    printf("Vehicle ID: %d\n", newOrder->vehicleId);
    printf("Total Selected Services: %d\n",
           newOrder->selectedCount);
    printf("============================================\n");
}


/* Display all service orders */

void displayServiceOrders(
    const Vehicle vehicles[],
    int vehicleCount,
    const ServiceOrder orders[],
    int orderCount
)
{
    if (orderCount == 0)
    {
        printf("\nNo service orders available.\n");
        return;
    }

    printf("\n================ SERVICE ORDER HISTORY ================\n");

    for (int i = 0; i < orderCount; i++)
    {
        printf("\nOrder ID: %d\n", orders[i].orderId);
        printf("Vehicle ID: %d\n", orders[i].vehicleId);

        for (int j = 0; j < vehicleCount; j++)
        {
            if (vehicles[j].vehicleId == orders[i].vehicleId)
            {
                printf("Registration Number: %s\n",
                       vehicles[j].registrationNumber);

                printf("Vehicle: %s %s\n",
                       vehicles[j].manufacturer,
                       vehicles[j].model);

                break;
            }
        }

        printf("\nSelected Services:\n");

        for (int j = 0; j < orders[i].selectedCount; j++)
        {
            const Service *service = getServiceById(
                orders[i].selectedServices[j].serviceId
            );

            if (service != NULL)
            {
                printf("%-27s Quantity: %d\n",
                       service->name,
                       orders[i].selectedServices[j].quantity);
            }
        }

        printf("------------------------------------------------------\n");
    }
}
