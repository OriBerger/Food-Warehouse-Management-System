#pragma once
#include <string>
#include <vector>

#include "../include/Order.h"
#include "../include/Customer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string& configFilePath);  //constructor
        WareHouse(const WareHouse &other);    //copy constructor
        WareHouse& operator=(const WareHouse &other); //copy assignment operator
        WareHouse(WareHouse &&other) noexcept;  //move constructor
        WareHouse& operator=(WareHouse &&other) noexcept;  //move assignment operator
        void start();
        void addOrder(Order* order);
        void addCustomer(Customer* customer);
        void addAction(BaseAction* action);
        Customer& getCustomer(int customerId) const;
        Volunteer& getVolunteer(int volunteerId) const;
        Order& getOrder(int orderId) const;
        const vector<BaseAction*>& getActions() const;
        void close();
        void open();
        //get the counters
        int getOrderCounter() const;
        int getCustomerCounter() const;
        int getVolunteerCounter() const;
        //increment the counters
        void incOrderCounter();
        void incCustomerCounter();
        void incVolunteerCounter();

        bool getIsOpen() const; //get isOpen
        void simulateStep(int numOfSteps); //perform steps in the warehouse depending the number of steps the user want
        void parse(string configFilePath); //parsing the config file
        ~WareHouse();
        
        //default pointers of customer,volunteer,order to return if we search something that doesn't exist in the vectors.
        Customer* fakeCustomer;
        Volunteer* fakeVolunteer;
        Order* fakeOrder;

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;

        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; //For assigning unique order IDs
        
        
};