#include <string>
#include <vector>
#include "../include/WareHouse.h"
#include "../include/Action.h"
#include "../include/Volunteer.h"
#include "../include/Customer.h"
#include <iostream>

using namespace std;
extern WareHouse* backup;

//BaseAction
BaseAction::BaseAction():
    errorMsg(), status()
{}
ActionStatus BaseAction::getStatus() const{
    return status;
}

void BaseAction::complete(){ //after an action completed we put a flag of completed on the status for the print log
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg){
    status = ActionStatus::ERROR; //if the action didnt work we throw an error
    (*this).errorMsg = errorMsg;
}

string BaseAction::getErrorMsg() const{
    return errorMsg;
}

BaseAction::~BaseAction() {} // Virtual destructor

//SimulateStep
SimulateStep::SimulateStep(int numOfSteps): numOfSteps(numOfSteps)
{}

void SimulateStep::act(WareHouse &wareHouse) {
    wareHouse.simulateStep(numOfSteps); //call the step on the warehouse 
    complete();
}

string SimulateStep::toString() const {
    string statusOfTheAction;
    if (getStatus() == ActionStatus::ERROR) {
        statusOfTheAction = "ERROR";
    }
    else {
        statusOfTheAction = "COMPLETED";
    }
    return "simulateStep " + to_string(numOfSteps)  + " " + statusOfTheAction;
}

SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
}


AddOrder::AddOrder(int id) : customerId(id) 
{}

void AddOrder::act(WareHouse &wareHouse) {
    Customer& customer = wareHouse.getCustomer(customerId);
    int dist = customer.getCustomerDistance();
    bool canMakeOrder = customer.canMakeOrder();
    int orderId = wareHouse.getOrderCounter();
    //check if the customer doesn't exist or if he can take the order 
    if (dist == -1 || !canMakeOrder) {
        error("Error: Cannot place this order");
        cout << getErrorMsg() << endl;
    }
    else {
        //if he can take we assign the order to him, increment the order counter and push the order to the vector of pending orders
        Order* newOrder = new Order(orderId, customerId, dist);
        customer.addOrder(orderId);
        wareHouse.incOrderCounter();
        wareHouse.addOrder(newOrder);
        complete();
    }
}

string AddOrder::toString() const {
    string statusOfTheAction;
    if (getStatus() == ActionStatus::ERROR) {
        statusOfTheAction = "ERROR";
    }
    else {
        statusOfTheAction = "COMPLETED";
    }
    return "order " + to_string(customerId) + " " + statusOfTheAction;
}

AddOrder* AddOrder::clone() const {
    return new AddOrder(*this);
}
   

AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) :
    customerName(customerName), customerType((customerType == "soldier") ? CustomerType::Soldier : CustomerType::Civilian), distance(distance), maxOrders(maxOrders)
{}   //check the type from the input string

void AddCustomer::act(WareHouse &wareHouse) {
    Customer* newCustomer;
    int customerId = wareHouse.getCustomerCounter();
    if (customerType == CustomerType::Civilian) {
        newCustomer = new CivilianCustomer(customerId, customerName, distance, maxOrders);
    }
    else if (customerType == CustomerType::Soldier) {
        newCustomer = new SoldierCustomer(customerId, customerName, distance, maxOrders);
    }
    //create new customer and perform increment on the counter and add him to the customers vector
    wareHouse.incCustomerCounter();
    wareHouse.addCustomer(newCustomer);
    complete(); 
}

AddCustomer* AddCustomer::clone() const {
    return new AddCustomer(*this);
}

string AddCustomer::toString() const {
    string statusOfTheAction;
    string type;
    if (getStatus() == ActionStatus::ERROR) {
        statusOfTheAction = "ERROR";
    }
    else {
        statusOfTheAction = "COMPLETED";
    }
    if (customerType == CustomerType::Civilian) {
        type = "civilian";
    }
    else if (customerType == CustomerType::Soldier) {
        type = "soldier";
    }
    return "customer " + customerName + " " + type + " " + to_string(distance) + " " + to_string(maxOrders) + " " + statusOfTheAction;
}



PrintOrderStatus::PrintOrderStatus(int id) : orderId(id)
{}

void PrintOrderStatus::act(WareHouse &wareHouse) {
    Order order = wareHouse.getOrder(orderId);
    int id = order.getId();
    if (id == -1) {
        error("Error: Order doesn't exist");
        cout << getErrorMsg() << endl;
    }
    else {
        cout << order.toString() << endl; //perform to string of the order header on it
        complete();
    }
}

PrintOrderStatus* PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const {
    string statusOfTheAction;
    if (getStatus() == ActionStatus::ERROR) {
        statusOfTheAction = "ERROR";
    }
    else {
        statusOfTheAction = "COMPLETED";
    }
    return ("orderStatus " + to_string(orderId) + " " + statusOfTheAction);
}
  

PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId)
{}


void PrintCustomerStatus::act(WareHouse &wareHouse) {
    if (wareHouse.getCustomer(customerId).getId() == -1) {   
        error("Error: Customer doesn't exist");
        cout << getErrorMsg() << endl;
    }
    else { //if the customer exist, print all his orders and their status
        Customer &customer = wareHouse.getCustomer(customerId);
        const vector<int> &customerOrders = customer.getOrdersIds();
        cout << "CustomerID: " << customerId << endl;
        string orderStatus;
        for(int order : customerOrders){
            const Order &customerOrder = wareHouse.getOrder(order);
            if (customerOrder.getStatus() == OrderStatus::PENDING) {
                orderStatus = "Pending";
            }
            else if (customerOrder.getStatus() == OrderStatus::COLLECTING) {
                orderStatus = "Collecting";
            }
            else if (customerOrder.getStatus() == OrderStatus::DELIVERING) {
                orderStatus = "Delivering";
            }
            
            else if (customerOrder.getStatus() == OrderStatus::COMPLETED) {
                orderStatus = "Completed";
            }
            cout << "OrderID: " << order << endl;
            cout << "OrderStatus: " << orderStatus << endl;
        }
        cout << "numOrdersLeft: " << customer.getNumOrdersLeft() << endl; //print how much orders left for the customer.
        complete();
    }
}

PrintCustomerStatus* PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const {
    string statusOfTheAction;
    if (getStatus() == ActionStatus::ERROR) {
        statusOfTheAction = "ERROR";
    }
    else {
        statusOfTheAction = "COMPLETED";
    }
    return ("customerStatus " + to_string(customerId) + " " + statusOfTheAction);
}



PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id) 
{}

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    Volunteer& volunteer = wareHouse.getVolunteer(volunteerId);
    if (volunteer.getId() == -1) {
        error("Error: Volunteer dosen't exist");
        cout << getErrorMsg() << endl;
    }
    else {
        cout << volunteer.toString() << endl; //perform to string of the volunteer header on him
        complete();
    }
}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const {
    string statusOfTheAction;
    if (getStatus() == ActionStatus::ERROR) {
        statusOfTheAction = "ERROR";
    }
    else {
        statusOfTheAction = "COMPLETED";
    }
    return ("volunteerStatus " + to_string(volunteerId) + " " + statusOfTheAction);
}



PrintActionsLog::PrintActionsLog() : BaseAction()
{}

void PrintActionsLog::act(WareHouse &wareHouse) {
    const vector<BaseAction*> &actions = wareHouse.getActions();
    for (auto &action : actions) { //print all actions performed at the warehouse and their status
        cout << action->toString() << endl;
    }
    complete();
}

PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const {
    string statusOfTheAction;
    if (getStatus() == ActionStatus::ERROR) {
        statusOfTheAction = "ERROR";
    }
    else {
        statusOfTheAction = "COMPLETED";
    }
    return ("log " + statusOfTheAction);
}


Close::Close()
{}


void Close::act(WareHouse &wareHouse){
    int size = wareHouse.getOrderCounter();
    string statusOfTheAction;
    for (int i = 0; i < size; i++) { //print all the orders from the first to last and their status
        Order* order = &wareHouse.getOrder(i);
        cout << "OrderID: " << order->getId() << ", CustomerID: " << order->getCustomerId() << ", OrderStatus: " << order->orderStatusToString(order->getStatus()) << endl;
    }
    complete();
    wareHouse.close(); //close the warehouse, change isOpen to close and get out of the start loop
}

Close* Close::clone() const {
    return new Close(*this);
}

string Close::toString() const {
    string statusOfTheAction;
    if (getStatus() == ActionStatus::ERROR) {
        statusOfTheAction = "ERROR";
    }
    else {
        statusOfTheAction = "COMPLETED";
    }
    return ("close " + statusOfTheAction);
}


BackupWareHouse::BackupWareHouse() 
{}

void BackupWareHouse::act(WareHouse &wareHouse) {
    if (backup != nullptr) {
        delete backup; //delete backup because we dont want assignment problems of the pointers
    }
    backup = new WareHouse(wareHouse); //if backup is null, call the copy constructor of mine and assign me to backup ("save")
    complete();
}

BackupWareHouse* BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
} 

string BackupWareHouse::toString() const {
    return "backup COMPLETED"; //always completed
}


RestoreWareHouse::RestoreWareHouse() 
{}

void RestoreWareHouse::act(WareHouse &wareHouse) {
    if (backup == nullptr) {
        error("Error: No backup available");
        cout << getErrorMsg() << endl;
    }
    else {
        wareHouse = *backup; //call the copy assignment opertor of mine, take backup and put it in me
        complete(); 
    }
}

RestoreWareHouse* RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const {
    string statusOfTheAction;
    if (getStatus() == ActionStatus::ERROR) {
        statusOfTheAction = "ERROR";
    }
    else {
        statusOfTheAction = "COMPLETED";
    }
    return ("restore " + statusOfTheAction);
}







