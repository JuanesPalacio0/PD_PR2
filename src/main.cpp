#include <Arduino.h>
/* 
///////////////////////// Parte 1  //////////////////////////////////////////////////////////////////////////////////////////////////////
struct Button{
const uint8_t PIN;
uint32_t numberKeyPresses;
bool pressed;
};


Button button1 = {23,0,false};


void IRAM_ATTR isr() {
button1.numberKeyPresses += 1;
button1.pressed = true;
}


static uint32_t lastMillis = 0;


void setup() {
Serial.begin(115200);
delay(1000);
Serial.println(button1.PIN);
pinMode(button1.PIN, INPUT_PULLUP);
attachInterrupt(button1.PIN, isr, FALLING);
}


void loop() {
if (button1.pressed) {
Serial.printf("Button 1 has been pressed");
button1.pressed = false;
}


//Detach Interrupt after 1 Minute
if (millis() - lastMillis > 60000) {
lastMillis = millis();
detachInterrupt(button1.PIN);
Serial.println("Interrupt Detached!");
}
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////    Parte 2 //////////////////////////////////////////////////
volatile int interruptCounter;
int totalInterruptCounter;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR onTimer() {
portENTER_CRITICAL_ISR(&timerMux);
interruptCounter++;
portEXIT_CRITICAL_ISR(&timerMux);
}
void setup() {
Serial.begin(115200);
timer = timerBegin(0, 10, true);
timerAttachInterrupt(timer, &onTimer, true);
timerAlarmWrite(timer, 500000, true);
timerAlarmEnable(timer);
}
void loop() {
if (interruptCounter > 0) {
portENTER_CRITICAL(&timerMux);
interruptCounter--;
portEXIT_CRITICAL(&timerMux);
totalInterruptCounter++;
Serial.print("An interrupt has occurred. Total number: ");
Serial.println(totalInterruptCounter);
}
}
