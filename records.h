
#ifndef RECORDS_H
#define RECORDS_H

#include "structures.h"

int loadAllRecords(
    Customer customers[],
    int *customerCount,
    Vehicle vehicles[],
    int *vehicleCount,
    ServiceOrder orders[],
    int *orderCount,
    Invoice invoices[],
    int *invoiceCount
);

int saveAllRecords(
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
