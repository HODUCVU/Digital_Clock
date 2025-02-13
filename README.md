# Digital_Clock
This project build a digital clock to manage time and serve as an alarm, it has some functions like alarm clock, time setting, seconds counting, countdown timer, the project is packaged into a finished clock product.. The project is implemented in MSP430G2553 in Code Compose Studio (CSS).

## Chức năng chính
| STT | Chức năng            | Công nghệ         | Phần cứng                  | Mô tả |
|-----|----------------------|-------------------|----------------------------|-------|
| 1   | Hiển thị giờ, ngày, tháng, năm. | GPIO, Timer A  | LCD 1602                   | Hiển thị giờ, phút, giây, ngày, tháng, năm. |
| 2   | Báo thức            | GPIO, interrupt   | Module còi 3.3-5V, LCD 1602, Nút nhấn | Có thể chọn giờ, phút để xác định thời điểm báo thức. Khi đến thời gian đã đặt thì còi sẽ kêu. |
| 3   | Đếm ngược           | Timer A, GPIO, interrupt | LCD 1602, Nút nhấn, Module còi 3.3-5V | Đặt thời gian ban đầu, sau đó đếm lùi về 0. Khi về 0 thì còi sẽ kêu. |
| 4   | Thiết lập thời gian | interrupt         | LCD 1602, Nút nhấn         | Tùy chỉnh giờ, phút, ngày, tháng, năm. |

## Thiết kế phần cứng

| STT | Tên linh kiện             | Giới thiệu |
|-----|---------------------------|------------|
| 1   | Màn hình LCD 1602         | Hiển thị thông tin, có khả năng hiển thị 2 dòng với mỗi dòng 16 ký tự. |
| 3   | MODULE CÒI 3.3V - 5V      | Được sử dụng để phát ra âm thanh khi kích tín hiệu, được ứng dụng trong báo thức. |
| 4   | MODULE HẠ ÁP AMS1117      | Mạch hạ áp DC-DC với dải điện áp đầu vào rộng cho điện áp đầu ra ổn định ở mức 3.3V. |
| 5   | MSP430G2553               | Bo mạch chính |
| 6   | Nút nhấn                  | Điều khiển đồng hồ |
| 7   | Adapter 5V 1A             | Cấp nguồn cho đồng hồ |

![image](https://github.com/user-attachments/assets/18db5a2f-d17e-494f-ad3e-5f8ab4342fac)

## Lưu đồ thuật toán
![image](https://github.com/user-attachments/assets/da211f3d-374e-4221-82f2-d18fb20a4cf4)

![image](https://github.com/user-attachments/assets/5d9028d2-5625-4e5d-b3bf-aa532c8a64cb)

![image](https://github.com/user-attachments/assets/16abaff6-32bb-4be3-9cfe-35be66674f87)

![image](https://github.com/user-attachments/assets/50de2ad9-1e08-4264-b6f7-f803f1e9734c)

![image](https://github.com/user-attachments/assets/ffc50381-1cf1-4a00-a96f-606a8628ff17)
