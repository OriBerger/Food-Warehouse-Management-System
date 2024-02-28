#include "../include/Order.h"
using std::string;
using std::to_string;

Order::Order(int id, int customerId, int distance): 
    id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER)
{}

int Order::getId() const{
    return id;
}

int Order::getCustomerId() const{
    return customerId;
}

void Order::setStatus(OrderStatus status){
    (*this).status = status;
}

void Order::setCollectorId(int collectorId){
    (*this).collectorId = collectorId;
}

void Order::setDriverId(int driverId){
    (*this).driverId = driverId;
}

int Order::getCollectorId() const{
    return collectorId;
}

int Order::getDriverId() const{
    return driverId;
}

OrderStatus Order::getStatus() const{
    return status;
}

const string Order::toString() const{
    string statusOfTheOrder = orderStatusToString(status);

    //check the status first, then print
    if (statusOfTheOrder == "Pending") {
        return("OrderId: " + to_string(id) + "\n" + 
                "OrderStatus: " + "Pending" + "\n" +
                "CustomerID: " + to_string(customerId) + "\n" +
                "Collector: None" + "\n" +
                "Driver: None");
    }
    else if (statusOfTheOrder == "Collecting") {
        return("OrderId: " + to_string(id) + "\n" + 
                "OrderStatus: " + "Collecting" + "\n" +
                "CustomerID: " + to_string(customerId) + "\n" +
                "Collector: " + to_string(collectorId) + "\n" +
                "Driver: None");
    }
    else if (statusOfTheOrder == "Delivering") {
        return("OrderId: " + to_string(id) + "\n" + 
                "OrderStatus: " + "Delivering" + "\n" +
                "CustomerID: " + to_string(customerId) + "\n" + 
                "Collector: " + to_string(collectorId) + "\n" +
                "Driver: " + to_string(driverId));
    }
    else if (statusOfTheOrder == "Completed") {
        return("OrderId: " + to_string(id) + "\n" + 
                "OrderStatus: " + "Completed" + "\n" +
                "CustomerID: " + to_string(customerId) + "\n" + 
                "Collector: " + to_string(collectorId) + "\n" +
                "Driver: " + to_string(driverId));
    }
    else { return " ";} //we never get to here, just avoiding warnings
}

/////
int Order::getDistance() const{
    return distance;
}

string Order::orderStatusToString(OrderStatus status) const { //take the status and make it a string lower letters like in the running example
    switch (status) {
    case OrderStatus::PENDING:
        return "Pending";
    case OrderStatus::COLLECTING:
        return "Collecting";
    case OrderStatus::DELIVERING:
        return "Delivering";
    case OrderStatus::COMPLETED:
        return "Completed";
    default:
        return "UNKNOWN"; //we never get to here, just avoiding warnings
    }
       
}

Order* Order::clone() const {
    return new Order(*this);
}