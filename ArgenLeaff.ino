#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#define PIN 6          // Pin al que está conectada la tira LED
#define NUMPIXELS 48   // Número de LEDs en la tira - 12 leds en 40cm

SoftwareSerial ARGLF(10, 11);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int estado = 0;
int nuevoEstado = 0;
String mainString;

const int THRESHOLD_LOW = 0;    // Umbral bajo de decibelios
const int THRESHOLD_HIGH = 30;   // Umbral alto de decibelios

void setup() {
  ARGLF.begin(9600);  // Inicializa la comunicación del módulo Bluetooth
  Serial.begin(9600); // Inicializa la comunicación del puerto serie del Arduino para depuración
  pinMode(9, OUTPUT);
  strip.begin();
  strip.show();       // Inicializa la tira LED
  Serial.println("Setup completo");
}

void loop() {
  // Lee datos del módulo Bluetooth
  if (ARGLF.available() > 0) {
    mainString = ARGLF.readString();
    byte firstCommaIndex = mainString.indexOf(',');
    byte secondCommaIndex = mainString.indexOf(',', firstCommaIndex + 1);

    String red = mainString.substring(0, firstCommaIndex);
    String green = mainString.substring(firstCommaIndex + 1, secondCommaIndex);
    String blue = mainString.substring(secondCommaIndex + 1);
    int g = red.toInt();
    int r = green.toInt();
    int b = blue.toInt();

    setColor(r, g, b); // Actualiza los LEDs con el color recibido

    Serial.println("Color recibido:");
    Serial.print("R: "); Serial.println(g);
    Serial.print("G: "); Serial.println(r);
    Serial.print("B: "); Serial.println(b);
  }

  // Ejecuta acciones según el estado recibido
  if (ARGLF.available() > 0) {
    nuevoEstado = ARGLF.read();
    if (nuevoEstado != estado) {
      estado = nuevoEstado;
    }
  }

  switch (estado) {
    case 'a':
      Serial.println("RGB");
      ARGLF.println("RGB");
      rgb();
      break;
    case 'b':
      Serial.println("FADE");
      ARGLF.println("FADE");
      fade();
      break;
    case 'c':
      Serial.println("BOUNCING RAINBOW");
      ARGLF.println("BOUNCING RAINBOW");
      bouncingRainbow();
      break;
    case 'd':
      Serial.println("MICROMODE");
      ARGLF.println("MICROMODE");
      micromode();
      break;
  }
}

void setColor(int r, int g, int b) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}

void rgb() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0)); // Verde
    strip.show();
    delay(50);
    if (ARGLF.available() > 0) {
      nuevoEstado = ARGLF.read();
      if (nuevoEstado != estado) {
        estado = nuevoEstado;
        return;
      }
    }
  }
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0)); // Rojo
    strip.show();
    delay(50);
    if (ARGLF.available() > 0) {
      nuevoEstado = ARGLF.read();
      if (nuevoEstado != estado) {
        estado = nuevoEstado;
        return;
      }
    }
  }
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255)); // Azul
    strip.show();
    delay(50);
    if (ARGLF.available() > 0) {
      nuevoEstado = ARGLF.read();
      if (nuevoEstado != estado) {
        estado = nuevoEstado;
        return;
      }
    }
  }
}

void fade() {
  uint32_t colors[] = {
    strip.Color(255, 0, 0),
    strip.Color(255, 127, 0),
    strip.Color(255, 255, 0),
    strip.Color(0, 255, 0),
    strip.Color(0, 255, 127),
    strip.Color(0, 255, 255),
    strip.Color(0, 127, 255),
    strip.Color(0, 0, 255),
    strip.Color(75, 0, 130),
    strip.Color(148, 0, 211),
    strip.Color(255, 0, 255),
    strip.Color(255, 0, 127),
    strip.Color(255, 20, 147),
    strip.Color(255, 69, 0),
    strip.Color(255, 105, 180),
    strip.Color(64, 224, 208),
    strip.Color(32, 178, 170),
    strip.Color(0, 128, 128),
    strip.Color(0, 100, 0),
    strip.Color(139, 0, 0)
  };
  
  int numSteps = 100;
  int delayTime = 5;
  
  for (int i = 0; i < 20; i++) {
    int nextColorIndex = (i + 1) % 20;
    uint32_t startColor = colors[i];
    uint32_t endColor = colors[nextColorIndex];
    
    int r1 = (startColor >> 16) & 0xFF;
    int g1 = (startColor >> 8) & 0xFF;
    int b1 = startColor & 0xFF;
    
    int r2 = (endColor >> 16) & 0xFF;
    int g2 = (endColor >> 8) & 0xFF;
    int b2 = endColor & 0xFF;
    
    for (int step = 0; step <= numSteps; step++) {
      int r = r1 + ((r2 - r1) * step / numSteps);
      int g = g1 + ((g2 - g1) * step / numSteps);
      int b = b1 + ((b2 - b1) * step / numSteps);
      
      setColor(r, g, b);
      delay(delayTime);

      if (ARGLF.available() > 0) {
        nuevoEstado = ARGLF.read();
        if (nuevoEstado != estado) {
          estado = nuevoEstado;
          return;
        }
      }
    }
  }
}

void bouncingRainbow() {
  uint32_t colores[] = {
    strip.Color(150, 0, 0),
    strip.Color(0, 150, 0),
    strip.Color(0, 0, 150),
    strip.Color(150, 150, 0),
    strip.Color(150, 0, 150),
    strip.Color(0, 150, 150),
    strip.Color(150, 150, 150)
  };

  int delayval = 50;
  int numColors = sizeof(colores) / sizeof(colores[0]);

  while (true) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.clear();
      strip.setPixelColor(i, colores[i % numColors]);
      strip.show();
      delay(delayval);

      if (ARGLF.available() > 0) {
        nuevoEstado = ARGLF.read();
        if (nuevoEstado != estado) {
          estado = nuevoEstado;
          return;
        }
      }
    }

    for (int i = strip.numPixels() - 1; i >= 0; i--) {
      strip.clear();
      strip.setPixelColor(i, colores[i % numColors]);
      strip.show();
      delay(delayval);

      if (ARGLF.available() > 0) {
        nuevoEstado = ARGLF.read();
        if (nuevoEstado != estado) {
          estado = nuevoEstado;
          return;
        }
      }
    }
  }
}

void micromode() {
  int sensorValue = analogRead(A0);  // Lee el valor analógico del micrófono (A0)

  // Determina el color basado en el nivel de sonido (dB)
  if (sensorValue < THRESHOLD_HIGH) {
    setColor(255, 0, 0);  // Rojo
  } else if (sensorValue > THRESHOLD_LOW) {
    setColor(0, 255, 0);  // Verde
  } else {
    setColor(0, 0, 255);  // Azul
  }

  // Asegúrate de que el cambio de estado se maneje fuera de la lectura Bluetooth en micromode
  // Esto significa que no se bloqueará el cambio de estado mientras estás en micromode
  if (ARGLF.available() > 0) {
    nuevoEstado = ARGLF.read();
    if (nuevoEstado != estado) {
      estado = nuevoEstado;
      // Aquí puedes agregar cualquier lógica adicional necesaria para cambiar de micromode a otro modo
    }
  }
}
