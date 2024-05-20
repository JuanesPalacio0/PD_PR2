# Parte 1

Esta parte del código maneja un botón conectado al pin 23 del microcontrolador. La funcionalidad incluye:

- Definición de una estructura `Button` para almacenar la información del botón.
- Configuración de una interrupción para contar las veces que se presiona el botón.
- Desactivación de la interrupción después de 1 minuto.

**Funciones clave:**

- `isr()`: Interrupción que se activa al presionar el botón.
- `setup()`: Configuración inicial del puerto serial y la interrupción del botón.
- `loop()`: Comprobación y registro de las presiones del botón y desactivación de la interrupción después de 1 minuto.

```cpp
#include <Arduino.h>

struct Button {
    const uint8_t PIN;
    uint32_t numberKeyPresses;
    bool pressed;
};

Button button1 = {23, 0, false};

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

    // Detach Interrupt after 1 Minute
    if (millis() - lastMillis > 60000) {
        lastMillis = millis();
        detachInterrupt(button1.PIN);
        Serial.println("Interrupt Detached!");
    }
}



# Parte 2
Esta parte del código maneja un temporizador de hardware que genera interrupciones periódicas.

Utiliza un temporizador de hardware para incrementar un contador de interrupciones.
Cada vez que ocurre una interrupción, se incrementa y muestra un contador total de interrupciones.

**Funciones clave:**
- onTimer(): Interrupción del temporizador que incrementa el contador de interrupciones.
- setup(): Configuración inicial del puerto serial y del temporizador de hardware.
- loop(): Comprobación y registro del número total de interrupciones ocurridas.

#include <Arduino.h>

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


