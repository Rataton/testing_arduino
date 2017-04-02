/**
petiteTimer v 1.0.0
Cuenta regresiva de tiempo con dos modalidades
HORAS y MINUTOS
*/

#include <Wire.h>
#include <TM1637Display.h>

int n = 0,
    mins = 0,
    secs = 0,
    res = 0,
    resBlink = 0,
    displayTime = 0, // El contador display
    displayTimer = 0, // El tiempo a contar display
    contador = 0, // El tiempo a contar
    contH = 0, // Horas por contar
    contM = 0, // Minutos por contar
    restaH = 0, // Horas por contar
    restaM = 0, // Minutos por contar
    restaS = 0, // Minutos por contar
    flagBtn1 = 0,
    flagBtn2 = 0,
    flagBtn3 = 0,
    btnState1 = 0,
    btnState2 = 0,
    btnState3 = 0,
    tiempoTotal = 0, /// El total en minutos u horas a lo qu ehay que restar el conteo
    startingMillis = 0, // Varibale propiar de millis
    enEstatus = 0, // Flag para saber si está en modo conteo o en los otros
    accionTerminar = 0, // Flag para saber si ya se ha ejecutado la acció terminar
    tipo = 0, // Tpo de medición, 1 = minutos, 2 = horas, 0 = sn valor, hay que elegir, se muestra HOLA
    estado = 0; // Tipo de estado del timer 0 = parado, 1 = corriendo

bool cuentaMin = false; // Boleanos para definir si tiene que contar por minutos
bool terminado = false; // Boleano para saber si el conteo se ha terminado
bool blinkColon = true; // Boleano para tratar de hacer que el colon haga blink

unsigned long tiempo = 0;
const int
CLK = 4, // Set the CLK pin connection to the display
DIO = 5, // Set the DIO pin connection to the display
PRELE = 8, // EL rele
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
},
FINE[] = {
  SEG_A | SEG_F | SEG_G | SEG_E,                    // F
  SEG_B | SEG_C,                                    // I
  SEG_A | SEG_E | SEG_F | SEG_B | SEG_C,            // N
  SEG_A | SEG_F | SEG_G | SEG_E | SEG_D             // E
},
NEL[] = {
  SEG_G, // -
  SEG_G, // -
  SEG_G, // -
  SEG_G  // -
};

TM1637Display display(CLK, DIO);  //set up the 4-Digit Display.

void setup() {
  pinMode(PSET1, OUTPUT);
  pinMode(PSET2, OUTPUT);
  pinMode(PSET3, OUTPUT);
  pinMode(PRELE, OUTPUT);
  display.setBrightness(0x0a);
  display.setSegments(HOLA);
  display.setColon(blinkColon);
  setZeros();
  terminado = false;
}
void loop() {
  btnState1 = digitalRead(PSET1);
  btnState2 = digitalRead(PSET2);
  btnState3 = digitalRead(PSET3);
  // Controla el botón 1, tipo de conteo, con un flag que envita hacer cambios mientras se mantiene presionado el botón
  if (btnState1 == HIGH) {
    if (flagBtn1 == 0) {
      if (enEstatus == 1) {
        tipo = 0;
      } else {
        switch (tipo) {
          case 0:
            tipo = 1;
            break;
          case 1:
            tipo = 2;
            break;
          case 2:
            tipo = 0;
            break;
        }
      }
      setZeros();
      terminado = false;
    }
  } else {
    flagBtn1 = 0;
  }
  // Controla el botón 2, tiempo a contar, con un flag que envita hacer cambios mientras se mantiene presionado el botón
  if (btnState2 == HIGH) {
    if (tipo > 0 && enEstatus == 0) {
      if (flagBtn2 == 0) {
        contador++;
        flagBtn2 = 1;
      }
      switch (tipo) {
        case 1:
          if (contador == 60) {
            contador = 0;
            contH++;
          }
          displayTimer = (contH * 100) + contador;
          display.showNumberDec(displayTimer, true);
          break;
        case 2:
          if (contador > 25) contador = 24;
          displayTimer = contador * 100;
          display.showNumberDec(displayTimer, true);
          break;
      }
    }
  } else {
    flagBtn2 = 0;
  }
  // Controla el botón 3, inicio/parada timer
  if (btnState3 == HIGH) {
    if (tipo > 0 && displayTimer > 0) {
      if (flagBtn3 == 0) {
        switch (estado) {
          case 0:
            estado = 1;
            if (startingMillis == 0) {
              startingMillis = millis();
            }
            break;
          case 1:
            estado = 0;
            break;
        }
        enEstatus = 1;
        if (estado == 0) {
          cuentaMin = false;
        } else if (estado == 1) {
          cuentaMin = true;
        }
        flagBtn3 = 1;
      }
    } else {
      enEstatus = 0;
    }
  } else {
    flagBtn3 = 0;
  }
  // Lo que se ve en el display
  if (contador == 0 && enEstatus == 0 && cuentaMin == false) {
    if (tipo == 1) {
      display.setSegments(MINS);
    } else if (tipo == 2) {
      display.setSegments(HORS);
    } else {
      display.setSegments(HOLA);
    }
  }
  // Con boleanos función de conteo en el loop
  if (cuentaMin && accionTerminar == 0) {
    conteoMinutos();
  }
}
void setZeros() {
  contador = 0;
  contH = 0;
  enEstatus = 0;
  displayTimer = 0;
  flagBtn1 = 1;
  flagBtn2 = 0;
  flagBtn3 = 0;
  estado = 0;
  cuentaMin = false;
  startingMillis = 0;
  restaM = 0;
  restaS = 0;
  tiempoTotal = 0;
  secs = 0;
  accionTerminar = 0;
}
void terminar() {
  accionTerminar = 1;
  display.setSegments(FINE);
}
void conteoMinutos() {
  tiempo = millis() - startingMillis;
  res = tiempo % 1000;
  resBlink = tiempoTotal % 500;
  if (res == 0) {
    if (blinkColon) {
      blinkColon = false;
    } else if (!blinkColon) {
      blinkColon = true;
    }
    display.setColon(blinkColon);

    switch (tipo) {
      case 1: // Minutos
        tiempoTotal = (((contH * 60) + contador) * 60) - secs ; // Pasa el total de minutos a segundos y resta los que llevamos
        // Luego lo pasa a formaato visible
        restaM = floor(tiempoTotal / 60);
        restaS = tiempoTotal - (restaM * 60);
        tiempoTotal = (restaM * 100) +  restaS;
        break;
      case 2: // Horas
        tiempoTotal = ((contador * 60) * 60) - secs ; // Pasa el total de horas a segundos y resta los que llevamos
        // // Luego lo pasa a formaato visible
        restaH = floor((tiempoTotal / 60) / 60);
        restaM = (tiempoTotal - (restaH * 60 * 60)) / 60;
        tiempoTotal = (restaH * 100) + restaM;
        break;
    }
    if (tiempoTotal > 0) {
      display.showNumberDec(tiempoTotal, true);
      digitalWrite(PRELE, HIGH);
    } else if (tiempoTotal == 0) {
      display.showNumberDec(tiempoTotal, true);
      digitalWrite(PRELE, HIGH);
    } else if (tiempoTotal <= 0) {
      if (accionTerminar == 0) {
        terminar();
        digitalWrite(PRELE, LOW);
      }
      terminado = true;
    }
    secs++;
    displayTime = (mins * 100) + secs;
  }
}