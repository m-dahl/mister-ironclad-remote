// "daemonbite" adapter for a VS1838B ir sensor. I use it in conjunction with the 
// ironclad mister itx board, where the IR sensor provides on/off, osd, user and reset
// buttons alread. This completes the functionality by providing arrow keys and two buttons
// which can be mapped for osd navigation.
// Additionally I mapped 0-3 for alt ini selection.

#include "src/Arduino-IRremote-4.2.0/src/IRremote.hpp"
#include "src/Gamepad.h"

// ATT: 20 chars max (including NULL at the end) according to Arduino source code.
// Additionally serial number is used to differentiate arduino projects to have different button maps!
const char *gp_serial = "IRremote to USB";

// VS1838B data pin
#define RECV_PIN 10

// osd navigation
#define UP 0xBF40FF00
#define DOWN 0xE619FF00
#define LEFT 0xF807FF00
#define RIGHT 0xF609FF00
#define BACK 0xBC43FF00
#define SELECT 0xEA15FF00

// Use 0-3 buttons to chose alt ini.
#define ZERO 0xE916FF00
#define ONE 0xF30CFF00
#define TWO 0xE718FF00
#define THREE 0xA15EFF00

#define TIMEOUT_MICROS 200000
#define SEND_MICROS 1000

Gamepad_ gamepad;

typedef struct button {
  uint32_t ir;
  uint8_t button;
};

const button button_map[] = {
  { UP, 'u' },
  { DOWN, 'd' },
  { LEFT, 'l' },
  { RIGHT, 'r' },
  { BACK, 'b' },
  { SELECT, 'a' },

  { ZERO, '0' },
  { ONE, '1' },
  { TWO, '2' },
  { THREE, '3' },
};

void setup() {
  Serial.begin(9600);
  pinMode(RECV_PIN, INPUT_PULLUP);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  IrReceiver.registerReceiveCompleteCallback(ReceiveCompleteCallbackHandler);
}

volatile char last = 0;
uint32_t last_time = 0;
volatile uint32_t timeout_time = 0;

void loop() {
  if ((micros() - last_time) > SEND_MICROS) {
    if ((micros() - timeout_time) > TIMEOUT_MICROS) {
      // reset last keypress
      last = 0;
      timeout_time = micros();
    }

    gamepad._GamepadReport.Y = 0;
    if (last == 'u') {
      gamepad._GamepadReport.Y = -1;
    }
    if (last == 'd') {
      gamepad._GamepadReport.Y = 1;
    }
    gamepad._GamepadReport.X = 0;
    if (last == 'l') {
      gamepad._GamepadReport.X = -1;
    }
    if (last == 'r') {
      gamepad._GamepadReport.X = 1;
    }
    gamepad._GamepadReport.buttons = 0;
    if (last == 'a') {
      gamepad._GamepadReport.buttons = 1;
    }
    if (last == 'b') {
      gamepad._GamepadReport.buttons = 2;
    }

    // handle alt ini
    // MiSTer.ini       Controller: Back + Right
    // MiSTer_alt_1.ini Controller: Back + Left
    // MiSTer_alt_2.ini Controller: Back + Up
    // MiSTer_alt_3.ini Controller: Back + Down
    if (last == '0') {
      gamepad._GamepadReport.buttons = 2;
      gamepad._GamepadReport.X = 1;
    }
    if (last == '1') {
      gamepad._GamepadReport.buttons = 2;
      gamepad._GamepadReport.X = -1;
    }
    if (last == '2') {
      gamepad._GamepadReport.buttons = 2;
      gamepad._GamepadReport.Y = -1;
    }
    if (last == '3') {
      gamepad._GamepadReport.buttons = 2;
      gamepad._GamepadReport.Y = 1;
    }

    gamepad.send();
    last_time = micros();
  }
}

void ReceiveCompleteCallbackHandler() {
  if (IrReceiver.decode()) {
    timeout_time = micros();
    for (int i = 0; i < sizeof(button_map) / sizeof(button); i++) {
      if (button_map[i].ir == IrReceiver.decodedIRData.decodedRawData) {
        last = button_map[i].button;  // will keep its value until new value or timeout_time too large
        break;
      }
    }
  }
  IrReceiver.resume();
}
