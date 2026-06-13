#ifndef VEHICLEFACTORY_H
#define VEHICLEFACTORY_H

#include "../models/Vehicle.h"
#include "../models/Car.h"
#include "../models/Bike.h"
#include "../models/Truck.h"
#include <iostream>
using namespace std;

class VehicleFactory {
public:
    // Static method — no object needed to call this
    static Vehicle* createVehicle(string type, string plate) {

        // Convert type to uppercase for safety
        for (int i = 0; i < type.length(); i++)
            type[i] = toupper(type[i]);

        if (type == "CAR")   return new Car(plate);
        if (type == "BIKE")  return new Bike(plate);
        if (type == "TRUCK") return new Truck(plate);

        // Unknown type
        cout << "Unknown vehicle type: " << type << endl;
        return nullptr;
    }

    // Helper — print supported types
    static void showSupportedTypes() {
        cout << "Supported types: CAR, BIKE, TRUCK" << endl;
    }
};

#endif
