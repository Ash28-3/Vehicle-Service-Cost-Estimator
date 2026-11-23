# Vehicle Service Cost Estimator

A console-based vehicle service information management and cost estimation application developed using the C programming language.

## Project Overview

The Vehicle Service Cost Estimator provides a structured method for managing customer and vehicle records, selecting vehicle services, calculating estimated service charges and generating invoices.

The application uses modular programming and file handling to organize and preserve records between program executions.

## Features

- Customer registration and management
- Vehicle registration and customer association
- Predefined catalogue of 12 vehicle services
- Multiple service selection with quantity input
- Automated parts and labour cost calculation
- Subtotal and configured GST calculation
- Invoice generation and text-file storage
- Binary file-based record persistence
- Customer, vehicle, service order and invoice search
- Vehicle-based service history

## Technologies Used

- Programming Language: C
- Compiler: GCC 16.2.0
- C Standard: C11
- Development Environment: Visual Studio Code
- Storage: Binary file handling and text invoice files

## Project Structure

The project is divided into separate source and header files:

- `main.c` – Main program and menu navigation
- `input.c / input.h` – Input handling
- `customer.c / customer.h` – Customer management
- `vehicle.c / vehicle.h` – Vehicle management
- `services.c / services.h` – Service catalogue and selection
- `estimation.c / estimation.h` – Cost estimation
- `invoice.c / invoice.h` – Invoice generation
- `records.c / records.h` – Record storage and retrieval
- `search.c / search.h` – Search functionality
- `history.c / history.h` – Vehicle service history
- `structures.h` – Shared data structures

## Compilation

Compile the application using GCC:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic main.c input.c customer.c vehicle.c services.c estimation.c invoice.c records.c search.c history.c -o vehicle_service_estimator.exe
```

## Execution

On Windows PowerShell:

```powershell
.\vehicle_service_estimator.exe
```

## Data Storage

The application stores customer, vehicle, service-order and invoice metadata in a binary file named `vehicle_service_data.dat`.

Generated invoices are stored separately as text files.

## Cost Estimation

The application calculates:

- Parts Cost
- Labour Cost
- Subtotal
- Configured GST Amount
- Final Estimated Cost

The project uses an illustrative configured GST rate of 18%.

## Limitations

- Console-based interface
- Predefined service catalogue and charges
- Local binary file storage
- No external database management system
- No authentication or encryption
- No online payment functionality
- No live vehicle diagnostics or inventory management

## Project Purpose

This project demonstrates the practical implementation of structures, arrays, functions, modular programming and file handling in C.

**Developed by:** Ashutosh Soni  
**University:** Chandigarh University  
**Degree:** Bachelor of Engineering – Information Technology  
**Academic Session:** 2026–2027