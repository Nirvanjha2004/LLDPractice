#include<string>
#include<unordered_map>
#include<unordered_set>
#include<vector>

using namespace std;

class User{
    public:
    int userId;
    string name;
    string email; 
    User(int id, string name, string email){
        this->userId = id;
        this->name = name; 
        this->email= email;
    }
};


class Group{
    public:
    int grpId;
    string grpName;
    vector<User*> members;
    Group(int id, string name){
        this->grpName = name;
        this->grpId = id; 
    }
};

class Expense{
    int expId; 
    User* paidUser;
    int amount;
    string desc;
    Group* grp;
    vector<User*> participants;

    Expense(int id, int amount, User* pd, string desc, Group* grp){
        this->expId = id;
        this->amount = amount;
        this->paidUser = pd;
        this->desc = desc;
        this->grp = grp; 
    }
};

class BalanceTracking{
    EqualSplit* eq;
    unordered_map<User*, unordered_map<User*, int>> Dues;
    void settle(User* main, User* settler, int amount){
        Dues[settler][main] -= amount;
    }

    void update_balances(User* main, int amount){
        for(auto [key, val] : Dues){
            if(key == main) continue;
            eq.split()
        }
    }
};

// Strategy Split Types
class SplitTypes {
    public:
    int amount;
    SplitTypes( int amnt ){
        this->amount = amnt;
    }

    virtual void split() = 0; 
}; 

class EqualSplit : public SplitTypes{
    int amount;
    EqualSplit(int amount) : SplitTypes(amount){};
    void split() override {
        // Update the balance of all the users
    }
};