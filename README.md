# ParkEase: Parking Management System

ParkEase is a lightweight, cross-platform C++17 Parking Management System that combines a classic console interactive terminal with a modern, responsive Web UI dashboard. It utilizes key OOP principles, design patterns, file stream persistence, and low-level Winsock TCP socket programming.

---

## 📂 Project Directory Structure

```
ParkingLotSystem/
├── README.md               ← This documentation file
├── main.cpp                ← Entry point, Console Loop & Winsock Web Server thread
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

## 🛠️ System Architecture & Design Concepts

### 1. Object-Oriented Programming (OOP)
* **Abstraction**: `Vehicle` and `Payment` are abstract base classes using pure virtual functions (`virtual string getType() = 0;` & `virtual void processPayment() = 0;`) preventing direct instantiation and defining strict interfaces.
* **Polymorphism**: Used throughout the system to dynamically process different vehicle types (Car, Bike, Truck) and payment modes (Cash, Card) at runtime.
* **Encapsulation**: Class member states are kept `private` or `protected` and modified solely through controlled interfaces (e.g., `assignVehicle`, `closeTicket`, `removeVehicle`).

### 2. Design Patterns
* **Singleton Pattern**: The `ParkingLot` class holds private constructors and static instance getters so only one manager coordinates the lot configurations.
* **Factory Pattern**: The `VehicleFactory` translates simple string inputs (e.g., `"car"`) into polymorphism-mapped subclass instances (`new Car(...)`).
* **Strategy Pattern**: Selects distinct payment algorithms at runtime depending on customer billing selection.

### 3. Networking & Multi-threading
* **Winsock API**: An embedded raw TCP web server is created using native sockets. It binds to port `8080` to serve the HTML file and handle asynchronous REST JSON API endpoints (`/api/status`, `/api/entry`, `/api/exit`, `/api/save`).
* **Cross-platform Threading**: Starts the web server in a background thread using native Windows `CreateThread` (and POSIX `pthread_create` fallback) so the console and browser UI can run in parallel without blocking.

### 4. File-System Storage
* **Flat-File Database**: `saveData()` and `loadData()` serialize and parse records sequentially (`REVENUE`, `TICKETS`, comma-separated values) to keep slots and ticket states persistent.

---

## 🔌 Web UI API Endpoints

The C++ server hosts the following HTTP endpoints to sync data dynamically:
* `GET /` — Serves the `ui/parking-lot.html` dashboard.
* `GET /api/status` — Returns a JSON array of active tickets and lot revenue.
* `POST /api/entry?plate=X&type=Y` — Allocates a slot and issues a ticket.
* `POST /api/exit?ticketId=X&paymentType=Y&cashGiven=Z` — Collects fare and frees slot.
* `POST /api/save` — Persists current slots state manually.

---

## 🚀 How to Compile and Run

To compile and launch the application on Windows, open your VS Code terminal and execute the following:

### Step 1: Navigate to the Project Folder
```bash
cd d:/ParkingLot/ParkingLotSystem
```

### Step 2: Compile the Code
```bash
g++ -std=c++17 main.cpp -o main -lws2_32
```
*(Note: `-lws2_32` links the Windows Sockets API).*

### Step 3: Run the Program
```bash
./main
```

### Step 4: Open the Web UI
Open your web browser and navigate to:
👉 **[http://localhost:8080](http://localhost:8080)**

---

## ⚙️ How to Control the Application

1. **Via the Browser (Recommended)**: Use the modern dashboard to register entries, look up tickets, process payments, see visual parking slot maps, and trigger data saves.
2. **Via the Terminal Console**: Use options `1-6` to perform the same actions.
3. **Save/Load**: The system automatically restores the active tickets from `data/parking_data.txt` on startup, and automatically updates the file when you exit using option `6` (Exit System).
