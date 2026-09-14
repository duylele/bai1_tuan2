cat << 'EOF' > README.md
# Bài tập 1 - Tuần 2: Lập trình Bare-metal UART STM32F103C8T6

## 1. Mô tả dự án
Chương trình triển khai giao tiếp UART1 trên vi điều khiển STM32F103C8T6 (môi trường Linux, không dùng KeilC):
- Cấu hình UART1 (TX: PA9, RX: PA10) ở chế độ cả TX và RX.
- Nhận dữ liệu truyền từ PC qua mạch USB-to-UART và lưu vào bộ đệm buffer.
- Khi nhận ký tự đánh dấu kết thúc là `!`, gửi ngược lại PC bản tin định dạng:
  `<Mã lớp><Mã nhóm>: <Bản tin đã nhận từ PC>\n\r`
## 2. Phần cứng sử dụng
- Bo mạch STM32F103C8T6 (Blue Pill)
- Mạch nạp ST-Link V2
- Mạch chuyển đổi USB-to-UART PL2303/CP2102
## 3. Biên dịch và nap
