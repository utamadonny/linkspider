#include <Arduino.h>
#include <SoftwareSerial.h>

#include "linkspider.h"

int pressed = 0;
int state_servoPWM[2][6][3];

LinkSpider_LegIndex_t state_legIndex = L1;
LinkSpider_ServoIndex_t state_servoIndex = SERVO_1;
LinkSpider_ComboIndex_t state_comboIndex = COMBO_A;
LinkSpider_ConnectorSSC32 connectorSSC;
SoftwareSerial serialSSC(19, 18);  // RX, TX

// Legs Alignment
const double config_anchorRotDegs[6] = {
    135, 45,  // L1, R1
    180, 0,   // L2, R2
    225, 315  // L3, R3
};

// Servo positional per combo
double config_servoDegs[2][6][3] = {
    {
        // Combo A
        {0, 0, 90},  // L1
        {0, 0, 90},  // R1
        {0, 0, 90},  // L2
        {0, 0, 90},  // R2
        {0, 0, 90},  // L3
        {0, 0, 90},  // R3
    },
    {
        // Combo B
        {-45, 45, 45},  // L1
        {45, 45, 45},   // R1
        {-45, 45, 45},  // L2
        {45, 45, 45},   // R2
        {-45, 45, 45},  // L3
        {45, 45, 45},   // R3
    },
};

void setup() {
  Serial.begin(9600);  // Serial Monitor
  serialSSC.begin(9600);     // Serial connection to SSC 32

  // Previous calibration generated by pressing "v" on keyboard
  // BEGIN ============================
state_servoPWM[COMBO_A][0][0] = 1480;
state_servoPWM[COMBO_B][0][0] = 890;
state_servoPWM[COMBO_A][0][1] = 1670;
state_servoPWM[COMBO_B][0][1] = 2060;
state_servoPWM[COMBO_A][0][2] = 1520;
state_servoPWM[COMBO_B][0][2] = 1970;
state_servoPWM[COMBO_A][1][0] = 1570;
state_servoPWM[COMBO_B][1][0] = 2060;
state_servoPWM[COMBO_A][1][1] = 1510;
state_servoPWM[COMBO_B][1][1] = 1020;
state_servoPWM[COMBO_A][1][2] = 1420;
state_servoPWM[COMBO_B][1][2] = 890;
state_servoPWM[COMBO_A][2][0] = 1240;
state_servoPWM[COMBO_B][2][0] = 930;
state_servoPWM[COMBO_A][2][1] = 1800;
state_servoPWM[COMBO_B][2][1] = 2350;
state_servoPWM[COMBO_A][2][2] = 1610;
state_servoPWM[COMBO_B][2][2] = 2110;
state_servoPWM[COMBO_A][3][0] = 1310;
state_servoPWM[COMBO_B][3][0] = 1630;
state_servoPWM[COMBO_A][3][1] = 1780;
state_servoPWM[COMBO_B][3][1] = 1290;
state_servoPWM[COMBO_A][3][2] = 1090;
state_servoPWM[COMBO_B][3][2] = 680;
state_servoPWM[COMBO_A][4][0] = 1380;
state_servoPWM[COMBO_B][4][0] = 1000;
state_servoPWM[COMBO_A][4][1] = 1530;
state_servoPWM[COMBO_B][4][1] = 2010;
state_servoPWM[COMBO_A][4][2] = 1820;
state_servoPWM[COMBO_B][4][2] = 2300;
state_servoPWM[COMBO_A][5][0] = 1190;
state_servoPWM[COMBO_B][5][0] = 1490;
state_servoPWM[COMBO_A][5][1] = 1450;
state_servoPWM[COMBO_B][5][1] = 1000;
state_servoPWM[COMBO_A][5][2] = 1400;
state_servoPWM[COMBO_B][5][2] = 950;

  
//state_servoPWM[COMBO_A][0][0] = 1480;
//state_servoPWM[COMBO_B][0][0] = 890;
//state_servoPWM[COMBO_A][0][1] = 1560;
//state_servoPWM[COMBO_B][0][1] = 2060;
//state_servoPWM[COMBO_A][0][2] = 1530;
//state_servoPWM[COMBO_B][0][2] = 1970;
//state_servoPWM[COMBO_A][1][0] = 1570;
//state_servoPWM[COMBO_B][1][0] = 2060;
//state_servoPWM[COMBO_A][1][1] = 1580;
//state_servoPWM[COMBO_B][1][1] = 1020;
//state_servoPWM[COMBO_A][1][2] = 1440;
//state_servoPWM[COMBO_B][1][2] = 890;
//state_servoPWM[COMBO_A][2][0] = 1240;
//state_servoPWM[COMBO_B][2][0] = 930;
//state_servoPWM[COMBO_A][2][1] = 1800;
//state_servoPWM[COMBO_B][2][1] = 2350;
//state_servoPWM[COMBO_A][2][2] = 1610;
//state_servoPWM[COMBO_B][2][2] = 2110;
//state_servoPWM[COMBO_A][3][0] = 1310;
//state_servoPWM[COMBO_B][3][0] = 1630;
//state_servoPWM[COMBO_A][3][1] = 1780;
//state_servoPWM[COMBO_B][3][1] = 1290;
//state_servoPWM[COMBO_A][3][2] = 1090;
//state_servoPWM[COMBO_B][3][2] = 680;
//state_servoPWM[COMBO_A][4][0] = 1380;
//state_servoPWM[COMBO_B][4][0] = 1000;
//state_servoPWM[COMBO_A][4][1] = 1530;
//state_servoPWM[COMBO_B][4][1] = 2010;
//state_servoPWM[COMBO_A][4][2] = 1860;
//state_servoPWM[COMBO_B][4][2] = 2300;
//state_servoPWM[COMBO_A][5][0] = 1190;
//state_servoPWM[COMBO_B][5][0] = 1490;
//state_servoPWM[COMBO_A][5][1] = 1450;
//state_servoPWM[COMBO_B][5][1] = 1000;
//state_servoPWM[COMBO_A][5][2] = 1400;
//state_servoPWM[COMBO_B][5][2] = 950;
  // END ==============================

  // Configure SSC 32
  connectorSSC.setInterval(200);

  connectorSSC.setServoPin(L1, SERVO_1, 4);
  connectorSSC.setServoPin(L1, SERVO_2, 5);
  connectorSSC.setServoPin(L1, SERVO_3, 6);
  connectorSSC.setServoPin(L2, SERVO_1, 8);
  connectorSSC.setServoPin(L2, SERVO_2, 9);
  connectorSSC.setServoPin(L2, SERVO_3, 10);
  connectorSSC.setServoPin(L3, SERVO_1, 12);
  connectorSSC.setServoPin(L3, SERVO_2, 13);
  connectorSSC.setServoPin(L3, SERVO_3, 14);

  connectorSSC.setServoPin(R1, SERVO_1, 20);
  connectorSSC.setServoPin(R1, SERVO_2, 21);
  connectorSSC.setServoPin(R1, SERVO_3, 22);
  connectorSSC.setServoPin(R2, SERVO_1, 24);
  connectorSSC.setServoPin(R2, SERVO_2, 25);
  connectorSSC.setServoPin(R2, SERVO_3, 26);
  connectorSSC.setServoPin(R3, SERVO_1, 28);
  connectorSSC.setServoPin(R3, SERVO_2, 29);
  connectorSSC.setServoPin(R3, SERVO_3, 30);
}

String translateLegIndex(LinkSpider_LegIndex_t index) {
  switch (index) {
    case L1:
      return "L1";
      break;
    case L2:
      return "L2";
      break;
    case L3:
      return "L3";
      break;
    case R1:
      return "R1";
      break;
    case R2:
      return "R2";
      break;
    case R3:
      return "R3";
      break;
    default:
      return "?";
  }
}

String translateComboIndex(LinkSpider_ComboIndex_t index) {
  switch (index) {
    case COMBO_A:
      return "COMBO A";
      break;
    case COMBO_B:
      return "COMBO B";
      break;
    default:
      return "?";
      break;
  }
}

void loop() {
  if (Serial.available() > 0) {
    // Get pressed button
    while (Serial.available() > 0) {
      pressed = Serial.read();
    }

    // Button conditional
    if (pressed == 'p') {
      // P to print serial commands
      Serial.println(connectorSSC.getPrintable());
    } else if (pressed == 'c') {
      // Print leg alignment, normal position and ratio PWM configuration codes
      for (size_t i = 0; i < 6; i++) {
        // Print Leg Alignment
        Serial.println(String("leg[") + i + "].setAnchorRotDeg(" + config_anchorRotDegs[i] + ");");

        // Print Normal Position PWM and Ratio PWM
        for (size_t j = 0; j < 3; j++) {
          double deltaServoDeg = config_servoDegs[COMBO_A][i][j] - config_servoDegs[COMBO_B][i][j];
          double deltaServoPWM = state_servoPWM[COMBO_A][i][j] - state_servoPWM[COMBO_B][i][j];
          double ratio = deltaServoDeg / deltaServoPWM;
          double normal = state_servoPWM[COMBO_A][i][j] - (deltaServoPWM / deltaServoDeg) * config_servoDegs[COMBO_A][i][j];
          Serial.println(String("leg[") + i + "].setNormalPosPWM(" + j + ", " + normal + ");");
          Serial.println(String("leg[") + i + "].setRatioDegPWM(" + j + ", " + ratio + ");");
        }
      }
    } else if (pressed == 'v') {
      // Print calibration for initial configuration
      for (size_t i = 0; i < 6; i++) {
        for (size_t j = 0; j < 3; j++) {
          Serial.println(String("state_servoPWM[COMBO_A][") + i + "][" + j + "] = " + state_servoPWM[COMBO_A][i][j] + ";");
          Serial.println(String("state_servoPWM[COMBO_B][") + i + "][" + j + "] = " + state_servoPWM[COMBO_B][i][j] + ";");
        }
      }
    } else {
      // Calibration config

      // Switch combo
      if (pressed == '[') state_comboIndex = COMBO_A;
      if (pressed == ']') state_comboIndex = COMBO_B;


      // Switch leg
      if (pressed == 'q') state_legIndex = L1;
      if (pressed == 'w') state_legIndex = R1;
      if (pressed == 'a') state_legIndex = L2;
      if (pressed == 's') state_legIndex = R2;
      if (pressed == 'z') state_legIndex = L3;
      if (pressed == 'x') state_legIndex = R3;

      // Switch servo index
      if (pressed == '1') state_servoIndex = SERVO_1;
      if (pressed == '2') state_servoIndex = SERVO_2;
      if (pressed == '3') state_servoIndex = SERVO_3;

      // Increase/decrease PWM
      if (pressed == 'n') state_servoPWM[state_comboIndex][state_legIndex][state_servoIndex] -= 10;
      if (pressed == 'm') state_servoPWM[state_comboIndex][state_legIndex][state_servoIndex] += 10;
      if (pressed == 'j') state_servoPWM[state_comboIndex][state_legIndex][state_servoIndex] -= 100;
      if (pressed == 'k') state_servoPWM[state_comboIndex][state_legIndex][state_servoIndex] += 100;
      // Set PWM value to SSC 32 connector
      for (size_t i = 0; i < 6; i++) {
        for (size_t j = 0; j < 3; j++) {
          connectorSSC.setServoValue(i, j, state_servoPWM[state_comboIndex][i][j]);
        }
      }

      // Compute & run SSC 32 commands
      connectorSSC.compute();
      serialSSC.println(String(connectorSSC.getPrintable()));

      // Debug Calibration
      Serial.print(translateComboIndex(state_comboIndex) + "-");
      Serial.print(translateLegIndex(state_legIndex) + "-" + (state_servoIndex + 1) + ": ");
      Serial.print(state_servoPWM[state_comboIndex][state_legIndex][state_servoIndex], DEC);
      Serial.print(" us = ");
      Serial.print(config_servoDegs[state_comboIndex][state_legIndex][state_servoIndex], DEC);
      Serial.print(" deg");
      Serial.println();
    }
  }

  delayMicroseconds(500);
}
