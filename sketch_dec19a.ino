#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6LKp4tRk9"
#define BLYNK_TEMPLATE_NAME "Dieukhienthietbi"
#define BLYNK_AUTH_TOKEN "HgrOhct3jfrio2Dyinq1P7S5yfzWlYV5"
#define SCREEN_WIDTH 16
#define SCREEN_HEIGHT 2
#define I2C_ADDR 0x27

#include <BlynkSimpleEsp8266.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <ESP8266Wifi.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>


// Chân kết nối với màn hình LCD
//#define LCD_SDA D2
//#define LCD_SCL D1

LiquidCrystal_I2C lcd(I2C_ADDR, SCREEN_WIDTH, SCREEN_HEIGHT); 

// Thông tin kết nối với Blynk
char auth[] = "HgrOhct3jfrio2Dyinq1P7S5yfzWlYV5";
char ssid[] = "Nissi";
char pass[] = "0974323674"; 

int D3State,  D4State;
void updateLCD(int pin = -1, int value = -1) {
 // static String lcdText = "D3: OFF  D4: OFF";
    if (pin == 3) {
    D3State = value;
  } else if (pin == 4) {
    D4State = value;
  }

  String lcdText = "LED:" + String(D3State ? "ON" : "OFF") + " FAN:" + String(D4State ? "ON" : "OFF");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(lcdText);
}

BLYNK_WRITE(V3)
{
  int pinValue = param.asInt();
  digitalWrite(D3,pinValue);
  updateLCD(3, pinValue);
  EEPROM.write(0, pinValue); // Lưu trạng thái D3 vào EEPROM
}
BLYNK_WRITE(V4)
{
  int pinValue = param.asInt();
  digitalWrite(D4,pinValue);
  updateLCD(4, pinValue);  
  EEPROM.write(1, pinValue); // Lưu trạng thái D4 vào EEPROM
}


void setup() {
  //lcd.begin(SCREEN_WIDTH, SCREEN_HEIGHT);// Khởi tạo màn hình LCD
  Serial.begin(115200);
  Blynk.begin(auth, ssid , pass);  // Khởi tạo kết nối với Blynk
  D3State = EEPROM.read(0);
  D4State = EEPROM.read(1);
  digitalWrite(D3, D3State);
  digitalWrite(D4, D4State);
  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);
  Wire.begin();  // Initialize I2C communication
  lcd.backlight(); // Bật đèn màn hình Màn hình
  lcd.begin(SCREEN_WIDTH, SCREEN_HEIGHT, I2C_ADDR);  // Initialize LCD
  lcd.clear();   // Clear LCD screen
  updateLCD();   // Display initial status
  
}


void loop() {
  // Kiểm tra sự kiện từ Blynk
  Blynk.run();
}
