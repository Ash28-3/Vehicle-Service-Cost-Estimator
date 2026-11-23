
#ifndef HISTORY_H
#define HISTORY_H

#include "structures.h"

void displayVehicleServiceHistory(
    const Customer customers[],
    int customerCount,
    const Vehicle vehicles[],
    int vehicleCount,
    const ServiceOrder orders[],
    int orderCount,
    const Invoice invoices[],
    int invoiceCount
);

#endif
