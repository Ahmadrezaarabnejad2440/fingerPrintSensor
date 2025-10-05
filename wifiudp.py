import socket
import threading
1

# آدرس و پورت آدرس آردوینو
arduino_ip = "192.168.4.1"
arduino_port = 8888

# ساخت کلاینت UDP برای ارسال داده‌ها به آردوینو
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def receive_responses():
    while True:
        try:
            # دریافت پاسخ از آردوینو
            response, address = client.recvfrom(1024)
            print("پاسخ دریافت شده: ", response.decode())
        except Exception as e:
            print("خطا در دریافت پاسخ: ", e)

# ایجاد نخ برای دریافت پاسخ‌ها
response_thread = threading.Thread(target=receive_responses)
response_thread.daemon = True
response_thread.start()

while True:
    # دریافت داده از کاربر
    data = input("    :     ")

    # ارسال رشته به آردوینو
    client.sendto(data.encode(), (arduino_ip, arduino_port))
