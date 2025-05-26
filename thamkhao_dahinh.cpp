#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

// Cấu trúc cho món ăn/uống
struct FoodItem {
    string name;
    float price;
};

// Cấu trúc cho bản ghi thuê
struct RentalRecord {
    string boatID;
    string ticketCode;
    float seatsRented;
    int shiftIndex;
    string action; // "RENT" hoặc "CANCEL"
    vector<FoodItem> foodItems;
    float foodCost;
};

// Cấu trúc cho ca làm việc
struct Shift {
    int startHour;
    int endHour;
    float pricePerHour;
    float seatsBooked;
    Shift() : startHour(0), endHour(0), pricePerHour(0), seatsBooked(0) {}
};

// Hàm hỗ trợ kiểm tra số hợp lệ
bool isValidNumber(const string& str) {
    try {
        size_t pos;
        stof(str, &pos);
        return pos == str.length();
    }
    catch (...) {
        return false;
    }
}

// Lớp cơ sở trừu tượng Boat
class Boat {
protected:
    string boatID;
    string boatName;
    char type; // 'S' cho SmallBoat, 'L' cho LargeBoat
    float weight;
    bool isAvailable;
    float seats;
    int numShifts;
    vector<Shift> shifts;
    int rentalCount;

public:
    Boat() : isAvailable(true), rentalCount(0), seats(0), numShifts(0), type(' ') {}
    virtual ~Boat() {}

    // Hàm ảo thuần túy cho đa hình
    virtual void display() const = 0;

    char getType() const { return type; }
    void setType(char t) { type = t; }

    void createBoat() {
        system("cls");
        cout << "\n\t\t======================================";
        cout << "\n\t\t           TAO THUYEN MOI    ";
        cout << "\n\t\t======================================\n";

        do {
            cout << "\t\tNhap ID thuyen (vi du: B001): ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, boatID);
            if (boatID.empty() || boatID.length() > 10) {
                cout << "\t\tLoi. Ten thuyen khong duoc de trong hoac phai nho hon 10 ki tu .\n";
            }
        } while (boatID.empty() || boatID.length() > 10);

        do {
            cout << "\t\tNhap ten thuyen: ";
            getline(cin, boatName);
            if (boatName.empty()) {
                cout << "\t\tLoi. Ten thuyen khong duoc de trong.\n";
            }
        } while (boatName.empty());

        string input;
        do {
            cout << "\t\tNhap trong luong cua thuyen( tan): ";
            cin >> input;
            if (!isValidNumber(input) || stof(input) <= 0) {
                cout << "\t\tLoi. Trong luong cua thuyen phai la so duong.\n";
            }
            else {
                weight = stof(input);
                break;
            }
        } while (true);

        do {
            cout << "\t\tNhap so ghe: ";
            cin >> input;
            if (!isValidNumber(input) || stof(input) <= 0) {
                cout << "\t\tLoi. So ghe phai la so duong.\n";
            }
            else {
                seats = stof(input);
                break;
            }
        } while (true);

        do {
            cout << "\t\tNhập so ca lam viec: ";
            cin >> input;
            if (!isValidNumber(input) || stoi(input) <= 0) {
                cout << "\t\tLoi. so ca lam viec phai la so duong.\n";
            }
            else if (stoi(input) >= 23) {
                cout << "\t\tLoi: So ca phai nho hon 23.\n";
            }
            else {
                numShifts = stoi(input);
                break;
            }
        } while (true);

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        shifts.resize(numShifts);

        for (int i = 0; i < numShifts; i++) {
            bool validShift = false;
            while (!validShift) {
                cout << "\t\tNhap chi tiet ca " << i + 1 << ":\n";
                do {
                    cout << "\t\tGio bat dau (0-22): ";
                    cin >> input;
                    if (!isValidNumber(input) || stoi(input) < 0 || stoi(input) > 22) {
                        cout << "\t\tLoi. Gio bat dau phai trong khoang 0-22.\n";
                    }
                    else {
                        shifts[i].startHour = stoi(input);
                        break;
                    }
                } while (true);

                do {
                    cout << "\t\tGio ket thuc (1-23): ";
                    cin >> input;
                    if (!isValidNumber(input) || stoi(input) <= shifts[i].startHour || stoi(input) > 23) {
                        cout << "\t\tLoi. Gio ket thuc phai sau gio bat dau va trong khoang 1-23.\n";
                    }
                    else {
                        shifts[i].endHour = stoi(input);
                        break;
                    }
                } while (true);

                validShift = true;
                for (int j = 0; j < i; j++) {
                    if (shifts[i].startHour < shifts[j].endHour && shifts[i].endHour > shifts[j].startHour) {
                        cout << "\t\tLoi. Ca nay trung voi ca " << j + 1 << ".\n";
                        validShift = false;
                        break;
                    }
                }

                if (validShift) {
                    do {
                        cout << "\t\tGia cho moi gio: ";
                        cin >> input;
                        if (!isValidNumber(input) || stof(input) <= 0) {
                            cout << "\t\tLoi. Gia phai la so duong.\n";
                        }
                        else {
                            shifts[i].pricePerHour = stof(input);
                            break;
                        }
                    } while (true);
                }
            }
        }
        cout << "\n\t\tThuyen duoc them thanh cong.\n";
    }

    void setAvailability(bool available) { isAvailable = available; }
    bool getAvailability() const { return isAvailable; }
    float getSeats() const { return seats; }
    string getBoatID() const { return boatID; }
    int getRentalCount() const { return rentalCount; }
    void incrementRentalCount() { rentalCount++; }
    const vector<Shift>& getShifts() const { return shifts; }
    int getNumShifts() const { return numShifts; }

    void bookSeats(int shiftIndex, float seatsBooked) {
        if (shiftIndex >= 0 && shiftIndex < numShifts) {
            shifts[shiftIndex].seatsBooked += seatsBooked;
            incrementRentalCount();
            bool allShiftsFull = true;
            for (const auto& shift : shifts) {
                if (shift.seatsBooked < seats) {
                    allShiftsFull = false;
                    break;
                }
            }
            setAvailability(!allShiftsFull);
        }
    }

    void cancelSeats(int shiftIndex, float seatsCancelled) {
        if (shiftIndex >= 0 && shiftIndex < numShifts) {
            shifts[shiftIndex].seatsBooked -= seatsCancelled;
            if (shifts[shiftIndex].seatsBooked < 0) {
                shifts[shiftIndex].seatsBooked = 0;
            }
            bool allShiftsFull = true;
            for (const auto& shift : shifts) {
                if (shift.seatsBooked < seats) {
                    allShiftsFull = false;
                    break;
                }
            }
                setAvailability(!allShiftsFull);
           
        }
    }

    void saveToFile(ofstream& outFile) const {
        outFile << boatID << "\n";
        outFile << boatName << "\n";
        outFile << type << "\n";
        outFile << weight << "\n";
        outFile << isAvailable << "\n";
        outFile << seats << "\n";
        outFile << numShifts << "\n";
        for (const auto& shift : shifts) {
            outFile << shift.startHour << " " << shift.endHour << " "
                << shift.pricePerHour << " " << shift.seatsBooked << "\n";
        }
        outFile << rentalCount << "\n";
    }

    void loadFromFile(ifstream& inFile) {
        getline(inFile, boatID);
        getline(inFile, boatName);
        inFile >> type >> weight >> isAvailable >> seats >> numShifts;
        inFile.ignore(numeric_limits<streamsize>::max(), '\n');
        shifts.resize(numShifts);
        for (int i = 0; i < numShifts; i++) {
            inFile >> shifts[i].startHour >> shifts[i].endHour
                >> shifts[i].pricePerHour >> shifts[i].seatsBooked;
            inFile.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        inFile >> rentalCount;
        inFile.ignore(numeric_limits<streamsize>::max(), '\n');
    }
};

// Lớp con SmallBoat
class SmallBoat : public Boat {
public:
    SmallBoat() { setType('S'); }
    void display() const override {
        cout << "\t\tID Thuyền Nhỏ: " << boatID << "\n";
        cout << "\t\tTên: " << boatName << "\n";
        cout << "\t\tLoại: Nhỏ\n";
        cout << "\t\tTrọng lượng: " << weight << " tấn\n";
        cout << "\t\tSố ghế: " << seats << "\n";
        cout << "\t\tTrạng thái: " << (isAvailable ? "Còn trống" : "Hết chỗ") << "\n";
        cout << "\t\tSố ca: " << numShifts << "\n";
        for (int i = 0; i < numShifts; i++) {
            cout << "\t\tCa " << i + 1 << ": " << shifts[i].startHour << "h-"
                << shifts[i].endHour << "h, Giá: $" << shifts[i].pricePerHour
                << "/giờ, Ghế đã đặt: " << shifts[i].seatsBooked << "\n";
        }
        cout << "\t\tSố lần thuê: " << rentalCount << "\n";
    }
};

// Lớp con LargeBoat
class LargeBoat : public Boat {
public:
    LargeBoat() { setType('L'); }
    void display() const override {
        cout << "\t\tID Thuyền Lớn: " << boatID << "\n";
        cout << "\t\tTên: " << boatName << "\n";
        cout << "\t\tLoại: Lớn\n";
        cout << "\t\tTrọng lượng: " << weight << " tấn\n";
        cout << "\t\tSố ghế: " << seats << "\n";
        cout << "\t\tTrạng thái: " << (isAvailable ? "Còn trống" : "Hết chỗ") << "\n";
        cout << "\t\tSố ca: " << numShifts << "\n";
        for (int i = 0; i < numShifts; i++) {
            cout << "\t\tCa " << i + 1 << ": " << shifts[i].startHour << "h-"
                << shifts[i].endHour << "h, Giá: $" << shifts[i].pricePerHour
                << "/giờ, Ghế đã đặt: " << shifts[i].seatsBooked << "\n";
        }
        cout << "\t\tSố lần thuê: " << rentalCount << "\n";
    }
};

// Hàm hỗ trợ
void displayHeader() {
    cout << "\n\t\t======================================";
    cout << "\n\t\t  He Thong Quan Ly Tau THuyen Tren Song Han  ";
    cout << "\n\t\t======================================\n";
}

bool isValidBoatType(char type) {
    return type == 'S' || type == 'L';
}

void pressEnterToContinue() {
    cout << "\n\t\tNhấn Enter để tiếp tục...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void displayMenu() {
    system("cls");
    displayHeader();
    cout << "\n\t\t1. Thêm thuyền mới";
    cout << "\n\t\t2. Hiển thị tất cả thuyền";
    cout << "\n\t\t3. Thuê ghế";
    cout << "\n\t\t4. Hủy ghế đã thuê";
    cout << "\n\t\t5. Hiển thị thuyền còn trống";
    cout << "\n\t\t6. Hiển thị lịch sử thuê";
    cout << "\n\t\t7. Hiển thị thuyền được thuê nhiều nhất";
    cout << "\n\t\t8. Tổng số ghế đã thuê trên một thuyền";
    cout << "\n\t\t9. Thoát";
    cout << "\n\t\tNhập lựa chọn (1-9): ";
}

string generateTicketCode(const vector<RentalRecord>& rentalHistory) {
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

// Lớp BoatManager
class BoatManager {
    vector<Boat*> boats;
    vector<RentalRecord> rentalHistory;
    vector<FoodItem> foodMenu = {
        {"Bánh mì", 5.0},
        {"Nước ngọt", 2.0},
        {"Đĩa trái cây", 8.0}
    };

public:
    BoatManager() {
        loadBoatsFromFile();
        loadRentalHistoryFromFile();
    }

    ~BoatManager() {
        for (Boat* boat : boats) {
            delete boat;
        }
    }

    void addBoat() {
        system("cls");
        displayHeader();
        char type;
        do {
            cout << "\t\tNhập loại thuyền (S cho Nhỏ, L cho Lớn): ";
            cin >> type;
            type = toupper(type);
            if (!isValidBoatType(type)) {
                cout << "\t\tLỗi: Loại thuyền phải là S hoặc L.\n";
            }
        } while (!isValidBoatType(type));

        Boat* boat;
        if (type == 'S') {
            boat = new SmallBoat();
        }
        else {
            boat = new LargeBoat();
        }
        boat->createBoat();
        for (const Boat* b : boats) {
            if (b->getBoatID() == boat->getBoatID()) {
                cout << "\t\tLỗi: ID thuyền đã tồn tại.\n";
                delete boat;
                pressEnterToContinue();
                return;
            }
        }
        boats.push_back(boat);
        saveBoatsToFile();
        cout << "\t\tThuyền được thêm thành công.\n";
        pressEnterToContinue();
    }

    void displayAllBoats() const {
        system("cls");
        displayHeader();
        if (boats.empty()) {
            cout << "\t\tKhông có thuyền nào.\n";
        }
        else {
            for (const Boat* boat : boats) {
                boat->display();
                cout << "\t\t-------------------\n";
            }
        }
        pressEnterToContinue();
    }

    void rentSeats() {
        system("cls");
        displayHeader();
        string boatID;
        cout << "\t\tNhập ID thuyền (ví dụ: B001): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, boatID);

        Boat* boat = nullptr;
        for (Boat* b : boats) {
            if (b->getBoatID() == boatID) {
                boat = b;
                break;
            }
        }

        if (!boat) {
            cout << "\t\tLỗi: Không tìm thấy thuyền.\n";
            pressEnterToContinue();
            return;
        }

        if (!boat->getAvailability()) {
            cout << "\t\tLỗi: Thuyền không còn trống.\n";
            pressEnterToContinue();
            return;
        }

        cout << "\t\tCác ca còn trống:\n";
        const auto& shifts = boat->getShifts();
        for (int i = 0; i < boat->getNumShifts(); i++) {
            cout << "\t\tCa " << i + 1 << ": " << shifts[i].startHour << "h-"
                << shifts[i].endHour << "h, Giá: $" << shifts[i].pricePerHour
                << "/giờ, Ghế còn trống: " << boat->getSeats() - shifts[i].seatsBooked << "\n";
        }

        string input;
        int shiftIndex;
        do {
            cout << "\t\tNhập số ca (1-" << boat->getNumShifts() << "): ";
            cin >> input;
            if (!isValidNumber(input) || stoi(input) <= 0 || stoi(input) > boat->getNumShifts()) {
                cout << "\t\tLỗi: Ca không hợp lệ.\n";
            }
            else {
                shiftIndex = stoi(input) - 1;
                break;
            }
        } while (true);

        float seats;
        do {
            cout << "\t\tNhập số ghế cần thuê: ";
            cin >> input;
            if (!isValidNumber(input) || stof(input) <= 0 ||
                stof(input) > (boat->getSeats() - shifts[shiftIndex].seatsBooked)) {
                cout << "\t\tLỗi: Số ghế không hợp lệ hoặc vượt quá số ghế trống.\n";
            }
            else {
                seats = stof(input);
                float rentalCost = seats * shifts[shiftIndex].pricePerHour * (shifts[shiftIndex].endHour - shifts[shiftIndex].startHour);
                cout << "\t\tChi phí thuê: $" << rentalCost << "\n";
                break;
            }
        } while (true);

        // Đặt món ăn/uống
        vector<FoodItem> selectedFoodItems;
        float foodCost = 0.0f;
        char orderFood;
        do {
            cout << "\t\tĐặt món ăn/uống? (y/n): ";
            cin >> orderFood;
            orderFood = tolower(orderFood);
            if (orderFood == 'y' || orderFood == 'n') break;
            cout << "\t\tLỗi: Nhập 'y' hoặc 'n'.\n";
        } while (true);

        if (orderFood == 'y') {
            cout << "\t\tMenu món ăn/uống:\n";
            for (size_t i = 0; i < foodMenu.size(); i++) {
                cout << "\t\t" << i + 1 << ". " << foodMenu[i].name << " - $" << foodMenu[i].price << "\n";
            }
            cout << "\t\tNhập số món (1-" << foodMenu.size() << ") hoặc 0 để kết thúc: ";
            int foodChoice;
            while (cin >> foodChoice && foodChoice != 0) {
                if (foodChoice < 1 || foodChoice > static_cast<int>(foodMenu.size())) {
                    cout << "\t\tLỗi: Lựa chọn không hợp lệ.\n";
                }
                else {
                    selectedFoodItems.push_back(foodMenu[foodChoice - 1]);
                    foodCost += foodMenu[foodChoice - 1].price;
                    cout << "\t\tĐã thêm " << foodMenu[foodChoice - 1].name << " ($" << foodMenu[foodChoice - 1].price << ")\n";
                }
                cout << "\t\tMón tiếp theo (1-" << foodMenu.size() << ") hoặc 0 để kết thúc: ";
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

        boat->bookSeats(shiftIndex, seats);
        saveRentalHistoryToFile();
        saveBoatsToFile();
        float totalCost = foodCost + seats * shifts[shiftIndex].pricePerHour * (shifts[shiftIndex].endHour - shifts[shiftIndex].startHour);
        cout << "\t\tThuê thành công! Mã vé: " << record.ticketCode << "\n";
        if (foodCost > 0) {
            cout << "\t\tChi phí món ăn/uống: $" << foodCost << "\n";
        }
        cout << "\t\tTổng chi phí: $" << totalCost << "\n";
        pressEnterToContinue();
    }

    void cancelRent() {
        system("cls");
        displayHeader();
        string ticketCode;
        cout << "\t\tNhập mã vé: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, ticketCode);

        auto it = find_if(rentalHistory.begin(), rentalHistory.end(),
            [&ticketCode](const RentalRecord& r) { return r.ticketCode == ticketCode && r.action == "RENT"; });

        if (it == rentalHistory.end()) {
            cout << "\t\tLỗi: Mã vé không hợp lệ hoặc đã bị hủy.\n";
            pressEnterToContinue();
            return;
        }

        RentalRecord& record = *it;
        Boat* boat = nullptr;
        for (Boat* b : boats) {
            if (b->getBoatID() == record.boatID) {
                boat = b;
                break;
            }
        }

        if (!boat) {
            cout << "\t\tLỗi: Không tìm thấy thuyền.\n";
            pressEnterToContinue();
            return;
        }

        RentalRecord cancelRecord = record;
        cancelRecord.action = "CANCEL";
        rentalHistory.push_back(cancelRecord);

        boat->cancelSeats(record.shiftIndex, record.seatsRented);
        saveRentalHistoryToFile();
        saveBoatsToFile();

        cout << "\t\tHủy thuê thành công.\n";
        pressEnterToContinue();
    }

    void showAvailableBoats() const {
        system("cls");
        displayHeader();
        bool found = false;
        for (const Boat* boat : boats) {
            if (boat->getAvailability()) {
                boat->display();
                cout << "\t\t-------------------\n";
                found = true;
            }
        }
        if (!found) {
            cout << "\t\tKhông có thuyền nào còn trống.\n";
        }
        pressEnterToContinue();
    }

    void displayRentalHistory() const {
        system("cls");
        displayHeader();
        if (rentalHistory.empty()) {
            cout << "\t\tKhông có lịch sử thuê.\n";
        }
        else {
            for (const auto& record : rentalHistory) {
                cout << "\t\tID Thuyền: " << record.boatID << "\n";
                cout << "\t\tMã vé: " << record.ticketCode << "\n";
                cout << "\t\tSố ghế: " << record.seatsRented << "\n";
                cout << "\t\tCa: " << record.shiftIndex + 1 << "\n";
                cout << "\t\tHành động: " << record.action << "\n";
                if (!record.foodItems.empty()) {
                    cout << "\t\tMón ăn/uống: ";
                    for (const auto& item : record.foodItems) {
                        cout << item.name << " ($" << item.price << "), ";
                    }
                    cout << "\n\t\tChi phí món ăn: $" << record.foodCost << "\n";
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
            cout << "\t\tKhông có thuyền nào.\n";
        }
        else {
            auto maxBoat = max_element(boats.begin(), boats.end(),
                [](const Boat* a, const Boat* b) { return a->getRentalCount() < b->getRentalCount(); });
            cout << "\t\tThuyền được thuê nhiều nhất:\n";
            (*maxBoat)->display();
        }
        pressEnterToContinue();
    }

    void showSeatsPerShift() const {
        system("cls");
        displayHeader();
        string boatID;
        cout << "\t\tNhập ID thuyền (ví dụ: B001): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, boatID);

        const Boat* boat = nullptr;
        for (const Boat* b : boats) {
            if (b->getBoatID() == boatID) {
                boat = b;
                break;
            }
        }

        if (!boat) {
            cout << "\t\tLỗi: Không tìm thấy thuyền.\n";
            pressEnterToContinue();
            return;
        }

        cout << "\t\tSố ghế đã thuê trên thuyền " << boatID << ":\n";
        const auto& shifts = boat->getShifts();
        for (int i = 0; i < boat->getNumShifts(); i++) {
            cout << "\t\tCa " << i + 1 << " (" << shifts[i].startHour << "h-"
                << shifts[i].endHour << "h): " << shifts[i].seatsBooked << " ghế\n";
        }
        pressEnterToContinue();
    }

    void saveBoatsToFile() const {
        ofstream outFile("boats.txt");
        if (!outFile) {
            cout << "\t\tLỗi: Không thể ghi vào boats.txt.\n";
            return;
        }
        outFile << boats.size() << "\n";
        for (const Boat* boat : boats) {
            outFile << boat->getType() << "\n";
            boat->saveToFile(outFile);
        }
        outFile.close();
    }

    void loadBoatsFromFile() {
        ifstream inFile("boats.txt");
        if (!inFile) {
            cout << "\t\tKhông tìm thấy file boats.txt.\n";
            return;
        }
        int numBoats;
        inFile >> numBoats;
        inFile.ignore(numeric_limits<streamsize>::max(), '\n');
        boats.clear();
        for (int i = 0; i < numBoats; i++) {
            char type;
            inFile >> type;
            inFile.ignore(numeric_limits<streamsize>::max(), '\n');
            Boat* boat = nullptr;
            if (type == 'S') {
                boat = new SmallBoat();
            }
            else if (type == 'L') {
                boat = new LargeBoat();
            }
            else {
                cout << "\t\tLỗi: Loại thuyền không hợp lệ trong file: " << type << "\n";
                continue; // Bỏ qua thuyền không hợp lệ
            }
            boat->loadFromFile(inFile);
            boats.push_back(boat);
        }
        inFile.close();
    }

    void saveRentalHistoryToFile() const {
        ofstream outFile("rental_history.txt");
        if (!outFile) {
            cout << "\t\tLỗi: Không thể ghi vào rental_history.txt.\n";
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

// Hàm chính
int main() {
    BoatManager manager;
    int choice;

    do {
        displayMenu();
        string input;
        cin >> input;
        if (!isValidNumber(input)) {
            cout << "\t\tLỗi: Vui lòng nhập số.\n";
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
            cout << "\t\tThoát hệ thống.\n";
            pressEnterToContinue();
            break;
        default:
            cout << "\t\tLựa chọn không hợp lệ.\n";
            pressEnterToContinue();
        }
    } while (choice != 9);

    return 0;
}
