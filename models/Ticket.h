#ifndef TICKET_H
#define TICKET_H

#include <iostream>
#include <string>
#include <ctime>
#include "Vehicle.h"
#include "ParkingSlot.h"

using namespace std;

class Ticket {
private:
    string   ticketId;
    Vehicle* vehicle;
    ParkingSlot* slot;
    time_t   entryTime;
    bool     isActive;

public:
    // Constructor — called when vehicle enters
    Ticket(string id, Vehicle* v, ParkingSlot* s) {
        ticketId  = id;
        vehicle   = v;
        slot      = s;
        entryTime = time(0);   // current time
        isActive  = true;
    }

    // Add this second constructor in Ticket class
    Ticket(string id, Vehicle* v, ParkingSlot* s, time_t savedTime) {
        ticketId  = id;
        vehicle   = v;
        slot      = s;
        entryTime = savedTime;   // use saved time not current
        isActive  = true;
    }

    // Calculate hours spent
    double getHoursSpent() {
        time_t now     = time(0);
        double seconds = difftime(now, entryTime);
        double hours   = seconds / 3600.0;

        // Minimum 1 hour always charged
        if (hours < 1.0) hours = 1.0;
        return hours;
    }

    // Close ticket at exit
    void closeTicket() {
        isActive = false;
    }

    // Print ticket details
    void printTicket() {
        cout << "\n=============================\n";
        cout << "       PARKING TICKET        \n";
        cout << "=============================\n";
        cout << "Ticket ID  : " << ticketId                          << "\n";
        cout << "Plate      : " << vehicle->getLicensePlate()        << "\n";
        cout << "Type       : " << vehicle->getType()                << "\n";
        cout << "Slot       : " << slot->getSlotId()                 << "\n";
        cout << "Entry Time : " << formatTime(entryTime)             << "\n";
        cout << "Status     : " << (isActive ? "ACTIVE" : "CLOSED") << "\n";
        cout << "=============================\n";
    }

    // Getters
    string       getTicketId()  { return ticketId;  }
    Vehicle*     getVehicle()   { return vehicle;   }
    ParkingSlot* getSlot()      { return slot;       }
    time_t       getEntryTime() { return entryTime;  }
    bool         getIsActive()  { return isActive;   }

private:
    // Helper — format time nicely
    string formatTime(time_t t) {
        char buf[30];
        struct tm* info = localtime(&t);
        strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M:%S", info);
        return string(buf);
    }
};

#endif
