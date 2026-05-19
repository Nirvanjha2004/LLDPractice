#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class User {
    public:
    int id;
    string name;
    string email;
    unordered_map<User*, string> chatHistory;
    User(int id, string name, string email){
        this->id = id;
        this->name = name; 
        this->email = email; 
    }

    unordered_map<User*, string> get_list(){
        return chatHistory;
    }
}; 

class Customer : public User{
    public:
    bool isOnline; 
    Customer(int id, string name, string email, bool isOnline) : User(id, name, email){
        this->isOnline = isOnline;
    }
};

class Agent : public User{
    public:
    Agent(int id, string name, string email) : User(id, name, email){}
}; 

class NotificationService{
    public:
    int id;
    string name;
    NotificationService(int id, string name){
        this->id = id;
        this->name = name; 
    }

    virtual void sendAll(vector<User*> users) = 0; 
};

class SMS : public NotificationService{
    public:
    SMS(int id, string name): NotificationService(id, name){}

    void sendAll(vector<User*> users) override {
        for(auto it : users){
            it->get_list();
            for(int i = 0; i < it.size(); i++){
                cout << "Sending SMS to " << it->name << ": " << it->get_list()[it] << endl;
            }
        }
    }
};

class Email : public NotificationService{
    public:
    Email(int id, string name): NotificationService(id, name){}
};

class Push : public NotificationService{
    public:
    Push(int id, string name): NotificationService(id, name){}
};

class Room{
    public:
    int id;
    string name;
    Room(int id, string name){
        this->id = id;
        this->name = name; 
    }
};

class GroupChatRoom : public Room{
    public:
    GroupChatRoom(int id, string name): Room(id, name){}
};

class OneOnOneChatRoom : public Room{
    public:
    OneOnOneChatRoom(int id, string name): Room(id, name){}
};



class Manager{
    Customer* cr;
    Agent* ag;
    unordered_map<Room*, string> chatHistory;

    void registerCustomer(Customer* cr){
        this->cr = cr;
    }

    void createRoom(){

    }

    void sendMessage(Room* room, User* sender, string message){
        chatHistory[room] += "Sent: " + message + "\n";
    }

    void markOnline(){
        cr->isOnline = true;
    }

    void markOffline(){
        cr->isOnline = false;
    }
}