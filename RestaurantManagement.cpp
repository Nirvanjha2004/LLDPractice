#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <ctime>

using namespace std;

/* =====================================
   ENUMS
===================================== */

enum class OrderStatus
{
    PLACED,
    CONFIRMED,
    PREPARING,
    READY_FOR_PICKUP,
    OUT_FOR_DELIVERY,
    DELIVERED,
    CANCELLED
};

enum class PaymentStatus
{
    PENDING,
    SUCCESS,
    FAILED
};

/* =====================================
   FORWARD DECLARATIONS
===================================== */

class Restaurant;
class Order;

/* =====================================
   MENU ITEM
===================================== */

class MenuItem
{

private:
    int itemId;
    string name;
    int price;
    Restaurant *restaurant;

public:
    MenuItem(
        int id,
        string name,
        int price,
        Restaurant *rt)
    {
        this->itemId = id;
        this->name = name;
        this->price = price;
        this->restaurant = rt;
    }

    int getId()
    {
        return itemId;
    }

    int getPrice()
    {
        return price;
    }

    string getName()
    {
        return name;
    }

    Restaurant *getRestaurant()
    {
        return restaurant;
    }
};

/* =====================================
   CUSTOMER
===================================== */

class Customer
{

private:
    int customerId;
    string name;

public:
    Customer(
        int id,
        string name)
    {
        this->customerId = id;
        this->name = name;
    }

    string getName()
    {
        return name;
    }
};

/* =====================================
   RESTAURANT
===================================== */

class Restaurant
{

private:
    int restaurantId;
    string name;
    bool open;

    unordered_map<
        int,
        MenuItem *>
        menu;

public:
    Restaurant(
        int id,
        string name)
    {
        this->restaurantId = id;
        this->name = name;
        this->open = true;
    }

    void addMenuItem(
        MenuItem *item)
    {
        menu[item->getId()] = item;
    }

    bool isOpen()
    {
        return open;
    }

    bool hasItem(
        MenuItem *item)
    {
        return menu.count(
            item->getId());
    }

    string getName()
    {
        return name;
    }
};

/* =====================================
   CART
===================================== */

class Cart
{

private:
    Restaurant *selectedRestaurant;

    unordered_map<
        MenuItem *,
        int>
        items;

public:
    Cart()
    {
        selectedRestaurant = nullptr;
    }

    bool addItem(
        MenuItem *item,
        int quantity)
    {

        if (
            selectedRestaurant ==
            nullptr)
        {

            selectedRestaurant =
                item->getRestaurant();
        }

        if (
            selectedRestaurant !=
            item->getRestaurant())
        {

            cout
                << "Items from only one restaurant allowed\n";

            return false;
        }

        items[item] += quantity;

        return true;
    }

    int getTotalPrice()
    {

        int total = 0;

        for (
            auto &entry :
            items)
        {

            total +=
                entry.first
                    ->getPrice() *
                entry.second;
        }

        return total;
    }

    Restaurant *getRestaurant()
    {
        return selectedRestaurant;
    }

    unordered_map<
        MenuItem *,
        int> &
    getItems()
    {

        return items;
    }

    void clear()
    {

        items.clear();

        selectedRestaurant =
            nullptr;
    }
};

/* =====================================
   PAYMENT STRATEGY
===================================== */

class PaymentStrategy
{

public:
    virtual bool pay(
        int amount) = 0;

    virtual ~PaymentStrategy() =
        default;
};

class UPI : public PaymentStrategy
{

public:
    bool pay(
        int amount) override
    {

        cout
            << "UPI payment of ₹"
            << amount
            << " successful\n";

        return true;
    }
};

class CardPayment
    : public PaymentStrategy
{

public:
    bool pay(
        int amount) override
    {

        cout
            << "Card payment of ₹"
            << amount
            << " successful\n";

        return true;
    }
};

/* =====================================
   DELIVERY PARTNER
===================================== */

class DeliveryPartner
{

private:
    int partnerId;
    string name;
    bool available;

public:
    DeliveryPartner(
        int id,
        string name)
    {

        this->partnerId = id;

        this->name = name;

        this->available = true;
    }

    bool isAvailable()
    {

        return available;
    }

    void assign()
    {

        available = false;
    }

    void completeDelivery()
    {

        available = true;
    }

    string getName()
    {

        return name;
    }
};

/* =====================================
   DELIVERY STRATEGY
===================================== */

class DeliveryStrategy
{

public:
    virtual DeliveryPartner *
    assignPartner(
        vector<
            DeliveryPartner *> &partners) = 0;

    virtual ~DeliveryStrategy() =
        default;
};

class FirstAvailablePartner
    : public DeliveryStrategy
{

public:
    DeliveryPartner *
    assignPartner(
        vector<
            DeliveryPartner *> &partners) override
    {

        for (
            auto partner :
            partners)
        {

            if (
                partner
                    ->isAvailable())
            {

                partner->assign();

                return partner;
            }
        }

        return nullptr;
    }
};

/* =====================================
   ORDER
===================================== */

class Order
{

private:
    int orderId;

    Customer *customer;

    Restaurant *restaurant;

    unordered_map<
        MenuItem *,
        int>
        items;

    int totalPrice;

    OrderStatus status;

    DeliveryPartner *
        deliveryPartner;

    time_t orderTime;

public:
    Order(
        int id,
        Customer *customer,
        Restaurant *rt,
        unordered_map<
            MenuItem *,
            int>
            items,
        int total)
    {

        this->orderId = id;

        this->customer = customer;

        this->restaurant = rt;

        this->items = items;

        this->totalPrice = total;

        this->status =
            OrderStatus::PLACED;

        this->deliveryPartner =
            nullptr;

        this->orderTime =
            time(nullptr);
    }

    void assignPartner(
        DeliveryPartner *partner)
    {

        deliveryPartner =
            partner;

        status =
            OrderStatus::
                OUT_FOR_DELIVERY;
    }

    void printOrder()
    {

        cout
            << "Order "
            << orderId
            << " placed by "
            << customer
                   ->getName()
            << endl;

        if (
            deliveryPartner)
        {

            cout
                << "Delivery Partner: "
                << deliveryPartner
                       ->getName()
                << endl;
        }
    }
};

/* =====================================
   MAIN SYSTEM
===================================== */

class FoodDeliverySystem
{

private:
    vector<
        DeliveryPartner *>
        partners;

    DeliveryStrategy *
        deliveryStrategy;

    int nextOrderId = 1;

public:
    FoodDeliverySystem(
        DeliveryStrategy *
            ds)
    {

        deliveryStrategy = ds;
    }

    void addPartner(
        DeliveryPartner *
            partner)
    {

        partners.push_back(
            partner);
    }

    Order *placeOrder(
        Customer *customer,
        Cart &cart,
        PaymentStrategy *
            payment)
    {

        Restaurant *rt =
            cart.getRestaurant();

        if (
            !rt ||
            !rt->isOpen())
        {

            cout
                << "Restaurant unavailable\n";

            return nullptr;
        }

        int amount =
            cart.getTotalPrice();

        bool success =
            payment->pay(
                amount);

        if (
            !success)
        {

            cout
                << "Payment failed\n";

            return nullptr;
        }

        Order *order =
            new Order(
                nextOrderId++,
                customer,
                rt,
                cart.getItems(),
                amount);

        DeliveryPartner *
            partner =
                deliveryStrategy
                    ->assignPartner(
                        partners);

        if (
            partner)
        {

            order
                ->assignPartner(
                    partner);
        }

        cart.clear();

        return order;
    }
};

/* =====================================
   DRIVER
===================================== */

int main()
{

    FoodDeliverySystem system(
        new FirstAvailablePartner());

    DeliveryPartner *d1 =
        new DeliveryPartner(
            1,
            "Rahul");

    DeliveryPartner *d2 =
        new DeliveryPartner(
            2,
            "Aman");

    system.addPartner(d1);

    system.addPartner(d2);

    Customer *customer =
        new Customer(
            1,
            "Nirvan");

    Restaurant *restaurant =
        new Restaurant(
            101,
            "Burger House");

    MenuItem *burger =
        new MenuItem(
            1,
            "Burger",
            150,
            restaurant);

    MenuItem *fries =
        new MenuItem(
            2,
            "Fries",
            100,
            restaurant);

    restaurant
        ->addMenuItem(
            burger);

    restaurant
        ->addMenuItem(
            fries);

    Cart cart;

    cart.addItem(
        burger,
        2);

    cart.addItem(
        fries,
        1);

    PaymentStrategy *
        payment =
            new UPI();

    Order *order =
        system.placeOrder(
            customer,
            cart,
            payment);

    if (
        order)
    {

        order
            ->printOrder();
    }

    return 0;
}