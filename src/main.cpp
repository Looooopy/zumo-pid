#include <Arduino.h>
#include <Wire.h>
#include "SBUS.h"
#include <Zumo32U4.h>

// Motor speed constants
const int16_t MOTOR_MAX_FORWARD = 400;
const int16_t MOTOR_OFF = 0;
const int16_t MOTOR_MAX_REVERSE = -400;

// Channel mapping
const int8_t  THROTTLE_CHANNEL = 2;
const int8_t  TURNING_CHANNEL = 0;
const int8_t  REVERSE_CHANNEL = 8;

// Max and min values for channels
const int16_t CHANNEL_MIN = 330;
const int16_t CHANNEL_MAX = 1600;
const int16_t CHANNEL_MID_MIN = 980;
const int16_t CHANNEL_MID_MAX = 1020;

// Uncomment to output debug logging on usb
// #define MY_DEBUG

// Forward declaration
#ifdef MY_DEBUG
void debugSBUS (uint16_t* channels, bool failSafe, bool lostFrame);
void terminalCls();
#endif
void mapTurningChannel(uint16_t turningChannel,  int16_t* speeds, bool reversed);
bool mapReverseChannel(uint16_t reverseChannel, int16_t* speeds);
void mapThrottleChannel(uint16_t throttleChannel, int16_t* speeds);

SBUS x8r(Serial1);
Zumo32U4Motors motors;

void setup() {
  #ifdef MY_DEBUG
  Serial.begin(115200);
  #endif
  // begin the SBUS communication
  x8r.begin();

  ledGreen(0);
  ledRed(0);
  ledYellow(0);
  motors.setSpeeds(0,0);
}

void loop() {
  uint16_t channels[16];
  bool failSafe ;
  bool lostFrame;

  if(x8r.read(&channels[0], &failSafe, &lostFrame)){
    #ifdef MY_DEBUG
    debugSBUS(&channels[0], failSafe, lostFrame);
    #endif
    if(lostFrame) {
      ledYellow(1);
    } else {
      ledYellow(0);
    }
    ledGreen(1);
  }

  if(failSafe) {
    // Set motor speed to 0
    motors.setSpeeds(0,0);
    ledRed(1);
    ledGreen(0);
  }
  else
  {
    int16_t speeds[2];
    bool reversed = false;
    mapThrottleChannel(channels[THROTTLE_CHANNEL], &speeds[0]);
    reversed = mapReverseChannel(channels[REVERSE_CHANNEL], &speeds[0]);
    mapTurningChannel(channels[TURNING_CHANNEL], &speeds[0], reversed);
    motors.setSpeeds(speeds[0],speeds[1]);
    ledRed(0);
  }
}

void mapThrottleChannel(uint16_t throttleChannel, int16_t* speeds) {
  if(throttleChannel > CHANNEL_MIN && throttleChannel < CHANNEL_MAX ) {
    speeds[0] = round( (throttleChannel - CHANNEL_MIN) / ((CHANNEL_MAX - CHANNEL_MIN) / MOTOR_MAX_FORWARD) );
    speeds[1] = round( (throttleChannel - CHANNEL_MIN) / ((CHANNEL_MAX - CHANNEL_MIN) / MOTOR_MAX_FORWARD) );
  } else if (throttleChannel < CHANNEL_MIN) {
    speeds[0] = MOTOR_OFF;
    speeds[1] = MOTOR_OFF;
  } else if (throttleChannel > CHANNEL_MAX) {
    speeds[0] = MOTOR_MAX_FORWARD;
    speeds[1] = MOTOR_MAX_FORWARD;
  }
}

bool mapReverseChannel(uint16_t reverseChannel, int16_t* speeds) {
  if( reverseChannel > CHANNEL_MAX ) {
    speeds[0] = -speeds[0];
    speeds[1] = -speeds[1];
    return true;
  }

  return false;
}

void mapTurningChannel(uint16_t turningChannel,  int16_t* speeds, bool reversed) {
  if(turningChannel > CHANNEL_MIN && turningChannel < CHANNEL_MAX ) {
    if(turningChannel < CHANNEL_MID_MIN ) {
      int16_t current_speed = round(speeds[0]  * (CHANNEL_MAX - turningChannel) / ( CHANNEL_MAX - CHANNEL_MID_MAX) );
      if( reversed ) {
        speeds[1] = current_speed;
      } else {
        speeds[0] = current_speed;
      }
    } else if (turningChannel > CHANNEL_MID_MAX) {
      int16_t current_speed = round(speeds[1]  * (turningChannel -CHANNEL_MIN) / (CHANNEL_MID_MIN - CHANNEL_MIN) );
      if( reversed ) {
        speeds[0] = current_speed;
      } else {
        speeds[1] = current_speed;
      }
    }
  } else if (turningChannel < CHANNEL_MIN) {
    speeds[0] = MOTOR_MAX_REVERSE;
    speeds[1] = MOTOR_MAX_FORWARD;
  } else if (turningChannel > CHANNEL_MAX) {
    speeds[0] = MOTOR_MAX_FORWARD;
    speeds[1] = MOTOR_MAX_REVERSE;
  }
}

}

#ifdef MY_DEBUG
void debugSBUS (uint16_t* channels, bool failSafe, bool lostFrame) {
    Serial.println("----- Recieived ------");
    failSafe && Serial.println("failSafe");
    lostFrame && Serial.println("lostFrame");
    Serial.println("----- Channels ------");
    char buff[20];
    for (int i = 0; i < 16; i++ ) {
      sprintf(buff, "%d: %d", i, channels[i]);
      Serial.println(buff);
    }
    Serial.println("--------------------");
    Serial.println("");
    delay(100);
    terminalCls();
}

void terminalCls() {
  Serial.write("\033\143");       // ESC command
}
#endif
