#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
using namespace std;

class Vehicle {
protected:
    string licensePlate;
    string vehicleType;

public:
    // Constructor
    Vehicle(string plate, string type) {
        licensePlate = plate;
        vehicleType  = type;
    }

    // Pure virtual — makes this class abstract
    virtual string getType() = 0;

    // Getters
    string getLicensePlate() { return licensePlate; }
    string getVehicleType()  { return vehicleType;  }

    // Virtual destructor — always add in base class
    virtual ~Vehicle() {}
};

#endif
