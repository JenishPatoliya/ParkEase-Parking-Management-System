#ifndef BIKE_H
#define BIKE_H

#include "Vehicle.h"

class Bike : public Vehicle {
public:
    Bike(string plate) : Vehicle(plate, "BIKE") {}

    string getType() override {
        return "BIKE";
    }
};

#endif
