#include <M5Unified.h>

// 3つのモーターピン
const int MOTOR1_PIN = 5;  // G5
const int MOTOR2_PIN = 6;  // G6
const int MOTOR3_PIN = 7;  // G7

// PWMチャンネル設定
const int MOTOR1_CH = 0;
const int MOTOR2_CH = 1;
const int MOTOR3_CH = 2;
const int PWM_FREQ = 200;   // 200Hz
const int PWM_RES = 8;      // 8bit (0-255)

// 出力強度設定
const int POWER_STOP = 0;      // 停止
const int POWER_WEAK = 80;     // 弱い
const int POWER_STRONG = 255;  // 強い

// テスト設定
const int TEST_DURATION = 2000; // 各モーター2秒間テスト

bool isRunning = false;

void testMotor(int pin, int channel, int motorNum, int power) {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.printf("Motor %d\n", motorNum);
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.printf("Pin: G%d\n", pin);
  M5.Display.printf("Power: %d\n", power);
  M5.Display.println("");
  
  if (power == POWER_WEAK) {
    M5.Display.println("Testing: WEAK");
  } else {
    M5.Display.println("Testing: STRONG");
  }
  
  Serial.printf("Motor%d G%d: PWM=%d\n", motorNum, pin, power);
  
  ledcWrite(channel, power);
  delay(TEST_DURATION);
  ledcWrite(channel, 0);
}

void runAllTests() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting");
  M5.Display.println("Test...");
  delay(1000);
  
  Serial.println("=== Starting Motor Test ===");
  
  // Motor 1 - Weak
  testMotor(MOTOR1_PIN, MOTOR1_CH, 1, POWER_WEAK);
  delay(500);
  
  // Motor 1 - Strong
  testMotor(MOTOR1_PIN, MOTOR1_CH, 1, POWER_STRONG);
  delay(500);
  
  // Motor 2 - Weak
  testMotor(MOTOR2_PIN, MOTOR2_CH, 2, POWER_WEAK);
  delay(500);
  
  // Motor 2 - Strong
  testMotor(MOTOR2_PIN, MOTOR2_CH, 2, POWER_STRONG);
  delay(500);
  
  // Motor 3 - Weak
  testMotor(MOTOR3_PIN, MOTOR3_CH, 3, POWER_WEAK);
  delay(500);
  
  // Motor 3 - Strong
  testMotor(MOTOR3_PIN, MOTOR3_CH, 3, POWER_STRONG);
  delay(500);
  
  Serial.println("=== Test Complete ===");
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Test");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("");
  M5.Display.println("Press button");
  M5.Display.println("to test again");
  
  delay(2000);
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(1);
  M5.Display.println("3 DC Motor Test");
  M5.Display.println("===============");
  M5.Display.println("");
  M5.Display.println("Motors: G5,G6,G7");
  M5.Display.printf("Freq: %dHz\n", PWM_FREQ);
  M5.Display.println("");
  M5.Display.println("Test sequence:");
  M5.Display.println("M1 Weak->Strong");
  M5.Display.println("M2 Weak->Strong");
  M5.Display.println("M3 Weak->Strong");
  M5.Display.println("");
  M5.Display.println("Press to start");
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  Serial.begin(115200);
  Serial.println("3 DC Motor Auto Test");
  
  // 3つのモーターのPWM初期化
  Serial.printf("Init Motor1: Pin=%d, Ch=%d\n", MOTOR1_PIN, MOTOR1_CH);
  ledcSetup(MOTOR1_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(MOTOR1_PIN, MOTOR1_CH);
  ledcWrite(MOTOR1_CH, 0);
  
  Serial.printf("Init Motor2: Pin=%d, Ch=%d\n", MOTOR2_PIN, MOTOR2_CH);
  ledcSetup(MOTOR2_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(MOTOR2_PIN, MOTOR2_CH);
  ledcWrite(MOTOR2_CH, 0);
  
  Serial.printf("Init Motor3: Pin=%d, Ch=%d\n", MOTOR3_PIN, MOTOR3_CH);
  ledcSetup(MOTOR3_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(MOTOR3_PIN, MOTOR3_CH);
  ledcWrite(MOTOR3_CH, 0);
  
  Serial.println("PWM Init Complete");
  
  M5.Display.clear();
  M5.Display.setTextSize(1);
  M5.Display.setCursor(0, 0);
  M5.Display.println("3 DC Motor Test");
  M5.Display.println("===============");
  M5.Display.println("");
  M5.Display.println("Motors: G5,G6,G7");
  M5.Display.printf("Freq: %dHz\n", PWM_FREQ);
  M5.Display.println("");
  M5.Display.println("Test sequence:");
  M5.Display.println("M1 Weak->Strong");
  M5.Display.println("M2 Weak->Strong");
  M5.Display.println("M3 Weak->Strong");
  M5.Display.println("");
  M5.Display.println("Press to start");
}

void loop() {
  M5.update();
  
  // ボタン押下：テスト開始
  if (M5.BtnA.wasPressed()) {
    Serial.println("Button pressed - Starting test");
    runAllTests();
  }
  
  delay(10);
}