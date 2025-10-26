CÁC BƯỚC THỰC HIỆN:
1. Cài đặt USART:

<img width="1020" height="588" alt="image" src="https://github.com/user-attachments/assets/e0172098-e36d-4ee6-ae77-d3ac6652f296" />

2. Cài đặt các cờ:
   - Chương trình tạo ra một "cờ" tên là uart_busy.
   - Nếu uart_busy = 0 (rảnh), thì một nhiệm vụ sẽ đặt nó = 1 (bận) rồi mới in.
   - Nếu nhiệm vụ khác thấy uart_busy = 1 (bận), nó sẽ không in.
  
<img width="408" height="42" alt="image" src="https://github.com/user-attachments/assets/e3735cf8-c960-4dc5-ace8-35d6e4224e50" />

3. In ra task1 và task2 nếu có xung đột thì in ra xung đột
   - In ra màn hình terminal:"  Hello from Task 1"
   - Với tần suất là 0,5s
   - In ra màn hình terminal:"  Hello from Task 2"
   - In ra với tần suất là 1s
   - Nếu có xung đột in ra:" XUNG DOT UART!"
   - Khi cả 2 sẽ tắt bật liên tục khi in ra sẽ bật cờ bận lên 1 còn bình thường sẽ là không vì vậy khi cả 2 cùng bật lên 1 sẽ bị xung đột

<img width="739" height="647" alt="image" src="https://github.com/user-attachments/assets/a31ccec8-1676-4f44-a4a8-90684d134958" />

4. Xuất ra terminal thông qua usart:

<img width="710" height="259" alt="image" src="https://github.com/user-attachments/assets/8c106607-7dc9-4553-9ad5-3bba0422f96e" />

5. Chương trình chính:

<img width="571" height="230" alt="image" src="https://github.com/user-attachments/assets/1b2a2992-3a0a-47dc-a9e1-ed4f04b99ce2" />

