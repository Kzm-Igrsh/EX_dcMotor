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
const int POWER_STRONG = 200;  // 強い

// テスト設定
const int TEST_DURATION = 2000; // 各モーター2秒間テスト

void testMotor(int pin, int channel, int motorNum, int power, const char* powerName) {
  Serial.printf("=== Motor%d G%d: %s (PWM=%d) ===\n", motorNum, pin, powerName, power);
  
  ledcWrite(channel, power);
  delay(TEST_DURATION);
  ledcWrite(channel, 0);
  
  delay(500);
}

void runAllTests() {
  Serial.println("\n=== Starting Motor Test ===");
  
  // Motor 1 - Weak
  testMotor(MOTOR1_PIN, MOTOR1_CH, 1, POWER_WEAK, "WEAK");
  
  // Motor 1 - Strong
  testMotor(MOTOR1_PIN, MOTOR1_CH, 1, POWER_STRONG, "STRONG");
  
  // Motor 2 - Weak
  testMotor(MOTOR2_PIN, MOTOR2_CH, 2, POWER_WEAK, "WEAK");
  
  // Motor 2 - Strong
  testMotor(MOTOR2_PIN, MOTOR2_CH, 2, POWER_STRONG, "STRONG");
  
  // Motor 3 - Weak
  testMotor(MOTOR3_PIN, MOTOR3_CH, 3, POWER_WEAK, "WEAK");
  
  // Motor 3 - Strong
  testMotor(MOTOR3_PIN, MOTOR3_CH, 3, POWER_STRONG, "STRONG");
  
  Serial.println("=== Test Complete ===\n");
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n3 DC Motor Auto Test (AtomS3 Lite)");
  Serial.println("====================================");
  
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
  Serial.println("\nPress button to start test\n");
  
  // LED常時点灯（電源ON表示）
  M5.Power.setLed(255);
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