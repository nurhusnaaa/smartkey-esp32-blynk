/********* BLYNK CONFIG *********/ 
#define BLYNK_TEMPLATE_ID   "TMPL6QZ1tRhg4" 
#define BLYNK_TEMPLATE_NAME "SmartKey" 
#define BLYNK_AUTH_TOKEN    "jBufomcQH0ge_uu-0VnAmB3B8dv4g5Lr" 
/********* LIBRARIES *********/ 
#include <WiFi.h> 
#include <BlynkSimpleEsp32.h> 
#include <Wire.h> 
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h> 
/********* WIFI CREDENTIALS *********/ 
char ssid[] = "rakyat 25-5@Maxis"; 
char pass[] = "Bakal4flat"; 
/********* OLED CONFIG *********/ 
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); 
/********* PIN DEFINITIONS *********/ 
#define VIB_PIN    27    // Vibration sensor SW-420 
#define BUTTON_PIN 26    // Physical confirm button 
#define BUZZ_PIN   14    // Buzzer 
#define LED_GENTLE 33    // Gentle mode LED 
#define LED_AGGR   32    // Aggressive mode LED 
/********* LOGIC VARIABLES *********/ 
bool gentleMode = true;        // Default mode 
bool loopingAlert = false;     // True when alert is active 
bool waitingConfirm = false;   // True during confirmation countdown 
unsigned long confirmStart = 0; 
const unsigned long CONFIRM_WINDOW = 5000; // 5-second countdown 
/********* BLYNK TIMER *********/ 
BlynkTimer timer; 
/********* BLYNK VIRTUAL PINS *********/ 
// Mode switch virtual pin 
BLYNK_WRITE(V1) { 
gentleMode = param.asInt(); 
showIdle(); 
} 
// Confirm button virtual pin 
BLYNK_WRITE(V4) { 
if (param.asInt()) { 
stopAlert(); 
} 
} 
/********* SETUP *********/ 
void setup() { 
Serial.begin(115200); 
// Initialize pins 
pinMode(VIB_PIN, INPUT_PULLDOWN); 
pinMode(BUTTON_PIN, INPUT_PULLUP); 
pinMode(BUZZ_PIN, OUTPUT); 
pinMode(LED_GENTLE, OUTPUT); 
pinMode(LED_AGGR, OUTPUT); 
delay(1500);  //   
// Initialize OLED 
Wire.begin(21, 22); 
Critical for power stability 
if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
Serial.println("OLED not found"); 
while (1); // Stop if OLED not detected 
} 
display.setTextColor(SSD1306_WHITE); 
showBoot();  // Show boot screen 
// Initialize Blynk (non-blocking) 
Blynk.config(BLYNK_AUTH_TOKEN); 
WiFi.begin(ssid, pass); 
} 
/********* LOOP *********/ 
void loop() { 
Blynk.run(); 
// Physical confirm button 
if (digitalRead(BUTTON_PIN) == LOW) { 
    stopAlert(); 
    delay(300); 
  } 
 
  // Vibration detection (edge-trigger) 
  static bool lastVib = false; 
  bool vibNow = digitalRead(VIB_PIN); 
 
  if (vibNow && !lastVib && !loopingAlert) { 
    loopingAlert = true; 
    waitingConfirm = true; 
    confirmStart = millis(); 
 
    // Send Blynk Event 
    Blynk.logEvent("smartkey_vibration", "    Vibration detected! Please 
confirm."); 
  } 
  lastVib = vibNow; 
 
  // Confirmation countdown 
  if (waitingConfirm) { 
    int left = (CONFIRM_WINDOW - (millis() - confirmStart)) / 1000; 
    if (left < 0) left = 0; 
    showConfirm(left); 
 
    // Countdown finished, trigger alert 
    if (millis() - confirmStart >= CONFIRM_WINDOW) { 
      waitingConfirm = false; 
      triggerAlert(); 
    } 
  } 
 
  // Repeating alert until confirmed 
  if (loopingAlert && !waitingConfirm) { 
    if (gentleMode) gentleAlert(); 
    else aggressiveAlert(); 
  } 
} 
 
/********* ALERT FUNCTIONS *********/ 
void triggerAlert() { 
  loopingAlert = true; 
  Blynk.virtualWrite(V3, 1); // Update Blynk LED 
} 
 
void gentleAlert() { 
  showAlert("Gentle"); 
  digitalWrite(LED_GENTLE, HIGH); 
  tone(BUZZ_PIN, 900, 200); 
  delay(400); 
  digitalWrite(LED_GENTLE, LOW); 
} 
 
void aggressiveAlert() { 
  showAlert("AGGRESSIVE"); 
  digitalWrite(LED_AGGR, HIGH); 
  tone(BUZZ_PIN, 1800, 200); 
  delay(300); 
  digitalWrite(LED_AGGR, LOW); 
} 
 
void stopAlert() { 
  loopingAlert = false; 
  waitingConfirm = false; 
  noTone(BUZZ_PIN); 
  digitalWrite(LED_GENTLE, LOW); 
  digitalWrite(LED_AGGR, LOW); 
  Blynk.virtualWrite(V3, 0); // Update Blynk LED 
  showIdle(); 
} 
 
/********* OLED FUNCTIONS *********/ 
void showBoot() { 
  display.clearDisplay(); 
  display.setTextSize(2); 
  display.setCursor(10, 18); 
  display.println("SMARTKEY"); 
  display.setTextSize(1); 
  display.setCursor(30, 50); 
  display.println("BOOTING..."); 
  display.display(); 
} 
 
void showIdle() { 
  display.clearDisplay(); 
  display.setTextSize(2); 
  display.setCursor(15, 5); 
  display.println("SMARTKEY"); 
  display.setTextSize(1); 
  display.setCursor(40, 32); 
  display.println("Idle"); 
  display.setCursor(0, 52); 
  display.print("Mode: "); 
  display.println(gentleMode ? "Gentle" : "Aggressive"); 
  display.display(); 
} 
 
void showConfirm(int sec) { 
  display.clearDisplay(); 
  display.setTextSize(2); 
  display.setCursor(5, 5); 
  display.println("CONFIRM"); 
  display.setCursor(55, 35); 
  display.print(sec); 
  display.display(); 
} 
 
void showAlert(const char *type) { 
  display.clearDisplay(); 
  display.setTextSize(2); 
  display.setCursor(20, 10); 
  display.println("ALERT"); 
  display.setCursor(5, 40); 
  display.println(type); 
  display.display(); 
}
