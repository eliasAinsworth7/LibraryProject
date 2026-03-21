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
class LibraryExeption : public std::runtime_error{
public:
    // Constructor
    LibraryExeption(const std::string& message) : std::runtime_error(message){}
};

// Specific Error Type - ItemNotFoundException

class ItemNotFoundException : public LibraryExeption{

    public:
    ItemNotFoundException(const std::string& id) : LibraryExeption("Item is not found, "+id){}

};

class NotValidNumberException : public LibraryExeption{

    public:
    NotValidNumberException(const std::string& id) : LibraryExeption("Item is not valid number, "+id){}

};
class NotAvailableException : public LibraryExeption{

    public:
    NotAvailableException(const std::string& id) : LibraryExeption("Item is not valid number, "+id){}

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
    LibraryItem(std::string i, std::string t) : id(std::move(i)), title(std::move(t)), available(true), dailyFine(0.0), maxLoanDays(0) {}

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
            throw NotAvailableException(id);
        }
        available = false;
    }

    void returnItem(){
        available = true;
    }
};

class Book : public LibraryItem{
protected:
    std::string author;
    std::string isbn;
    std::string genre;

public:
    Book(std::string id, std::string title, std::string auth, std::string isB, std::string genr) : LibraryItem(std::move(id), std::move(title)), author(std::move(auth)), isbn(std::move(isB)), genre(std::move(genr)){
        dailyFine = 1.4; // Fine rate for book
    }
    ~Book() = default;
    //Getters
    std::string getAuthor() const {return author;}
    std::string getIsBn() const {return isbn;}
    std::string getGenre() const {return genre;}

    //Implement pure virtual methdds
    std::string getItemType() const override{ return "book";}

    double calculateFine(int daysOverdue) override{ 
        if(daysOverdue < 0) throw NotValidNumberException(getId());
        return dailyFine*daysOverdue;
    }

    std::string getDetails() const override{
        return "The book's Author:" + author+", Isbn: "+isbn+", Genre: "+genre+"\n";
    }
};

//Derived Classes From Book
class Fiction : public Book{
private:
    std::string subGen_;
    std::string targetAudience_;
    std::string seriesName_;
    int seriesIndex_;
    std::string setting_;
public:
    Fiction(std::string id,std::string author,std::string genre,std::string isbn,std::string title,std::string subGeb,
         std::string targetAudience, std::string seriesName, int seriesIndex, std::string setting):
         Book(std::move(id),std::move(title),std::move(author),std::move(isbn),std::move(genre)) 
         ,subGen_(std::move(subGeb)), targetAudience_(std::move(targetAudience)), seriesIndex_(std::move(seriesIndex)), seriesName_(std::move(seriesName)), setting_(std::move(setting)) {
            std::cout << "Fiction book is created" << std::endl; 
            dailyFine = 1.6; // Fine rate for Fiction Book
    }
    ~Fiction() = default;

    //Getter
    std::string getSubGen() { return subGen_;};
    std::string getTargetAudience() { return targetAudience_;};
    std::string getSeriesName() { return seriesName_;};
    std::string getSetting() { return setting_;};
    int getSeriesIndex() { return seriesIndex_;};

    //Override
    std::string getItemType() const override{
        return "Fiction Book";
    }

    double calculateFine(int daysOverdue) override{
        if(daysOverdue < 0) throw NotValidNumberException(getId());
        return daysOverdue * dailyFine;
    }

    std::string getDetails() const override{
        return "The fiction book's Author:" + author+", Isbn: "+isbn+", Genre: "+genre+",\nSubGenre: "+subGen_+", Series: "+seriesName_+", Audience: "+targetAudience_;
    }

    //Behaviours
    std::string getReadingRecmmedantion(){
        if(subGen_ == "Fantasy"){
            return "Recommended for long-term reading";
        } else if(subGen_ == "standalone"){
            return "Quick read";
        } else {
            return "It can be read";
        }
    }
};

class NonFiction : public Book{
private:
    std::string subject_;
    std::string edition_;
    std::string level_;
    int publicationYear_;
public:
    NonFiction(std::string id, std::string title,std::string author, std::string isbn, std::string genre,
         std::string subject, std::string edition, std::string level, int publicationYear): Book(std::move(id),std::move(title),std::move(author),std::move(isbn),
         std::move(genre)), subject_(std::move(subject)), edition_(std::move(edition)), level_(std::move(level)), publicationYear_(publicationYear) {
            dailyFine = 1.5; // Fine rate for Non-Fiction Book
    }
    ~NonFiction() = default;

    //Overrides
    std::string getItemType() const override{ return "Non Fiction Book";}
    std::string getDetails() const override{ return "The fiction book's Author:" + author+", Isbn: "+isbn+", Genre: "+genre+
        ",\nSubject: "+subject_+", Edition: "+edition_+", Level: "+level_+", Publication Year: "+std::to_string(publicationYear_);}
    double calculateFine(int daysOverdue) override{ if(daysOverdue < 0) throw NotValidNumberException(getId()); return daysOverdue* dailyFine;}

    //Getter
    std::string getSubject() const { return subject_;}
    std::string getEdition() const {return edition_;}
    std::string getLevel() const {return level_;}
    int getPublicationYear() const {return publicationYear_;}
    //Behaviours
    bool isOutDated(int currentyear) {
        if(currentyear < 0) throw NotValidNumberException(getId());
        return (currentyear - publicationYear_ >= 10) ? true : false;
    }

    bool isAdvancedLevel() {return (level_ == "Beginner") ? true : false;}

};

//Derived Class From Library Item
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
        Magazine(const std::string& id,const std::string& title,const std::string& publisher,const int issueNumber,const 
            std::string& publicationDate, int pageCount,const std::string& category, int volume,const std::string language) : LibraryItem(id,title), publisher_(publisher),
            issueNumber_(issueNumber), publicationDate_(publicationDate), pageCount_(pageCount), category_(category), volume_(volume), language_(language) {
                std::cout << "Magazine, "+title+", "+"created."<<std::endl;
                dailyFine = 1.2;
        }
        ~Magazine() = default;
        double calculateFine( int daysOverDue){
            if(daysOverDue < 0){
                throw NotValidNumberException(getId());
            }
            return daysOverDue * dailyFine;
        }
        std::string getPublisher() const {
            return publisher_;
        }
        int getIssueNumber() const{
            return issueNumber_;
        }
        std::string getPublicationDate() const {
            return publicationDate_;
        }
        int getPageCount() const{
            return pageCount_;
        }
        std::string getCategory() const {
            return category_;
        }
        int getVolume() const{
            return volume_;
        }
        std::string getLanguage() const {
            return language_;
        }
        std::string getDetails() const override{
            return "The Magazine's publisher:" + publisher_+",\nIssue Number: "+std::to_string(issueNumber_)+",\nPublicition Date: "+publicationDate_+", \nPage Count: " 
            + std::to_string(pageCount_)+",\nCategory: "+category_+", \nVolume: "+std::to_string(volume_)+",\nLanguage: "+language_+"\n";
        }
        std::string getItemType() const override{
            return "Magazine";
        }
};

class DvD : public LibraryItem{
    private:
        std::string director_;
        int duration_;
        std::string rating_;
        std::string genre_;
        std::vector<std::string> actors_; //That one might be changed
        std::string releaseDate_;
        std::string language_;
    public:
        DvD(const std::string& id,const std::string& title,const std::string& director, int duration,const std::string& rating,const std::string& genre,
             std::vector<std::string> actors,const std::string& releaseData,const std::string& language): LibraryItem(id, title),  director_(director),
                duration_(duration), rating_(rating), genre_(genre), actors_(actors), releaseDate_(releaseData), language_(language){
                std::cout << "DvD, "+title+", "+"created."<<std::endl;
                dailyFine = 1.5;
             }
        ~DvD() = default;
        std::string getDirector() const {
            return director_;
        }
        int getDuration() const {
            return duration_;
        }
        std::string getRating() const {
            return rating_;
        }
        std::string getGenre() const {
            return genre_;
        }
        std::vector<std::string> getActors() const {
            return actors_;
        }
        std::string getReleaseDate() const {
            return releaseDate_;
        }
        std::string getLanguage() const {
            return language_;
        }
        std::string getDetails() const override{
            return "The DvD's Director:" + director_+",\nDuration: "+std::to_string(duration_)+",\nRating: "+rating_+
            ",\Genre: "+genre_+", \nRelease Data: "+releaseDate_+",\nLanguage: "+language_+"\n";
        }
        std::string getItemType() const override {
            return "DvD";
        }
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
    LibraryPatron(const std::string& id,const std::string& name,const std::string& contactInfo): id_(id), name_(name), contactInfo_(contactInfo), active_(true), maxBorrowItems(0) {
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
    void setContactInfo(const std::string& contactinfo){ contactInfo_ = contactinfo;}

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
    Student( std::string id, std::string name, std::string contactInfo, std::string studentId, std::string major) 
    : LibraryPatron(std::move(id), std::move(name), std::move(contactInfo)), studentId_(std::move(studentId)), major_(std::move(major)) {
        maxBorrowItems = 20;
        std::cout << "Type: " << getPatronType() + ", Max Number of Borrow Item : " + std::to_string(maxBorrowItems) << std::endl;
    }
    ~Student() = default;
    //Getter 
    std::string getStudentId() const{ return studentId_;}
    std::string getMajor() const { return major_;}

    //Implement pure virtual functions
    std::string getPatronType() const override {return "Student";}
    int getLoanExtensionDays() const override{ return 0;}
};

class Faculty : public LibraryPatron{
private:
    std::string department_;
    std::string position_;
    int officeNumber_;
public:
    Faculty(std::string id, std::string name, std::string contactInfo, std::string department, std::string position,int officeNumber) :
     LibraryPatron(std::move(id),std::move(name), std::move(contactInfo)), department_(std::move(department)), position_(std::move(position)), officeNumber_(std::move(officeNumber)){

    }
    ~Faculty() =default;


    //Overrides
    std::string getPatronType() const override {return "Faculty";}
    int getLoanExtensionDays() const override{ return 3;}

    //Behavirous
    std::string getFullRoleInfo() { return getName()+"'s Deparment: "+department_+", Position: "+position_+", and Office Number: "+ std::to_string(officeNumber_);}
};

class GeneralPublic : public LibraryPatron{
private:
    std::string membershipType_;
    int membershipExperyYear_;
    bool registrationFreePaid_;
public:
    GeneralPublic(std::string id, std::string name, std::string contactInfo,std::string membershipType, int membershipExperyYear, bool registrationFreePaid):
     LibraryPatron(std::move(id),std::move(name), std::move(contactInfo)),
      membershipType_(std::move(membershipType)), membershipExperyYear_(std::move(membershipExperyYear)), registrationFreePaid_(std::move(registrationFreePaid)) {}
    ~GeneralPublic() = default;


    //Overrides
    std::string getPatronType() const override {return "General Public";}
    int getLoanExtensionDays() const override{ return 7;}

    //Getters
    std::string getMembershipType() const { return membershipType_;}
    int getMembershipExperyYear() const {return membershipExperyYear_;}
    bool getRegistrationFreePaid() const {return registrationFreePaid_;}

    //Behavirous
    bool hasValidMembership(int currentYear) { return (currentYear <= membershipExperyYear_) ? true : false;}

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

//Derived Class

class Return : public Transaction{
private:
    std::shared_ptr<LibraryItem> item_;
    std::shared_ptr<LibraryPatron> patron_;
    std::chrono::system_clock::time_point dueDate_;
    int fineAmount_;
public:
    Return(std::shared_ptr<LibraryItem> item, std::shared_ptr<LibraryPatron> patron) : item_(item), patron_(patron){
        dueDate_ = getTimestamp() + std::chrono::hours(24*patron_->getLoanExtensionDays());
    }
    ~Return() = default;

    //Getter
    std::shared_ptr<LibraryItem> getItem() const { return item_;}
    std::shared_ptr<LibraryPatron> getPatron() const {return patron_;}
    std::chrono::system_clock::time_point getDueDate() const { return dueDate_;}    

    //Overrides
    std::string getTransactionType() const override{
        return "Return";
    }

    std::string getDetails() const override{
        auto time_t_due = std::chrono::system_clock::to_time_t(dueDate_);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t_due), "%Y-%m-%d");

        return "RETURN: Item: "+ item_->getTitle()+", Patron: "+patron_->getName()+", and Overdue Date: "+ ss.str();
    }

    //Behavirous

    bool isLate(){
        return dueDate_ > std::chrono::system_clock::now();
    }

    int getDaysLate(){
        auto now = std::chrono::system_clock::now();
        auto overdueLate = now-dueDate_;
        auto delayDays = std::chrono::duration_cast<std::chrono::hours>(overdueLate).count();

        return delayDays;
    }

    int calculateFine(){
        auto now = std::chrono::system_clock::now();
        auto overdueLate = now-dueDate_;
        auto delayDays = std::chrono::duration_cast<std::chrono::hours>(overdueLate).count();

        return item_->calculateFine(delayDays*fineAmount_);
    }
};

//Checkout Derives

class Checkout : public Transaction {

    private:
        std::shared_ptr<LibraryItem> item_;
        std::shared_ptr<LibraryPatron> patron_;
        std::chrono::system_clock::time_point dueDate_;

    public:
        Checkout(std::shared_ptr<LibraryItem> item, std::shared_ptr<LibraryPatron> patron) : item_(item), patron_(patron) {
            dueDate_ = getTimestamp() + std::chrono::hours(24*patron_->getLoanExtensionDays());
        }

        //Getter
        std::shared_ptr<LibraryItem> getItem() const { return item_;}
        std::shared_ptr<LibraryPatron> getPatron() const {return patron_;}
        std::chrono::system_clock::time_point getDueDate() const { return dueDate_;}

        //Format duedate as string
        std::string getFormattedDueDate() const {
            //Formating here
            dueDate_ = std::chrono::system_clock::now() + std::chrono::hours(24*patron_->getLoanExtensionDays());
            auto time_t_due = std::chrono::system_clock::to_time_t(dueDate_);
            std::stringstream ss;

            ss<< std::put_time(std::localtime(&time_t_due), "%Y-%m-%d");

            return ss.str();
        }

        // Check if item is overdue
        bool isOverdue() const {
            return std::chrono::system_clock::now() > dueDate_;
        }

        // Calculate overdue fine

        double calculateFine() const{
            auto now = std::chrono::system_clock::now();
            auto overdueDuration = now - dueDate_;
            auto daysLate = std::chrono::duration_cast<std::chrono::hours>(overdueDuration).count() / 24;

            return item_->calculateFine(daysLate);
        }

        //Implement pure virtual function
        std::string getTransactionType() const override{
            return "Checkout";
        }

        std::string getDetails() const override{
        auto time_t_due = std::chrono::system_clock::to_time_t(dueDate_);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t_due), "%Y-%m-%d");

        return "CHECKOUT: Item: "+ item_->getTitle()+", Patron: "+patron_->getName()+", and Overdue Date: "+ ss.str();
        }
};

class Reservation : public Transaction{
private:
        std::shared_ptr<LibraryItem> item_;
        std::shared_ptr<LibraryPatron> patron_;
        std::string status_;
        std::chrono::system_clock::time_point pickupDeadline_;
public:
    Reservation(std::shared_ptr<LibraryItem> item,std::shared_ptr<LibraryPatron> patron,std::string status): item_(item), patron_(patron), status_(std::move(status)){
        pickupDeadline_ = getTimestamp() + std::chrono::hours(24*patron_->getLoanExtensionDays());
    }
    ~Reservation() = default;

    //Overrides
    std::string getTransactionType() const override{
        return "Reservation";
    }

    std::string getDetails() const override{
        auto time_t_due = std::chrono::system_clock::to_time_t(dueDate_);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t_due), "%Y-%m-%d");

        return "RESERVATION: Item: "+ item_->getTitle()+", Patron: "+patron_->getName()+", and Pickup Deadline: "+ ss.str();
    }

    bool isExpired() {
        return pickupDeadline_ < std::chrono::system_clock::now();
    }

    bool cancelReservation() {
        status = "Canceled"; 
        return true;
    }

    bool markReadyForPickup() {
        return true;
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
