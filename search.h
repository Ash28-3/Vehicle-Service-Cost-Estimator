
#ifndef SEARCH_H
#define SEARCH_H

#include "structures.h"

void searchMenu(
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
