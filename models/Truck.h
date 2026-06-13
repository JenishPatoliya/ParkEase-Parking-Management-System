#ifndef TRUCK_H
#define TRUCK_H

#include "Vehicle.h"

class Truck : public Vehicle {
public:
    Truck(string plate) : Vehicle(plate, "TRUCK") {}

    string getType() override {
        return "TRUCK";
    }
};

#endif
