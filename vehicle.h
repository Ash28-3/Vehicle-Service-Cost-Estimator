
#ifndef VEHICLE_H
#define VEHICLE_H

#include "structures.h"

void registerVehicle(Vehicle vehicles[], int *vehicleCount,
                     Customer customers[], int customerCount);

void displayVehicles(const Vehicle vehicles[], int vehicleCount);

#endif
