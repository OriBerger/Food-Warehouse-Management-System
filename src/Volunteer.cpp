# include "../include/Volunteer.h"
#include "../include/Order.h"

using namespace std;

 //Volunteer 
Volunteer::Volunteer(int id, const string &name):
    completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name)  
{}

int Volunteer::getId() const {
    return id;
}

const string& Volunteer::getName() const{
    return name;
}

int Volunteer::getActiveOrderId() const{
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const{
    return completedOrderId;
}

bool Volunteer::isBusy() const{
    return activeOrderId != NO_ORDER;
}  

Volunteer::~Volunteer() {} // Virtual destructor


//CollectorVolunteer
CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown):
    Volunteer(id, name), coolDown(coolDown), timeLeft(0)
{}

CollectorVolunteer* CollectorVolunteer::clone() const {//create a new identical CollectorVolunteer in the Heap, and return iths address
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step(){
    if ((*this).decreaseCoolDown()){ //true if timeLeft=0
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

int CollectorVolunteer::getCoolDown() const{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const{
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown(){ //Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
    timeLeft = timeLeft - 1;
    return timeLeft == 0;
}

bool CollectorVolunteer::hasOrdersLeft() const{
    return true;
}


bool CollectorVolunteer::canTakeOrder(const Order &order) const{
    return (!(*this).isBusy() && (timeLeft == 0));
}

void CollectorVolunteer::acceptOrder(const Order &order){
    activeOrderId = order.getId();
    timeLeft = coolDown;
}

string CollectorVolunteer::toString() const{
    int id = getId();
    bool isBusyNow = isBusy();
    int orderId = -1;
    int timeLeft = -1;
    if (isBusyNow == true) {
        orderId = getActiveOrderId();
        timeLeft = getTimeLeft();
        return ("VolunteerID: " + to_string(id) + "\n" +
                "isBusy: " + "True" + "\n" + 
                "OrderID: " + to_string(orderId) + "\n" + 
                "TimeLeft: " + to_string(timeLeft) + "\n" + 
                "OrdersLeft: No Limit");
    }
    //int ordersLeft = getNumOrdersLeft();
    return ("VolunteerID: " + to_string(id) + "\n" +
            "isBusy: " + "False" + "\n" + 
            "OrderID: None" + "\n" + 
            "TimeLeft: None" + "\n" + 
            "OrdersLeft: No Limit");
}


void CollectorVolunteer::setTimeLeft(int time){
    timeLeft = time;
}



//LimitedCollectorVolunteer
LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown ,int maxOrders):
    CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders)
{}

LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const{
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const{
    return ordersLeft != 0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const{
    return (!(*this).isBusy() && ((*this).getTimeLeft() == 0) && ordersLeft != 0);
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order){
    activeOrderId = order.getId();
    (*this).setTimeLeft((*this).getCoolDown());
    ordersLeft = ordersLeft - 1;
}

int LimitedCollectorVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

int LimitedCollectorVolunteer::decNumOrdersLeft() {
    ordersLeft = ordersLeft -1;
    return ordersLeft;
}


string LimitedCollectorVolunteer::toString() const{
    int id = getId();
    bool isBusyNow = isBusy();
    int orderId = -1;
    int timeLeft = -1;
    int ordersLeft = getNumOrdersLeft();
    if (isBusyNow == true) {
        orderId = getActiveOrderId();
        timeLeft = getTimeLeft();
        return ("VolunteerID: " + to_string(id) + "\n" +
                "isBusy: " + "True" + "\n" + 
                "OrderID: " + to_string(orderId) + "\n" + 
                "TimeLeft: " + to_string(timeLeft) + "\n" + 
                "OrdersLeft: " + to_string(ordersLeft));
    }
    return ("VolunteerID: " + to_string(id) + "\n" +
            "isBusy: " + "False" + "\n" + 
            "OrderID: None" + "\n" + 
            "TimeLeft: None" + "\n" + 
            "OrdersLeft: " + to_string(ordersLeft));
}



//DriverVolunteer
DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep):
    Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(NO_ORDER)
{}

DriverVolunteer* DriverVolunteer::clone() const{
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const{
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const{
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const{
    return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft(){ //Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
    distanceLeft = distanceLeft - distancePerStep;
    if (distanceLeft < 0){ //avoid negative distance
        distanceLeft = 0;
    }
    return distanceLeft==0;
}

bool DriverVolunteer::hasOrdersLeft() const{
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const{ // Signal if the volunteer is not busy and the order is within the maxDistance
    return (!(*this).isBusy() && order.getDistance() <= maxDistance);
}

void DriverVolunteer::acceptOrder(const Order &order){ // Assign distanceLeft to order's distance
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
}

void DriverVolunteer::step(){ // Decrease distanceLeft by distancePerStep
    if ((*this).decreaseDistanceLeft()){ //true if distanceLeft<=0, so order is finished
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

string DriverVolunteer::toString() const{
    int id = getId();
    bool isBusyNow = isBusy();
    int orderId = -1;
    int timeLeft = -1;
    if (isBusyNow == true) {
        orderId = getActiveOrderId();
        return ("VolunteerID: " + to_string(id) + "\n" +
                "isBusy: " + "True" + "\n" + 
                "OrderID: " + to_string(orderId) + "\n" + 
                "TimeLeft: " + to_string(timeLeft) + "\n" + 
                "OrdersLeft: No Limit");
    }
    return ("VolunteerID: " + to_string(id) + "\n" +
            "isBusy: " + "False" + "\n" + 
            "OrderID: None" + "\n" + 
            "TimeLeft: None" + "\n" + 
            "OrdersLeft: No Limit");
}



/////
void DriverVolunteer::setDistanceLeft(int distance){
    distanceLeft = distance;
}

//LimitedDriverVolunteer
LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders):
    DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders)
{}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const{
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

int LimitedDriverVolunteer::decNumOrdersLeft() {
    ordersLeft = ordersLeft -1;
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const{
    return ordersLeft != 0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const{ // Signal if the volunteer is not busy, the order is within the maxDistance.
    return ((!(*this).isBusy()) && (order.getDistance() <= (*this).getMaxDistance()) && (ordersLeft > 0));
}

void LimitedDriverVolunteer::acceptOrder(const Order &order){ // Assign distanceLeft to order's distance and decrease ordersLeft
    activeOrderId = order.getId();
    (*this).setDistanceLeft(order.getDistance());
    ordersLeft = ordersLeft - 1;
}

string LimitedDriverVolunteer::toString() const{
    int id = getId();
    bool isBusyNow = isBusy();
    int orderId = -1;
    int distanceLeft = -1;
    int ordersLeft = getNumOrdersLeft();
    if (isBusyNow == true) {
        orderId = getActiveOrderId();
        distanceLeft = getDistanceLeft();
        return ("VolunteerID: " + to_string(id) + "\n" +
                "isBusy: " + "True" + "\n" + 
                "OrderID: " + to_string(orderId) + "\n" + 
                "TimeLeft: " + to_string(distanceLeft) + "\n" + 
                "OrdersLeft: " + to_string(ordersLeft));
    }
    return ("VolunteerID: " + to_string(id) + "\n" +
            "isBusy: " + "False" + "\n" + 
            "OrderID: None" + "\n" + 
            "TimeLeft: None" + "\n" + 
            "OrdersLeft: " + to_string(ordersLeft));
}