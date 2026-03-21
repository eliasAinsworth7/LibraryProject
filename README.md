# Library Management System

This is a mini C++ OOP project about a simple library management system.  
I made this project to practice object-oriented programming concepts in C++ and improve my understanding of how real-life systems can be designed with classes.

## Project Idea

The project is based on a library system where different types of items and users are managed.  
It includes books, magazines, DVDs, library members, and transactions like borrowing, returning, and reserving items.

This project is still not fully finished, and I plan to continue improving it.

## What I Practiced

With this project, I practiced:

- Classes and objects
- Inheritance
- Polymorphism
- Encapsulation
- Abstract classes
- Exception handling
- Smart pointers in C++

## Classes in the Project

### Library Items
- `LibraryItem`
- `Book`
- `Fiction`
- `NonFiction`
- `Magazine`
- `DvD`

### Library Users
- `LibraryPatron`
- `Student`
- `Faculty`
- `GeneralPublic`

### Transactions
- `Transaction`
- `Checkout`
- `Return`
- `Reservation`

### Other
- `Library`
- Custom exception classes

## Features

- Different library item types
- Different user types
- Borrowing system
- Returning system
- Reservation system
- Fine calculation
- Custom exceptions for errors

## Project Status

This project is still in progress.

Things I want to improve:
- Complete missing methods in the `Library` class
- Add better search features
- Add more test cases
- Improve the program structure
- Make the main function more interactive

## How To Run

Compile the program with a C++ compiler such as `g++`.

```bash
g++ main.cpp -o library_project
./library_project
