
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "records.h"


#define DATABASE_FILE "vehicle_service_data.dat"
#define TEMP_FILE "vehicle_service_data.tmp"
#define BACKUP_FILE "vehicle_service_data.bak"

#define DATABASE_VERSION 1


/* Database header */

typedef struct
{
    char magic[8];

    int version;

    int customerCount;
    int vehicleCount;
    int orderCount;
    int invoiceCount;

} DatabaseHeader;


static const char DATABASE_MAGIC[8] =
{
    'V', 'S', 'E', 'D', 'B', '0', '1', '\0'
};


/* Load all application records */

int loadAllRecords(
    Customer customers[],
    int *customerCount,
    Vehicle vehicles[],
    int *vehicleCount,
    ServiceOrder orders[],
    int *orderCount,
    Invoice invoices[],
    int *invoiceCount
)
{
    FILE *file = fopen(DATABASE_FILE, "rb");

    if (file == NULL)
    {
        if (errno == ENOENT)
        {
            printf("\nNo previous database found.\n");
            printf("Starting a new application database.\n");

            *customerCount = 0;
            *vehicleCount = 0;
            *orderCount = 0;
            *invoiceCount = 0;

            return 1;
        }

        printf("\nError opening database file.\n");
        return 0;
    }

    DatabaseHeader header;

    if (fread(&header, sizeof(DatabaseHeader), 1, file) != 1)
    {
        printf("\nDatabase file is empty or corrupted.\n");
        fclose(file);
        return 0;
    }

    /* Validate database identification */

    if (memcmp(header.magic, DATABASE_MAGIC, sizeof(DATABASE_MAGIC)) != 0)
    {
        printf("\nInvalid database file format.\n");
        fclose(file);
        return 0;
    }

    /* Validate database version */

    if (header.version != DATABASE_VERSION)
    {
        printf("\nUnsupported database version.\n");
        fclose(file);
        return 0;
    }

    /* Validate stored record counts */

    if (header.customerCount < 0 ||
        header.customerCount > MAX_CUSTOMERS ||
        header.vehicleCount < 0 ||
        header.vehicleCount > MAX_VEHICLES ||
        header.orderCount < 0 ||
        header.orderCount > MAX_ORDERS ||
        header.invoiceCount < 0 ||
        header.invoiceCount > MAX_INVOICES)
    {
        printf("\nInvalid record counts in database.\n");
        fclose(file);
        return 0;
    }

    /* Clear arrays before loading */

    memset(customers, 0, sizeof(Customer) * MAX_CUSTOMERS);
    memset(vehicles, 0, sizeof(Vehicle) * MAX_VEHICLES);
    memset(orders, 0, sizeof(ServiceOrder) * MAX_ORDERS);
    memset(invoices, 0, sizeof(Invoice) * MAX_INVOICES);

    /* Read customer records */

    if (header.customerCount > 0 &&
        fread(customers, sizeof(Customer),
              header.customerCount, file) !=
              (size_t)header.customerCount)
    {
        printf("\nUnable to read customer records.\n");
        fclose(file);
        return 0;
    }

    /* Read vehicle records */

    if (header.vehicleCount > 0 &&
        fread(vehicles, sizeof(Vehicle),
              header.vehicleCount, file) !=
              (size_t)header.vehicleCount)
    {
        printf("\nUnable to read vehicle records.\n");
        fclose(file);
        return 0;
    }

    /* Read service orders */

    if (header.orderCount > 0 &&
        fread(orders, sizeof(ServiceOrder),
              header.orderCount, file) !=
              (size_t)header.orderCount)
    {
        printf("\nUnable to read service orders.\n");
        fclose(file);
        return 0;
    }

    /* Read invoice records */

    if (header.invoiceCount > 0 &&
        fread(invoices, sizeof(Invoice),
              header.invoiceCount, file) !=
              (size_t)header.invoiceCount)
    {
        printf("\nUnable to read invoice records.\n");
        fclose(file);
        return 0;
    }

    if (fclose(file) != 0)
    {
        printf("\nError closing database file.\n");
        return 0;
    }

    /* Update record counts after successful loading */

    *customerCount = header.customerCount;
    *vehicleCount = header.vehicleCount;
    *orderCount = header.orderCount;
    *invoiceCount = header.invoiceCount;

    printf("\nDatabase loaded successfully!\n");

    printf("Customers Loaded : %d\n", *customerCount);
    printf("Vehicles Loaded  : %d\n", *vehicleCount);
    printf("Orders Loaded    : %d\n", *orderCount);
    printf("Invoices Loaded  : %d\n", *invoiceCount);

    return 1;
}


/* Save all application records */

int saveAllRecords(
    const Customer customers[],
    int customerCount,
    const Vehicle vehicles[],
    int vehicleCount,
    const ServiceOrder orders[],
    int orderCount,
    const Invoice invoices[],
    int invoiceCount
)
{
    /* Validate record counts */

    if (customerCount < 0 || customerCount > MAX_CUSTOMERS ||
        vehicleCount < 0 || vehicleCount > MAX_VEHICLES ||
        orderCount < 0 || orderCount > MAX_ORDERS ||
        invoiceCount < 0 || invoiceCount > MAX_INVOICES)
    {
        printf("\nInvalid record counts. Database not saved.\n");
        return 0;
    }

    /* Prepare database header */

    DatabaseHeader header = {0};

    memcpy(header.magic, DATABASE_MAGIC, sizeof(DATABASE_MAGIC));

    header.version = DATABASE_VERSION;

    header.customerCount = customerCount;
    header.vehicleCount = vehicleCount;
    header.orderCount = orderCount;
    header.invoiceCount = invoiceCount;

    /* Write to temporary file first */

    FILE *file = fopen(TEMP_FILE, "wb");

    if (file == NULL)
    {
        printf("\nUnable to create temporary database file.\n");
        return 0;
    }

    int writeSuccess = 1;

    if (fwrite(&header, sizeof(DatabaseHeader), 1, file) != 1)
    {
        writeSuccess = 0;
    }

    if (writeSuccess && customerCount > 0)
    {
        if (fwrite(customers, sizeof(Customer),
                   customerCount, file) != (size_t)customerCount)
        {
            writeSuccess = 0;
        }
    }

    if (writeSuccess && vehicleCount > 0)
    {
        if (fwrite(vehicles, sizeof(Vehicle),
                   vehicleCount, file) != (size_t)vehicleCount)
        {
            writeSuccess = 0;
        }
    }

    if (writeSuccess && orderCount > 0)
    {
        if (fwrite(orders, sizeof(ServiceOrder),
                   orderCount, file) != (size_t)orderCount)
        {
            writeSuccess = 0;
        }
    }

    if (writeSuccess && invoiceCount > 0)
    {
        if (fwrite(invoices, sizeof(Invoice),
                   invoiceCount, file) != (size_t)invoiceCount)
        {
            writeSuccess = 0;
        }
    }

    if (fclose(file) != 0)
    {
        writeSuccess = 0;
    }

    if (!writeSuccess)
    {
        printf("\nError writing temporary database.\n");
        remove(TEMP_FILE);
        return 0;
    }

    /*
       Preserve the existing database as a backup before
       replacing it with the newly written temporary file.
    */

    remove(BACKUP_FILE);

    int backupCreated = 0;

    if (rename(DATABASE_FILE, BACKUP_FILE) == 0)
    {
        backupCreated = 1;
    }
    else if (errno != ENOENT)
    {
        printf("\nUnable to preserve existing database.\n");
        remove(TEMP_FILE);
        return 0;
    }

    /* Replace database with the new file */

    if (rename(TEMP_FILE, DATABASE_FILE) != 0)
    {
        printf("\nUnable to replace database file.\n");

        if (backupCreated)
        {
            if (rename(BACKUP_FILE, DATABASE_FILE) != 0)
            {
                printf("Warning: Database backup could not be restored.\n");
            }
        }

        remove(TEMP_FILE);
        return 0;
    }

    /* Remove backup after successful replacement */

    if (backupCreated)
    {
        remove(BACKUP_FILE);
    }

    return 1;
}
