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

// Lớp Boat
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

    virtual void display() const = 0;

    char getType() const { return type; }
    void setType(char t) { type = t; }

   virtual void createBoat() {    // hàm tạo thuyền
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
            cout << "\t\tNhap so ca lam viec: ";
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
    }

    void setAvailability(bool available) { isAvailable = available; }       // hàm đặt giá trị hoạt động 
    bool getAvailability() const { return isAvailable; }                    // hàm trả về giá trị hoạt động
    float getSeats() const { return seats; }                                // hàm trả về giá trị số ghế của thuyền
    string getBoatName() const { return boatName; }                         // hàm trả về tên thuyền
    string getBoatID() const { return boatID; }                             // hàm trẻ về ID thuyền
    int getRentalCount() const { return rentalCount; }                      // hàm trả về số lần đã thuê
    void incrementRentalCount() { rentalCount++; }                          // hàm tăng số lần thuê
    const vector<Shift>& getShifts() const { return shifts; }               // hàm trả về mảng các thông số của các ca trực của thuyền
    int getNumShifts() const { return numShifts; }                          // hàm trả về số ca trực 

    void bookSeats(int shiftIndex, float seatsBooked) {        // hàm đặt ghế
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

    void saveToFile(ofstream& outFile) const {        // hàm lưu vào file
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

    void loadFromFile(ifstream& inFile) {            // hàm tải giá trị từ file lên 
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
void createBoat() override {                // đa hình cho hàm tạo thuyền, phân biệt giữa Small và Large Boat, tương tự đa hình cho display
    system("cls");
    cout << "\n\t\t======================================";
    cout << "\n\t\t           TAO THUYEN NHO    ";
    cout << "\n\t\t======================================\n";
    
    do{
        cout << "\t\tNhap ID thuyen ( Vi du: B001): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, boatID);
        if (boatID.empty() || boatID.length() > 10) {
            cout << "ID cua thuyen khong duoc de trong va phai nho hon 10 ky tu. \n";
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
        cout << "\t\tNhap trong luong cua thuyen (tan, toi da 50): ";
        cin >> input;
        if (!isValidNumber(input) || stof(input) <= 0 || stof(input) >= 50) {
            cout << "\t\tLoi. Trong luong cua thuyen phai la so duong va nho hon 50 tan.\n";
        }
        else {
            weight = stof(input);
            break;
        }
    } while (true);

    do {
        cout << "\t\tNhap so ghe (toi da 100): ";
        cin >> input;
        if (!isValidNumber(input) || stof(input) <= 0 || stof(input) >= 100) {
            cout << "\t\tLoi. So ghe phai la so duong va nho hon 100.\n";
        }
        else {
            seats = stof(input);
            break;
        }
    } while (true);

    do {
        cout << "\t\tNhap so ca lam viec: ";
        cin >> input;
        if (!isValidNumber(input) || stoi(input) <= 0) {
            cout << "\t\tLoi. So ca lam viec phai la so duong.\n";
        }
        else if (stoi(input) >= 12) {
            cout << "\t\tLoi: So ca phai nho hon 12.\n";
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
    cout << "\n\t\tThuyen nho duoc them thanh cong.\n";
}
    void display() const override {            // áp dụng đa hình (ghi đè) cho 2 loại Small và Large
        cout << "\t\tID Thuyen: " << boatID <<"\n";
        cout << "\t\tTen: " << boatName << "\n";
        cout << "\t\tLoai: Nho\n";
        cout << "\t\tKhoi luong: " << weight << " tan\n";
        cout << "\t\tSo ghe: " << seats << "\n";
        cout << "\t\tTrang thai: " << (isAvailable ? "Con trong" : "Het cho") << "\n";
        cout << "\t\tSo ca trong ngay: " << numShifts << "\n";
        for (int i = 0; i < numShifts; i++) {
            cout << "\t\tCa " << i + 1 << ": " << shifts[i].startHour << "h-"
                << shifts[i].endHour << "h, Gia: $" << shifts[i].pricePerHour
                << "/gio, so ghe da dat: " << shifts[i].seatsBooked << "\n";
        }
        cout << "\t\tSo lan da thue: " << rentalCount << "\n";
    }
};

// Lớp con LargeBoat
class LargeBoat : public Boat {
public:
    LargeBoat() { setType('L'); }
 void createBoat() override {
     system("cls");
     cout << "\n\t\t======================================";
     cout << "\n\t\t           TAO THUYEN LON    ";
     cout << "\n\t\t======================================\n";

     do {
         cout << "\t\tNhap ID thuyen ( Vi du: B001): ";
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         getline(cin, boatID);
         if (boatID.empty() || boatID.length() > 10) {
             cout << "ID cua thuyen khong duoc de trong va phai nho hon 10 ky tu. \n";
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
         cout << "\t\tNhap trong luong cua thuyen (tan, toi thieu 50): ";
         cin >> input;
         if (!isValidNumber(input) || stof(input) <= 50) {
             cout << "\t\tLoi. Trong luong cua thuyen phai la so duong va lon hon 50 tan.\n";
         }
         else {
             weight = stof(input);
             break;
         }
     } while (true);

     do {
         cout << "\t\tNhap so ghe (toi thieu 100): ";
         cin >> input;
         if (!isValidNumber(input) || stof(input) <= 100) {
             cout << "\t\tLoi. So ghe phai la so duong va lon 100.\n";
         }
         else {
             seats = stof(input);
             break;
         }
     } while (true);

     do {
         cout << "\t\tNhap so ca lam viec: ";
         cin >> input;
         if (!isValidNumber(input) || stoi(input) <= 0) {
             cout << "\t\tLoi. So ca lam viec phai la so duong.\n";
         }
         else if (stoi(input) >= 12) {
             cout << "\t\tLoi: So ca phai nho hon 12.\n";
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
     cout << "\n\t\tThuyen lon duoc them thanh cong.\n";
 }
    void display() const override {
        cout << "\t\tID Thuyen: " << boatID << "\n";
        cout << "\t\tTen: " << boatName << "\n";
        cout << "\t\tLoai: Lon\n";
        cout << "\t\tKhoi luong: " << weight << " tan\n";
        cout << "\t\tSo ghe: " << seats << "\n";
        cout << "\t\tTrang thai: " << (isAvailable ? "Con trong" : "Het cho") << "\n";
        cout << "\t\tSố ca trong ngay: " << numShifts << "\n";
        for (int i = 0; i < numShifts; i++) {
            cout << "\t\tCa " << i + 1 << ": " << shifts[i].startHour << "h-"
                << shifts[i].endHour << "h, Gia: $" << shifts[i].pricePerHour
                << "/gio, so ghe da dat: " << shifts[i].seatsBooked << "\n";
        }
        cout << "\t\tSo lan thue: " << rentalCount << "\n";
    }
};

// Các hàm hỗ trợ
void displayHeader() {        // Hàm hiển thị tên chương trình
    cout << "\n\t\t======================================";
    cout << "\n\t\t  He Thong Quan Ly Tau THuyen Tren Song Han  ";
    cout << "\n\t\t======================================\n";
}

bool isValidBoatType(char type) {        //hàm kiểm tra tính đúng sai của biến Type
    return type == 'S' || type == 'L';
}

void pressEnterToContinue() {            // delay cho tới khi người dùng enter
    cout << "\n\t\tNhan Enter de tiep tuc...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
void pressEnterToContinue2() {           // cùng tác dụng nhưng dùng cho các hàm bị lỗi phải nhấn enter 2 lần
    cout << "\n\t\tNhan Enter de tiep tuc...";
    cin.get();
}

void displayMenu() {                      // hàm hiển thị menu
    system("cls");
    displayHeader();
    cout << "\n\t\t1. Them thuyen moi";
    cout << "\n\t\t2. Xoa thuyen";
    cout << "\n\t\t3. Hien thi tat ca cac tau";
    cout << "\n\t\t4. Thue ghe";
    cout << "\n\t\t5. Huy ghe da thue";
    cout << "\n\t\t6. Hien thi thuyen con trong";
    cout << "\n\t\t7. Hien thi lich su da thue";
    cout << "\n\t\t8. Hien thi so thuyen duoc thue nhieu nhat";
    cout << "\n\t\t9. Tong so ghe duoc thue tren mot thuyen";
    cout << "\n\t\t10. Thoat";
    cout << "\n\t\tNhap lua chon (1-10): ";
}

string generateTicketCode(const vector<RentalRecord>& rentalHistory) {        // hàm random tên vé, xem nó như một loại KEY để hủy vé
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
        {"Banh Mi", 5.0},
        {"Coca Cola", 2.0},
        {"Trai cay", 8.0}
    };

public:
    BoatManager() {            // constructor, dùng để tải dữ liệu ban đầu ở file lên
        loadBoatsFromFile();
        loadRentalHistoryFromFile();
    }

    ~BoatManager() {            // destructor, hàm hủy dùng để tránh bị tràn bộ nhớ
        for (Boat* boat : boats) {
            delete boat;
        }
    }

    void addBoat() {            // hàm thêm thuyền,
        system("cls");
        displayHeader();
        char type;
        do {
            cout << "\t\tNhap loai thuyen( S cho size Nho, L cho size Lon): ";
            cin >> type;
            type = toupper(type);
            if (!isValidBoatType(type)) {
                cout << "\t\tLoi. Loai thuyen can nhap phai la S hoac L.\n";
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
        bool status = true;
        for (const Boat* b : boats) {
           
            if (b->getBoatID() == boat->getBoatID()) {
                cout << "\t\tLoi. ID thuyen da ton tai.\n";
                delete boat;
                status = false;
                pressEnterToContinue();
                return;
            }
            else if (b->getBoatName() == boat->getBoatName()) {
                cout << "\t\tLoi. Ten thuyen da ton tai.\n";
                delete boat;
                status = false;
                pressEnterToContinue();
                return;
            }
        }
        if (status) {
            cout << "\n\t\tThuyen duoc them thanh cong.\n";
        }
        boats.push_back(boat);
        saveBoatsToFile();
        pressEnterToContinue();
    }
    void deleteBoat() {
        int choice;
        do {        
            system("cls");
            displayHeader();
            cout << "\n\t\tNhap phuong thuc muon tim thuyen de xoa: ";
            cout << "\n\t\t1. Tim theo ID cua thuyen ";
            cout << "\n\t\t2. Tim theo ten cua thuyen ";
            cout << "\n\t\tNhan chon(1-2): ";
            string input;
            cin >> input;
            if (!isValidNumber(input)) {
                cout << "\t\tLoi. Vui long nhap so.\n";
                pressEnterToContinue();
                continue;
            }
            choice = stoi(input);
            if (choice < 1 || choice > 2) {
                cout << "\t\tLua chon khong hop le. Vui long chon lai.\n";
                pressEnterToContinue();
                continue;
            }

            switch (choice) {
            case 1: {
                string boatID;
                cout << "\n\t\tNhap ID thuyen can xoa: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, boatID);
                Boat* boat = nullptr;
                auto it = boats.begin();
                for (Boat* b : boats) {
                    if (b->getBoatID() == boatID) {
                        boat = b;
                        break;
                    }
                    ++it;
                }
                if (!boat) {
                    pressEnterToContinue2();
                    return;
                }
                // Kiểm tra xem thuyền có bản ghi thuê không
                bool hasRental = false;
                for (const auto& record : rentalHistory) {
                    if (record.boatID == boatID && record.action == "RENT") {
                        hasRental = true;
                        break;
                    }
                }
                if (hasRental) {
                    cout << "\t\tLoi. Thuyen dang co ban ghi thue, khong the xoa.\n";
                    pressEnterToContinue();
                    return;
                }
                boats.erase(it);
                delete boat;
                saveBoatsToFile();
                cout << "\t\tThuyen duoc xoa thanh cong.\n";
                pressEnterToContinue();
                return;
            }
            case 2: {
                string nameboat;
                cout << "\n\t\tNhap ten cua thuyen can xoa: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, nameboat);
                Boat* boat = nullptr;
                auto it = boats.begin();
                for (Boat* b : boats) {
                    if (b->getBoatName() == nameboat) {
                        boat = b;
                        break;
                    }
                    ++it;
                }
                if (!boat) {
                    cout << "\t\tLoi. Khong tim thay ten thuyen.\n";
                    pressEnterToContinue2();
                    return; 
                }
                // Kiểm tra xem thuyền có bản ghi thuê không
                bool hasRental = false;
                for (const auto& record : rentalHistory) {
                    if (record.boatID == boat->getBoatID() && record.action == "RENT") {
                        hasRental = true;
                        break;
                    }
                }
                if (hasRental) {
                    cout << "\t\tLoi. Thuyen dang co ban ghi thue, khong the xoa.\n";
                    pressEnterToContinue();
                    return;
                }
                boats.erase(it);
                delete boat;
                saveBoatsToFile();
                cout << "\t\tThuyen duoc xoa thanh cong.\n";
                pressEnterToContinue();
                return;
            }
            }
        } while (true);
    }

    void displayAllBoats() const {
        system("cls");
        displayHeader();
        if (boats.empty()) {
            cout << "\t\t\Khong ton tai thuyen nao.\n";
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
        cout << "\t\tNhap ID thuyen: ";
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
            cout << "\t\tLoi. Khong tim thay thuyen.\n";
            pressEnterToContinue2();
            return;
        }

        if (!boat->getAvailability()) {
            cout << "\t\tXin loi quy khach. Thuyen da het cho trong.\n";
            pressEnterToContinue();
            return;
        }

        cout << "\t\tCac ca con trong:\n";
        const auto& shifts = boat->getShifts();
        for (int i = 0; i < boat->getNumShifts(); i++) {
            cout << "\t\tCa " << i + 1 << ": " << shifts[i].startHour << "h-"
                << shifts[i].endHour << "h, Gia: $" << shifts[i].pricePerHour
                << "/gio, Ghe con trong: " << boat->getSeats() - shifts[i].seatsBooked << "\n";
        }

        string input;
        int shiftIndex;
        do {
            cout << "\t\tNhap so ca (1-" << boat->getNumShifts() << "): ";
            cin >> input;
            if (!isValidNumber(input) || stoi(input) <= 0 || stoi(input) > boat->getNumShifts()) {
                cout << "\t\tLoi. Ca khong hop le.\n";
            }
            else {
                shiftIndex = stoi(input) - 1;
                break;
            }
        } while (true);

        float seats;
        do {
            cout << "\t\tNhap so ghe can thue: ";
            cin >> input;
            if (!isValidNumber(input) || stof(input) <= 0 ||
                stof(input) > (boat->getSeats() - shifts[shiftIndex].seatsBooked)) {
                cout << "\t\tLoi. So ghe can thue khong hop le.\n";
            }
            else {
                seats = stof(input);
                float rentalCost = seats * shifts[shiftIndex].pricePerHour * (shifts[shiftIndex].endHour - shifts[shiftIndex].startHour);
                cout << "\t\tChi phi thue: $" << rentalCost << "\n";
                break;
            }
        } while (true);

        // Đặt món ăn/uống
        vector<FoodItem> selectedFoodItems;
        float foodCost = 0.0f;
        char orderFood;
        do {
            cout << "\t\tBan co dat do an them khong? (y/n): ";
            cin >> orderFood;
            orderFood = tolower(orderFood);
            if (orderFood == 'y' || orderFood == 'n') break;
            cout << "\t\tLoi. Nhap y de dong y hoac n de tu choi.\n";
        } while (true);

        if (orderFood == 'y') {
            cout << "\t\tMenu:\n";
            for (size_t i = 0; i < foodMenu.size(); i++) {
                cout << "\t\t" << i + 1 << ". " << foodMenu[i].name << " - $" << foodMenu[i].price << "\n";
            }
            cout << "\t\tNhap so mon (1-" << foodMenu.size() << ")hoac 0 de ket thuc: ";
            int foodChoice;
            while (cin >> foodChoice && foodChoice != 0) {
                if (foodChoice < 1 || foodChoice > static_cast<int>(foodMenu.size())) {
                    cout << "\t\tLoi. lua chon khong hop le.\n";
                }
                else {
                    selectedFoodItems.push_back(foodMenu[foodChoice - 1]);
                    foodCost += foodMenu[foodChoice - 1].price;
                    cout << "\t\tDa them " << foodMenu[foodChoice - 1].name << " ($" << foodMenu[foodChoice - 1].price << ")\n";
                }
                cout << "\t\tChon mon tiep theo (1-" << foodMenu.size() << ") hoac khong de ket thuc: ";
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
        cout << "\t\tMua ve thanh cong! Ma ve: " << record.ticketCode << "\n";
        if (foodCost > 0) {
            cout << "\t\tChi phi cho an uong: $" << foodCost << "\n";
        }
        cout << "\t\tTong chi phi: $" << totalCost << "\n";
        pressEnterToContinue();
    }

    void cancelRent() {
        system("cls");
        displayHeader();
        string ticketCode;
        cout << "\t\tNhap ma ve cua ban: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, ticketCode);

        auto it = find_if(rentalHistory.begin(), rentalHistory.end(),
            [&ticketCode](const RentalRecord& r) { return r.ticketCode == ticketCode && r.action == "RENT"; });

        if (it == rentalHistory.end()) {
            cout << "\t\tLoi. Ma ve khong ton tai hoac da bi huy.\n";
            pressEnterToContinue2();
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
            cout << "\t\tLoi. khong tim thay thuyen! \n";
            pressEnterToContinue();
            return;
        }

        RentalRecord cancelRecord = record;
        cancelRecord.action = "CANCEL";
        rentalHistory.push_back(cancelRecord);

        boat->cancelSeats(record.shiftIndex, record.seatsRented);
        saveRentalHistoryToFile();
        saveBoatsToFile();

        cout << "\t\tHuy thue thanh cong.\n";
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
            cout << "\t\tKhong co thuyen nao con trong.\n";
        }
        pressEnterToContinue();
    }

    void displayRentalHistory() const {
        system("cls");
        displayHeader();
        if (rentalHistory.empty()) {
            cout << "\t\tKhon ton tai lich su thue.\n";
        }
        else {
            for (const auto& record : rentalHistory) {
                cout << "\t\tID thuyen: " << record.boatID << "\n";
                cout << "\t\tMa ve: " << record.ticketCode << "\n";
                cout << "\t\tSo ghe: " << record.seatsRented << "\n";
                cout << "\t\tCa: " << record.shiftIndex + 1 << "\n";
                cout << "\t\tTrang thai cua ve: " << record.action << "\n";
                if (!record.foodItems.empty()) {
                    cout << "\t\tMon an uong: ";
                    for (const auto& item : record.foodItems) {
                        cout << item.name << " ($" << item.price << "), ";
                    }
                    cout << "\n\t\tChi phi cho mon an: $" << record.foodCost << "\n";
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
            cout << "\t\tKhong ton tai thuyen.\n";
        }
        else {
            auto maxBoat = max_element(boats.begin(), boats.end(),
                [](const Boat* a, const Boat* b) { return a->getRentalCount() < b->getRentalCount(); });
            cout << "\t\tThuyen duoc thue nhieu nhat:\n";
            (*maxBoat)->display();
        }
        pressEnterToContinue();
    }

    void showSeatsPerShift() const {
        system("cls");
        displayHeader();
        string boatID;
        cout << "\t\tNhap ID thuyen: ";
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
            cout << "\t\tLoi. Khong tim thay thuyen.\n";
            pressEnterToContinue2();
            return;
        }

        cout << "\t\tSo ghe da thue tren thuyen " << boatID << ":\n";
        const auto& shifts = boat->getShifts();
        for (int i = 0; i < boat->getNumShifts(); i++) {
            cout << "\t\tCa " << i + 1 << " (" << shifts[i].startHour << "h-"
                << shifts[i].endHour << "h): " << shifts[i].seatsBooked << " ghe\n";
        }
        pressEnterToContinue();
    }

    void saveBoatsToFile() const {
        ofstream outFile("boats.txt");
        if (!outFile) {
            cout << "\t\tLoi: Khong the mo  file boats.txt.\n";
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
            cout << "\t\tKhong the mo file boats.txt.\n";
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
                cout << "\t\tLoi: Loai thuyen khong hop le trong file: " << type << "\n";
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
            cout << "\t\tLoi: Khong the mo rental_history.txt.\n";
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
            cout << "\t\tLoi.Vui long nhap so.\n";
            pressEnterToContinue();
            continue;
        }
        choice = stoi(input);

        switch (choice) {
        case 1: manager.addBoat();
            break;
        case 2:manager.deleteBoat();
            break;
        case 3: manager.displayAllBoats();
            break;
        case 4: manager.rentSeats();
            break;
        case 5: manager.cancelRent();
            break;
        case 6: manager.showAvailableBoats();
            break;
        case 7: manager.displayRentalHistory();
            break;
        case 8: manager.showMostRentedBoat();
            break;
        case 9: manager.showSeatsPerShift();
            break;
        case 10:
            cout << "\t\tThoat he thong.";
            pressEnterToContinue();
            break;
        default:
            cout << "\t\tLua chon khong hop le.\n";
            pressEnterToContinue();
        }
    } while (choice != 10);

    return 0;
}
