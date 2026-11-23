
#ifndef ESTIMATION_H
#define ESTIMATION_H

#include "structures.h"

void generateCostEstimate(
    const Vehicle vehicles[],
    int vehicleCount,
    const ServiceOrder orders[],
    int orderCount
);

#endif
