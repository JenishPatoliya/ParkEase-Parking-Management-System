#ifndef PARKINGFLOOR_H
#define PARKINGFLOOR_H

#include <vector>
#include "ParkingSlot.h"

class ParkingFloor {
private:
    int floorNumber;
    string floorType;         // "CAR", "BIKE", "TRUCK"
    vector<ParkingSlot> slots;

public:
    // Constructor — creates all slots automatically
    ParkingFloor(int number, string type, int totalSlots) {
        floorNumber = number;
        floorType   = type;

        // Create all slots for this floor
        for (int i = 1; i <= totalSlots; i++) {
            slots.push_back(ParkingSlot(i, type));
        }
    }

    // Find first available slot
    ParkingSlot* getAvailableSlot() {
        for (int i = 0; i < slots.size(); i++) {
            if (slots[i].getIsEmpty()) {
                return &slots[i];
            }
        }
        return nullptr;    // no slot available
    }

    // Count available slots
    int availableCount() {
        int count = 0;
        for (int i = 0; i < slots.size(); i++) {
            if (slots[i].getIsEmpty()) count++;
        }
        return count;
    }

    // Display all slots on this floor
    void displayFloor() {
        cout << "\n--- Floor " << floorNumber
             << " [" << floorType << "] ---" << endl;
        for (int i = 0; i < slots.size(); i++) {
            slots[i].displaySlot();
        }
        cout << "Available: " << availableCount()
             << "/" << slots.size() << endl;
    }

    // Getters
    int    getFloorNumber() { return floorNumber; }
    string getFloorType()   { return floorType;   }
    const vector<ParkingSlot>& getSlots() const { return slots; }

    ParkingSlot* getSlotById(int id) {
        for (int i = 0; i < slots.size(); i++) {
            if (slots[i].getSlotId() == id)
                return &slots[i];
        }
        return nullptr;
    }
};

#endif
