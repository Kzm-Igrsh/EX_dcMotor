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

// 長押し判定時間（ミリ秒）
const int LONG_PRESS_TIME = 1000;  // 1秒以上で長押し

// 10パターンの動作時間（1000-3000ms）とインターバル（0-500ms）
const int patternRunTimes[10] = {1800, 2400, 1200, 2700, 1500, 2100, 1000, 2900, 1600, 2300};  // ms
const int patternIntervals[10] = {350, 100, 450, 200, 0, 300, 500, 150, 250, 50};  // ms (0-500ms)

void testMotor(int pin, int channel, int motorNum, int power, const char* powerName) {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.printf("Motor %d\n", motorNum);
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.printf("Pin: G%d\n", pin);
  M5.Display.printf("%s (PWM=%d)\n", powerName, power);
  
  Serial.printf("=== Motor%d G%d: %s (PWM=%d) ===\n", motorNum, pin, powerName, power);
  
  ledcWrite(channel, power);
  
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(2);
  M5.Display.println("RUNNING");
  
  delay(TEST_DURATION);
  ledcWrite(channel, 0);
  
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(2);
  M5.Display.println("STOP");
  
  delay(500);
}

void runAllTests() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting");
  M5.Display.println("Full Test");
  delay(1000);
  
  Serial.println("\n=== Starting Full Motor Test ===");
  
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
  
  Serial.println("=== Full Test Complete ===\n");
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Full Test");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 10x pattern");
  
  delay(2000);
}

void executePattern(int motorNum, int power, int moveNum, int runTime, int intervalTime) {
  // 全モーター停止
  ledcWrite(MOTOR1_CH, 0);
  ledcWrite(MOTOR2_CH, 0);
  ledcWrite(MOTOR3_CH, 0);
  delay(100);
  
  int channel;
  int pin;
  
  switch(motorNum) {
    case 1:
      channel = MOTOR1_CH;
      pin = MOTOR1_PIN;
      break;
    case 2:
      channel = MOTOR2_CH;
      pin = MOTOR2_PIN;
      break;
    case 3:
      channel = MOTOR3_CH;
      pin = MOTOR3_PIN;
      break;
  }
  
  const char* powerName = (power == POWER_WEAK) ? "WEAK" : "STRONG";
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(1);
  M5.Display.printf("Move %d/10\n", moveNum);
  M5.Display.println("==============");
  M5.Display.println("");
  M5.Display.setTextSize(2);
  M5.Display.printf("Motor %d\n", motorNum);
  M5.Display.println(powerName);
  M5.Display.setTextSize(1);
  M5.Display.printf("G%d: PWM=%d\n", pin, power);
  M5.Display.printf("Time:%dms\n", runTime);
  M5.Display.printf("Wait:%dms", intervalTime);
  
  Serial.printf("Move %d/10: Motor%d G%d %s (PWM=%d) Time:%dms Wait:%dms\n", 
                moveNum, motorNum, pin, powerName, power, runTime, intervalTime);
  
  // モーター駆動
  ledcWrite(channel, power);
  delay(runTime);
  ledcWrite(channel, 0);
  
  delay(intervalTime);
}

void run10Pattern() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting");
  M5.Display.println("10x Pattern");
  delay(1000);
  
  Serial.println("\n=== 10 Pattern Fixed Sequence ===");
  
  // 固定の10パターン（順番と強度は固定、時間だけバラバラ）
  executePattern(2, POWER_STRONG, 1, patternRunTimes[0], patternIntervals[0]);   // Motor2 強い 1800ms / 350ms
  executePattern(3, POWER_STRONG, 2, patternRunTimes[1], patternIntervals[1]);   // Motor3 強い 2400ms / 100ms
  executePattern(1, POWER_WEAK, 3, patternRunTimes[2], patternIntervals[2]);     // Motor1 弱い 1200ms / 450ms
  executePattern(1, POWER_STRONG, 4, patternRunTimes[3], patternIntervals[3]);   // Motor1 強い 2700ms / 200ms
  executePattern(2, POWER_WEAK, 5, patternRunTimes[4], patternIntervals[4]);     // Motor2 弱い 1500ms / 0ms
  executePattern(1, POWER_STRONG, 6, patternRunTimes[5], patternIntervals[5]);   // Motor1 強い 2100ms / 300ms
  executePattern(2, POWER_STRONG, 7, patternRunTimes[6], patternIntervals[6]);   // Motor2 強い 1000ms / 500ms
  executePattern(3, POWER_WEAK, 8, patternRunTimes[7], patternIntervals[7]);     // Motor3 弱い 2900ms / 150ms
  executePattern(3, POWER_WEAK, 9, patternRunTimes[8], patternIntervals[8]);     // Motor3 弱い 1600ms / 250ms
  executePattern(1, POWER_STRONG, 10, patternRunTimes[9], patternIntervals[9]);  // Motor1 強い 2300ms / 50ms
  
  // 全モーター停止
  ledcWrite(MOTOR1_CH, 0);
  ledcWrite(MOTOR2_CH, 0);
  ledcWrite(MOTOR3_CH, 0);
  
  Serial.println("=== 10 Pattern Complete ===\n");
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("10x Pattern");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 10x pattern");
  
  delay(2000);
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
  
  // LED常時点灯（電源ON表示）
  M5.Power.setLed(255);
  
  M5.Display.clear();
  M5.Display.setTextSize(1);
  M5.Display.setCursor(0, 0);
  M5.Display.println("3 DC Motor Test");
  M5.Display.println("================");
  M5.Display.println("");
  M5.Display.println("Short press:");
  M5.Display.println(" Full test");
  M5.Display.println("");
  M5.Display.println("Long press:");
  M5.Display.println(" 10x pattern");
  M5.Display.println("");
  M5.Display.println("Press to start");
  
  Serial.println("\nShort press: Full test");
  Serial.println("Long press: 10x pattern\n");
}

void loop() {
  M5.update();
  
  // ボタンが押された瞬間の時刻を記録
  if (M5.BtnA.wasPressed()) {
    unsigned long pressStartTime = millis();
    
    // ボタンが離されるまで待つ
    while (M5.BtnA.isPressed()) {
      M5.update();
      delay(10);
    }
    
    // 押していた時間を計算
    unsigned long pressDuration = millis() - pressStartTime;
    
    if (pressDuration >= LONG_PRESS_TIME) {
      // 長押し：10パターン実行
      Serial.printf("Long press detected (%lums)\n", pressDuration);
      run10Pattern();
    } else {
      // 短押し：フルテスト実行
      Serial.printf("Short press detected (%lums)\n", pressDuration);
      runAllTests();
    }
  }
  
  delay(10);
}