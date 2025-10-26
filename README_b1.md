CÁC BƯỚC THỰC HIỆN:
1. Cấu hình các chân :
   - Dùng chân PA1, PA2 cho led
   - PA0 cho nút nhấn
   - Ngắt (EXTI & NVIC): Cấu hình hệ thống ngắt EXTI0 để "theo dõi" chân PA0. Nó được lệnh phải kích hoạt khi có "cạnh xuống" (EXTI_Trigger_Falling), tức là ngay khoảnh khắc bạn nhấn nút.

<img width="1013" height="478" alt="image" src="https://github.com/user-attachments/assets/1a300160-99b0-4460-adbc-f82cb065b02d" />

<img width="734" height="413" alt="image" src="https://github.com/user-attachments/assets/00999f35-2142-490c-84f5-21ddbd1f0632" />

2. Cài đặt các task:
   - Cài đặt Task 2 có mức độ ưu tiên cao hơn nhưng ban đầu ở trạng thái block
   - Cài đặt Task 1 có mức độ ưu tiên thấp hơn nhưng ở trạng thái nháy led mỗi 0.5s
  
   <img width="825" height="531" alt="image" src="https://github.com/user-attachments/assets/fed542ef-bdc4-4af6-baee-7c0b85489625" />

  
  <img width="838" height="101" alt="image" src="https://github.com/user-attachments/assets/f830a259-4903-436e-b6e6-b74f63bec87d" />

<img width="1207" height="243" alt="image" src="https://github.com/user-attachments/assets/4a712540-7e20-4ed2-a799-d15aea46f3e2" />

3. Hàm ngắt
   - Khi nhấn nút dừng task 1 lại
  
<img width="920" height="357" alt="image" src="https://github.com/user-attachments/assets/cb7c407e-b067-45c8-910e-850cb5914e7a" />

4. Khi hàm ngắt kết thúc dừng task 1. Sau khi tắt led 2 thì Task1 lại hoạt động lại bình thường

<img width="943" height="531" alt="image" src="https://github.com/user-attachments/assets/e63ee16a-20cf-4072-b5ad-a8e74c81f85c" />

5. Chương trình chính:

<img width="903" height="512" alt="image" src="https://github.com/user-attachments/assets/a5e3c75f-2fb1-463d-ad3d-230f5d77126f" />

6. Kết quả:
   - Bình thường led 1 sẽ nháy liên tục 0,5s mỗi lần
   - Khi nhấn nút nhấn led 2 sẽ sáng 2s và led 1 sẽ dừng lại
   - Hết 2s led2 tắt và led 1 lại nháy tiếp 0,5s mỗi lần

