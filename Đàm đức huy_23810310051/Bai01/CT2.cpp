#include <iostream>
#include <fstream>
#include <cstdint>
#include <thread>
#include <chrono>

// Hàm thực thi của luồng ở CT2:
// 3. Viết 1 hàm có 1 vòng lặp vô hạn đọc số nguyên 4 byte ở file dulieu.dat.
// Đọc được số nguyên thì đóng file. Hiển thị giá trị đọc được.
// Nếu số nguyên đọc được chia hết cho 2021 thì thoát vòng lặp.
void DocSoTuFile() {
    std::cout << "========================================================\n";
    std::cout << "   [CT2] BAT DAU LUONG DOC SO TU FILE DULIEU.DAT      \n";
    std::cout << "========================================================\n";

    uint32_t lastReadValue = 0xFFFFFFFF;
    bool hasReadAtLeastOnce = false;

    // Vòng lặp vô hạn
    while (true) {
        // Mở file dulieu.dat ở chế độ nhị phân
        std::ifstream inFile("dulieu.dat", std::ios::binary);

        if (inFile.is_open()) {
            uint32_t val = 0;
            // Đọc số nguyên 4 byte (sizeof(uint32_t) == 4)
            if (inFile.read(reinterpret_cast<char*>(&val), sizeof(val))) {
                // Đọc được số nguyên thì đóng file theo đúng yêu cầu đề bài
                inFile.close();

                // Chỉ hiển thị khi có giá trị mới được CT1 ghi vào
                if (!hasReadAtLeastOnce || val != lastReadValue) {
                    hasReadAtLeastOnce = true;
                    lastReadValue = val;

                    // Hiển thị giá trị đọc được
                    std::cout << "[CT2] Doc duoc so nguyen: " << val;

                    // Nếu số nguyên đọc được chia hết cho 2021 thì thoát vòng lặp
                    if (val % 2021 == 0) {
                        std::cout << " -> CHIA HET CHO 2021! Thoat khoi vong lap.\n";
                        break;
                    } else {
                        std::cout << " -> Khong chia het cho 2021. Tiep tuc cho doc so moi...\n";
                    }
                }
            } else {
                // File rỗng hoặc chưa ghi xong, đóng lại để thử lại sau
                inFile.close();
            }
        } else {
            // File dulieu.dat chưa tồn tại hoặc đang bị khóa tạm thời
            // Đợi một chút rồi thử lại
        }

        // Nghỉ 150ms để kiểm tra lại
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    std::cout << "========================================================\n";
    std::cout << "   [CT2] LUONG DA HOAN TAT (DOC DUOC SO CHIA HET 2021)  \n";
    std::cout << "========================================================\n";
}

int main() {
    std::cout << "CHUONG TRINH 2 (CT2): DOC SO NGUYEN 4 BYTE TU DULIEU.DAT\n\n";

    // 4. Đặt hàm vào 1 luồng, kích hoạt luồng trong hàm main()
    std::thread t2(DocSoTuFile);

    // Chờ luồng hoàn thành
    t2.join();

    std::cout << "\nNhan Enter de thoat CT2...";
    std::cin.get();
    return 0;
}
