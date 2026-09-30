#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

// Hàm thực thi của luồng ở CT1:
// 1. Viết 1 hàm gồm 1 vòng lặp tạo liên tục 1 số nguyên không âm (số nguyên 4 byte),
// nếu số được tạo chia hết cho 2021 thì thoát khỏi vòng lặp,
// trái lại thì ghi số nguyên vào file dulieu.dat (file mở ở mode ghi đè nếu file đã tồn tại), khi ghi được thì đóng file.
void PhatSinhVaGhiSo() {
    std::cout << "========================================================\n";
    std::cout << "   [CT1] BAT DAU LUONG SINH SO VA GHI VAO DULIEU.DAT   \n";
    std::cout << "========================================================\n";

    srand(static_cast<unsigned int>(time(NULL)));
    int dem = 0;

    while (true) {
        dem++;
        uint32_t num = 0;

        // Sinh số nguyên không âm (4 byte = uint32_t)
        // Để chương trình chạy minh họa rõ ràng và kết thúc sau một số lần lặp:
        if (dem >= 15 && (rand() % 6 == 0)) {
            // Tạo số chia hết cho 2021
            num = 2021 * (1 + rand() % 50);
        } else {
            // Tạo số ngẫu nhiên không âm
            num = static_cast<uint32_t>(rand() % 100000 + 1);
            if (num % 2021 == 0) {
                num += 1; // tránh chia hết quá sớm ở các lần đầu
            }
        }

        std::cout << "[CT1 - Lan " << dem << "] Sinh so: " << num;

        // Kiểm tra điều kiện: nếu số được tạo chia hết cho 2021 thì thoát khỏi vòng lặp
        if (num % 2021 == 0) {
            std::cout << " -> CHIA HET CHO 2021! Ghi file va thoat vong lap.\n";
            // Ghi số kết thúc vào file để CT2 đọc được và cũng thoát vòng lặp
            std::ofstream outFile("dulieu.dat", std::ios::binary | std::ios::trunc);
            if (outFile.is_open()) {
                outFile.write(reinterpret_cast<const char*>(&num), sizeof(num));
                outFile.close();
            }
            break;
        } else {
            // Trái lại thì ghi số nguyên vào file dulieu.dat (mode ghi đè nếu đã tồn tại)
            std::cout << " -> Ghi vao dulieu.dat roi dong file.\n";
            std::ofstream outFile("dulieu.dat", std::ios::binary | std::ios::trunc);
            if (outFile.is_open()) {
                outFile.write(reinterpret_cast<const char*>(&num), sizeof(num));
                outFile.close(); // Ghi được thì đóng file theo yêu cầu
            } else {
                std::cerr << "[CT1] Loi: Khong the mo file dulieu.dat de ghi!\n";
            }
        }

        // Nghỉ 400ms để CT2 kịp đọc và người dùng quan sát được trên màn hình
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }

    std::cout << "========================================================\n";
    std::cout << "   [CT1] LUONG DA HOAN TAT (GAP SO CHIA HET CHO 2021)   \n";
    std::cout << "========================================================\n";
}

int main() {
    std::cout << "CHUONG TRINH 1 (CT1): SINH SO VA GHI VAO FILE DULIEU.DAT\n\n";

    // 2. Đặt hàm vào 1 luồng, kích hoạt luồng trong hàm main()
    std::thread t1(PhatSinhVaGhiSo);

    // Chờ luồng hoàn thành
    t1.join();

    std::cout << "\nNhan Enter de thoat CT1...";
    std::cin.get();
    return 0;
}
