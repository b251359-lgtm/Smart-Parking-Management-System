#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <ctime>
#include <iomanip>
using namespace std;

class Vehicle {
public:
    string vehicleNumber;
    int slotNumber;
    time_t entryTime;

    Vehicle(string number, int slot, time_t time) {
        vehicleNumber = number;
        slotNumber = slot;
        entryTime = time;
    }
};

class ParkingSystem {
private:
    int totalSlots;
    int occupiedSlots;

    vector<Vehicle> activeVehicles;
    map<string, int> vehicleSlotMap;

public:
    ParkingSystem(int slots) {
        totalSlots = slots;
        occupiedSlots = 0;
    }

    // Save parking history to file
    void saveHistory(string vehicleNo, int slot, double billAmount, double hours) {
        ofstream file("parking_history.txt", ios::app);

        if (!file) {
            cout << "Error opening history file!" << endl;
            return;
        }

        time_t now = time(0);
        char* dt = ctime(&now);

        file << "Vehicle Number : " << vehicleNo << endl;
        file << "Slot Number    : " << slot << endl;
        file << "Parking Hours  : " << fixed << setprecision(2) << hours << endl;
        file << "Bill Amount    : Rs. " << billAmount << endl;
        file << "Exit Time      : " << dt;
        file << "----------------------------------------" << endl;

        file.close();
    }

    // Park vehicle
    void parkVehicle() {
        if (occupiedSlots >= totalSlots) {
            cout << "\nParking Full!" << endl;
            return;
        }

        string vehicleNo;

        cout << "\nEnter Vehicle Number: ";
        cin >> vehicleNo;

        // Check duplicate vehicle
        if (vehicleSlotMap.find(vehicleNo) != vehicleSlotMap.end()) {
            cout << "Vehicle already parked!" << endl;
            return;
        }

        int slot = occupiedSlots + 1;
        time_t currentTime = time(0);

        Vehicle newVehicle(vehicleNo, slot, currentTime);

        activeVehicles.push_back(newVehicle);
        vehicleSlotMap[vehicleNo] = slot;

        occupiedSlots++;

        cout << "\nVehicle Parked Successfully!" << endl;
        cout << "Allocated Slot : " << slot << endl;
    }

    // Remove vehicle and calculate bill
    void removeVehicle() {
        if (occupiedSlots == 0) {
            cout << "\nParking Empty!" << endl;
            return;
        }

        string vehicleNo;

        cout << "\nEnter Vehicle Number to Remove: ";
        cin >> vehicleNo;

        bool found = false;

        for (int i = 0; i < activeVehicles.size(); i++) {

            if (activeVehicles[i].vehicleNumber == vehicleNo) {

                found = true;

                time_t exitTime = time(0);

                double seconds = difftime(exitTime, activeVehicles[i].entryTime);
                double hours = seconds / 3600.0;

                if (hours < 1)
                    hours = 1;

                double bill = hours * 50; // Rs.50 per hour

                cout << "\nVehicle Removed Successfully!" << endl;
                cout << "Slot Number   : " << activeVehicles[i].slotNumber << endl;
                cout << "Parking Hours : " << fixed << setprecision(2) << hours << endl;
                cout << "Total Bill    : Rs. " << bill << endl;

                saveHistory(vehicleNo,
                            activeVehicles[i].slotNumber,
                            bill,
                            hours);

                vehicleSlotMap.erase(vehicleNo);
                activeVehicles.erase(activeVehicles.begin() + i);

                occupiedSlots--;

                break;
            }
        }

        if (!found) {
            cout << "Vehicle Not Found!" << endl;
        }
    }

    // Display active parked vehicles
    void displayActiveVehicles() {
        cout << "\n========== ACTIVE VEHICLES ==========" << endl;

        if (activeVehicles.empty()) {
            cout << "No Vehicles Parked!" << endl;
            return;
        }

        for (auto &vehicle : activeVehicles) {
            cout << "Vehicle Number : " << vehicle.vehicleNumber << endl;
            cout << "Slot Number    : " << vehicle.slotNumber << endl;
            cout << "-----------------------------------" << endl;
        }
    }

    // Search vehicle
    void searchVehicle() {
        string vehicleNo;

        cout << "\nEnter Vehicle Number to Search: ";
        cin >> vehicleNo;

        auto it = vehicleSlotMap.find(vehicleNo);

        if (it != vehicleSlotMap.end()) {
            cout << "Vehicle Found!" << endl;
            cout << "Allocated Slot : " << it->second << endl;
        }
        else {
            cout << "Vehicle Not Found!" << endl;
        }
    }

    // Display parking statistics
    void displayStatus() {
        cout << "\n========== PARKING STATUS ==========" << endl;

        cout << "Total Slots     : " << totalSlots << endl;
        cout << "Occupied Slots  : " << occupiedSlots << endl;
        cout << "Available Slots : " << totalSlots - occupiedSlots << endl;
    }

    // View parking history file
    void viewHistory() {
        ifstream file("parking_history.txt");

        if (!file) {
            cout << "\nNo parking history found!" << endl;
            return;
        }

        cout << "\n========== PARKING HISTORY ==========" << endl;

        string line;

        while (getline(file, line)) {
            cout << line << endl;
        }

        file.close();
    }
};

// Menu
void displayMenu() {
    cout << "\n========== SMART PARKING MANAGEMENT SYSTEM ==========" << endl;
    cout << "1. Park Vehicle" << endl;
    cout << "2. Remove Vehicle" << endl;
    cout << "3. Display Parking Status" << endl;
    cout << "4. Display Active Vehicles" << endl;
    cout << "5. Search Vehicle" << endl;
    cout << "6. View Parking History" << endl;
    cout << "7. Exit" << endl;
    cout << "Enter Your Choice: ";
}

int main() {

    int slots;

    cout << "========== SMART PARKING MANAGEMENT SYSTEM ==========" << endl;
    cout << "Enter Total Parking Slots: ";
    cin >> slots;

    ParkingSystem parking(slots);

    int choice;

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {

            case 1:
                parking.parkVehicle();
                break;

            case 2:
                parking.removeVehicle();
                break;

            case 3:
                parking.displayStatus();
                break;

            case 4:
                parking.displayActiveVehicles();
                break;

            case 5:
                parking.searchVehicle();
                break;

            case 6:
                parking.viewHistory();
                break;

            case 7:
                cout << "\nExiting Program..." << endl;
                break;

            default:
                cout << "\nInvalid Choice!" << endl;
        }

    } while (choice != 7);

    return 0;
}

