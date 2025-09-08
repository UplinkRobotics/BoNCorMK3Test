
#include "driver/ledc.h"
#include "motor_library.h"

#define RXD2 16
#define TXD2 17

#define BOARD_PWR 23
#define POWER_SW 26

#define INDICATOR_1 13
#define INDICATOR_2 18

#define LED_CH_1 4
#define LED_CH_2 0
#define LED_CH_3 5
#define LED_CH_4 19

#define SDA 21
#define SCL 22
#define IO2 2

#define BATTERY_VOLT_IO 27    // analog pin to the battery level voltage

Motors mot; // initialize motors

void setup(){

  Serial.begin(921600);     // debug info
  pinMode(BOARD_PWR, OUTPUT);
  digitalWrite(BOARD_PWR, LOW);
  pinMode(POWER_SW, OUTPUT);
  digitalWrite(POWER_SW, LOW); //set pins to starting value
  // Setup on board indicator LEDs
  pinMode(INDICATOR_1, OUTPUT);
  digitalWrite(INDICATOR_1, LOW);
  pinMode(INDICATOR_2, OUTPUT);
  digitalWrite(INDICATOR_2, LOW);

  pinMode(RXD2, OUTPUT);
  digitalWrite(RXD2, LOW);
  pinMode(TXD2, OUTPUT);
  digitalWrite(TXD2, LOW);
  pinMode(SDA, OUTPUT);
  digitalWrite(SDA, LOW);
  pinMode(SCL, OUTPUT);
  digitalWrite(SCL, LOW);
  pinMode(IO2, OUTPUT);
  digitalWrite(IO2, LOW);
  
  delay(1000);
  digitalWrite(BOARD_PWR, HIGH);
  mot.setup();
  
  mot.ledc_init(LEDC_CHANNEL_0, LED_CH_1, 12, 15625);
  mot.ledc_init(LEDC_CHANNEL_1, LED_CH_2, 12, 15625);
  mot.ledc_init(LEDC_CHANNEL_4, LED_CH_3, 12, 15625); // motors are on ch 2 & 3
  mot.ledc_init(LEDC_CHANNEL_5, LED_CH_4, 12, 15625);
  ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 4096); // headlights are off
  ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0); // apply the duty cycle
  ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 4096); // headlights are off
  ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1); // apply the duty cycle
  ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, 4096); // headlights are off
  ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4); // apply the duty cycle
  ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5, 4096); // headlights are off
  ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5); // apply the duty cycle

}

int throttle = 0;
int led1 = 4096;
int led2 = 4096;
int led3 = 4096;
int led4 = 4096;

void loop(){
  unsigned long time = millis();
  unsigned short LEDs = (((time) / 1000) % 6);
  switch (LEDs){
    case 0:
      digitalWrite(RXD2, HIGH);
      digitalWrite(IO2, LOW);
    break;
    case 1:
      digitalWrite(POWER_SW, HIGH);
      digitalWrite(RXD2, LOW);
    break;
    case 2:
      digitalWrite(SCL, HIGH);
      digitalWrite(POWER_SW, LOW);
    break;
    case 3:
      digitalWrite(SDA, HIGH);
      digitalWrite(SCL, LOW);
    break;
    case 4:
      digitalWrite(TXD2, HIGH);
      digitalWrite(SDA, LOW);
    break;
    case 5:
      digitalWrite(IO2, HIGH);
      digitalWrite(TXD2, LOW);
    break;
    default:
    break;
  }

  unsigned short timer = (((time) / 1000) % 8);
  switch (timer){
    case 0:
      throttle = constrain((time % 1000)/10, 0, 100);
      led1 = constrain(map(1000 - (time % 1000), 0, 1000, 0, 4096), 0, 4096);
      led4 = 4096;
    break;
    case 1:
      throttle = 100;
      led1 = constrain(map(time % 1000, 0, 1000, 0, 4096), 0, 4096);
    break;
    case 2:
      throttle = 100;
      led2 = constrain(map(1000 - (time % 1000), 0, 1000, 0, 4096), 0, 4096);
      led1 = 4096;
    break;
    case 3:
      throttle = constrain((100 - (time % 1000)/10), 0, 100);
      led2 = constrain(map(time % 1000, 0, 1000, 0, 4096), 0, 4096);
    break;
    case 4:
      throttle = constrain((100 - (time % 1000)/10), 0, 100) -100;
      led3 = constrain(map(1000 - (time % 1000), 0, 1000, 0, 4096), 0, 4096);
      led2 = 4096;
    break;
    case 5:
      throttle = -100;
      led3 = constrain(map(time % 1000, 0, 1000, 0, 4096), 0, 4096);
    break;
    case 6:
      throttle = -100;
      led4 = constrain(map(1000 - (time % 1000), 0, 1000, 0, 4096), 0, 4096);
      led3 = 4096;
    break;
    case 7:
      throttle = constrain((time % 1000)/10, 0, 100) - 100;
      led4 = constrain(map(time % 1000, 0, 1000, 0, 4096), 0, 4096);
    break;
    default:
      led1 = 0;
      led2 = 0;
      led3 = 0;
      led4 = 0;
    break;
  }
  //delay(100);
  //Serial.println(led4);
  mot.left_motors(throttle);
  mot.right_motors(throttle);

  ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, led1); // set to value
  ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0); // apply the duty cycle
  ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, led2); // set to value
  ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1); // apply the duty cycle
  ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4, led3); // set to value
  ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_4); // apply the duty cycle
  ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5, led4); // set to value
  ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_5); // apply the duty cycle

}

