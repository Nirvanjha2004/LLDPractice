#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <ctime>

using namespace std;

class Customer{
    private:
    string name;
    int custId;

    Customer(string name, int id){
        this->custId = id; 
        this->name = name; 
    }
};

class MenuItem {
    int itemId;
    string name;
    MenuItem(int id, string name){
        this->name = name;
        this->itemId = id; 
    }
};

class OrderState {
    public:
    virtual void next(Order* order) = 0;
    virtual string getName() = 0;
};

class PlacedState;
class ConfirmedState;
class PreparingState;
class ReadyForPickupState;
class OutForDeliveryState;
class DeliveredState;
class CancelledState;

class Order{
    int orderId;
    Customer* cr; 
    vector<MenuItem*> items;
    int totalAmount;
    OrderState* st;
    int timestamp;
    public: 
    Order(int id, Customer* cr, vector<MenuItem*> items, int total, OrderState* st, int time){
        this->cr = cr;
        this->items = items;
        this->orderId = id;
        this->st = st; 
        this->timestamp = time;
        this->totalAmount = total; 
    }

    void nextState(){
        st->next(this);
    }

    void setState(OrderState* newState){
        this->st = newState;
    }

    OrderState* getState(){
        return st;
    }

    void printOrderDetails(){
        cout << "Current State: " << st->getName() << endl;
    }
};

class PlacedState : public OrderState{
    public:
    void next(Order* order) override {
        order->setState(new ConfirmedState());
    }
    string getName() override {
        return "Placed";
    }
};

class ConfirmedState : public OrderState{
    public:
    void next(Order* order) override {
        order->setState(new PreparingState());
    }
    string getName() override {
        return "Confirmed";
    }
};

class PreparingState : public OrderState{
    public:
    void next(Order* order) override {
        order->setState(new ReadyForPickupState());
    }
    string getName() override {
        return "Preparing";
    }
};

class ReadyForPickupState : public OrderState{
    public:
    void next(Order* order) override {
        order->setState(new OutForDeliveryState());
    }
    string getName() override {
        return "Ready for Pickup";
    }
};

class OutForDeliveryState : public OrderState{
    public:
    void next(Order* order) override {
        order->setState(new DeliveredState());
    }
    string getName() override {
        return "Out for Delivery";
    }
};

class DeliveredState : public OrderState{
    public:
    void next(Order* order) override {
        cout << "Order already delivered. No next state.\n";
    }
    string getName() override {
        return "Delivered";
    }
};

class CancelledState : public OrderState{
    public:
    void next(Order* order) override {
        cout << "Order cancelled. No next state.\n";
    }
    string getName() override {
        return "Cancelled";
    }
};

class MainOrderManager{
    public:
    Customer* cr;
    vector<MenuItem*> menuItems;
    int ordId = 0;
    OrderState* os; 
    vector<Order*> orders;

    MainOrderManager(Customer* cr, vector<MenuItem*> menuItems, int id, OrderState* os){
        this->cr = cr;
        this->menuItems = menuItems;
        this->ordId = id; 
        this->os = os;
    }

    void createOrder(){
        int total = 0;
        Order* or = new Order(ordId, cr, menuItems, total, os, (int)time(nullptr));
        orders.push_back(or);
        ordId++;
        or->printOrderDetails();
        or->nextState(); 
    }
    
};