#include <M5Unified.h>

// 振動モーターピン（AtomS3のGrove互換ピン）
const int MOTOR_PIN = 5;  // G5 (Grove Yellow)

// PWMチャンネル設定
const int MOTOR_CH = 0;
const int PWM_FREQ = 200;   // 200Hz に変更（元は1000Hz）
const int PWM_RES = 8;      // 8bit (0-255)

// 振動強度設定
const int POWER_STOP = 0;      // 停止
const int POWER_WEAK = 80;     // 弱い（100から80に下げた）
const int POWER_STRONG = 255;  // 強い（255から200に下げた）

int powerMode = 0; // 0=停止, 1=弱い, 2=強い
int currentPower = POWER_STOP;

void updateDisplay() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  
  switch(powerMode) {
    case 0:
      M5.Display.println("STOP");
      break;
    case 1:
      M5.Display.println("WEAK");
      M5.Display.println("");
      M5.Display.setTextSize(1);
      M5.Display.printf("PWM: %d/255\n", POWER_WEAK);
      M5.Display.printf("Freq: %dHz\n", PWM_FREQ);
      break;
    case 2:
      M5.Display.println("STRONG");
      M5.Display.println("");
      M5.Display.setTextSize(1);
      M5.Display.printf("PWM: %d/255\n", POWER_STRONG);
      M5.Display.printf("Freq: %dHz\n", PWM_FREQ);
      break;
  }
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  // PWM初期化
  ledcSetup(MOTOR_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(MOTOR_PIN, MOTOR_CH);
  ledcWrite(MOTOR_CH, 0);  // 停止状態
  
  M5.Display.clear();
  M5.Display.setTextSize(1);
  M5.Display.setCursor(0, 0);
  M5.Display.println("Vibration Motor");
  M5.Display.println("===============");
  M5.Display.println("");
  M5.Display.println("Press button");
  M5.Display.println("to change power");
  M5.Display.println("");
  M5.Display.println("Stop -> Weak");
  M5.Display.println("-> Strong -> Stop");
  M5.Display.println("");
  M5.Display.printf("Freq: %dHz\n", PWM_FREQ);
  M5.Display.println("");
  M5.Display.println("Status: Stop");
  
  delay(2000);
  updateDisplay();
}

void loop() {
  M5.update();
  
  // ボタン押下：振動強度切り替え
  if (M5.BtnA.wasPressed()) {
    powerMode++;
    if (powerMode > 2) {
      powerMode = 0;
    }
    
    switch(powerMode) {
      case 0:
        currentPower = POWER_STOP;
        break;
      case 1:
        currentPower = POWER_WEAK;
        break;
      case 2:
        currentPower = POWER_STRONG;
        break;
    }
    
    ledcWrite(MOTOR_CH, currentPower);
    updateDisplay();
  }
  
  delay(10);
}