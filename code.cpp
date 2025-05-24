#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>
        using namespace std;

    // cấu trúc Food/Drink Item
    struct FoodItem {
        string name;
        float price;
        };

    // cấu trúc RentalRecord
    struct RentalRecord {
        string boatID;
        string ticketCode;
        float seatsRented;
        int shiftIndex;
        string action; // "RENT" or "CANCEL"
        vector<FoodItem> foodItems;
        float foodCost;
    };

    // cấu trúc Shift
    struct Shift {
        int startHour;
        int endHour;
        float pricePerHour;
        float seatsBooked;
        Shift() : startHour(0), endHour(0), pricePerHour(0), seatsBooked(0) {}
    };

    // Class Boat
    class Boat {
        string boatID;
        string boatName;
        char type;
        float weight;
        bool isAvailable;
        float seats;
        int numberOfTrips;
        vector<Shift> timeTrips;
        int rentalCount;
    public:
        Boat() :isAvailable(true) {};
        ~Boat() {};
        void createBoat();
        void showBoat() const;
        void setAvailability(bool available) {
            if (isAvailable != available) {
                isAvailable = available;
          
            }
        }
        bool getAvailability() const { return isAvailable; }
        float getSeats() const { return seats; }
        string getBoatID() const { return boatID; }
        int getRentalCount() const { return rentalCount; }
        void incrementRentalCount() { rentalCount++; }
        const vector<Shift>& getTimeTrips() const { return timeTrips; }
        int getNumberOfTrips() const { return numberOfTrips; }
        void bookSeats(int shiftIndex, float seats);
        void cancelSeats(int shiftIndex, float seats);
        void saveToFile(ofstream& outFile) const;
        void loadFromFile(ifstream& inFile);
    };

    // mấy hàm tiện ích, rút ngắn code
    void displayHeader() {
        cout << "\n\t\t======================================";
        cout << "\n\t\t    BOAT RENTAL MANAGEMENT SYSTEM    ";
        cout << "\n\t\t======================================\n";
    }

    bool isValidBoatType(char type) {
        return type == 'S' || type == 'M' || type == 'L';
    }

    bool isValidNumber(const string & str) {
        try {
            size_t pos;
            stof(str, &pos);
            return pos == str.length();
        }
        catch (...) {
            return false;
        }
    }

    void pressEnterToContinue() {
        cout << "\n\t\tPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    void displayMenu() {
        system("cls");
        displayHeader();
        cout << "\n\t\t1. Add new boat";
        cout << "\n\t\t2. Display all boats";
        cout << "\n\t\t3. Rent seats";
        cout << "\n\t\t4. Cancel rented seats";
        cout << "\n\t\t5. Display available boats";
        cout << "\n\t\t6. Display rental history";
        cout << "\n\t\t7. Display most rented boat";
        cout << "\n\t\t8. Total seats rented on a boat";
        cout << "\n\t\t9. Exit";
        cout << "\n\t\tEnter selection (1-9): ";
    }

    string generateTicketCode(const vector<RentalRecord>&rentalHistory) {
        srand(static_cast<unsigned int>(time(0)));
        const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        string ticket;
        bool unique;
        do {
            ticket = "TICKET";
            for (int i = 0; i < 6; i++) {
                ticket += chars[rand() % chars.length()];
            }
            unique = true;
            for (const auto& record : rentalHistory) {
                if (record.ticketCode == ticket) {
                    unique = false;
                    break;
                }
            }
        } while (!unique);
        return ticket;
    }

    // hàm trong class Boat
    void Boat::createBoat() {
        system("cls");
        displayHeader();

        do {
            cout << "\t\tEnter Boat ID (e.g., B001): ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, boatID);
            if (boatID.empty() || boatID.length() > 10) {
                cout << "\t\tError: Boat ID must be 1-10 characters.\n";
            }
        } while (boatID.empty() || boatID.length() > 10);

        do {
            cout << "\t\tEnter Boat name: ";
            getline(cin, boatName);
            if (boatName.empty()) {
                cout << "\t\tError: Boat name cannot be empty.\n";
            }
        } while (boatName.empty());

        do {
            cout << "\t\tEnter Boat Type (S/M/L): ";
            cin >> type;
            type = toupper(type);
            if (!isValidBoatType(type)) {
                cout << "\t\tError: Boat type must be S, M, or L.\n";
            }
        } while (!isValidBoatType(type));

        string input;
        do {
            cout << "\t\tEnter boat weight (tons): ";
            cin >> input;
            if (!isValidNumber(input) || stof(input) <= 0) {
                cout << "\t\tError: Weight must be positive.\n";
            }
            else {
                weight = stof(input);
                break;
            }
        } while (true);

        do {
            cout << "\t\tEnter number of seats: ";
            cin >> input;
            if (!isValidNumber(input) || stof(input) <= 0) {
                cout << "\t\tError: Seats must be positive.\n";
            }
            else {
                seats = stof(input);
                break;
            }
        } while (true);

        do {
            cout << "\t\tEnter number of shifts: ";
            cin >> input;
            if (!isValidNumber(input) || stoi(input) <= 0) {
                cout << "\t\tError: Shifts must be positive.\n";
            }
            else {
                numberOfTrips = stoi(input);
                break;
            }
        } while (true);

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        timeTrips.resize(numberOfTrips);

        for (int i = 0; i < numberOfTrips; i++) {
            bool validShift = false;
            while (!validShift) {
                cout << "\t\tEnter shift " << i + 1 << " details:\n";
                do {
                    cout << "\t\tStart time (0-23): ";
                    cin >> input;
                    if (!isValidNumber(input) || stoi(input) < 0 || stoi(input) > 23) {
                        cout << "\t\tError: Start time must be 0-23.\n";
                    }
                    else {
                        timeTrips[i].startHour = stoi(input);
                        break;
                    }
                } while (true);

                do {
                    cout << "\t\tEnd time (0-23): ";
                    cin >> input;
                    if (!isValidNumber(input) || stoi(input) <= timeTrips[i].startHour || stoi(input) > 23) {
                        cout << "\t\tError: End time must be after start time and 0-23.\n";
                    }
                    else {
                        timeTrips[i].endHour = stoi(input);
                        break;
                    }
                } while (true);

                validShift = true;
                for (int j = 0; j < i; j++) {
                    if (timeTrips[i].startHour < timeTrips[j].endHour && timeTrips[i].endHour > timeTrips[j].startHour) {
                        cout << "\t\tError: Shift overlaps with shift " << j + 1 << ".\n";
                        validShift = false;
                        break;
                    }
                }

                if (validShift) {
                    do {
                        cout << "\t\tPrice per hour: ";
                        cin >> input;
                        if (!isValidNumber(input) || stof(input) <= 0) {
                            cout << "\t\tError: Price must be positive.\n";
                        }
                        else {
                            timeTrips[i].pricePerHour = stof(input);
                            break;
                        }
                    } while (true);
                }
            }
        }
        cout << "\n\t\tBoat added successfully.\n";
        pressEnterToContinue();
    }

    void Boat::showBoat() const {
        cout << "\t\tBoat ID: " << boatID << "\n";
        cout << "\t\tName: " << boatName << "\n";
        cout << "\t\tType: " << type << "\n";
        cout << "\t\tWeight: " << weight << " tons\n";
        cout << "\t\tSeats: " << seats << "\n";
        cout << "\t\tStatus: " << (isAvailable ? "Available" : "Unavailable") << "\n";
        cout << "\t\tShifts: " << numberOfTrips << "\n";
        for (int i = 0; i < numberOfTrips; i++) {
            cout << "\t\tShift " << i + 1 << ": " << timeTrips[i].startHour << "h-"
                << timeTrips[i].endHour << "h, Price: $" << timeTrips[i].pricePerHour
                << "/hr, Seats Booked: " << timeTrips[i].seatsBooked << "\n";
        }
        cout << "\t\tRentals: " << rentalCount << "\n";
    }

    void Boat::bookSeats(int shiftIndex, float seatsBooked) {
        if (shiftIndex >= 0 && shiftIndex < numberOfTrips) {
            timeTrips[shiftIndex].seatsBooked += seatsBooked;
            if (timeTrips[shiftIndex].seatsBooked >= seats) {
                setAvailability(false);
            }
            incrementRentalCount();
        }
    }

    void Boat::cancelSeats(int shiftIndex, float seatsCancelled) {
        if (shiftIndex >= 0 && shiftIndex < numberOfTrips) {
            timeTrips[shiftIndex].seatsBooked -= seatsCancelled;
            if (timeTrips[shiftIndex].seatsBooked < 0) timeTrips[shiftIndex].seatsBooked = 0;
            bool fullyBooked = false;
            for (const auto& shift : timeTrips) {
                if (shift.seatsBooked >= seats) {
                    fullyBooked = true;
                    break;
                }
            }
            if (!fullyBooked) {
                setAvailability(true);
            }
        }
    }

    void Boat::saveToFile(ofstream & outFile) const {
        outFile << boatID << "\n";
        outFile << boatName << "\n";
        outFile << type << "\n";
        outFile << weight << "\n";
        outFile << isAvailable << "\n";
        outFile << seats << "\n";
        outFile << numberOfTrips << "\n";
        for (const auto& shift : timeTrips) {
            outFile << shift.startHour << " " << shift.endHour << " "
                << shift.pricePerHour << " " << shift.seatsBooked << "\n";
        }
        outFile << rentalCount << "\n";
    }

    void Boat::loadFromFile(ifstream & inFile) {
        getline(inFile, boatID);
        getline(inFile, boatName);
        inFile >> type >> weight >> isAvailable >> seats >> numberOfTrips;
        inFile.ignore(numeric_limits<streamsize>::max(), '\n');
        timeTrips.resize(numberOfTrips);
        for (int i = 0; i < numberOfTrips; i++) {
            inFile >> timeTrips[i].startHour >> timeTrips[i].endHour
                >> timeTrips[i].pricePerHour >> timeTrips[i].seatsBooked;
            inFile.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        inFile >> rentalCount;
        inFile.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Class BoatManager
    class BoatManager {
        vector<Boat> boats;
        vector<RentalRecord> rentalHistory;
        vector<FoodItem> foodMenu = {
            {"Sandwich", 5.0},
            {"Soft Drink", 2.0},
            {"Fruit Platter", 8.0}
        };

    public:
        void addBoat() {
            Boat boat;
            boat.createBoat();
            boats.push_back(boat);
            saveBoatsToFile();
        }

        void displayAllBoats() const {
            system("cls");
            displayHeader();
            if (boats.empty()) {
                cout << "\t\tNo boats available.\n";
            }
            else {
                for (const auto& boat : boats) {
                    boat.showBoat();
                    cout << "\t\t-------------------\n";
                }
            }
            pressEnterToContinue();
        }

        void rentSeats() {
            system("cls");
            displayHeader();
            string boatID;
            cout << "\t\tEnter Boat ID (e.g., B001): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, boatID);

            auto it = find_if(boats.begin(), boats.end(),
                [&boatID](const Boat& b) { return b.getBoatID() == boatID; });

            if (it == boats.end()) {
                cout << "\t\tError: Boat not found.\n";
                pressEnterToContinue();
                return;
            }

            Boat& boat = *it;
            if (!boat.getAvailability()) {
                cout << "\t\tError: Boat is not available.\n";
                pressEnterToContinue();
                return;
            }

            cout << "\t\tAvailable shifts:\n";
            const auto& shifts = boat.getTimeTrips();
            for (int i = 0; i < boat.getNumberOfTrips(); i++) {
                cout << "\t\tShift " << i + 1 << ": " << shifts[i].startHour << "h-"
                    << shifts[i].endHour<<"h, price per Hours"<<shifts[i].pricePerHour << " $, Seats Available: "
                    << boat.getSeats() - shifts[i].seatsBooked << "\n";
            }

            string input;
            int shiftIndex;
            do {
                cout << "\t\tEnter shift number (1-" << boat.getNumberOfTrips() << "): ";
                cin >> input;
                if (!isValidNumber(input) || stoi(input) <= 0 || stoi(input) > boat.getNumberOfTrips()) {
                    cout << "\t\tError: Invalid shift.\n";
                }
                else {
                    shiftIndex = stoi(input) - 1;
                    break;
                }
            } while (true);

            float seats;
            do {
                cout << "\t\tEnter number of seats to rent: ";
                cin >> input;
                if (!isValidNumber(input) || stof(input) <= 0 ||
                    stof(input) >( boat.getSeats() - shifts[shiftIndex].seatsBooked)) {
                    cout << "\t\tError: Invalid or excessive seats.\n";
                }   
                else {
                    seats = stof(input);
                    cout << "\t\tCost: " << stof(input) * shifts[shiftIndex].pricePerHour<<" $\n";
                    break;
                }
            } while (true);

            // Food/Drink Ordering
            vector<FoodItem> selectedFoodItems;
            float foodCost = 0.0f;
            char orderFood;
            do {
                cout << "\t\tOrder food/drinks? (y/n): ";
                cin >> orderFood;
                orderFood = tolower(orderFood);
                if (orderFood == 'y' || orderFood == 'n') break;
                cout << "\t\tError: Enter 'y' or 'n'.\n";
            } while (true);

            if (orderFood == 'y') {
                cout << "\t\tFood/Drink Menu:\n";
                for (size_t i = 0; i < foodMenu.size(); i++) {
                    cout << "\t\t" << i + 1 << ". " << foodMenu[i].name << " - $" << foodMenu[i].price << "\n";
                }
                cout << "\t\tEnter item number (1-" << foodMenu.size() << ") or 0 to finish: ";
                int foodChoice;
                while (cin >> foodChoice && foodChoice != 0) {
                    if (foodChoice < 1 || foodChoice > static_cast<int>(foodMenu.size())) {
                        cout << "\t\tError: Invalid choice.\n";
                    }
                    else {
                        selectedFoodItems.push_back(foodMenu[foodChoice - 1]);
                        foodCost += foodMenu[foodChoice - 1].price;
                        cout << "\t\tAdded " << foodMenu[foodChoice - 1].name << " ($" << foodMenu[foodChoice - 1].price << ")\n";
                    }
                    cout << "\t\tNext item (1-" << foodMenu.size() << ") or 0 to finish: ";
                }
            }

            RentalRecord record;
            record.boatID = boatID;
            record.ticketCode = generateTicketCode(rentalHistory);
            record.seatsRented = seats;
            record.shiftIndex = shiftIndex;
            record.action = "RENT";
            record.foodItems = selectedFoodItems;
            record.foodCost = foodCost;
            rentalHistory.push_back(record);

            boat.bookSeats(shiftIndex, seats);
            saveRentalHistoryToFile();
            saveBoatsToFile();
            float FinalCost = foodCost + stof(input) * shifts[shiftIndex].pricePerHour;
            cout << "\t\tRental successful! Ticket: " << record.ticketCode << "\n";
            if (foodCost > 0) {
                cout << "\t\tFood/Drink Cost: $" << foodCost << "\n";
            }
            if (FinalCost > 0) {
                cout << "\t\tfood and seat tickets total: " << FinalCost << " $";
            }
            pressEnterToContinue();
        }

        void cancelRent() {
            system("cls");
            displayHeader();
            string ticketCode;
            cout << "\t\tEnter ticket code: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, ticketCode);

            auto it = find_if(rentalHistory.begin(), rentalHistory.end(),
                [&ticketCode](const RentalRecord& r) { return r.ticketCode == ticketCode && r.action == "RENT"; });

            if (it == rentalHistory.end()) {
                cout << "\t\tError: Invalid or cancelled ticket.\n";
                pressEnterToContinue();
                return;
            }

            RentalRecord& record = *it;
            auto boatIt = find_if(boats.begin(), boats.end(),
                [&record](const Boat& b) { return b.getBoatID() == record.boatID; });

            if (boatIt == boats.end()) {
                cout << "\t\tError: Boat not found.\n";
                pressEnterToContinue();
                return;
            }

            Boat& boat = *boatIt;
            RentalRecord cancelRecord = record;
            cancelRecord.action = "CANCEL";
            rentalHistory.push_back(cancelRecord);

            boat.cancelSeats(record.shiftIndex, record.seatsRented);
            saveRentalHistoryToFile();
            saveBoatsToFile();

            cout << "\t\tCancellation successful.\n";
            pressEnterToContinue();
        }

        void showAvailableBoats() const {
            system("cls");
            displayHeader();
            bool found = false;
            for (const auto& boat : boats) {
                if (boat.getAvailability()) {
                    boat.showBoat();
                    cout << "\t\t-------------------\n";
                    found = true;
                }
            }
            if (!found) {
                cout << "\t\tNo boats available.\n";
            }
            pressEnterToContinue();
        }

        void displayRentalHistory() const {
            system("cls");
            displayHeader();
            if (rentalHistory.empty()) {
                cout << "\t\tNo rental history.\n";
            }
            else {
                for (const auto& record : rentalHistory) {
                    cout << "\t\tBoat ID: " << record.boatID << "\n";
                    cout << "\t\tTicket: " << record.ticketCode << "\n";
                    cout << "\t\tSeats: " << record.seatsRented << "\n";
                    cout << "\t\tShift: " << record.shiftIndex + 1 << "\n";
                    cout << "\t\tAction: " << record.action << "\n";
                    if (!record.foodItems.empty()) {
                        cout << "\t\tFood/Drinks: ";
                        for (const auto& item : record.foodItems) {
                            cout << item.name << " ($" << item.price << "), ";
                        }
                        cout << "\n\t\tFood Cost: $" << record.foodCost << "\n";
                    }
                    cout << "\t\t-------------------\n";
                }
            }
            pressEnterToContinue();
        }

        void showMostRentedBoat() const {
            system("cls");
            displayHeader();
            if (boats.empty()) {
                cout << "\t\tNo boats available.\n";
            }
            else {
                auto maxBoat = max_element(boats.begin(), boats.end(),
                    [](const Boat& a, const Boat& b) { return a.getRentalCount() < b.getRentalCount(); });
                cout << "\t\tMost rented boat:\n";
                maxBoat->showBoat();
            }
            pressEnterToContinue();
        }

        void showSeatsPerShift() const {
            system("cls");
            displayHeader();
            string boatID;
            cout << "\t\tEnter Boat ID (e.g., B001): ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, boatID);

            auto it = find_if(boats.begin(), boats.end(),
                [&boatID](const Boat& b) { return b.getBoatID() == boatID; });

            if (it == boats.end()) {
                cout << "\t\tError: Boat not found.\n";
                pressEnterToContinue();
                return;
            }

            const Boat& boat = *it;
            cout << "\t\tSeats rented on boat " << boatID << ":\n";
            const auto& shifts = boat.getTimeTrips();
            for (int i = 0; i < boat.getNumberOfTrips(); i++) {
                cout << "\t\tShift " << i + 1 << " (" << shifts[i].startHour << "h-"
                    << shifts[i].endHour << "h): " << shifts[i].seatsBooked << " seats\n";
            }
            pressEnterToContinue();
        }

        void saveBoatsToFile() const {
            ofstream outFile("boats.txt");
            if (!outFile) {
                cout << "\t\tError: Cannot write to boats.txt.\n";
                return;
            }
            outFile << boats.size() << "\n";
            for (const auto& boat : boats) {
                boat.saveToFile(outFile);
            }
            outFile.close();
        }

        void loadBoatsFromFile() {
            ifstream inFile("boats.txt");
            if (!inFile) return;
            int numBoats;
            inFile >> numBoats;
            inFile.ignore(numeric_limits<streamsize>::max(), '\n');
            boats.clear();
            for (int i = 0; i < numBoats; i++) {
                Boat boat;
                boat.loadFromFile(inFile);
                boats.push_back(boat);
            }
            inFile.close();
        }

        void saveRentalHistoryToFile() const {
            ofstream outFile("rental_history.txt");
            if (!outFile) {
                cout << "\t\tError: Cannot write to rental_history.txt.\n";
                return;
            }
            outFile << rentalHistory.size() << "\n";
            for (const auto& record : rentalHistory) {
                outFile << record.boatID << "\n";
                outFile << record.ticketCode << "\n";
                outFile << record.seatsRented << "\n";
                outFile << record.shiftIndex << "\n";
                outFile << record.action << "\n";
                outFile << record.foodItems.size() << "\n";
                for (const auto& item : record.foodItems) {
                    outFile << item.name << "\n" << item.price << "\n";
                }
                outFile << record.foodCost << "\n";
            }
            outFile.close();
        }

        void loadRentalHistoryFromFile() {
            ifstream inFile("rental_history.txt");
            if (!inFile) return;
            int numRecords;
            inFile >> numRecords;
            inFile.ignore(numeric_limits<streamsize>::max(), '\n');
            rentalHistory.clear();
            for (int i = 0; i < numRecords; i++) {
                RentalRecord record;
                getline(inFile, record.boatID);
                getline(inFile, record.ticketCode);
                inFile >> record.seatsRented >> record.shiftIndex;
                inFile.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(inFile, record.action);
                int numFoodItems;
                inFile >> numFoodItems;
                inFile.ignore(numeric_limits<streamsize>::max(), '\n');
                for (int j = 0; j < numFoodItems; j++) {
                    FoodItem item;
                    getline(inFile, item.name);
                    inFile >> item.price;
                    inFile.ignore(numeric_limits<streamsize>::max(), '\n');
                    record.foodItems.push_back(item);
                }
                inFile >> record.foodCost;
                inFile.ignore(numeric_limits<streamsize>::max(), '\n');
                rentalHistory.push_back(record);
            }
            inFile.close();
        }
    };

    // Main Function
    int main() {
        BoatManager manager;
        manager.loadBoatsFromFile();
        manager.loadRentalHistoryFromFile();
        int choice;

        do {
            displayMenu();
            string input;
            cin >> input;
            if (!isValidNumber(input)) {
                cout << "\t\tError: Enter a number.\n";
                pressEnterToContinue();
                continue;
            }
            choice = stoi(input);

            switch (choice) {
            case 1: manager.addBoat(); break;
            case 2: manager.displayAllBoats(); break;
            case 3: manager.rentSeats(); break;
            case 4: manager.cancelRent(); break;
            case 5: manager.showAvailableBoats(); break;
            case 6: manager.displayRentalHistory(); break;
            case 7: manager.showMostRentedBoat(); break;
            case 8: manager.showSeatsPerShift(); break;
            case 9:
                cout << "\t\tExiting system.\n";
                pressEnterToContinue();
                break;
            default:
                cout << "\t\tInvalid choice.\n";
                pressEnterToContinue();
            }
        } while (choice != 9);

        return 0;
    }
