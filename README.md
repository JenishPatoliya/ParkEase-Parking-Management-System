# 🅿️ ParkEase — Parking Management System

ParkEase is a lightweight, cross-platform C++17 Parking Management System that runs a dedicated Web UI server. It utilizes key OOP principles, design patterns, file stream persistence, and low-level Winsock TCP socket programming.

---

## 📂 Project Directory Structure

```
ParkingLotSystem/
├── README.md               ← This documentation file
├── main.cpp                ← Entry point, Winsock Web Server running synchronously
├── main.exe                ← Compiled executable binary
├── models/                 ← Object-oriented core entities
│   ├── Vehicle.h           ← Abstract base class for vehicles
│   ├── Car.h               ← Derived Car implementation
│   ├── Bike.h              ← Derived Bike implementation
│   ├── Truck.h             ← Derived Truck implementation
│   ├── ParkingSlot.h       ← Spot assignment, type-safety and state checks
│   ├── ParkingFloor.h      ← Aggregates slots, checks vacancy counts
│   └── Ticket.h            ← Ticket parameters, time calculation, status
├── payment/                ← Billing strategy classes
│   ├── Payment.h           ← Abstract billing base class
│   ├── CashPayment.h       ← Derived cash transaction strategy
│   └── CardPayment.h       ← Derived card transaction strategy
├── factory/                ← Creation patterns
│   └── VehicleFactory.h    ← Static factory method to instantiate vehicles
├── core/                   ← System orchestration
│   └── ParkingLot.h        ← Centralized singleton controller (data maps, entry/exit orchestrator)
├── data/                   ← Persistent storage
│   └── parking_data.txt    ← Flat-file database storing revenue & active tickets
└── ui/                     ← Frontend interface
    └── parking-lot.html    ← Web UI dashboard communicating via fetch() REST APIs
```

---

## 🏗️ Architecture Overview

```
User (Browser)
      │
 Web UI (localhost:8080)
      │
 ParkingLot (Singleton)
      │
┌─────┼─────┐
│     │     │
Floors Tickets Revenue
│
Slots ← Vehicle (via Factory)
          │
       Ticket ← Payment (Cash/Card via Strategy)
```

---

## 🛠️ System Architecture & Design Concepts

### 1. Object-Oriented Programming (OOP)
- **Abstraction**: `Vehicle` and `Payment` are abstract base classes using pure virtual functions (`virtual string getType() = 0;` and `virtual void processPayment() = 0;`) preventing direct instantiation and defining strict interfaces.
- **Polymorphism**: Used throughout the system to dynamically process different vehicle types (Car, Bike, Truck) and payment modes (Cash, Card) at runtime.
- **Encapsulation**: Class member states are kept `private` or `protected` and modified solely through controlled interfaces (e.g., `assignVehicle`, `closeTicket`, `removeVehicle`).

### 2. Design Patterns
- **Singleton Pattern**: The `ParkingLot` class holds private constructors and static instance getters so only one manager coordinates the lot configurations.
- **Factory Pattern**: The `VehicleFactory` translates simple string inputs (e.g., `"car"`) into polymorphism-mapped subclass instances (`new Car(...)`).
- **Strategy Pattern**: Selects distinct payment algorithms at runtime depending on customer billing selection.

### 3. Networking & TCP Server
- **Winsock API**: An embedded raw TCP web server is created using native sockets. It binds to port `8080` to serve the HTML file and handle REST JSON API endpoints (`/api/status`, `/api/entry`, `/api/exit`, `/api/save`).
- **Synchronous Server Thread**: The server loops and processes HTTP requests synchronously on the main application thread, maintaining lightweight operation and dedicated UI serving.

### 4. File-System Storage
- **Flat-File Database**: `saveData()` and `loadData()` serialize and parse records sequentially (`REVENUE`, `TICKETS`, comma-separated values) to keep slots and ticket states persistent.

---

## 🔌 Web UI API Endpoints

The C++ server hosts the following HTTP endpoints to sync data dynamically:

| Method | Endpoint | Description |
|---|---|---|
| `GET` | `/` | Serves the `ui/parking-lot.html` dashboard |
| `GET` | `/api/status` | Returns a JSON array of active tickets and lot revenue |
| `POST` | `/api/entry?plate=X&type=Y` | Allocates a slot and issues a ticket |
| `POST` | `/api/exit?ticketId=X&paymentType=Y&cashGiven=Z` | Collects fare and frees slot |
| `POST` | `/api/save` | Persists current slots state manually |

---

## 🚀 How to Compile and Run

### Step 1 — Navigate to the Project Folder
```bash
cd d:/ParkingLot/ParkingLotSystem
```

### Step 2 — Compile the Code
```bash
g++ -std=c++17 main.cpp -o main -lws2_32
```
> `-lws2_32` links the Windows Sockets API.

### Step 3 — Run the Program
```bash
./main
```

### Step 4 — Open the Web UI
Open your browser and navigate to:

👉 **[http://localhost:8080](http://localhost:8080)**

---

## ⚙️ How to Control the Application

1. **Via the Browser**: Use the modern dashboard to register entries, look up tickets, process payments, see visual parking slot maps, and trigger data saves.
2. **Closing the Server**: Press `Ctrl+C` in your terminal to shut down the server.
3. **Save/Load**: The system automatically restores active tickets from `data/parking_data.txt` on startup, and updates the state in memory. You can trigger data saving directly from the Web UI interface.
