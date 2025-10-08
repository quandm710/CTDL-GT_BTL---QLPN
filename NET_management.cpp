#include <iostream>
#include <string>
#include <iomanip> // map
#include <algorithm> //sort
#include <vector> //tạo vector động cho dsPC và dsHóaĐơn gần nhất
#include <ctime> // time_t và time()
#include <cmath> // ceil() làm tròn lên giờ sử dụng PC
#include <limits> // numeric_limits để dọn dẹp vùng nhớ tạm thời lưu trữ dữ liệu nhập từ bàn phím (\n)

using namespace std;

/*
    --- CẤU TRÚC DỮ LIỆU CƠ BẢN (CLASSES) ---
*/
// 1. Class PC (Máy tính)
class PC {
public:
    int stt; // Số thứ tự / ID
    string trangThai; // ON/OFF/DANG SU DUNG
    string taiKhoanDangDung; // TK của Khách đang sử dụng
    PC(): stt(0), trangThai("OFF"), taiKhoanDangDung("N/A") {}
};
// 2. Class Khach (Dùng trong Linked List và Queue)
class Khach {
public:
    string taiKhoan; // TK
    string matKhau; // MK
    int soGioDaMua; // Số giờ đã nạp
    time_t thoiGianBatDau; // Thời gian bắt đầu sử dụng
    Khach(): soGioDaMua(0), thoiGianBatDau(0) {}
};
// 3. Class Nhân Viên (Dùng trong Linked List)
class NhanVien {
public:
    string hoTen;
    int namSinh;
    string sdt;
};
// 4. Class Hóa Đơn (Dùng trong Stack)
class HoaDon {
public:
    string thoiGian;
    string taiKhoanKhach;
    double tongTien;
    string moTa;
};

/*
    --- TRIỂN KHAI LINKED LIST CHO NHÂN VIÊN VÀ KHÁCH HÀNG ---
*/
// Node chung cho Linked List
template <typename T>
struct Node {
    T data;
    Node* next;
    Node(T val): data(val), next(nullptr) {}
    ~Node() = default;
};
// Lớp quản lý Linked List
template <typename T>
class LinkedList {
private:
    Node<T>* head;
public:
    LinkedList(): head(nullptr) {}
    // Thêm Node vào đầu danh sách
    void add(T data) {
        Node<T>* newNode = new Node<T>(data);
        newNode->next = head;
        head = newNode;
    }
    // Hiển thị tất cả
    void display(const string& title, void (*displayFunc)(const T&)) {
        cout << "\n--- DANH SÁCH " << title << " ---\n";
        if (!head) {
            cout << "Danh sach hien tai dang rong.\n";
            return;
        }
        Node<T>* current = head;
        while (current) {
            displayFunc(current->data);
            current = current->next;
        }
        cout << "-----------------------------------\n";
    }
    Node<T>* getHead() const { return head; }
    // Dọn bộ nhớ
    ~LinkedList() {
        Node<T>* current = head;
        while (current) {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
    }
};
// Hàm hiển thị riêng cho Nhân Viên
void displayNhanVien(const NhanVien& nv) {
    cout << "  - Ho ten: " << nv.hoTen << " | Nam sinh: " << nv.namSinh << " | SDT: " << nv.sdt << endl;
}
// Hàm hiển thị riêng cho Khách
void displayKhach(const Khach& kh) {
    cout << "  - TK: " << kh.taiKhoan << " | MK: " << kh.matKhau
         << " | Gio con lai: " << kh.soGioDaMua << "h" << endl;
}

/*
    --- TRIỂN KHAI STACK CHO HÓA ĐƠN GẦN NHẤT ---
*/
class InvoiceStack {
private:
    Node<HoaDon>* top;
public:
    InvoiceStack(): top(nullptr) {}
    // Thêm Hóa đơn vào Stack (Push - Nhập)
    void push(HoaDon hd) {
        Node<HoaDon>* newNode = new Node<HoaDon>(hd);
        newNode->next = top;
        top = newNode;
        cout << "[LOG] Them hoa don moi vao ngan xep: " << hd.moTa << endl;
    }
    // Lấy Hóa đơn gần nhất ra
    HoaDon pop() {
        if (!top) {
            throw runtime_error("Ngan xep Hoa Don rong!");
        }
        Node<HoaDon>* temp = top;
        HoaDon hd = temp->data;
        top = top->next;
        delete temp;
        cout << "[LOG] Lay ra hoa don gan nhat.\n";
        return hd;
    }
    // Hiển thị các hóa đơn gần nhất (Sử dụng lặp - iteration)
    void displayRecentInvoices() {
        cout << "\n--- 5 HÓA ĐƠN GẦN NHẤT (Sử dụng Stack) ---\n";
        if (!top) {
            cout << "Hien khong co hoa don nao.\n";
            return;
        }
        Node<HoaDon>* current = top;
        int count = 0;
        while (current && count < 5) {
            cout << "  [" << count + 1 << "] TG: " << current->data.thoiGian
                 << " | TK: " << current->data.taiKhoanKhach
                 << " | Tong: " << fixed << setprecision(0) << current->data.tongTien << " VND" << endl;
            current = current->next;
            count++;
        }
        cout << "-------------------------------------------\n";
    }
    // Dọn bộ nhớ
    ~InvoiceStack() {
        Node<HoaDon>* current = top;
        while (current) {
            Node<HoaDon>* next = current->next;
            delete current;
            current = next;
        }
    }
};

/*
    --- TRIỂN KHAI QUEUE CHO HÀNG CHỜ MÁY TÍNH ---
*/
class WaitlistQueue {
private:
    Node<string>* front; // TK Khách hàng
    Node<string>* rear;
public:
    WaitlistQueue(): front(nullptr), rear(nullptr) {}
    // Thêm khách vào hàng đợi (Enqueue)
    void enqueue(string taiKhoan) {
        Node<string>* newNode = new Node<string>(taiKhoan);
        if (!rear) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        cout << "[LOG] Khach hang " << taiKhoan << " da duoc them vao hang cho.\n";
    }
    // Lấy khách đầu tiên ra khỏi hàng đợi (Dequeue)
    string dequeue() {
        if (!front) {
            return ""; // Hàng đợi rỗng
        }
        Node<string>* temp = front;
        string taiKhoan = temp->data;
        front = front->next;
        if (!front) {
            rear = nullptr;
        }
        delete temp;
        cout << "[LOG] Khach hang " << taiKhoan << " da duoc xoa khoi hang cho (duoc xep may).\n";
        return taiKhoan;
    }
    // Hiển thị hàng đợi
    void displayWaitlist() {
        cout << "\n--- DANH SÁCH CHỜ MÁY (Sử dụng Queue) ---\n";
        if (!front) {
            cout << "Hien khong co khach hang nao trong hang cho.\n";
            return;
        }
        Node<string>* current = front;
        int count = 1;
        while (current) {
            cout << "  [" << count << "] TK: " << current->data << endl;
            current = current->next;
            count++;
        }
        cout << "------------------------------------------\n";
    }
    ~WaitlistQueue() {
        Node<string>* current = front;
        while (current) {
            Node<string>* next = current->next;
            delete current;
            current = next;
        }
    }
};

/*
    --- LOGIC CHÍNH CỦA PHẦN MỀM ---
*/
class NetCafeManager {
public:
    LinkedList<NhanVien> dsNhanVien;
    LinkedList<Khach> dsKhach;
    InvoiceStack dsHoaDonGanNhat;
    WaitlistQueue hangChoPC;
    vector<PC> dsPC;
    //Tìm kiếm Khách hàng theo TK
    Node<Khach>* findKhach(const string& tk) {
        Node<Khach>* current = dsKhach.getHead();
        while (current) {
            if (current->data.taiKhoan == tk) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }
    NetCafeManager(int soLuongPC) {
        // Khởi tạo danh sách PC
        for (int i = 1; i <= soLuongPC; ++i) {
            PC newPC;
            newPC.stt = i;
            dsPC.push_back(newPC);
        }
    }

    // --- CHỨC NĂNG 1: QUẢN LÝ NHÂN VIÊN (LINKED LIST) ---
    // Nhập Nhân Viên
    void addNhanVien() {
        NhanVien nv;
        cout << "Nhap Ho ten: ";
        getline(cin, nv.hoTen);
        cout << "Nhap Nam sinh: ";
        if (!(cin >> nv.namSinh)) {
             cin.clear();
             cin.ignore(numeric_limits<streamsize>::max(), '\n');
             cout << "[LOI] Nam sinh khong hop le. Dat mac dinh 0.\n";
             nv.namSinh = 0;
        }
        cout << "Nhap SDT: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, nv.sdt);
        dsNhanVien.add(nv);
        cout << "[THANH CONG] Da them nhan vien: " << nv.hoTen << endl;
    }
    // Tra Cứu Nhân Viên
    void searchNhanVien(const string& ten) {
        Node<NhanVien>* current = dsNhanVien.getHead();
        bool found = false;
        cout << "\n--- KET QUA TRA CUU NHAN VIEN ---\n";
        while (current) {
            // Tìm kiếm một phần
            if (current->data.hoTen.find(ten) != string::npos) {
                displayNhanVien(current->data);
                found = true;
            }
            current = current->next;
        }
        if (!found) {
            cout << "Khong tim thay nhan vien nao co ten/chua ten: " << ten << endl;
        }
        cout << "-----------------------------------\n";
    }
    // Sắp xếp Nhân Viên theo Tên
    void sortNhanVien() {
        vector<NhanVien> temp;
        Node<NhanVien>* current = dsNhanVien.getHead();
        while (current) {
            temp.push_back(current->data);
            current = current->next;
        }
        sort(temp.begin(), temp.end(), [](const NhanVien& a, const NhanVien& b) {
            return a.hoTen < b.hoTen;
        });
        cout << "\n--- DANH SÁCH NHÂN VIÊN SAU KHI SẮP XẾP THEO TÊN ---\n";
        for (const auto& nv: temp) {
            displayNhanVien(nv);
        }
        cout << "--------------------------------------------------------\n";
    }

    // --- CHỨC NĂNG 2: QUẢN LÝ KHÁCH HÀNG (LINKED LIST & STACK/QUEUE) ---
    // Nhập (Add) Khách
    void addKhach() {
        Khach kh;
        cout << "Nhap Tai khoan (TK): ";
        getline(cin, kh.taiKhoan);
        if (findKhach(kh.taiKhoan)) {
            cout << "[LOI] Tai khoan da ton tai. Vui long chon chuc nang chinh sua neu muon nap them gio.\n";
            return;
        }
        cout << "Nhap Mat khau (MK): ";
        getline(cin, kh.matKhau);
        cout << "Nhap So gio muon nap ban dau: ";
        if (!(cin >> kh.soGioDaMua)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[LOI] Dau vao gio khong hop le. Dat mac dinh 0 gio.\n";
            kh.soGioDaMua = 0;
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        dsKhach.add(kh);
        // Tạo hóa đơn khi tạo tài khoản
        HoaDon hd;
        time_t now = time(0);
        hd.thoiGian = ctime(&now);
        if (!hd.thoiGian.empty() && hd.thoiGian.back() == '\n') {
            hd.thoiGian.pop_back();
        }
        hd.taiKhoanKhach = kh.taiKhoan;
        hd.tongTien = kh.soGioDaMua * 10000; // Giả sử 10K VND/giờ
        hd.moTa = "Nap " + to_string(kh.soGioDaMua) + " gio ban dau";
        dsHoaDonGanNhat.push(hd);
        cout << "[THANH CONG] Da tao tai khoan " << kh.taiKhoan << " va nap " << kh.soGioDaMua << " gio.\n";
    }
    // Chỉnh sửa - Nạp thêm giờ
    void editKhach() {
        string tk;
        int gioNap;
        cout << "Nhap Tai khoan can nap them gio: ";
        getline(cin, tk);
        Node<Khach>* node = findKhach(tk);
        if (node) {
            cout << "TK hien tai co " << node->data.soGioDaMua << " gio. Nhap so gio muon nap them: ";
            if (!(cin >> gioNap)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "[LOI] Dau vao gio khong hop le. Thao tac huy.\n";
                return;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            node->data.soGioDaMua += gioNap;
            // Tạo hóa đơn nạp giờ
            HoaDon hd;
            time_t now = time(0);
            hd.thoiGian = ctime(&now);
            if (!hd.thoiGian.empty() && hd.thoiGian.back() == '\n') {
                hd.thoiGian.pop_back();
            }
            hd.taiKhoanKhach = tk;
            hd.tongTien = gioNap * 5000;
            hd.moTa = "Nap them " + to_string(gioNap) + " gio";
            dsHoaDonGanNhat.push(hd);
            cout << "[THANH CONG] Da nap them " << gioNap << " gio cho TK " << tk
                 << ". Tong gio hien tai: " << node->data.soGioDaMua << "h." << endl;
        } else {
            cout << "[LOI] Khong tim thay tai khoan " << tk << endl;
        }
    }
    // Kiểm tra số giờ còn lại
    void checkRemainingHours() {
        string tk;
        cout << "Nhap Tai khoan can kiem tra: ";
        getline(cin, tk);
        Node<Khach>* node = findKhach(tk);
        if (node) {
            cout << "\n--- THONG TIN GIO CON LAI ---\n";
            cout << "Tai khoan: " << tk << endl;
            cout << "So gio con lai: " << node->data.soGioDaMua << "h" << endl;
            cout << "--------------------------------\n";
        } else {
            cout << "[LOI] Khong tim thay tai khoan " << tk << endl;
        }
    }
    // Tra cứu Khách hàng
    void searchKhach(const string& tk) {
        Node<Khach>* node = findKhach(tk);
        cout << "\n--- KET QUA TRA CUU KHÁCH HÀNG (CHÍNH XÁC) ---\n";
        if (node) {
            displayKhach(node->data);
        } else {
            cout << "Khong tim thay tai khoan " << tk << endl;
        }
        cout << "--------------------------------------\n";
    }

    // --- CHỨC NĂNG 3: QUẢN LÝ PC (ARRAY/VECTOR & QUEUE) ---
    // Xuất trạng thái PC
    void displayPCStatus() {
        cout << "\n--- TRẠNG THÁI MÁY TÍNH (PC) ---\n";
        for (const auto& pc: dsPC) {
            // Căn chỉnh giao diện
            cout << "  [PC " << setw(2) << pc.stt << "] | Trang thai: "
                 << setw(15) << left << pc.trangThai
                 << " | Khach: " << pc.taiKhoanDangDung << endl;
        }
        cout << "-----------------------------------\n";
    }
    // Khách đăng nhập (Bắt đầu sử dụng)
    void loginKhach(const string& tk) {
        Node<Khach>* node = findKhach(tk);
        if (!node) {
            cout << "[LOI] Tai khoan khong ton tai.\n";
            return;
        }
        if (node->data.soGioDaMua <= 0) {
            cout << "[LOI] Tai khoan khong du gio de su dung. Vui long nap them gio.\n";
            return;
        }
        //---BƯỚC KIỂM TRA ĐĂNG NHẬP TRÙNG LẶP (1 TK chỉ được dùng 1 PC)---
        for (const auto& pc: dsPC) {
            if (pc.trangThai == "DANG SU DUNG" && pc.taiKhoanDangDung == tk) {
                cout << "[LOI] Tai khoan " << tk << " hien dang su dung PC " << pc.stt << ". Khong the dang nhap vao nhieu PC cung luc.\n";
                return;
            }
        }
        //---KẾT THÚC KIỂM TRA TRÙNG LẶP---
        // Tìm PC trống
        for (auto& pc: dsPC) {
            if (pc.trangThai == "OFF" || pc.trangThai == "ON") {
                pc.trangThai = "DANG SU DUNG";
                pc.taiKhoanDangDung = tk;
                node->data.thoiGianBatDau = time(0);
                cout << "[THANH CONG] TK " << tk << " dang nhap vao PC so " << pc.stt << endl;
                return;
            }
        }
        // Nếu không có PC trống, thêm vào hàng chờ (Queue)
        hangChoPC.enqueue(tk);
        cout << "[THONG BAO] Tat ca PC deu ban. Khach hang " << tk << " da duoc them vao hang cho.\n";
    }
    // Khách đăng xuất (Kết thúc sử dụng)
    void logoutKhach(int sttPC) {
        if (sttPC < 1 || sttPC > dsPC.size()) {
            cout << "[LOI] STT PC khong hop le.\n";
            return;
        }
        PC& pc = dsPC[sttPC - 1];
        if (pc.trangThai != "DANG SU DUNG") {
            cout << "[LOI] PC " << sttPC << " hien khong co khach su dung.\n";
            return;
        }
        Node<Khach>* node = findKhach(pc.taiKhoanDangDung);
        if (!node) {
            cout << "[LOI HE THONG] Khong tim thay thong tin khach hang.\n";
            return;
        }
        // Tính toán thời gian sử dụng
        time_t thoiGianKetThuc = time(0);
        double giaySuDung = difftime(thoiGianKetThuc, node->data.thoiGianBatDau);
        // Làm tròn lên giờ (Ví dụ: 1 giờ 1 phút vẫn tính là 2 giờ)
        int gioSuDung = static_cast<int>(ceil(giaySuDung / 3600.0));
        if (gioSuDung > node->data.soGioDaMua) {
            gioSuDung = node->data.soGioDaMua; // Đảm bảo không trừ quá số giờ đã nạp
        }
        node->data.soGioDaMua -= gioSuDung;
        // Tạo Hóa đơn
        HoaDon hd;
        hd.thoiGian = ctime(&thoiGianKetThuc);
        if (!hd.thoiGian.empty() && hd.thoiGian.back() == '\n') {
            hd.thoiGian.pop_back();
        }
        hd.taiKhoanKhach = pc.taiKhoanDangDung;
        hd.tongTien = gioSuDung * 10000; // Giá tiền: 10K VND/giờ
        hd.moTa = "Su dung PC " + to_string(sttPC) + " het " + to_string(gioSuDung) + " gio";
        dsHoaDonGanNhat.push(hd);
        string tkVuaKetThuc = pc.taiKhoanDangDung;
        pc.trangThai = "ON";
        pc.taiKhoanDangDung = "N/A";
        node->data.thoiGianBatDau = 0;
        cout << "[THANH CONG] TK " << tkVuaKetThuc << " da dang xuat khoi PC " << sttPC
             << ". Su dung " << gioSuDung << " gio. Gio con lai: " << node->data.soGioDaMua << "h.\n";
        // Kiểm tra hàng chờ (Queue)
        string nextCustomer = hangChoPC.dequeue();
        if (!nextCustomer.empty()) {
            cout << "\n[THONG BAO] Xep may cho khach hang trong hang cho...\n";
            Node<Khach>* nextNode = findKhach(nextCustomer);
            if(nextNode && nextNode->data.soGioDaMua > 0) {
                loginKhach(nextCustomer);
            }
            else {
                cout << "\n[THONG BAO] Khach hang " << nextCustomer << " khong du gio choi. Vui long nap them gio!";
            }
        }
    }

    // --- CHỨC NĂNG 4: QUẢN LÝ HÓA ĐƠN (STACK) ---
    void displayRecentInvoices() {
        dsHoaDonGanNhat.displayRecentInvoices();
    }
};

/*
    --- MENU VÀ HÀM MAIN ---
*/
//Hiển thị MENU CHÍNH
void displayMainMenu() {
    cout << "\n===== PHẦN MỀM QUẢN LÝ PHÒNG NET =====";
    cout << "\n1. Quan ly Khach hang (Linked List)";
    cout << "\n2. Quan ly Nhan vien (Linked List)";
    cout << "\n3. Quan ly PC va Giao dich (Queue & Array)";
    cout << "\n4. Xem Hoa don gan nhat (Stack)";
    cout << "\n0. Thoat chuong trinh";
    cout << "\nNhap lua chon: ";
}
//Hiển thị MENU Khách hàng
void displayCustomerMenu() {
    cout << "\n--- QUAN LY KHACH HANG ---";
    cout << "\n1. Nhap (Tao moi) tai khoan";
    cout << "\n2. Xuat (Hien thi) tat ca tai khoan";
    cout << "\n3. Chinh sua (Nap them gio)";
    cout << "\n4. Tra cuu tai khoan";
    cout << "\n5. Kiem tra so gio con lai";
    cout << "\n0. Quay lai Menu chinh";
    cout << "\nNhap lua chon: ";
}
//Hiển thị MENU Nhân viên
void displayStaffMenu() {
    cout << "\n--- QUAN LY NHAN VIEN ---";
    cout << "\n1. Nhap (Them moi) nhan vien";
    cout << "\n2. Xuat (Hien thi) tat ca nhan vien";
    cout << "\n3. Tra cuu nhan vien theo ten";
    cout << "\n4. Sap xep nhan vien theo ten";
    cout << "\n0. Quay lai Menu chinh";
    cout << "\nNhap lua chon: ";
}
//Hiển thị MENU PC và Giao dịch
void displayPCMenu() {
    cout << "\n--- QUAN LY PC VA GIAO DICH ---";
    cout << "\n1. Xuat (Hien thi) trang thai PC";
    cout << "\n2. Khach dang nhap (Bat dau su dung)";
    cout << "\n3. Khach dang xuat (Ket thuc su dung)";
    cout << "\n4. Xem danh sach cho may (Queue)";
    cout << "\n0. Quay lai Menu chinh";
    cout << "\nNhap lua chon: ";
}
//Chạy chương trình
void runProgram() {
    int choice;
    int soLuongPC = 10;
    NetCafeManager manager(soLuongPC);

    do {
        displayMainMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }
        switch (choice) {
            case 1: // Quan ly Khach hang
            {
                int khachChoice;
                do {
                    displayCustomerMenu();       
                    if (!(cin >> khachChoice)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        khachChoice = -1;
                    } else {
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }

                    switch (khachChoice) {
                        case 1: manager.addKhach(); break;
                        case 2: manager.dsKhach.display("KHACH HANG", displayKhach); break;
                        case 3: manager.editKhach(); break;
                        case 4: {
                            string tk;
                            cout << "Nhap TK can tra cuu: ";
                            getline(cin, tk);
                            manager.searchKhach(tk);
                            break;
                        }
                        case 5: manager.checkRemainingHours(); break;
                        case 0: break;
                        default: cout << "[LOI] Lua chon khong hop le.\n";
                    }
                } while (khachChoice != 0);
                break;
            }
            case 2: // Quan ly Nhan vien
            {
                int nvChoice;
                do {
                    displayStaffMenu();
                    if (!(cin >> nvChoice)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        nvChoice = -1;
                    } else {
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }

                    switch (nvChoice) {
                        case 1: manager.addNhanVien(); break;
                        case 2: manager.dsNhanVien.display("NHAN VIEN", displayNhanVien); break;
                        case 3: {
                            string ten;
                            cout << "Nhap ten can tra cuu: ";
                            getline(cin, ten);
                            manager.searchNhanVien(ten);
                            break;
                        }
                        case 4: manager.sortNhanVien(); break;
                        case 0: break;
                        default: cout << "[LOI] Lua chon khong hop le.\n";
                    }
                } while (nvChoice != 0);
                break;
            }
            case 3: // Quan ly PC va Giao dich
            {
                int pcChoice;
                do {
                    displayPCMenu();
                    if (!(cin >> pcChoice)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        pcChoice = -1;
                    } else {
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }

                    switch (pcChoice) {
                        case 1: manager.displayPCStatus(); break;
                        case 2: {
                            string tk;
                            cout << "Nhap Tai khoan dang nhap: ";
                            getline(cin, tk);
                            manager.loginKhach(tk);
                            break;
                        }
                        case 3: {
                            int stt;
                            cout << "Nhap STT PC dang xuat: ";
                            if (!(cin >> stt)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "[LOI] STT PC khong hop le.\n";
                                break;
                            }
                            manager.logoutKhach(stt);
                            break;
                        }
                        case 4: manager.hangChoPC.displayWaitlist(); break;
                        case 0: break;
                        default: cout << "[LOI] Lua chon khong hop le.\n";
                    }
                } while (pcChoice != 0);
                break;
            }
            case 4: // Xem Hoa don gan nhat
            {
                manager.displayRecentInvoices();
                break;
            }
            case 0:
                cout << "Ket thuc chuong trinh. Tam biet!\n";
                break;
            default:
                cout << "[LOI] Lua chon khong hop le. Vui long chon lai.\n";
                break;
        }
    } while (choice != 0);
}

/*
    --- Hàm MAIN ---
*/
int main() {
    srand(time(0));
    runProgram();
    return 0;
}
