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
class LibraryItem;
class Book;
class Magazine;
class DVD;
class LibraryPatron;
class Student;
class Faculty;
class Transaction;
class Checkout;
class Return;
class LibraryException;

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
    private:
        std::string publisher_;
        int issueNumber_;
        std::string publicationDate_;
        int pageCount_;
        std::string category_;
        int volume_;
        std::string language_;
    public:
        Magazine(std::string& id, std::string& title, std::string& publisher, int issueNumber, 
            std::string& publicationDate, int pageCount, std::string& category, int volume, std::string language) : LibraryItem(id,title), publisher_(publisher),
            issueNumber_(issueNumber), publicationDate_(publicationDate), pageCount_(pageCount), category_(category), volume_(volume), language_(language) {
                std::cout << "Magazine, "+title+", "+"added."<<std::endl;
        }
};

class DvD : public LibraryItem{
    private:
        std::string director;
        int duration;
        std::string rating;
        std::string genre;
        std::vector<std::string> actors;
        std::string releaseDate;
        std::string language;
    public:

};

class Return : public LibraryItem{

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
    LibraryPatron(std::string& id, std::string& name, std::string& contactInfo): id_(id), name_(name), contactInfo_(contactInfo), active_(true), maxBorrowItems(0) {
        std::cout << "Name: " + name + ", Contact Info: " + contactInfo <<std::endl;
    }

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
    : LibraryPatron(std::move(id), std::move(name), std::move(contactInfo)), studentId_(std::move(studentId)), major_(std::move(major)) {
        maxBorrowItems = 20;
        std::cout << "Type: " << getPatronType() + ", Max Number of Borrow Item : " + std::to_string(maxBorrowItems) << std::endl;
    }

    //Getter 
    std::string getStudentId() const{ return studentId_;}
    std::string getMajor() const { return major_;}

    //Implement pure virtual functions
    std::string getPatronType() const override {return "Student";}
    int getLoanExtensionDays() const override{ return 0;}
};

class Faculty : public LibraryPatron{

};

// Base Class For Transaction

class Transaction  {
    private:
        std::string transactionId_;
        std::chrono::system_clock::time_point timestamp_;

    public:
        Transaction() : timestamp_(std::chrono::system_clock::now()){
            //Generate a simple transaction ID based on timestamp

            auto time_t_now = std::chrono::system_clock::to_time_t(timestamp_);
            std::stringstream ss;
            ss << "TXN" << time_t_now;
            transactionId_ = ss.str();
        }

        //Virtual destructor
        virtual ~Transaction() = default;

        // Getters
        std::string getTransactionId() const {return transactionId_;}
        std::chrono::system_clock::time_point getTimestamp() const { return timestamp_;}

        // Format timestamp as string 
        std::string getFormattedTimestamp() const {
            auto time_t_now = std::chrono::system_clock::to_time_t(timestamp_);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S");
            return ss.str();
        }


        //Pure virtual functions
        virtual std::string getTransactionType() const = 0;
        virtual std::string getDetails() const = 0;
};

//Checkout Derives

class Checkout : public Transaction {

    private:
        std::shared_ptr<LibraryItem> item_;
        std::shared_ptr<LibraryPatron> patron_;
        std::chrono::system_clock::time_point dueDate_;

    public:
        Checkout(std::shared_ptr<LibraryItem> item, std::shared_ptr<LibraryPatron> patron) : item_(item), patron_(patron) {
        }

        //Getter
        std::shared_ptr<LibraryItem> getItem() const { return item_;}
        std::shared_ptr<LibraryPatron> getPatron() const {return patron_;}
        std::chrono::system_clock::time_point getDueDate() const { return dueDate_;}

        //Format due date as string
        std::string getFormattedDueDate() const {
            auto time_t_due = std::chrono::system_clock::to_time_t(dueDate_);
            std::stringstream ss;

            ss<< std::put_time(std::localtime(&time_t_due), "%Y-%m-%d");

            return ss.str();
        }

        // Check if item is overdue
        bool isOverdue() const {

            return false;
        }

        // Calculate overdue fine

        double calculateFine() const{

            return 0.0;
        }

        //Implement pure virtual function
        std::string getTransactionType() const override{
            return "Checkout";
        }

        std::string getDetails() const override{
            return "";
        }
};

class Library {
    private:
        std::vector<std::unique_ptr<LibraryItem>> items_;
        std::vector<std::unique_ptr<LibraryPatron>> patrons_;
        std::vector<std::unique_ptr<Transaction>> transactions_;

        //Helper Methods
        LibraryPatron* findPatronById( const std::string& id){
            return nullptr;
        }

        LibraryItem* findItemById(const std::string& id){
            return nullptr;
        }
    public:
        //Constructor
        Library() = default;

        // Add Methods
        void addItem(std::unique_ptr<LibraryItem> item){

        }

        // Add Patron
        void addPatron(std::unique_ptr<LibraryPatron> patron){

        }

        // Checkout and return methods
        std::unique_ptr<Checkout> checkoutItem(const std::string& itemId, const std::string& patronId){

            return nullptr;
        }

        std::unique_ptr<Return> returnItem(const std::string& itemId){

            return nullptr;
        }

        //Search Method
        std::vector<LibraryItem*> searchItem(const std::function<bool(const LibraryItem&)>& predicate){

        }

        //Report meethdos
        void printOverdueItems() const{

        }

        void printPatronHistory(std::string& patronId) const {

        }

        void printInventory() const{

        }


};

// Simple Test Framework for unit test

class UnitTest{

};

// Function to run all units

void runTest(){

}


int main(int, char**){
    std::cout << "Hello, from LibraryProject!\n";
}
