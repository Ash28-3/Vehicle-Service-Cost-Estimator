
#ifndef INVOICE_H
#define INVOICE_H

#include "structures.h"

void generateInvoice(
    const Customer customers[],
    int customerCount,
    const Vehicle vehicles[],
    int vehicleCount,
    const ServiceOrder orders[],
    int orderCount,
    Invoice invoices[],
    int *invoiceCount
);

void displayInvoices(
    const Invoice invoices[],
    int invoiceCount
);

#endif
