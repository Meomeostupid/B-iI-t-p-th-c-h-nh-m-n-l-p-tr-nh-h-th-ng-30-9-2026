#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

// Biến tổng thể st theo yêu cầu đề bài
std::string st = "";

// Mutex để đồng bộ truy cập biến st giữa 2 luồng
std::mutex mtx;

// Cờ báo trạng thái hoạt động của chương trình
std::atomic<bool> isRunning(true);

// Hàm tiện ích loại bỏ các dấu cách ở đầu và cuối xâu
std::string Trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// ==========================================
// TASK 1:
// 1. Vòng lặp nhập xâu kí tự st từ bàn phím.
//    Loại bỏ các dấu cách ở đầu và cuối xâu st.
//    Nếu xâu st được nhập và xử lý là "bye" thì thoát khỏi vòng lặp.
// ==========================================
void Task1_Input() {
    std::cout << "[Task1] Luong nhap du lieu da san sang!\n";

    while (isRunning) {
        std::cout << "\n[Task1] Moi ban nhap xau ky tu st (nhap 'bye' de thoat): ";
        std::string rawInput;
        if (!std::getline(std::cin, rawInput)) {
            break;
        }

        // Loại bỏ dấu cách ở đầu và cuối
        std::string processed = Trim(rawInput);

        // Cập nhật biến tổng thể st
        {
            std::lock_guard<std::mutex> lock(mtx);
            st = processed;
        }

        std::cout << "[Task1] -> Da cap nhat bien tong the st = \"" << processed << "\"\n";

        // Nếu xâu st là "bye" thì thoát khỏi vòng lặp
        if (processed == "bye") {
            std::cout << "[Task1] Phat hien chuoi 'bye'. Ket thuc Task1!\n";
            isRunning = false;
            break;
        }
    }
}

// ==========================================
// TASK 2:
// 3. Vòng lặp vô hạn hiển thị biến xâu kí tự st.
//    Nếu st là "bye" thì thoát vòng lặp.
// ==========================================
void Task2_Display() {
    std::cout << "[Task2] Luong hien thi du lieu da san sang!\n";
    std::string lastDisplayed = "";

    while (isRunning) {
        std::string currentVal;
        {
            std::lock_guard<std::mutex> lock(mtx);
            currentVal = st;
        }

        // Nếu st là "bye" thì thoát vòng lặp
        if (currentVal == "bye") {
            std::cout << "\n[Task2] Nhan duoc 'bye' tu bien tong the st. Ket thuc Task2!\n";
            break;
        }

        // Hiển thị biến st khi có giá trị mới (tránh spam console gây giật lag nhập liệu)
        if (!currentVal.empty() && currentVal != lastDisplayed) {
            std::cout << "\n>>> [Task2 - Hien thi] Gia tri bien st hien tai: \"" << currentVal << "\"\n";
            lastDisplayed = currentVal;
        }

        // Nghỉ một khoảng thời gian nhỏ trước lần kiểm tra tiếp theo
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "   BAI 02: 2 TAC VU (TASK1, TASK2) HOAT DONG BAT DONG BO\n";
    std::cout << "========================================================\n";

    // 2. Thiết lập Task1 bằng cách đặt hàm vào 1 luồng, kích hoạt trong main()
    std::thread t1(Task1_Input);

    // 4. Thiết lập Task2 bằng cách đặt hàm vào 1 luồng, kích hoạt trong main()
    std::thread t2(Task2_Display);

    // Chờ 2 luồng hoàn thành công việc
    t1.join();
    t2.join();

    std::cout << "\n[Main] Ca 2 Task da ket thuc thanh cong. Nhan Enter de thoat...";
    std::cin.get();
    return 0;
}
