#ifndef PARKINGSLOT_H
#define PARKINGSLOT_H

#include "Vehicle.h"
#include <iostream>

class ParkingSlot {
private:
    int slotId;
    bool isEmpty;
    string slotType;      // "CAR", "BIKE", "TRUCK"
    Vehicle* parkedVehicle;

public:
    // Constructor
    ParkingSlot(int id, string type) {
        slotId         = id;
        slotType       = type;
        isEmpty        = true;
        parkedVehicle  = nullptr;
    }

    // Assign vehicle to this slot
    bool assignVehicle(Vehicle* v) {
        if (!isEmpty) {
            cout << "Slot " << slotId << " is already occupied!" << endl;
            return false;
        }
        if (v->getType() != slotType) {
            cout << "Wrong slot type!" << endl;
            return false;
        }
        parkedVehicle = v;
        isEmpty       = false;
        return true;
    }

    // Remove vehicle from slot
    void removeVehicle() {
        parkedVehicle = nullptr;
        isEmpty       = true;
    }

    // Getters
    bool   getIsEmpty()  { return isEmpty;        }
    int    getSlotId()   { return slotId;          }
    string getSlotType() { return slotType;        }
    Vehicle* getVehicle(){ return parkedVehicle;   }

    // Display slot info
    void displaySlot() {
        cout << "Slot " << slotId
             << " [" << slotType << "] "
             << (isEmpty ? "EMPTY" : "OCCUPIED");
        if (!isEmpty)
            cout << " — " << parkedVehicle->getLicensePlate();
        cout << endl;
    }
};

#endif
