#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <ctime>

using namespace std;

class Customer
{
public:
    int cid;
    string name;
    string email;

    Customer(int id, string name, string email)
    {
        this->cid = id;
        this->email = email;
        this->name = name;
    }
};

class MenuItem
{
public:
    int itid;
    string name;
    string category;
    int price;

    MenuItem(int id, string n, string c, int p)
    {
        this->category = c;
        this->name = n;
        this->itid = id;
        this->price = p;
    }
};

class Menu
{
public:
    unordered_map<MenuItem *, int> menuitems;

    void add_item(MenuItem *itm, int price)
    {
        menuitems[itm] = price;
    }

    void rem_item(MenuItem *itm)
    {
        menuitems.erase(itm);
    }

    void update_price(MenuItem *itm, int price)
    {
        menuitems[itm] = price;
    }


    int get_price(MenuItem* it){
        return menuitems[it];
    }
};

class Inventory
{
public:
    unordered_map<MenuItem *, int> inv;

    bool items_exist(MenuItem *itm)
    {
        if (inv.count(itm))
        {
            return true;
        }
        else
            return false;
    }

    bool item_avail(MenuItem *itm)
    {
        if (inv[itm] > 0)
        {
            return true;
        }
        else
            return false;
    }

    bool suff(MenuItem *itm, int num)
    {
        if (inv[itm] >= num)
            return true;
        else
            return false;
    }
};

class Cart
{
private:
    unordered_map<MenuItem *, int> CartItems;

public:
    void add_to_cart(MenuItem *mt)
    {
        CartItems[mt]++;
    }

    void remove_items(MenuItem *mt)
    {
        CartItems[mt]--;
    }

    void update(MenuItem *mt, int freq)
    {
        CartItems[mt] = freq;
    }

    unordered_map<MenuItem *, int> get_item()
    {
        return CartItems;
    }
};

class Order
{
public:
    OrderLifeCycle status;
    int orderId;
    Customer *cr;
    unordered_map<MenuItem *, int> items;
    int totalAmnt = 0;
    int timestamp;
    Order(int id, Customer *cr, unordered_map<MenuItem *, int> items, int total, int timestamp)
    {
        this->orderId = id;
    }
};

enum class OrderLifeCycle
{
    PLACED,
    CONFIRMED,
    PREPARING,
    READY,
    DELIVERED,
    CANCELLED
};

class RestManagement
{
    // User Will select the menuItems
    Menu *mn;
    Cart *ct;
    Order *ord;
    Customer *cr;
    void add_to_cart(MenuItem *item)
    {
        ct->add_to_cart(item);
    }

    void remove_items(MenuItem *item)
    {
        ct->remove_items(item);
    }
    unordered_map<MenuItem *, int> it = ct->get_item();
    int total = 0;
    void place_order()
    {
        for (auto [key, qt] : it)
        {
            int price = mn->get_price(key);
            total += (qt*price);
        }
        ord = new Order(1, cr, it, total, time(NULL));
        ord->status = OrderLifeCycle::PLACED;
    }
};
