
#include <stdio.h>
#include <string.h>

#include "vehicle.h"
#include "customer.h"
#include "input.h"


void registerVehicle(Vehicle vehicles[], int *vehicleCount,
                     Customer customers[], int customerCount)
{
    if (*vehicleCount >= MAX_VEHICLES)
    {
        printf("\nVehicle storage is full!\n");
        return;
    }

    if (customerCount == 0)
    {
        printf("\nNo customers registered yet!\n");
        printf("Please register a customer first.\n");
        return;
    }

    Vehicle *newVehicle = &vehicles[*vehicleCount];

    printf("\n========== VEHICLE REGISTRATION ==========\n");

    int ownerId = readInteger("Enter customer ID: ", 1, MAX_CUSTOMERS);

    Customer *owner = findCustomerById(customers, customerCount, ownerId);

    if (owner == NULL)
    {
        printf("\nCustomer ID not found! Vehicle registration cancelled.\n");
        return;
    }

    printf("Vehicle owner: %s\n", owner->name);

    readString("Enter vehicle registration number: ",
               newVehicle->registrationNumber,
               sizeof(newVehicle->registrationNumber));

    /* Check for duplicate registration numbers */

    for (int i = 0; i < *vehicleCount; i++)
    {
        if (strcmp(vehicles[i].registrationNumber,
                   newVehicle->registrationNumber) == 0)
        {
            printf("\nThis vehicle is already registered!\n");
            return;
        }
    }

    printf("\nSelect vehicle category:\n");
    printf("1. Two-Wheeler\n");
    printf("2. Four-Wheeler\n");

    int categoryChoice = readInteger("Enter choice: ", 1, 2);

    if (categoryChoice == 1)
    {
        strcpy(newVehicle->category, "Two-Wheeler");
    }
    else
    {
        strcpy(newVehicle->category, "Four-Wheeler");
    }

    readString("Enter manufacturer: ",
               newVehicle->manufacturer,
               sizeof(newVehicle->manufacturer));

    readString("Enter vehicle model: ",
               newVehicle->model,
               sizeof(newVehicle->model));

    printf("\nSelect fuel type:\n");
    printf("1. Petrol\n");
    printf("2. Diesel\n");
    printf("3. CNG\n");
    printf("4. Electric\n");

    int fuelChoice = readInteger("Enter choice: ", 1, 4);

    switch (fuelChoice)
    {
        case 1:
            strcpy(newVehicle->fuelType, "Petrol");
            break;

        case 2:
            strcpy(newVehicle->fuelType, "Diesel");
            break;

        case 3:
            strcpy(newVehicle->fuelType, "CNG");
            break;

        case 4:
            strcpy(newVehicle->fuelType, "Electric");
            break;
    }

    newVehicle->manufacturingYear =
        readInteger("Enter manufacturing year (1900-2100): ", 1900, 2100);

    newVehicle->vehicleId = *vehicleCount + 1;
    newVehicle->customerId = ownerId;

    (*vehicleCount)++;

    printf("\nVehicle registered successfully!\n");
    printf("Vehicle ID: %d\n", newVehicle->vehicleId);
    printf("Owner: %s\n", owner->name);
}


void displayVehicles(const Vehicle vehicles[], int vehicleCount)
{
    if (vehicleCount == 0)
    {
        printf("\nNo vehicle records available.\n");
        return;
    }

    printf("\n================ VEHICLE RECORDS ================\n");

    for (int i = 0; i < vehicleCount; i++)
    {
        printf("\nVehicle ID          : %d\n", vehicles[i].vehicleId);
        printf("Customer ID         : %d\n", vehicles[i].customerId);
        printf("Registration Number : %s\n", vehicles[i].registrationNumber);
        printf("Category            : %s\n", vehicles[i].category);
        printf("Manufacturer        : %s\n", vehicles[i].manufacturer);
        printf("Model               : %s\n", vehicles[i].model);
        printf("Fuel Type           : %s\n", vehicles[i].fuelType);
        printf("Manufacturing Year  : %d\n", vehicles[i].manufacturingYear);

        printf("--------------------------------------------------\n");
    }
}
