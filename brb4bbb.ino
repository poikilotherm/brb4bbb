#include "Button2.h";
#include "HID-Project.h"

#define BUTTON_MIC 2
#define LED_MIC 3
#define LED_MIC_BOOT_STATUS HIGH
#define KEY_MIC_MUTE KEY_F20
#define CLICK_TIME 100 /* A single "click" is max 100ms. */
#define PRESSED_TIME 300 /* in milliseconds. How long the button has to be pressed to trigger the Push-To-Talk mechanism. */

/* https://github.com/LennartHennigs/Button2 */
Button2 buttonMic = Button2(BUTTON_MIC);

void setup() {

  pinMode(LED_MIC, OUTPUT);
  digitalWrite(LED_MIC, LED_MIC_BOOT_STATUS);

  buttonMic.setPressedHandler(pressed);
  buttonMic.setReleasedHandler(released);
  buttonMic.setDoubleClickHandler(doubleClick);

  BootKeyboard.begin();
}

void loop() {
  buttonMic.loop();
}

/*
 * Each time the button is pressed (no matter what type of click/longpress/...),
 * the LED and Mic Mute status will be inverted, but only if this was no double or multi click...
 */
void pressed(Button2& btn) {
  if (btn == buttonMic && btn.getNumberOfClicks() < 2) {
    digitalWrite(LED_MIC, !digitalRead(LED_MIC));
    BootKeyboard.write(KEY_MIC_MUTE);
  }
}

/*
 * When the button is released, we check what type of click this was.
 * A PTT type of use is assumed when the mic button has been pressed
 * for longer than PRESSED_TIME milliseconds.
 */
void released(Button2& btn) {
  unsigned int down_time = btn.wasPressedFor();

  /* disable only when PTT triggered. */
  if ( btn == buttonMic && down_time > PRESSED_TIME ) {
    digitalWrite(LED_MIC, !digitalRead(LED_MIC));
    BootKeyboard.write(KEY_MIC_MUTE);
  }
}

/*
 * A double click type lets the user invert the LED ring, so it can
 * be aligned to the actual mic status (which cannot be transfered to 
 * the keyboard HID we use...)
 */
void doubleClick(Button2& btn) {
  if ( btn == buttonMic ) {
    digitalWrite(LED_MIC, !digitalRead(LED_MIC));
  }
}
