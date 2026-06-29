#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <thread>
#include <cctype>
#include "core/ParkingLot.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
typedef int SOCKET;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close
#endif

using namespace std;

// ── Web Server Helper Methods ────────────────────────────────
string readFile(const string& filepath) {
    ifstream file(filepath, ios::binary);
    if (!file.is_open()) return "";
    stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void sendHttpResponse(SOCKET clientSocket, const string& status, const string& contentType, const string& body) {
    string headers = "HTTP/1.1 " + status + "\r\n"
                     "Content-Type: " + contentType + "; charset=utf-8\r\n"
                     "Content-Length: " + to_string(body.length()) + "\r\n"
                     "Connection: close\r\n\r\n";
    send(clientSocket, headers.c_str(), headers.length(), 0);
    send(clientSocket, body.c_str(), body.length(), 0);
}

void runHttpServer() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "\n[Web Server] Failed to initialize Winsock." << endl;
        return;
    }
#endif

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cout << "\n[Web Server] Failed to create socket." << endl;
        return;
    }

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    if (::bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "\n[Web Server] Bind failed." << endl;
        closesocket(serverSocket);
        return;
    }

    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        cout << "\n[Web Server] Listen failed." << endl;
        closesocket(serverSocket);
        return;
    }

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) continue;

        char buffer[2048] = {0};
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
            closesocket(clientSocket);
            continue;
        }

        string request(buffer);
        size_t firstLineEnd = request.find("\r\n");
        if (firstLineEnd == string::npos) {
            closesocket(clientSocket);
            continue;
        }
        string firstLine = request.substr(0, firstLineEnd);
        stringstream ss(firstLine);
        string method, url;
        ss >> method >> url;

        string path = url;
        string query = "";
        size_t qPos = url.find('?');
        if (qPos != string::npos) {
            path = url.substr(0, qPos);
            query = url.substr(qPos + 1);
        }

        auto getQueryParam = [](const string& q, const string& key) -> string {
            size_t pos = q.find(key + "=");
            if (pos == string::npos) return "";
            size_t valStart = pos + key.length() + 1;
            size_t ampPos = q.find('&', valStart);
            if (ampPos == string::npos) {
                return q.substr(valStart);
            }
            return q.substr(valStart, ampPos - valStart);
        };

        ParkingLot* lot = ParkingLot::getInstance();

        if (method == "GET" && (path == "/" || path == "/index.html")) {
            string html = readFile("ui/parking-lot.html");
            if (html.empty()) {
                sendHttpResponse(clientSocket, "404 Not Found", "text/plain", "ui/parking-lot.html not found.");
            } else {
                sendHttpResponse(clientSocket, "200 OK", "text/html", html);
            }
        } 
        else if (method == "GET" && path == "/api/status") {
            stringstream json;
            json << "{\n";
            json << "  \"revenue\": " << lot->getTotalRevenue() << ",\n";
            json << "  \"ticketCounter\": " << lot->getTicketCounter() << ",\n";
            json << "  \"tickets\": [\n";
            
            auto tickets = lot->getActiveTickets();
            bool first = true;
            for (auto& pair : tickets) {
                if (!first) json << ",\n";
                first = false;
                
                Ticket* t = pair.second;
                Vehicle* v = t->getVehicle();
                
                int floorIdx = 0;
                int slotIdx = 0;
                auto fls = lot->getFloors();
                bool found = false;
                for (int f = 0; f < fls.size(); f++) {
                    auto slotsList = fls[f].getSlots();
                    for (int s = 0; s < slotsList.size(); s++) {
                        if (slotsList[s].getSlotId() == t->getSlot()->getSlotId() && fls[f].getFloorType() == v->getType()) {
                            floorIdx = f;
                            slotIdx = s;
                            found = true;
                            break;
                        }
                    }
                    if (found) break;
                }

                json << "    {\n";
                json << "      \"ticketId\": \"" << t->getTicketId() << "\",\n";
                json << "      \"plate\": \"" << v->getLicensePlate() << "\",\n";
                json << "      \"type\": \"" << v->getType() << "\",\n";
                json << "      \"floor\": " << floorIdx << ",\n";
                json << "      \"slot\": " << slotIdx << ",\n";
                json << "      \"entryTime\": " << t->getEntryTime() << "\n";
                json << "    }";
            }
            json << "\n  ]\n";
            json << "}";
            
            sendHttpResponse(clientSocket, "200 OK", "application/json", json.str());
        }
        else if (method == "POST" && path == "/api/entry") {
            string plate = getQueryParam(query, "plate");
            string type = getQueryParam(query, "type");
            
            for (auto& c : plate) c = toupper(c);
            for (auto& c : type) c = toupper(c);
            
            string ticketId = lot->vehicleEntry(plate, type);
            stringstream json;
            if (ticketId.empty()) {
                json << "{\"error\": \"Failed to register vehicle entry (Lot full or Duplicate plate)\"}";
            } else {
                json << "{\"success\": true, \"ticketId\": \"" << ticketId << "\"}";
            }
            sendHttpResponse(clientSocket, "200 OK", "application/json", json.str());
        }
        else if (method == "POST" && path == "/api/exit") {
            string ticketId = getQueryParam(query, "ticketId");
            string paymentType = getQueryParam(query, "paymentType");
            string cashStr = getQueryParam(query, "cashGiven");
            double cashGiven = cashStr.empty() ? 0 : stod(cashStr);
            
            for (auto& c : ticketId) c = toupper(c);
            for (auto& c : paymentType) c = toupper(c);
            
            auto tickets = lot->getActiveTickets();
            if (tickets.find(ticketId) == tickets.end()) {
                sendHttpResponse(clientSocket, "200 OK", "application/json", "{\"error\": \"Ticket not found\"}");
            } else {
                lot->vehicleExit(ticketId, paymentType, cashGiven);
                sendHttpResponse(clientSocket, "200 OK", "application/json", "{\"success\": true}");
            }
        }
        else if (method == "POST" && path == "/api/save") {
            lot->saveData();
            sendHttpResponse(clientSocket, "200 OK", "application/json", "{\"success\": true}");
        }
        else {
            sendHttpResponse(clientSocket, "404 Not Found", "text/plain", "Endpoint not found.");
        }

        closesocket(clientSocket);
    }

    closesocket(serverSocket);
#ifdef _WIN32
    WSACleanup();
#endif
}

int main() {
    ParkingLot* lot = ParkingLot::getInstance("ParkEase Mall");

    // Load saved data on startup
    cout << "\n====================================\n";
    cout << "      PARKEASE PARKING SYSTEM       \n";
    cout << "====================================\n";
    lot->loadData();

    cout << "\nWeb UI Server is running at: http://localhost:8080\n";
    cout << "Press Ctrl+C to stop the server.\n\n";

    runHttpServer();

    return 0;
}

