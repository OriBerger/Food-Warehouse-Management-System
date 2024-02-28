#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ios>
#include <algorithm>
#include "../include/WareHouse.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include "../include/Order.h"
#include "../include/Action.h"

using namespace std;

WareHouse::WareHouse(const string& configFilePath) : //initialize all fields
    fakeCustomer(),
    fakeVolunteer(),
    fakeOrder(),
	isOpen(true),
    actionsLog(),
	volunteers(),
	pendingOrders(),
	inProcessOrders(),
	completedOrders(),
	customers(),
	customerCounter(0),
	volunteerCounter(0),
    orderCounter(0)    
{   //create fake pointers to the case the method "get" can't find someone
    fakeCustomer = new CivilianCustomer(-1, "can't found", -1, -1);
    fakeVolunteer = new CollectorVolunteer(-1, "can't found", -1);
    fakeOrder = new Order(-1, -1, -1);
    parse(configFilePath); //parsing the text file
}

//Copy constructor
WareHouse::WareHouse(const WareHouse& other) : //clone all fields
    fakeCustomer(other.fakeCustomer ? other.fakeCustomer->clone() : nullptr),
    fakeVolunteer(other.fakeVolunteer ? other.fakeVolunteer->clone() : nullptr),
    fakeOrder(other.fakeOrder ? other.fakeOrder->clone() : nullptr),
    isOpen(other.getIsOpen()),
    actionsLog(other.actionsLog), 
    volunteers(other.volunteers), 
    pendingOrders(other.pendingOrders), 
    inProcessOrders(other.inProcessOrders), 
    completedOrders(other.completedOrders), 
    customers(other.customers),
    customerCounter(other.getCustomerCounter()),
    volunteerCounter(other.getVolunteerCounter()),
    orderCounter(other.getOrderCounter())
{
    for (auto& order : pendingOrders) {
        order = order->clone();
    }
    for (auto& order : inProcessOrders) {
        order = order->clone();
    }
    for (auto& order : completedOrders) {
        order = order->clone();
    }
    for (auto& customer : customers) {
        customer = customer->clone();
    }
    for (auto& volunteer : volunteers) {
        volunteer = volunteer->clone();
    }
    for (auto& action : actionsLog) {
        action = action->clone();
    }
}


//Copy assignment operator
WareHouse& WareHouse::operator=(const WareHouse& other) {
    if (this == &other) {
        return *this; //avoid if other is the same like me
    }

    // Clear existing resources
    for (Customer* customer : customers) {
        delete customer;
    }
    for (Volunteer* volunteer : volunteers) {
        delete volunteer;
    }
    for (BaseAction* BaseAction : actionsLog) {
        delete BaseAction;
    }
    for (Order* completedOrder : completedOrders) {
        delete completedOrder;
    }
    for (Order* inProcessOrder : inProcessOrders) {
        delete inProcessOrder;
    }
    for (Order* pendingOrder : pendingOrders) {
        delete pendingOrder;
    }

    delete fakeCustomer;
    delete fakeVolunteer;
    delete fakeOrder;
    
    customers.clear();
    volunteers.clear();
    actionsLog.clear();
    completedOrders.clear();
    inProcessOrders.clear();
    pendingOrders.clear();

    // copy data from other
    isOpen = other.getIsOpen();
    orderCounter = other.getOrderCounter();
    customerCounter = other.getCustomerCounter();
    volunteerCounter = other.getVolunteerCounter();

    // clone fakeCustomer, fakeVolunteer and fakeOrder if they are not null
    fakeCustomer = other.fakeCustomer ? other.fakeCustomer->clone() : nullptr;
    fakeVolunteer = other.fakeVolunteer ? other.fakeVolunteer->clone() : nullptr;
    fakeOrder = other.fakeOrder ? other.fakeOrder->clone() : nullptr;

    // copy elements from other vectors
    for (auto order : other.pendingOrders) {
        pendingOrders.push_back(order->clone());
    }
    for (auto order : other.inProcessOrders) {
        inProcessOrders.push_back(order->clone());
    }
    for (auto order : other.completedOrders) {
        completedOrders.push_back(order->clone());
    }
    for (auto customer : other.customers) {
        customers.push_back(customer->clone());
    }
    for (auto volunteer : other.volunteers) {
        volunteers.push_back(volunteer->clone());
    }
    for (auto action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }
    return *this;
}


// Move constructor
WareHouse::WareHouse(WareHouse&& other) noexcept : 
    fakeCustomer(other.fakeCustomer ? other.fakeCustomer->clone() : nullptr),
    fakeVolunteer(other.fakeVolunteer ? other.fakeVolunteer->clone() : nullptr),
    fakeOrder(other.fakeOrder ? other.fakeOrder->clone() : nullptr),
    isOpen(other.isOpen),
    actionsLog(move(other.actionsLog)),
    volunteers(move(other.volunteers)),
    pendingOrders(move(other.pendingOrders)),
    inProcessOrders(move(other.inProcessOrders)),
    completedOrders(move(other.completedOrders)),
    customers(move(other.customers)),
    customerCounter(other.customerCounter),
    volunteerCounter(other.volunteerCounter),
    orderCounter(other.orderCounter)
{} //steal other resources


//Move assignment operator
WareHouse& WareHouse::operator=(WareHouse&& other) noexcept {
    if (this != &other) {
        // Release resources from the current object
        for (Customer* customer : customers) {
            delete customer;
        }
        for (Volunteer* volunteer : volunteers) {
            delete volunteer;
        }
        for (BaseAction* action : actionsLog) {
            delete action;
        }
        for (Order* completedOrder : completedOrders) {
            delete completedOrder;
        }
        for (Order* inProcessOrder : inProcessOrders) {
            delete inProcessOrder;
        }
        for (Order* pendingOrder : pendingOrders) {
            delete pendingOrder;
        }

        // Clear vectors in the current object
        customers.clear();
        volunteers.clear();
        actionsLog.clear();
        completedOrders.clear();
        pendingOrders.clear();
        inProcessOrders.clear();

        // Move resources from the other object
        isOpen = other.isOpen;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        customerCounter = other.customerCounter;
        pendingOrders = move(other.pendingOrders);
        inProcessOrders = move(other.inProcessOrders);
        completedOrders = move(other.completedOrders);
        volunteers = move(other.volunteers);
        customers = move(other.customers);
        actionsLog = move(other.actionsLog);
        fakeCustomer = other.fakeCustomer;
        fakeOrder = other.fakeOrder;
        fakeVolunteer = other.fakeVolunteer;

        // Reset other object
        other.isOpen = false;
        other.volunteerCounter = 0;
        other.orderCounter = 0;
        other.customerCounter = 0;
        other.fakeCustomer = nullptr;
        other.fakeVolunteer = nullptr;
        other.fakeOrder = nullptr;
    }
    return *this;
}



void WareHouse::start() {
	cout << "Warehouse is open!" <<endl;
    string userInput;
    while (isOpen) {
        getline(cin, userInput);
        istringstream iss(userInput);
        string command;
        int customerID;
        int orderID;
        int volunteerID;
        int numOfSteps;
        iss >> command;

        //check user input and perform the act he wants, create the action, call for its act and then push the action to the actions vector

        if (command == "order") {
            iss >> customerID;
            AddOrder* newOrder = new AddOrder(customerID);
            newOrder->act(*this);
            this->addAction(newOrder);
        } 
        else if (command == "step") {
            iss >> numOfSteps;
            SimulateStep* simulateStep = new SimulateStep(numOfSteps);
            simulateStep->act(*this);
            this->addAction(simulateStep);
        } 
        else if (command == "customer") {
            string name;
            iss >> name;
            string type;
            iss >> type;
            int distance;
            iss >> distance;
            int maxOrders;
            iss >> maxOrders;
            AddCustomer* newCustomer = new AddCustomer(name, type, distance, maxOrders);
            newCustomer->act(*this);
            this->addAction(newCustomer);
        } 
        else if (command == "log") {
            PrintActionsLog* printActionsLog = new PrintActionsLog();
            printActionsLog->act(*this);
            this->addAction(printActionsLog);
        } 
        else if (command == "close") {
            Close* close = new Close();
            close->act(*this);
            this->addAction(close);
            isOpen = false;
            break;
        }   
        else if (command == "orderStatus") {
            iss >> orderID;
            PrintOrderStatus* orderStatus = new PrintOrderStatus(orderID);
            orderStatus->act(*this);
            this->addAction(orderStatus);            
        } 
        else if (command == "volunteerStatus") {
            iss >> volunteerID;
            PrintVolunteerStatus* volunteerStatus = new PrintVolunteerStatus(volunteerID);
            volunteerStatus->act(*this);
            this->addAction(volunteerStatus); 
        } 
        else if (command == "customerStatus") {
            iss >> customerID;
            PrintCustomerStatus* customerStatus = new PrintCustomerStatus(customerID);
            customerStatus->act(*this);
            this->addAction(customerStatus);  
        }
        else if (command == "backup") {
            BackupWareHouse* backUp = new BackupWareHouse();
            backUp->act(*this);
            this->addAction(backUp); 
        }
        else if (command == "restore") {
            RestoreWareHouse* restore = new RestoreWareHouse();
            restore->act(*this);
            this->addAction(restore); 
        }
    }
}

//get counters
int WareHouse::getOrderCounter() const {
    return orderCounter;
}

int WareHouse::getCustomerCounter() const {
    return customerCounter;
}

int WareHouse::getVolunteerCounter() const {
    return volunteerCounter;
}

//increment counters
void WareHouse::incOrderCounter() {
    orderCounter += 1;
}
void WareHouse::incCustomerCounter() {
    customerCounter += 1;
}
void WareHouse::incVolunteerCounter() {
    volunteerCounter += 1;
}

bool WareHouse::getIsOpen() const {
    return isOpen;
}

//push pointers to warehouse vectors
void WareHouse::addOrder(Order* order) {
    pendingOrders.push_back(order);
}

void WareHouse::addCustomer(Customer* customer) {
    customers.push_back(customer);
}

void WareHouse::addAction(BaseAction* action) {
    actionsLog.push_back(action);
}


//perform steps in the simulation according to the number of steps the user wants
void WareHouse::simulateStep(int numOfSteps) {
    int countdown = numOfSteps;
    while (countdown > 0) { //loop the perform number of steps until countdown is 0
        
        auto pendingOrdersIter = pendingOrders.begin(); // Use iterators for safe deletion while iterating       
        while (pendingOrdersIter != pendingOrders.end()){ //go over all pending orders and assign them volunteers, if they can take orders
            bool iterIsPromoted = false;         
            Order* p_order = *pendingOrdersIter;
            if (p_order->getStatus() == OrderStatus::PENDING){ //take pending order and search a collector for it
                for (Volunteer* p_volunteer : volunteers ){
                    if (CollectorVolunteer* p_collector = dynamic_cast<CollectorVolunteer*>(p_volunteer)){ //dynamic_cast returns a pointer of the target type if the conversion is possible; otherwise, it returns nullptr. This allows you to identify the specific type of each Volunteer during runtime.
                        if (p_collector->canTakeOrder(*p_order)){
                            p_collector->acceptOrder(*p_order); //if the collector can take, assign the order to him and set all fields as shown down
                            p_order->setCollectorId(p_collector->getId());
                            p_order->setStatus(OrderStatus::COLLECTING);
                            pendingOrdersIter = pendingOrders.erase(pendingOrdersIter); //erase the order from pending vector and push it to the in process vector
                            inProcessOrders.push_back(p_order);
                            iterIsPromoted = true; //flag for the loop that we promote the iter by erase and take the element after the order that erased
                            break;
                        }
                    }
                }
                if (!iterIsPromoted) {
                    pendingOrdersIter++; //if we didnt assign, promote here
                }
            }
            else if ((*p_order).getStatus() == OrderStatus::COLLECTING){ //take in process order(collected and still in the pending vector) and search a driver for it
                for (Volunteer* p_volunteer : volunteers ){
                    if (DriverVolunteer* p_driver = dynamic_cast<DriverVolunteer*>(p_volunteer)){   
                        if (p_driver->canTakeOrder(*p_order)){  //if the driver can take, assign the order to him and set all fields as shown down
                            p_driver->acceptOrder(*p_order);                            
                            p_order->setDriverId((*p_driver).getId());
                            p_order->setStatus(OrderStatus::DELIVERING);
                            pendingOrdersIter = pendingOrders.erase(pendingOrdersIter); //erase the order from pending vector and push it to the in process vector
                            inProcessOrders.push_back(p_order);
                            iterIsPromoted = true;  //flag for the loop that we promote the iter by erase and take the element after the order that erased
                            break;
                        }
                    }
                }
                if (!iterIsPromoted) {
                    pendingOrdersIter++; //if we didnt assign, promote here
                }            
            }
        } 
        auto volunteersListIter = volunteers.begin(); // Use iterators for safe deletion while iterating       
        while (volunteersListIter != volunteers.end()){ //go over all volunteers, perform a step on them, and delete them if they got to maxOrders of them
            bool isPromoted = false;         
            Volunteer* p_volunteer = *volunteersListIter;
            if (p_volunteer->isBusy()){
                (*p_volunteer).step(); //perform a step according to the cool down/distance per step
            }
            if (CollectorVolunteer* p_collector = dynamic_cast<CollectorVolunteer*>(p_volunteer)){ 
                if ((p_collector->getTimeLeft() == 0)){   //if the collector just finished take the order and transfer it from in process vector back to pending vector but with a flag the show it was collected and wait for driver     
                    Order* p_order = &this->getOrder(p_collector->getCompletedOrderId());
                    if (p_order) {
                        auto iter = find(inProcessOrders.begin(), inProcessOrders.end(), p_order);
                        if (iter != inProcessOrders.end() && p_order->getStatus() == OrderStatus::COLLECTING) {
                            pendingOrders.push_back(p_order);
                            inProcessOrders.erase(iter);
                        }
                    }
                    if (LimitedCollectorVolunteer* p_Lcollector = dynamic_cast<LimitedCollectorVolunteer*>(p_volunteer)){
                        if(!(*p_Lcollector).hasOrdersLeft()){ //if this is a limited collector, delete it and take it out from the volunteers vector
                            delete p_Lcollector;
                            volunteersListIter = volunteers.erase(volunteersListIter);
                            isPromoted = true; //go to the volunteer after the one that deleted
                        }                 
                    }
                } 
                if (!isPromoted)  {
                    volunteersListIter++; //if we didnt delete him, go over the one who is busy now
                } 
            }
            else if (DriverVolunteer* p_driver = dynamic_cast<DriverVolunteer*>(p_volunteer)){ 
                if ((p_driver->getDistanceLeft() == 0)){ //if the driver just finished take the order and transfer it from in process vector back to completed vector     
                    Order* p_order =&this->getOrder(p_driver->getCompletedOrderId());
                    if (p_order) {
                        auto iter = find(inProcessOrders.begin(), inProcessOrders.end(), p_order);
                        if (iter != inProcessOrders.end() && p_order->getStatus() == OrderStatus::DELIVERING) {
                            completedOrders.push_back(p_order);
                            inProcessOrders.erase(iter);
                            p_order->setStatus(OrderStatus::COMPLETED); //here we change the status, because once it gets to completed vector, no one else take the order ever
                        }                        
                    }
                    if (LimitedDriverVolunteer* p_Ldriver = dynamic_cast<LimitedDriverVolunteer*>(p_volunteer)){
                        if (!(*p_Ldriver).hasOrdersLeft()){  //if this is a limited driver, delete it and take it out from the volunteers vector
                            delete p_Ldriver;
                            volunteersListIter = volunteers.erase(volunteersListIter);
                            isPromoted = true; //go to the volunteer after the one that deleted
                        }
                    }
                }
                if (!isPromoted)  {
                    volunteersListIter++;  //if we didnt delete him, go over the one who is busy now
                }    
            }        
        }
        countdown = countdown - 1;  //afer one step in the simulation, decrease countdown  
    }
}

//getters of the pointers, return fake pointer if the pointer doesn't exist in the vector
Customer& WareHouse::getCustomer(int customerId) const {
    for (const auto& customer : customers) {
        if (customer->getId() == customerId) {
            return *customer;
        }
    }
     return *fakeCustomer;
}

Volunteer& WareHouse::getVolunteer(int volunteerId) const {
    for (const auto& volunteer : volunteers) {
        if (volunteer->getId() == volunteerId) {
            return *volunteer;
        }
    }
    return *fakeVolunteer;
}

Order& WareHouse::getOrder(int orderId) const {
    for (const auto& order : pendingOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    for (const auto& order : inProcessOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    for (const auto& order : completedOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    return *fakeOrder;
}

const vector<BaseAction*> &WareHouse::getActions() const {
    return actionsLog;
}

void WareHouse::close() {
    isOpen = false;
}

void WareHouse::open() {
    isOpen = true;
}

//Destuctor
WareHouse::~WareHouse() {
    for (Customer* customer : customers) {
        delete customer;
    }
    for (Volunteer* volunteer : volunteers) {
        delete volunteer;
    }
    for (BaseAction* BaseAction : actionsLog) {
        delete BaseAction;
    }
    for (Order* completedOrder : completedOrders) {
        delete completedOrder;
    }
    for (Order* inProcessOrder : inProcessOrders) {
        delete inProcessOrder;
    }
    for (Order* pendingOrder : pendingOrders) {
        delete pendingOrder;
    }
    delete fakeCustomer;
    delete fakeVolunteer;
    delete fakeOrder;
}

//parse the input text 
void WareHouse::parse(string configFilePath) {
    ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        cerr << "Error: Unable to open configuration file " << endl;
        return;
    }
    string line;
    while (getline(configFile, line)) {
        istringstream iss(line);
        string command;
        if (!(iss >> command)) {
            continue;
        }
        if (command == "customer") {
            string name, type;
            int distance, maxOrders;
            if (!(iss >> name >> type >> distance >> maxOrders)) {
                cerr << " " << endl;
                continue;
            }
            if(type == "soldier") {
                Customer* soldier_customer = new SoldierCustomer(getCustomerCounter(), name, distance, maxOrders);
                customers.push_back(soldier_customer);
            }
            else if(type == "civilian") {
                Customer* civilian_customer = new CivilianCustomer(getCustomerCounter(), name, distance, maxOrders);
                customers.push_back(civilian_customer);
            }
            incCustomerCounter();
        } 
        else if (command == "volunteer") {
            string name, type;
            int param1, param2 = 0, param3 = 0;
            if (!(iss >> name >> type >> param1)) {
                cerr << " " << endl;
                continue;
            }
            if (type == "collector") { 
                Volunteer* collector_volunteer = new CollectorVolunteer(getVolunteerCounter(), name, param1);
                volunteers.push_back(collector_volunteer);
            } 
            else if (type == "limited_collector") {
                if (!(iss >> param2)) {
                    cerr << " " << endl;
                    continue;
                }
                Volunteer* limited_collector_volunteer = new LimitedCollectorVolunteer(getVolunteerCounter(), name, param1, param2);
                volunteers.push_back(limited_collector_volunteer);
            } 
            else if (type == "driver") {
                if (!(iss >> param2)) {
                    cerr << " " << endl;
                    continue;
                }
                Volunteer* driver_volunteer = new DriverVolunteer(getVolunteerCounter(), name, param1, param2);
                volunteers.push_back(driver_volunteer);
            } 
            else if (type == "limited_driver") {
                if (!(iss >> param2 >> param3)) {
                    cerr << " " << endl;
                    continue;
                }
                Volunteer* limited_driver_volunteer = new LimitedDriverVolunteer(getVolunteerCounter(), name, param1, param2, param3);
                volunteers.push_back(limited_driver_volunteer);
            } 
            else {
                cerr <<  " "  << endl;
            }
            incVolunteerCounter();
        } 
    }
    configFile.close();
}
