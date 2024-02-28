#include "../include/Customer.h"
using namespace std;

Customer::Customer (int id, const string &name, int locationDistance, int maxOrders):
    id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersLeft(maxOrders), ordersId() {
}

int Customer::getId() const {
    return id;
}

const string& Customer::getName() const {
    return name;
}

int Customer::getCustomerDistance() const {
    return locationDistance;
}

int Customer::getMaxOrders() const {
    return maxOrders;
}

int  Customer::getNumOrders() const {
    return ordersId.size();
} //Returns num of orders the customer has made so far

int Customer::getNumOrdersLeft() const{
    return ordersLeft;
}

bool Customer::canMakeOrder() const {
    if (ordersLeft > 0) {
        return true;
    }
    return false;
}//Returns true if the customer didn't reach max orders

Customer::~Customer() {} // Virtual destructor

const vector<int>& Customer::getOrdersIds() const { //get the vector of the orders the customer ordered
    return ordersId;
}

int Customer::addOrder(int orderId) {
    if (canMakeOrder() == true){
        ordersId.push_back(orderId);
        ordersLeft = maxOrders - getNumOrders(); //set the orders left for the customer
        return orderId;
    }
    return -1;
}//return OrderId if order was added successfully, -1 otherwise

SoldierCustomer::SoldierCustomer(int id, const string& name, int locationDistance, int maxOrders) :
    Customer(id, name, locationDistance, maxOrders) { //call its father constructor
}

SoldierCustomer* SoldierCustomer::clone() const {
    return new SoldierCustomer(*this);
}


CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders) :
    Customer(id, name, locationDistance, maxOrders) {//call its father constructor
}

CivilianCustomer* CivilianCustomer::clone() const {
    return new CivilianCustomer(*this);
}

