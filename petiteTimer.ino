#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TM1637Display.h>

int n = 0,
    mins = 0,
    secs = 0,
    hors = 0,
    res = 0,
    displayTime = 0, // El contador display
    displayTimer = 0, // El tiempo a contar display
    contador = 0, // El tiempo a contar
    contH = 0, // Horas por contar
    contM = 0, // Minutos por contar
    flagBtn1 = 0,
    flagBtn2 = 0,
    flagBtn3 = 0,
    btnState1 = 0,
    btnState2 = 0,
    btnState3 = 0,
    preTipo1 = 0,
    preTipo2 = 0,
    preTipo3 = 0,
    tipo = 0; // Tpo de medición, 1 = minutos, 2 = horas, 0 = sn valor, hay que elegir, se muestra HOLA
unsigned long tiempo = 0;
const int PLED = 13, // EL led
          CLK = 4, // Set the CLK pin connection to the display
          DIO = 5, // Set the DIO pin connection to the display
          PSET1 = 9, // Boton 1, define el tipo de medición
          PSET2 = 10, // Boton 1, define el tiempo a contar
          PSET3 = 11; // Botón 3, empieza/detiene la acción

byte HOLA[] = {
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,             // H
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,     // O
  SEG_D | SEG_E | SEG_F,                             // L
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G      // A
},
MINS[] = {
  SEG_B | SEG_A | SEG_F | SEG_E,                    // M
  SEG_F | SEG_A | SEG_B | SEG_C,                    // I
  SEG_B | SEG_C,                                    // S
  SEG_A | SEG_E | SEG_F | SEG_B | SEG_C             // N
},
HORS[] = {
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,             // H
  SEG_E | SEG_G | SEG_C | SEG_D,                     // O
  SEG_E | SEG_G,                                     // r
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G      // A
};

TM1637Display display(CLK, DIO);  //set up the 4-Digit Display.
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(PLED, OUTPUT);
  pinMode(PSET1, OUTPUT);
  pinMode(PSET2, OUTPUT);
  pinMode(PSET3, OUTPUT);

  display.setBrightness(0x0a);
  display.setSegments(HOLA);
  display.setColon(true);

  digitalWrite(PLED, false);

  lcd.begin();
  lcd.backlight();
  lcd.print("rTimer v0.0.0");
  lcd.setCursor(0, 1);
}
void loop() {
  btnState1 = digitalRead(PSET1);
  btnState2 = digitalRead(PSET2);
  btnState3 = digitalRead(PSET3);
  // Controla el botón 1, tipo de conteo, con un flag que envita hacer cambios mientras se mantiene presionado el botón
  if (btnState1 == HIGH) {
    if (flagBtn1 == 0) {
      switch (tipo) {
        case 0:
          tipo = 1;
          contador = 0;
          break;
        case 1:
          tipo = 2;
          contador = 0;
          break;
        case 2:
          tipo = 0;
          contador = 0;
          break;
      }
      flagBtn1 = 1;
    }
    lcd.clear();
    lcd.print("rTimer v0.0.0");
    lcd.setCursor(0, 1);
    lcd.print("btn1 : ");
    lcd.print(tipo);
  } else {
    flagBtn1 = 0;
  }

  // Controla el botón 2, tiempo a contar, con un flag que envita hacer cambios mientras se mantiene presionado el botón
  if (btnState2 == HIGH) {
    if (tipo > 0) {
      if (flagBtn2 == 0) {
        contador++;
        flagBtn2 = 1;
      }
      lcd.clear();
      lcd.print("rTimer v0.0.0");
      lcd.setCursor(0, 1);
      lcd.print("Defino ");
      switch (tipo) {
        case 1:
          lcd.print(" min: ");
          if (contador == 60) {
            contador = 0;
          }
          displayTimer = (contH * 100) + contador;
          display.showNumberDec(displayTimer, true);
          break;
        case 2:
          lcd.print(" hora: ");
          displayTimer = contador * 100;
          display.showNumberDec(displayTimer, true);
          break;
      }
      lcd.print(contador);
    }
  } else {
    flagBtn2 = 0;
  }

  if (contador == 0) {
    if (tipo == 1) {
      display.setSegments(MINS);
    } else if (tipo == 2) {
      display.setSegments(HORS);
    } else {
      display.setSegments(HOLA);
    }
  }

}

void componeTiempo(int valor) {
  valor = (mins * 100) + secs;
}

void conteoMinutos() {
  tiempo = millis();
  res = tiempo % 1000;
  if (res == 0) {
    secs++;
    if (secs == 60) {
      mins++;
      secs = 0;
    }
    if (mins == 60) {
      hors ++;
      mins = 0;
    }
    componeTiempo(displayTime);
    display.showNumberDec(displayTime, true);
  }
}