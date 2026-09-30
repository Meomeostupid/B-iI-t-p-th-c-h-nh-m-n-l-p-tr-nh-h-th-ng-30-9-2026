#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <windows.h>
#include <conio.h>

// Các biến toàn cục dùng chung giữa 2 tác vụ
int max1 = -1; // Số lớn nhất được Task1 tạo ra
int max2 = -1; // Số lớn nhì (thứ hai) được Task1 tạo ra
int lastGenerated = -1; // Số vừa được sinh gần nhất
long long totalCount = 0; // Đếm số lượng số đã sinh

std::mutex dataMtx; // Mutex đồng bộ hóa dữ liệu giữa 2 luồng
std::atomic<bool> isTask1Running(true); // Cờ báo Task1 đang chạy

// ==============================================================
// TASK 1:
// 1. Viết 1 hàm gồm 1 vòng lặp sinh số nguyên ngẫu nhiên cho đến
//    khi gặp số lớn hơn 10000 và chia hết cho 2021 thì dừng vòng lặp.
// 2. Thiết lập Task1 bằng cách đặt hàm trên vào 1 timer chu kỳ 1 mili giây
//    hoặc 1 luồng, kích hoạt trong main().
// ==============================================================
void Task1_SinhSo() {
    std::cout << "[Task1] Khoi dong Task1 (chu ky 1ms) - Bat dau sinh so ngau nhien...\n";
    srand(static_cast<unsigned int>(time(NULL)));

    while (true) {
        totalCount++;

        // Sinh số ngẫu nhiên.
        // Để chương trình chạy minh họa tự nhiên (kéo dài khoảng vài giây để quan sát),
        // ta sinh trong khoảng 1 .. 60000.
        // Sau 1000 lần sinh (khoảng 2-3 giây), tăng xác suất sinh số chia hết 2021 > 10000.
        int num = 0;
        if (totalCount > 1200 && (rand() % 40 == 0)) {
            // Sinh số thỏa điều kiện: > 10000 và chia hết cho 2021
            int k = 5 + (rand() % 20); // k >= 5 => 2021 * 5 = 10105 > 10000
            num = 2021 * k;
        } else {
            num = rand() % 60000 + 1;
        }

        // Cập nhật số lớn nhất và số lớn nhì
        {
            std::lock_guard<std::mutex> lock(dataMtx);
            lastGenerated = num;

            if (max1 == -1) {
                max1 = num;
            } else if (num > max1) {
                max2 = max1; // Số lớn nhất cũ trở thành số lớn nhì
                max1 = num;  // Cập nhật số lớn nhất mới
            } else if (num < max1 && (max2 == -1 || num > max2)) {
                max2 = num;  // Cập nhật số lớn nhì
            }
        }

        // Kiểm tra điều kiện dừng của Task1: số > 10000 và chia hết cho 2021
        if (num > 10000 && (num % 2021 == 0)) {
            std::cout << "\n------------------------------------------------------------\n";
            std::cout << "[Task1] DA TIM THAY SO: " << num 
                      << " (> 10000 va chia het cho 2021) tai lan sinh thu " << totalCount << "!\n";
            std::cout << "[Task1] DUNG TASK1!\n";
            std::cout << "------------------------------------------------------------\n";
            
            isTask1Running = false; // Báo hiệu cho Task2 dừng
            break;
        }

        // Chu kỳ 1 mili giây theo yêu cầu đề bài
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

// ==============================================================
// TASK 2:
// 3. Viết 1 hàm có 1 vòng lặp chỉ dừng lặp khi vòng lặp của Task1 dừng lại.
//    Vòng lặp này luôn hiển thị số lớn nhất và số lớn nhì được Task1 tạo ra
//    tại mọi thời điểm chạy.
// 4. Đặt hàm vào 1 luồng/timer. Thêm lệnh phát ra tiếng beep khi hiển thị.
// ==============================================================
void Task2_HienThiVaBeep() {
    std::cout << "[Task2] Khoi dong Task2 - Luon theo doi va hien thi Max1, Max2...\n";

    // Vòng lặp chỉ dừng khi vòng lặp của Task1 dừng lại
    while (isTask1Running) {
        int m1, m2, cur;
        long long cnt;
        {
            std::lock_guard<std::mutex> lock(dataMtx);
            m1 = max1;
            m2 = max2;
            cur = lastGenerated;
            cnt = totalCount;
        }

        if (m1 != -1) {
            std::cout << "[Task2 - So luong: " << cnt << " | So vua sinh: " << cur << "] "
                      << "-> SO LON NHAT: " << m1 << " | ";
            if (m2 != -1) {
                std::cout << "SO LON NHI: " << m2 << "\n";
            } else {
                std::cout << "SO LON NHI: (Chua co)\n";
            }

            // Thêm lệnh phát ra tiếng beep khi hiển thị
            Beep(900, 25);
        }

        // Nghỉ một khoảng thời gian ngắn để hiển thị vừa mắt và beep không bị nghẽn
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Hiển thị kết quả chốt cuối cùng khi Task1 đã dừng
    int finalM1, finalM2;
    {
        std::lock_guard<std::mutex> lock(dataMtx);
        finalM1 = max1;
        finalM2 = max2;
    }

    std::cout << "\n============================================================\n";
    std::cout << "[Task2 - KET QUA CUOI CUNG]\n";
    std::cout << "   - So lon nhat (Max 1) : " << finalM1 << "\n";
    std::cout << "   - So lon nhi  (Max 2) : " << finalM2 << "\n";
    std::cout << "   - Tong so da sinh     : " << totalCount << "\n";
    std::cout << "[Task2] Task1 da dung -> Task2 ket thuc vong lap!\n";
    std::cout << "============================================================\n";

    // Tiếng beep kết thúc
    Beep(1200, 150);
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "   BAI 04: 2 TAC VU BAT DONG BO (TASK1 & TASK2)\n";
    std::cout << "========================================================\n\n";

    // Kích hoạt Task1 và Task2 trong 2 luồng
    std::thread t1(Task1_SinhSo);
    std::thread t2(Task2_HienThiVaBeep);

    // Chờ 2 luồng kết thúc
    t1.join();
    t2.join();

    std::cout << "\nNhan Enter de thoat chuong trinh...";
    std::cin.get();
    return 0;
}
