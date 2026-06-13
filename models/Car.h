#ifndef CAR_H
#define CAR_H

#include "Vehicle.h"

class Car : public Vehicle {
public:
    // Call parent constructor
    Car(string plate) : Vehicle(plate, "CAR") {}

    string getType() override {
        return "CAR";
    }
};

#endif
