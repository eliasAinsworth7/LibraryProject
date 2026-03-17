#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <chrono>
#include <iomanip>
#include <functional>
#include <sstream>


// Base exception class for library-related erros
class LibraryEception : public std::runtime_error{
public:
    // Constructor
    LibraryEception(const std::string& message) : std::runtime_error(message){}
};

// Specific Error Type - ItemNotFoundException

class ItemNotFoundException : public LibraryEception{

    public:
    ItemNotFoundException(std::string& id) : LibraryEception("Item is not found, "+id){}

};


// Base Class For Library Items

class LibraryItem {
private:
    std::string id;
    std::string title;
    bool available;

protected:
    double dailyFine;
    int maxLoanDays;
public:
    LibraryItem(std::string& i, std::string& t) : id(std::move(i)), title(std::move(t)), available(true), dailyFine(0.0), maxLoanDays(0) {}

    virtual ~LibraryItem() {};

    //Getters
    std::string getId() const {return id;}
    std::string getTitle() const { return title;}
    bool isAvailable() const {return available;}
    int getMaxLoanDays() const { return maxLoanDays;}

    //Setter 
    void setAvailable(bool a) { available = a;}

    //Pure virtual methods to be implemented by derived classes
    virtual std::string getItemType() const = 0;
    virtual double calculateFine(int daysOverdue) = 0;
    virtual std::string getDetails() const = 0;

    //Common Functionality
    void checkOut(){
        if(!available){
            throw LibraryEception("Item is not avaialable for check out");
        }
        available = false;
    }

    void returnItem(){
        available = true;
    }
};

class Book : public LibraryItem{
private:
    std::string author;
    std::string isbn;
    std::string genre;

public:
    Book(std::string& id, std::string& title, std::string& auth, std::string& isB, std::string& genr) : LibraryItem(std::move(id), std::move(title)), author(std::move(auth)), isbn(std::move(isB)), genre(std::move(genr)){}

    //Getters
    std::string getAuthor() const {return author;}
    std::string getIsBn() const {return isbn;}
    std::string getGenre() const {return genre;}

    //Implement pure virtual methdds
    std::string getItemType() const override{ return "book";}

    double calculateFine(int daysOverdue){ 
        if(daysOverdue < 0) throw LibraryEception("Days overdue cannot be negative");
        return dailyFine*daysOverdue;
    }

    std::string getDetails() const override{
        return "The book's author:" + author+", isbn: "+isbn+", genre: "+genre+"\n";
    }
};

class Magazine : public LibraryItem{

};

class DvD : public LibraryItem{

};

// Base class for all library patrons

class LibraryPatron {
private:
    std::string id_;
    std::string name_;
    std::string contactInfo_;
    bool active_;
protected:
    int maxBorrowItems;
public:
    //Constructor
    LibraryPatron(std::string& id, std::string& name, std::string& contactInfo): id_(id), name_(name), contactInfo_(contactInfo), active_(true), maxBorrowItems(0) {}

    //Virtual desructor
    virtual ~LibraryPatron() = default;

    //Getters
    std::string getId() const {return id_;};
    std::string getName() const {return name_;}
    std::string getContactInfo() const {return contactInfo_;}

    //Setter
    void setActive(bool active) { active_ = active;}
    void setContactInfo(std::string& contactinfo){ contactInfo_ = contactinfo;}

    //Pure virutal functions
    virtual std::string getPatronType() const = 0;
    virtual int getLoanExtensionDays() const = 0;

    //Common functionality 
    void deactivate() {active_ = false;}
    void activate() { active_ = true;}
};

//Derived class student
class Student : public LibraryPatron{
private:
    std::string studentId_;
    std::string major_;

public:
    Student(std::string& id, std::string& name, std::string contactInfo, std::string studentId, std::string major) 
    : LibraryPatron(std::move(id), std::move(name), std::move(contactInfo)), studentId_(std::move(studentId)), major_(std::move(major)) {}

    //Getter 
};

int main(int, char**){
    std::cout << "Hello, from LibraryProject!\n";
}
