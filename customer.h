
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "structures.h"

void registerCustomer(Customer customers[], int *customerCount);

void displayCustomers(const Customer customers[], int customerCount);

Customer *findCustomerById(Customer customers[], int customerCount, int id);

#endif
