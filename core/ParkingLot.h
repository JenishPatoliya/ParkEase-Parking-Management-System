#ifndef PARKINGLOT_H
#define PARKINGLOT_H

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include "../models/ParkingFloor.h"
#include "../models/Ticket.h"
#include "../factory/VehicleFactory.h"
#include "../payment/CashPayment.h"
#include "../payment/CardPayment.h"

using namespace std;

class ParkingLot {
private:
    // ── Singleton setup ──────────────────────────
    static ParkingLot* instance;
    
    // ── Data ─────────────────────────────────────
    string name;
    vector<ParkingFloor> floors;
    map<string, Ticket*>   activeTickets;  // ticketId → Ticket
    map<string, Vehicle*>  activeVehicles; // plate   → Vehicle
    int ticketCounter;
    double totalRevenue;

    // ── Private constructor ───────────────────────
    ParkingLot(string lotName) {
        name           = lotName;
        ticketCounter  = 1;
        totalRevenue   = 0;

        // Create 3 floors
        floors.push_back(ParkingFloor(1, "CAR",   12));
        floors.push_back(ParkingFloor(2, "BIKE",  14));
        floors.push_back(ParkingFloor(3, "TRUCK", 10));
    }

public:
    // ── Singleton getter ─────────────────────────
    static ParkingLot* getInstance(string name = "ParkEase") {
        if (instance == nullptr)
            instance = new ParkingLot(name);
        return instance;
    }

    // ── Vehicle Entry ────────────────────────────
    string vehicleEntry(string plate, string type) {
        // Check duplicate
        if (activeVehicles.count(plate)) {
            cout << "Vehicle " << plate
                 << " is already parked!" << endl;
            return "";
        }

        // Create vehicle via factory
        Vehicle* vehicle = VehicleFactory::createVehicle(type, plate);
        if (!vehicle) return "";

        // Find correct floor
        int floorIdx = -1;
        for (int i = 0; i < floors.size(); i++) {
            if (floors[i].getFloorType() == vehicle->getType()) {
                floorIdx = i;
                break;
            }
        }

        // Find available slot
        ParkingSlot* slot = floors[floorIdx].getAvailableSlot();
        if (!slot) {
            cout << "No slots available for "
                 << type << "!" << endl;
            delete vehicle;
            return "";
        }

        // Assign slot
        slot->assignVehicle(vehicle);

        // Generate ticket ID
        string ticketId = "TKT-"
            + string(4 - to_string(ticketCounter).length(), '0')
            + to_string(ticketCounter++);

        // Create and store ticket
        Ticket* ticket = new Ticket(ticketId, vehicle, slot);
        activeTickets[ticketId]  = ticket;
        activeVehicles[plate]    = vehicle;

        ticket->printTicket();
        return ticketId;
    }

    // ── Vehicle Exit ─────────────────────────────
    void vehicleExit(string ticketId, string paymentType,
                     double cashGiven = 0) {
        // Find ticket
        if (!activeTickets.count(ticketId)) {
            cout << "Ticket " << ticketId
                 << " not found!" << endl;
            return;
        }

        Ticket*  ticket  = activeTickets[ticketId];
        Vehicle* vehicle = ticket->getVehicle();

        // Calculate fee
        double hours = ticket->getHoursSpent();
        double rate  = 0;
        if (vehicle->getType() == "CAR")   rate = 50;
        if (vehicle->getType() == "BIKE")  rate = 20;
        if (vehicle->getType() == "TRUCK") rate = 100;
        double fee = hours * rate;

        cout << "\nVehicle : " << vehicle->getLicensePlate() << endl;
        cout << "Hours   : " << hours << endl;
        cout << "Fee     : Rs." << fee << endl;

        // Process payment
        Payment* payment = nullptr;
        if (paymentType == "CASH")
            payment = new CashPayment(fee, cashGiven);
        else
            payment = new CardPayment(fee, "4242");

        payment->processPayment();

        // Update revenue
        totalRevenue += fee;

        // Free slot
        ticket->getSlot()->removeVehicle();
        ticket->closeTicket();

        // Cleanup
        activeVehicles.erase(vehicle->getLicensePlate());
        activeTickets.erase(ticketId);

        delete payment;
        delete vehicle;
        delete ticket;
    }

    // ── Display Availability ─────────────────────
    void displayAvailability() {
        cout << "\n===== " << name << " =====\n";
        for (int i = 0; i < floors.size(); i++)
            floors[i].displayFloor();
        cout << "Total Revenue: Rs."
             << totalRevenue << endl;
    }

    // ── Display Active Tickets ───────────────────
    void displayActiveTickets() {
        cout << "\n=== ACTIVE TICKETS ===" << endl;
        if (activeTickets.empty()) {
            cout << "No active tickets." << endl;
            return;
        }
        for (auto& pair : activeTickets)
            pair.second->printTicket();
    }

    // ── Save Data to File ────────────────────────
    void saveData() {
        ofstream file("data/parking_data.txt");
        if (!file.is_open()) {
            cout << "Error opening file!" << endl;
            return;
        }

        // Save revenue
        file << "REVENUE:" << totalRevenue << "\n";

        // Save active tickets
        file << "TICKETS:" << activeTickets.size() << "\n";
        for (auto& pair : activeTickets) {
            Ticket*  t = pair.second;
            Vehicle* v = t->getVehicle();
            file << t->getTicketId()            << ","
                 << v->getLicensePlate()        << ","
                 << v->getType()               << ","
                 << t->getSlot()->getSlotId()  << ","
                 << t->getEntryTime()          << "\n";
        }

        file.close();
        cout << "Data saved successfully!" << endl;
    }

    // ── Load Data from File ──────────────────────
    void loadData() {
        ifstream file("data/parking_data.txt");
        if (!file.is_open()) {
            cout << "No saved data found. Starting fresh.\n";
            return;
        }

        string line;

        // Load revenue
        getline(file, line);
        totalRevenue = stod(line.substr(8));

        // Load tickets count
        getline(file, line);
        int count = stoi(line.substr(8));

        cout << "Loading " << count << " active ticket(s)...\n";

        // Load each ticket
        for (int i = 0; i < count; i++) {
            getline(file, line);

            // Parse comma separated line
            // Format: ticketId,plate,type,slotId,entryTime
            string parts[5];
            int    idx = 0;
            string token = "";

            for (char c : line) {
                if (c == ',') {
                    parts[idx++] = token;
                    token = "";
                } else {
                    token += c;
                }
            }
            parts[idx] = token;

            string ticketId  = parts[0];
            string plate     = parts[1];
            string type      = parts[2];
            int    slotId    = stoi(parts[3]);
            time_t entryTime = stoll(parts[4]);

            // Recreate vehicle
            Vehicle* vehicle = VehicleFactory::createVehicle(type, plate);

            // Find correct floor and slot
            for (int f = 0; f < floors.size(); f++) {
                if (floors[f].getFloorType() == type) {
                    ParkingSlot* slot =
                        floors[f].getSlotById(slotId);
                    if (slot) {
                        slot->assignVehicle(vehicle);
                        Ticket* ticket = new Ticket(
                            ticketId, vehicle, slot, entryTime
                        );
                        activeTickets[ticketId]        = ticket;
                        activeVehicles[plate]          = vehicle;
                        ticketCounter++;
                        cout << "Restored: " << ticketId
                             << " - " << plate << endl;
                    }
                    break;
                }
            }
        }

        file.close();
        cout << "Data loaded successfully!\n";
    }

    double getTotalRevenue() const { return totalRevenue; }
    int getTicketCounter() const { return ticketCounter; }
    const map<string, Ticket*>& getActiveTickets() const { return activeTickets; }
    const vector<ParkingFloor>& getFloors() const { return floors; }

    ~ParkingLot() {
        for (auto& pair : activeTickets) {
            delete pair.second->getVehicle();
            delete pair.second;
        }
    }
};

// Initialize static member
ParkingLot* ParkingLot::instance = nullptr;

#endif
