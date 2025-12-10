#include <M5Unified.h>

// 3つのモーターピン
const int MOTOR_LEFT_PIN = 5;    // G5 - Left
const int MOTOR_CENTER_PIN = 6;  // G6 - Center
const int MOTOR_RIGHT_PIN = 7;   // G7 - Right

// PWMチャンネル設定
const int MOTOR_LEFT_CH = 0;
const int MOTOR_CENTER_CH = 1;
const int MOTOR_RIGHT_CH = 2;
const int PWM_FREQ = 200;   // 200Hz
const int PWM_RES = 8;      // 8bit (0-255)

// 出力強度設定
const int POWER_STOP = 0;      // Stop
const int POWER_WEAK = 80;     // Weak
const int POWER_STRONG = 200;  // Strong

// テスト設定
const int TEST_DURATION = 2000; // 各モーター2秒間テスト

// 長押し判定時間（ミリ秒）
const int LONG_PRESS_TIME = 1000;  // 1秒以上で長押し

// 10パターンの動作時間（1000-3000ms）とインターバル（0-500ms）
const int patternRunTimes[10] = {1800, 2400, 1200, 2700, 1500, 2100, 1000, 2900, 1600, 2300};  // ms
const int patternIntervals[10] = {350, 100, 450, 200, 0, 300, 500, 150, 250, 50};  // ms (0-500ms)

void testMotor(int pin, int channel, const char* position, int power, const char* strengthName) {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.printf("%s\n", position);
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.printf("Pin: G%d\n", pin);
  M5.Display.printf("%s (PWM=%d)\n", strengthName, power);
  
  Serial.printf("=== %s G%d: %s (PWM=%d) ===\n", position, pin, strengthName, power);
  
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
  
  // Left - Weak
  testMotor(MOTOR_LEFT_PIN, MOTOR_LEFT_CH, "Left", POWER_WEAK, "Weak");
  
  // Left - Strong
  testMotor(MOTOR_LEFT_PIN, MOTOR_LEFT_CH, "Left", POWER_STRONG, "Strong");
  
  // Center - Weak
  testMotor(MOTOR_CENTER_PIN, MOTOR_CENTER_CH, "Center", POWER_WEAK, "Weak");
  
  // Center - Strong
  testMotor(MOTOR_CENTER_PIN, MOTOR_CENTER_CH, "Center", POWER_STRONG, "Strong");
  
  // Right - Weak
  testMotor(MOTOR_RIGHT_PIN, MOTOR_RIGHT_CH, "Right", POWER_WEAK, "Weak");
  
  // Right - Strong
  testMotor(MOTOR_RIGHT_PIN, MOTOR_RIGHT_CH, "Right", POWER_STRONG, "Strong");
  
  Serial.println("=== Full Test Complete ===\n");
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Full Test");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 20x pattern");
  
  delay(2000);
}

void executePattern(const char* position, int power, int moveNum, int runTime, int intervalTime) {
  // 全モーター停止
  ledcWrite(MOTOR_LEFT_CH, 0);
  ledcWrite(MOTOR_CENTER_CH, 0);
  ledcWrite(MOTOR_RIGHT_CH, 0);
  delay(100);
  
  int channel;
  int pin;
  
  if (strcmp(position, "Left") == 0) {
    channel = MOTOR_LEFT_CH;
    pin = MOTOR_LEFT_PIN;
  } else if (strcmp(position, "Center") == 0) {
    channel = MOTOR_CENTER_CH;
    pin = MOTOR_CENTER_PIN;
  } else {
    channel = MOTOR_RIGHT_CH;
    pin = MOTOR_RIGHT_PIN;
  }
  
  const char* strengthName = (power == POWER_WEAK) ? "Weak" : "Strong";
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(1);
  M5.Display.printf("Move %d/20\n", moveNum);
  M5.Display.println("==============");
  M5.Display.println("");
  M5.Display.setTextSize(2);
  M5.Display.printf("%s\n", position);
  M5.Display.printf("%s\n", strengthName);
  M5.Display.setTextSize(1);
  M5.Display.printf("G%d: PWM=%d\n", pin, power);
  M5.Display.printf("Time:%dms\n", runTime);
  M5.Display.printf("Wait:%dms", intervalTime);
  
  Serial.printf("Move %d/20: %s G%d %s (PWM=%d) Time:%dms Wait:%dms\n", 
                moveNum, position, pin, strengthName, power, runTime, intervalTime);
  
  // モーター駆動
  ledcWrite(channel, power);
  delay(runTime);
  ledcWrite(channel, 0);
  
  // インターバル中は「None」を表示
  if (intervalTime > 0) {
    M5.Display.clear();
    M5.Display.setCursor(0, 0);
    M5.Display.setTextSize(1);
    M5.Display.printf("Move %d/20\n", moveNum);
    M5.Display.println("==============");
    M5.Display.println("");
    M5.Display.setTextSize(2);
    M5.Display.println("None");
    M5.Display.println("");
    M5.Display.setTextSize(1);
    M5.Display.printf("Wait:%dms", intervalTime);
    
    Serial.printf("  Interval: None (Wait:%dms)\n", intervalTime);
    delay(intervalTime);
  }
}

void run20Pattern() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting");
  M5.Display.println("20x Pattern");
  delay(1000);
  
  Serial.println("\n=== 20 Pattern Fixed Sequence ===");
  
  // 1~10回目
  executePattern("Center", POWER_STRONG, 1, patternRunTimes[0], patternIntervals[0]);  // Center Strong 1800ms / 350ms
  executePattern("Right", POWER_STRONG, 2, patternRunTimes[1], patternIntervals[1]);   // Right Strong 2400ms / 100ms
  executePattern("Left", POWER_WEAK, 3, patternRunTimes[2], patternIntervals[2]);      // Left Weak 1200ms / 450ms
  executePattern("Left", POWER_STRONG, 4, patternRunTimes[3], patternIntervals[3]);    // Left Strong 2700ms / 200ms
  executePattern("Center", POWER_WEAK, 5, patternRunTimes[4], patternIntervals[4]);    // Center Weak 1500ms / 0ms
  executePattern("Left", POWER_STRONG, 6, patternRunTimes[5], patternIntervals[5]);    // Left Strong 2100ms / 300ms
  executePattern("Center", POWER_STRONG, 7, patternRunTimes[6], patternIntervals[6]);  // Center Strong 1000ms / 500ms
  executePattern("Right", POWER_WEAK, 8, patternRunTimes[7], patternIntervals[7]);     // Right Weak 2900ms / 150ms
  executePattern("Right", POWER_WEAK, 9, patternRunTimes[8], patternIntervals[8]);     // Right Weak 1600ms / 250ms
  executePattern("Left", POWER_STRONG, 10, patternRunTimes[9], patternIntervals[9]);   // Left Strong 2300ms / 50ms
  
  // 11~20回目（配列の順番をめちゃくちゃに、位置と強度も変更）
  executePattern("Right", POWER_WEAK, 11, patternRunTimes[5], patternIntervals[8]);    // Right Weak 2100ms / 250ms
  executePattern("Center", POWER_STRONG, 12, patternRunTimes[9], patternIntervals[3]); // Center Strong 2300ms / 200ms
  executePattern("Left", POWER_WEAK, 13, patternRunTimes[7], patternIntervals[6]);     // Left Weak 2900ms / 500ms
  executePattern("Right", POWER_STRONG, 14, patternRunTimes[2], patternIntervals[9]);  // Right Strong 1200ms / 50ms
  executePattern("Center", POWER_WEAK, 15, patternRunTimes[4], patternIntervals[1]);   // Center Weak 1500ms / 100ms
  executePattern("Left", POWER_STRONG, 16, patternRunTimes[8], patternIntervals[5]);   // Left Strong 1600ms / 300ms
  executePattern("Right", POWER_WEAK, 17, patternRunTimes[1], patternIntervals[7]);    // Right Weak 2400ms / 150ms
  executePattern("Center", POWER_STRONG, 18, patternRunTimes[6], patternIntervals[4]); // Center Strong 1000ms / 0ms
  executePattern("Right", POWER_STRONG, 19, patternRunTimes[3], patternIntervals[2]);  // Right Strong 2700ms / 450ms
  executePattern("Center", POWER_WEAK, 20, patternRunTimes[0], patternIntervals[0]);   // Center Weak 1800ms / 350ms
  
  // 全モーター停止
  ledcWrite(MOTOR_LEFT_CH, 0);
  ledcWrite(MOTOR_CENTER_CH, 0);
  ledcWrite(MOTOR_RIGHT_CH, 0);
  
  Serial.println("=== 20 Pattern Complete ===\n");
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("20x Pattern");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 20x pattern");
  
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
  Serial.printf("Init Left Motor: Pin=%d, Ch=%d\n", MOTOR_LEFT_PIN, MOTOR_LEFT_CH);
  ledcSetup(MOTOR_LEFT_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(MOTOR_LEFT_PIN, MOTOR_LEFT_CH);
  ledcWrite(MOTOR_LEFT_CH, 0);
  
  Serial.printf("Init Center Motor: Pin=%d, Ch=%d\n", MOTOR_CENTER_PIN, MOTOR_CENTER_CH);
  ledcSetup(MOTOR_CENTER_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(MOTOR_CENTER_PIN, MOTOR_CENTER_CH);
  ledcWrite(MOTOR_CENTER_CH, 0);
  
  Serial.printf("Init Right Motor: Pin=%d, Ch=%d\n", MOTOR_RIGHT_PIN, MOTOR_RIGHT_CH);
  ledcSetup(MOTOR_RIGHT_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(MOTOR_RIGHT_PIN, MOTOR_RIGHT_CH);
  ledcWrite(MOTOR_RIGHT_CH, 0);
  
  Serial.println("PWM Init Complete");
  
  // LED常時点灯（電源ON表示）
  M5.Power.setLed(255);
  
  M5.Display.clear();
  M5.Display.setTextSize(1);
  M5.Display.setCursor(0, 0);
  M5.Display.println("3 DC Motor Test");
  M5.Display.println("================");
  M5.Display.println("");
  M5.Display.setTextSize(2);
  M5.Display.println("None");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 20x pattern");
  
  Serial.println("\nShort press: Full test");
  Serial.println("Long press: 20x pattern\n");
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
      // 長押し：20パターン実行
      Serial.printf("Long press detected (%lums)\n", pressDuration);
      run20Pattern();
    } else {
      // 短押し：フルテスト実行
      Serial.printf("Short press detected (%lums)\n", pressDuration);
      runAllTests();
    }
    
    // テスト完了後、Noneを表示
    M5.Display.clear();
    M5.Display.setTextSize(1);
    M5.Display.setCursor(0, 0);
    M5.Display.println("3 DC Motor Test");
    M5.Display.println("================");
    M5.Display.println("");
    M5.Display.setTextSize(2);
    M5.Display.println("None");
    M5.Display.println("");
    M5.Display.setTextSize(1);
    M5.Display.println("Short: Full test");
    M5.Display.println("Long: 20x pattern");
  }
  
  delay(10);
}