CÁC BƯỚC THỰC HIỆN:
1. Cài đặt USART:

<img width="1045" height="552" alt="image" src="https://github.com/user-attachments/assets/2905c105-79af-4ccd-8ef7-a789a3d7e5a5" />

2. Thực hiện Task1:
  - Giả sử Task1 (ưu tiên 1) được chạy trước.
  - Lấy Mutex
  - Kiểm tra: Lần đầu lấy Mutex nên nó có sẵn
  - In An Toàn: Task1 chạy lệnh UART1_SendString(msg). Nó in ra "Hello from Task 1".
  - Trả Chìa Khóa: In xong, nó chạy xSemaphoreGive(uartMutex) để Mutex lại.
  - Ngủ: Nó ngủ 1 giây (vTaskDelay(1000)).

<img width="1052" height="207" alt="image" src="https://github.com/user-attachments/assets/6ee831fe-9e24-4cf7-8ece-0678d01d5f87" />

3. Thực hiện Task2:
  - Khi Task1 lấy Mutex thì Task2 sẽ ở trạng thái block
  - Khi Task1 thực hiện xong việc sẽ trả lại Mutex và khi này Task2 sẽ lấy và thực hiện

<img width="939" height="195" alt="image" src="https://github.com/user-attachments/assets/25bc8b9a-667e-46e6-a602-e47ed68478b2" />

4. In qua terminal:

<img width="827" height="223" alt="image" src="https://github.com/user-attachments/assets/4387d85f-53fb-4313-9a2a-3fc8d68bb270" />

5. Chương trình chính:

<img width="711" height="279" alt="image" src="https://github.com/user-attachments/assets/cac6e3e0-3650-4073-9a0b-edc63df15dce" />

6. Kết quả:
  - In lần lượt mà không bị xung đột
