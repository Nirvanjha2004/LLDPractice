#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>
#include <memory>
#include <ctime>
#include <cmath>

using namespace std;

/* ===========================
   ENUMS
   =========================== */

enum class VehicleType {
    BIKE,
    CAR,
    TRUCK
};

enum class SpotType {
    BIKE,
    COMPACT,
    LARGE
};


/* ===========================
   VEHICLE
   =========================== */

class Vehicle {
protected:
    string licenseNumber;

public:
    Vehicle(string license)
        : licenseNumber(license) {}

    virtual VehicleType getType() = 0;

    string getLicenseNumber() {
        return licenseNumber;
    }

    virtual ~Vehicle() = default;
};

class Bike : public Vehicle {
public:
    Bike(string license)
        : Vehicle(license) {}

    VehicleType getType() override {
        return VehicleType::BIKE;
    }
};

class Car : public Vehicle {
public:
    Car(string license)
        : Vehicle(license) {}

    VehicleType getType() override {
        return VehicleType::CAR;
    }
};

class Truck : public Vehicle {
public:
    Truck(string license)
        : Vehicle(license) {}

    VehicleType getType() override {
        return VehicleType::TRUCK;
    }
};


/* ===========================
   PARKING SPOT
   =========================== */

class ParkingSpot {
protected:
    string spotId;
    bool occupied;
    Vehicle* parkedVehicle;

public:
    ParkingSpot(string id)
        : spotId(id),
          occupied(false),
          parkedVehicle(nullptr) {}

    virtual bool canFit(Vehicle* vehicle) = 0;

    virtual SpotType getSpotType() = 0;

    bool isOccupied() {
        return occupied;
    }

    void parkVehicle(Vehicle* vehicle) {
        occupied = true;
        parkedVehicle = vehicle;
    }

    void removeVehicle() {
        occupied = false;
        parkedVehicle = nullptr;
    }

    string getSpotId() {
        return spotId;
    }

    virtual ~ParkingSpot() = default;
};


class BikeSpot : public ParkingSpot {
public:
    BikeSpot(string id)
        : ParkingSpot(id) {}

    bool canFit(Vehicle* vehicle) override {
        return vehicle->getType() == VehicleType::BIKE;
    }

    SpotType getSpotType() override {
        return SpotType::BIKE;
    }
};


class CompactSpot : public ParkingSpot {
public:
    CompactSpot(string id)
        : ParkingSpot(id) {}

    bool canFit(Vehicle* vehicle) override {
        return vehicle->getType() == VehicleType::BIKE ||
               vehicle->getType() == VehicleType::CAR;
    }

    SpotType getSpotType() override {
        return SpotType::COMPACT;
    }
};


class LargeSpot : public ParkingSpot {
public:
    LargeSpot(string id)
        : ParkingSpot(id) {}

    bool canFit(Vehicle* vehicle) override {
        return true;
    }

    SpotType getSpotType() override {
        return SpotType::LARGE;
    }
};


/* ===========================
   TICKET
   =========================== */

class Ticket {
private:
    int ticketId;
    Vehicle* vehicle;
    ParkingSpot* spot;
    time_t entryTime;

public:
    Ticket(
        int id,
        Vehicle* v,
        ParkingSpot* s
    ) : ticketId(id),
        vehicle(v),
        spot(s) {

        entryTime = time(nullptr);
    }

    int getTicketId() {
        return ticketId;
    }

    Vehicle* getVehicle() {
        return vehicle;
    }

    ParkingSpot* getSpot() {
        return spot;
    }

    time_t getEntryTime() {
        return entryTime;
    }
};


/* ===========================
   PRICING STRATEGY
   =========================== */

class PricingStrategy {
public:
    virtual int calculatePrice(
        Vehicle* vehicle,
        int hours
    ) = 0;

    virtual ~PricingStrategy() = default;
};


class DefaultPricingStrategy : public PricingStrategy {
public:

    int calculatePrice(
        Vehicle* vehicle,
        int hours
    ) override {

        switch(vehicle->getType()) {

            case VehicleType::BIKE:
                return 10 * hours;

            case VehicleType::CAR:
                return 20 * hours;

            case VehicleType::TRUCK:
                return 50 * hours;
        }

        return 0;
    }
};


/* ===========================
   PARKING LOT
   =========================== */

class ParkingLot {

private:

    queue<ParkingSpot*> bikeSpots;
    queue<ParkingSpot*> compactSpots;
    queue<ParkingSpot*> largeSpots;

    unordered_map<int, Ticket*> activeTickets;

    unordered_map<string, int> activeVehicles;

    PricingStrategy* pricingStrategy;

    int nextTicketId = 1;


private:

    ParkingSpot* getSpotForVehicle(
        Vehicle* vehicle
    ) {

        VehicleType type =
            vehicle->getType();

        // Bike
        if(type == VehicleType::BIKE) {

            if(!bikeSpots.empty()) {
                auto spot =
                    bikeSpots.front();

                bikeSpots.pop();

                return spot;
            }

            if(!compactSpots.empty()) {
                auto spot =
                    compactSpots.front();

                compactSpots.pop();

                return spot;
            }

            if(!largeSpots.empty()) {
                auto spot =
                    largeSpots.front();

                largeSpots.pop();

                return spot;
            }
        }

        // Car
        else if(type == VehicleType::CAR) {

            if(!compactSpots.empty()) {
                auto spot =
                    compactSpots.front();

                compactSpots.pop();

                return spot;
            }

            if(!largeSpots.empty()) {
                auto spot =
                    largeSpots.front();

                largeSpots.pop();

                return spot;
            }
        }

        // Truck
        else {

            if(!largeSpots.empty()) {
                auto spot =
                    largeSpots.front();

                largeSpots.pop();

                return spot;
            }
        }

        return nullptr;
    }


    void releaseSpot(
        ParkingSpot* spot
    ) {

        SpotType type =
            spot->getSpotType();

        if(type == SpotType::BIKE) {
            bikeSpots.push(spot);
        }

        else if(type == SpotType::COMPACT) {
            compactSpots.push(spot);
        }

        else {
            largeSpots.push(spot);
        }
    }


public:

    ParkingLot(
        PricingStrategy* pricing
    ) : pricingStrategy(pricing) {}


    void addSpot(
        ParkingSpot* spot
    ) {

        SpotType type =
            spot->getSpotType();

        if(type == SpotType::BIKE) {
            bikeSpots.push(spot);
        }

        else if(type == SpotType::COMPACT) {
            compactSpots.push(spot);
        }

        else {
            largeSpots.push(spot);
        }
    }


    Ticket* parkVehicle(
        Vehicle* vehicle
    ) {

        string license =
            vehicle->getLicenseNumber();

        // Prevent duplicate parking
        if(activeVehicles.count(
            license
        )) {

            cout << "Vehicle already parked\n";

            return nullptr;
        }

        ParkingSpot* spot =
            getSpotForVehicle(vehicle);

        if(!spot) {

            cout << "No spot available\n";

            return nullptr;
        }

        spot->parkVehicle(vehicle);

        Ticket* ticket =
            new Ticket(
                nextTicketId++,
                vehicle,
                spot
            );

        activeTickets[
            ticket->getTicketId()
        ] = ticket;

        activeVehicles[
            license
        ] = ticket->getTicketId();

        cout
            << "Allocated Spot: "
            << spot->getSpotId()
            << "\n";

        return ticket;
    }


    int unparkVehicle(
        int ticketId
    ) {

        if(
            !activeTickets.count(
                ticketId
            )
        ) {

            cout << "Invalid ticket\n";

            return -1;
        }

        Ticket* ticket =
            activeTickets[
                ticketId
            ];

        time_t exitTime =
            time(nullptr);

        double seconds =
            difftime(
                exitTime,
                ticket->getEntryTime()
            );

        int hours =
            max(
                1,
                (int)ceil(
                    seconds / 3600.0
                )
            );

        int amount =
            pricingStrategy
            ->calculatePrice(
                ticket->getVehicle(),
                hours
            );

        ParkingSpot* spot =
            ticket->getSpot();

        spot->removeVehicle();

        releaseSpot(spot);

        string license =
            ticket
            ->getVehicle()
            ->getLicenseNumber();

        activeVehicles.erase(
            license
        );

        activeTickets.erase(
            ticketId
        );

        cout
            << "Vehicle removed.\n";

        return amount;
    }

};


/* ===========================
   MAIN
   =========================== */

int main() {

    PricingStrategy* pricing =
        new DefaultPricingStrategy();

    ParkingLot lot(pricing);

    // Add spots
    lot.addSpot(
        new BikeSpot("B1")
    );

    lot.addSpot(
        new CompactSpot("C1")
    );

    lot.addSpot(
        new CompactSpot("C2")
    );

    lot.addSpot(
        new LargeSpot("L1")
    );

    // Vehicles
    Vehicle* car =
        new Car("CAR123");

    Vehicle* truck =
        new Truck("TRUCK999");

    // Park
    Ticket* t1 =
        lot.parkVehicle(car);

    Ticket* t2 =
        lot.parkVehicle(truck);

    // Unpark
    int amount =
        lot.unparkVehicle(
            t1->getTicketId()
        );

    cout
        << "Fare = ₹"
        << amount
        << "\n";

    return 0;
}