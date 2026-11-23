
#ifndef SERVICES_H
#define SERVICES_H

#include "structures.h"

void displayServiceCatalogue(void);

const Service *getServiceById(int serviceId);

void createServiceOrder(
    Vehicle vehicles[],
    int vehicleCount,
    ServiceOrder orders[],
    int *orderCount
);

void displayServiceOrders(
    const Vehicle vehicles[],
    int vehicleCount,
    const ServiceOrder orders[],
    int orderCount
);

#endif
