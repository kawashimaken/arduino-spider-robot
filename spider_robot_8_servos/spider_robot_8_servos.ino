// STILL WORK IN PROGRESS!!

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 4
#define DATA_PIN 7
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

void stand();
void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);
  }
}

void ledplay() {
  static uint8_t hue = 0;
  Serial.print("x");
  // First slide the led in one direction
  for (int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red
    hue = hue + 5;
    leds[i] = CHSV(hue, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(5);
  }
  Serial.print("x");

  // Now go in the other direction.
  for (int i = (NUM_LEDS) - 1; i >= 0; i--) {
    // Set the i'th led to red
    hue = hue + 5;
    leds[i] = CHSV(hue, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(5);
  }
}
void setup() {
  Serial.begin(9600);

  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(64);


  pwm.begin();
  /*
     In theory the internal oscillator (clock) is 25MHz but it really isn't
     that precise. You can 'calibrate' this by tweaking this number until
     you get the PWM update frequency you're expecting!
     The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
     is used for calculating things like writeMicroseconds()
     Analog servos run at ~50 Hz updates, It is importaint to use an
     oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
     1) Attach the oscilloscope to one of the PWM signal pins and ground on
        the I2C PCA9685 chip you are setting the value for.
     2) Adjust setOscillatorFrequency() until the PWM update frequency is the
        expected value (50Hz for most ESCs)
     Setting the value here is specific to each individual I2C PCA9685 chip and
     affects the calculations for the PWM update frequency.
     Failure to correctly set the int.osc value will cause unexpected PWM results
  */
  pwm.setOscillatorFrequency(23000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  for (int i = 0; i < 16; i++) {
    pwm.setPWM(i, 0, 375);
  }


  stand();
  delay(2000);
}

// You can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!
//void setServoPulse(uint8_t n, double pulse) {
//  double pulselength;
//
//  pulselength = 1000000;   // 1,000,000 us per second
//  pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates
//  Serial.print(pulselength); Serial.println(" us per period");
//  pulselength /= 4096;  // 12 bits of resolution
//  Serial.print(pulselength); Serial.println(" us per bit");
//  pulse *= 1000000;  // convert input seconds to us
//  pulse /= pulselength;
//  Serial.println(pulse);
//  pwm.setPWM(n, 0, pulse);
//}

//////////////////////////////////////////////////////////////

void stand() {
  pwm.setPWM(0, 0, 375);
  pwm.setPWM(1, 0, 375);
  pwm.setPWM(2, 0, 375);
  pwm.setPWM(3, 0, 375);
  //
  pwm.setPWM(4, 0, 600); // 左2-1
  // 5-->8
  pwm.setPWM(5, 0, 600); //2-2
  //
  pwm.setPWM(6, 0, 220); //右2-3
  pwm.setPWM(7, 0, 220); //2-4

}
//=================== LEFT
void frontleftfoot(String command, int duration) {
  // servo 0
  if (command == "center") {
    pwm.setPWM(0, 0, 375); // 真ん中に
  }
  if (command == "forward") {
    pwm.setPWM(0, 0, 275); // 前に
  }
  if (command == "backward") {
    pwm.setPWM(0, 0, 475); // 後ろに
  }
  delay(duration);

}
void frontlefttoe(String command, int duration) {
  // servo 0
  if (command == "level") {
    pwm.setPWM(4, 0, 375); // 水平に
  }
  if (command == "low") {
    pwm.setPWM(4, 0, 475); // ちょっと低い
  }
  if (command == "down") {
    pwm.setPWM(4, 0, 575); // 完全に縦に
  }
  delay(duration);

}
//
void backleftfoot(String command, int duration) {
  // servo 0
  if (command == "center") {
    pwm.setPWM(1, 0, 375); // 真ん中に
  }
  if (command == "forward") {
    pwm.setPWM(1, 0, 275); // 前に
  }
  if (command == "backward") {
    pwm.setPWM(1, 0, 475); // 後ろに
  }
  delay(duration);

}
void backlefttoe(String command, int duration) {
  // servo 0
  if (command == "level") {
    pwm.setPWM(5, 0, 375); // 水平に
    Serial.println("level done");
  }
  if (command == "low") {
    pwm.setPWM(5, 0, 475); // ちょっと低い
    Serial.println("low done");
  }
  if (command == "down") {
    pwm.setPWM(5, 0, 575); // 完全に縦に
    Serial.println("down done");
  }
  delay(duration);
}
//============ RIGHT
void frontrightfoot(String command, int duration) {
  // servo 0
  if (command == "center") {
    pwm.setPWM(3, 0, 375); // 真ん中に
  }
  if (command == "forward") {
    pwm.setPWM(3, 0, 475); // 前に
  }
  if (command == "backward") {
    pwm.setPWM(3, 0, 275); // 後ろに
  }
  delay(duration);

}
void frontrighttoe(String command, int duration) {
  // servo 0
  if (command == "level") {
    pwm.setPWM(7, 0, 375); // 水平に
  }
  if (command == "low") {
    pwm.setPWM(7, 0, 300); // ちょっと低い
  }
  if (command == "down") {
    pwm.setPWM(7, 0, 200); // 完全に縦に
  }
  delay(duration);

}
void backrightfoot(String command, int duration) {
  // servo 0
  if (command == "center") {
    pwm.setPWM(2, 0, 375); // 真ん中に
  }
  if (command == "forward") {
    pwm.setPWM(2, 0, 475); // 前に
  }
  if (command == "backward") {
    pwm.setPWM(2, 0, 275); // 後ろに
  }
  delay(duration);

}
void backrighttoe(String command, int duration) {
  // servo 0
  if (command == "level") {
    pwm.setPWM(6, 0, 375); // 水平に
  }
  if (command == "low") {
    pwm.setPWM(6, 0, 300); // ちょっと低い
  }
  if (command == "down") {
    pwm.setPWM(6, 0, 200); // 完全に縦に
  }
  delay(duration);

}

void step1() {
  int duration = 150;
  backrighttoe("low", duration);
  frontlefttoe("level", duration);
  frontleftfoot("forward", duration);
  frontlefttoe("down", duration);
  backrighttoe("down", duration);
  frontlefttoe("low", duration);
  frontleftfoot("back", duration);
  backrighttoe("level", duration);

  backrightfoot("forward", duration);
  frontlefttoe("down", duration);
  backrighttoe("down", duration);
  //
  //  frontrighttoe("level",duration);
  backlefttoe("level", duration);
  //
  frontleftfoot("center", duration);
  backrightfoot("center", duration);
  backlefttoe("down", duration);

}

void step2() {
  int duration = 150;
  backlefttoe("low", duration);
  frontrighttoe("level", duration);
  frontrightfoot("forward", duration);
  frontrighttoe("down", duration);
  backlefttoe("down", duration);
  frontrighttoe("low", duration);
  frontrightfoot("back", duration);
  backlefttoe("level", duration);

  backleftfoot("forward", duration);
  frontrighttoe("down", duration);
  backlefttoe("down", duration);
  //
  //  frontlefttoe("level",duration);
  backrighttoe("level", duration);
  //
  frontrightfoot("center", duration);
  backleftfoot("center", duration);
  backrighttoe("down", duration);
}

void crawl() {
  int duration = 100;
  frontleftfoot("center", duration);
  frontrightfoot("center", duration);
  backleftfoot("center", duration);
  backrightfoot("center", duration);
  //
  frontlefttoe("level", duration);
  backlefttoe("level", duration);
  frontrighttoe("level", duration);
  backrighttoe("level", duration);
  //
  frontleftfoot("forward", duration);
  frontrightfoot("forward", duration);
  backleftfoot("forward", duration);
  backrightfoot("forward", duration);
  //
  frontlefttoe("down", duration);
  backlefttoe("down", duration);
  frontrighttoe("down", duration);
  backrighttoe("down", duration);
  //
  frontleftfoot("backward", duration);
  frontrightfoot("backward", duration);
  backleftfoot("backward", duration);
  backrightfoot("backward", duration);
  //
  frontlefttoe("level", duration);
  backlefttoe("level", duration);
  frontrighttoe("level", duration);
  backrighttoe("level", duration);


}
void crawl2() {
  int duration = 100;
  backrighttoe("level", duration);
  backrightfoot("forward", duration);
  backrighttoe("down", duration);
  //step2
  frontrighttoe("level", duration);
  frontrightfoot("forward", duration);
  frontrighttoe("down", duration);
  //move
  frontrightfoot("center", duration);
  backrightfoot("center", duration);
  frontleftfoot("backward", duration);
  backleftfoot("backward", duration);
  //
  backlefttoe("level", duration);
  backleftfoot("forward", duration);
  backlefttoe("down", duration);
  //
  frontlefttoe("level", duration);
  frontleftfoot("forward", duration);
  frontlefttoe("down", duration);
  //move
  frontrightfoot("backward", duration);
  backrightfoot("backward", duration);
  frontleftfoot("center", duration);
  backleftfoot("center", duration);



}
//
void swing() {
  int duration = 100;
  frontleftfoot("center", duration);
  backleftfoot("center", duration);
  frontrightfoot("center", duration);
  backrightfoot("center", duration);
  //
  frontleftfoot("forward", duration);
  backleftfoot("forward", duration);
  frontrightfoot("forward", duration);
  backrightfoot("forward", duration);
  //
  frontleftfoot("backward", duration);
  backleftfoot("backward", duration);
  frontrightfoot("backward", duration);
  backrightfoot("backward", duration);
  //
  frontleftfoot("center", duration);
  backleftfoot("center", duration);
  frontrightfoot("center", duration);
  backrightfoot("center", duration);
  delay(300);

}
void testSeq() {
  int duration = 500;
  //  //
  //  frontleftfoot("center", duration);
  //  frontleftfoot("forward", duration);
  //  frontleftfoot("backward", duration);
  //  frontleftfoot("center", duration);
  //  //
  //  frontlefttoe("level", duration);
  //  frontlefttoe("low", duration);
  //  frontlefttoe("down", duration);
  //  frontlefttoe("level", duration);
  //  //
  //    backleftfoot("center", duration);
  //    backleftfoot("forward", duration);
  //    backleftfoot("backward", duration);
  //    backleftfoot("center", duration);
  //  //  //
  //    backlefttoe("level", duration);
  //    backlefttoe("low", duration);
  //    backlefttoe("down", duration);
  //    backlefttoe("level", duration);
  //  //
  //  frontrightfoot("center", duration);
  //  frontrightfoot("forward", duration);
  //  frontrightfoot("backward", duration);
  //  frontrightfoot("center", duration);
  //  //
  //  frontrighttoe("level", duration);
  //  frontrighttoe("low", duration);
  //  frontrighttoe("down", duration);
  //  frontrighttoe("level", duration);
  //  //
  //  backrightfoot("center", duration);
  //  backrightfoot("forward", duration);
  //  backrightfoot("backward", duration);
  //  backrightfoot("center", duration);
  //  //
  //  backrighttoe("level", duration);
  //  backrighttoe("low", duration);
  //  backrighttoe("down", duration);
  //  backrighttoe("level", duration);
  //
  //  step1();
  //  step2();
  //   crawl();
  crawl2();


  //  frontrighttoe("level", duration);
  //  frontrighttoe("low", duration);
  //  frontrighttoe("down", duration);
  //  frontrighttoe("level", duration);
  //
  //  frontlefttoe("level", duration);
  //  frontlefttoe("low", duration);
  //  frontlefttoe("down", duration);
  //  frontlefttoe("level", duration);
  //
  //  backlefttoe("level", duration);
  //  backlefttoe("low", duration);
  //  backlefttoe("down", duration);
  //  backlefttoe("level", duration);
  //
  //  backrighttoe("level", duration);
  //  backrighttoe("low", duration);
  //  backrighttoe("down", duration);
  //  backrighttoe("level", duration);


  //for(int i=275; i<575;i++){
  //   pwm.setPWM(5, 0, i);
  //   delay(50);
  //}
  //for(int i=575; i>275;i--){
  //   pwm.setPWM(5, 0, i);
  //   delay(50);
  //}


}
void loop() {
  ledplay();
  stand();
  //  delay(1000);
  testSeq();
}
