#include<string>
#include<unordered_map>
#include<vector>
using namespace std; 

class Rider{
    string name; 
    int riderId; 
    public:
    Rider(string name, int riderId){
        this->name = name;
        this->riderId = riderId;
    }

    string get_name(){
        return this->name;
    }
};

class Driver {
    public:
    string name;
    int driver_id;
    string location; 
    Driver(string name, int drid, string location){
        this->name = name;
        this->driver_id = drid;
        this->location = location; 
    }
};
enum class VehType{
    SEDAN,
    SUV
}; 
class Vehicle{
    public:
    int vehicleNum;
    Vehicle(int veh){
        this->vehicleNum = veh;
    }

    virtual VehType get_type() = 0;
};

class Sedan : public Vehicle{
    public:
    int VehicleNum;
    Sedan(int vehicleNum) : Vehicle(vehicleNum) {}

    VehType get_type () override {
        return VehType::SEDAN; 
    }
};

class Booking{
    Driver* dr;
    Rider* rd; 
    string pickup;
    string dropoff; 
    Booking(Driver* dr, Rider* rd, string pickup, string dropoff){
        this->dr= dr; 
        this->rd = rd; 
        this->dropoff = dropoff;
        this->pickup = pickup; 
    }

    void book(){
        
    }
}

class DriverFindStrategy{
    public:
    Driver* dr; // has a relation with driver
    
    virtual Driver* findDriver() = 0; 
};

class findNearbyDriver : public DriverFindStrategy{
    Driver* findDriver() override {
        return this->dr; 
    }
}; 

class FareCalculation {
    int dist;
    Vehicle* veh;

    FareCalculation(int dist, Vehicle* vh){
        this->dist = dist;
        this->veh = veh;
    }

    public:
    int get_fare(){
        if(veh->get_type()  == VehType::SEDAN){
            return 10*dist; 
        }else return 20*dist; 
    }
}; 

class RideBooking{
    // Add Drivers
    private:
    vector<Driver*> drivers;
    DriverFindStrategy *dr;
    public:
    RideBooking(DriverFindStrategy* ds){
        dr = ds; 
    }

    public:
    void add_driver(string name, int id, string location){
        drivers.push_back(new Driver(name, id, location));
    }

    Driver* get_driver(){
        Driver* newdr = dr->findDriver();
        return newdr; 
    }

    int fare()


};
