#include <iostream>
#include <iomanip>
#include <thread>
#include <atomic>
#include <chrono>
#include <windows.h>
#include <conio.h>

// 1. Biến tổng thể c
char c = '\0';

// Cờ điều khiển trạng thái chạy của 2 Timer
std::atomic<bool> isRunning(true);

// ==============================================================
// TIMER 1:
// 1. Viết 1 hàm gồm 1 vòng lặp nhập 1 kí tự từ bàn phím, lưu vào 1
//    biến tổng thể c, hiển thị dạng hexa của kí tự. Nếu kí tự là '*'
//    thì thoát khỏi vòng lặp.
// 2. Đặt hàm vào 1 timer (Timer1) cứ 15ms thực hiện 1 lần.
// ==============================================================
void Timer1_Func() {
    std::cout << "[Timer1] Khoi dong Timer1 (Chu ky 15ms) - San sang nhan ky tu...\n";

    while (isRunning) {
        // Kiểm tra xem có phím nào được bấm trên bàn phím không (_kbhit)
        // để không làm block luồng Timer
        if (_kbhit()) {
            c = static_cast<char>(_getch());

            // Hiển thị ký tự và dạng Hexa của nó
            std::cout << "\n[Timer1 - 15ms] Ky tu nhap: '" << c << "' | Dang Hexa: 0x"
                      << std::uppercase << std::hex << std::setw(2) << std::setfill('0')
                      << (static_cast<int>(static_cast<unsigned char>(c))) 
                      << std::dec << "\n";

            // Nếu kí tự là '*' thì thoát khỏi vòng lặp
            if (c == '*') {
                std::cout << "[Timer1] Phat hien ky tu '*'. Dung Timer1!\n";
                isRunning = false;
                break;
            }
        }

        // Timer1 cứ 15ms thực hiện 1 lần
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}

// ==============================================================
// TIMER 2:
// 3. Viết 1 hàm có 1 vòng lặp phát ra tiếng beep, nếu biến tổng thể c
//    là '*' thì thoát vòng lặp.
// 4. Đặt hàm vào 1 timer (Timer2) cứ 7ms thực hiện 1 lần.
// ==============================================================
void Timer2_Func() {
    std::cout << "[Timer2] Khoi dong Timer2 (Chu ky 7ms) - Phat tieng Beep...\n";

    int tickCount = 0;
    while (isRunning) {
        // Nếu biến tổng thể c là '*' thì thoát vòng lặp
        if (c == '*') {
            std::cout << "[Timer2] Nhan tin hieu c = '*'. Dung Timer2!\n";
            break;
        }

        // Phát ra tiếng beep (sử dụng MessageBeep không chặn hoặc Beep ngắn tần số 850Hz)
        // Cứ mỗi một số tick phát âm thanh để âm thanh mượt mà, không bị xung đột loa
        tickCount++;
        if (tickCount % 50 == 0) { // Phát tiếng beep đều đặn
            MessageBeep(MB_OK);
            // Hoặc có thể dùng: Beep(800, 10);
        }

        // Timer2 cứ 7ms thực hiện 1 lần
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
    }
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "   BAI 03: 2 TIMER (TIMER1: 15ms, TIMER2: 7ms)\n";
    std::cout << "   Huong dan: Nhap cac phim tren ban phim de xem ma Hex\n";
    std::cout << "              Nhap '*' de thoat ca 2 Timer\n";
    std::cout << "========================================================\n\n";

    // Kích hoạt Timer1 (chu kỳ 15ms) và Timer2 (chu kỳ 7ms) trong hàm main()
    std::thread t1(Timer1_Func);
    std::thread t2(Timer2_Func);

    // Chờ 2 timer hoàn thành
    t1.join();
    t2.join();

    std::cout << "\n[Main] Ca 2 Timer da ket thuc. Nhan phim bat ky de thoat...";
    _getch();
    return 0;
}
