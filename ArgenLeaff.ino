#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#define PIN 5          // Pin al que está conectada la tira LED
#define NUMPIXELS 36   // Número de LEDs en la tira - 12 leds en 40cm

SoftwareSerial ARGLF(10, 11);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int estado = 0;
int nuevoEstado = 1;
String mainString;

const int THRESHOLD_LOW = 200;   // Ajusta según el ruido de fondo
const int THRESHOLD_HIGH = 800;  // Ajusta según el nivel que deseas detectar

uint32_t currentColor = strip.Color(0, 0, 0);  // Color actual de los LEDs
bool colorChanged = false;  // Indica si se ha cambiado el color

void setup() {
  ARGLF.begin(9600);  // Inicializa la comunicación del módulo Bluetooth
  Serial.begin(9600); // Inicializa la comunicación del puerto serie del Arduino para depuración
  pinMode(9, OUTPUT);
  strip.begin();
  strip.show();       // Inicializa la tira LED
  delay(3000);
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
    
    if (ARGLF.available() > 0) {
    nuevoEstado = ARGLF.read();
    if (nuevoEstado != estado) {
      estado = nuevoEstado;
    }
  }
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
      Serial.println("COLORES");
      ARGLF.println("COLORES");
      colores();
      break;
    case 'd':
      Serial.println("ARCOIRIS");
      ARGLF.println("ARCOIRIS");
      arcoiris();
      break;
    case 'e':
      Serial.println("LATIDO");
      ARGLF.println("LATIDO");
      latido();
      break;
    case 'f':
      Serial.println("COMETA");
      ARGLF.println("COMETA");
      cometa();
      break;
    case 'g':
      Serial.println("CIELO");
      ARGLF.println("CIELO");
      cielo();
      break;
    case 'h':
      Serial.println("FUEGO");
      ARGLF.println("FUEGO");
      chispa();
      break;
    case 'i':
      Serial.println("DESTELLOS");
      ARGLF.println("DESTELLOS");
      destellos();
      break;
    case 'z':
      Serial.println("APAGAR");
      ARGLF.println("APAGAR");
      apagarluces();
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
    delay(75);
    
    // Verifica si hay un cambio de estado
    if (ARGLF.available() > 0) {
      nuevoEstado = ARGLF.read();
      if (nuevoEstado != estado) {
        estado = nuevoEstado;
        return; // Salir para permitir cambios de estado
      }
    }
  }
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0)); // Rojo
    strip.show();
    delay(75);
    
    // Verifica si hay un cambio de estado
    if (ARGLF.available() > 0) {
      nuevoEstado = ARGLF.read();
      if (nuevoEstado != estado) {
        estado = nuevoEstado;
        return; // Salir para permitir cambios de estado
      }
    }
  }
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255)); // Azul
    strip.show();
    delay(75);
    
    // Verifica si hay un cambio de estado
    if (ARGLF.available() > 0) {
      nuevoEstado = ARGLF.read();
      if (nuevoEstado != estado) {
        estado = nuevoEstado;
        return; // Salir para permitir cambios de estado
      }
    }
  }
}

void apagarluces() {
    setColor(0, 0, 0); // Apaga todos los LEDs

    if (ARGLF.available() > 0) {
      nuevoEstado = ARGLF.read();
    if (nuevoEstado != estado) {
      estado = nuevoEstado;
      return; // Salir para permitir cambios de estado
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
  int delayTime = 10;
  
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

void colores() {
    uint32_t colores[] = {
        strip.Color(255, 0, 0),    // Rojo
        strip.Color(255, 127, 0),  // Naranja
        strip.Color(255, 255, 0),  // Amarillo
        strip.Color(0, 255, 0),    // Verde
        strip.Color(0, 255, 255),  // Cian
        strip.Color(0, 0, 255),    // Azul
        strip.Color(127, 0, 255),  // Púrpura
        strip.Color(255, 0, 255)   // Magenta
    };

    int numColors = sizeof(colores) / sizeof(colores[0]);
    int numPixels = strip.numPixels();

    for (int j = 0; j < numPixels; j++) {
        int colorIndex = j / 4 % numColors; // Cambia de color cada 4 LEDs
        uint32_t color = colores[colorIndex];
        strip.setPixelColor(j, color);
    }
    strip.show();

    // Mantiene el color fijo hasta que cambie el estado
    while (true) {
        // Verifica si hay un cambio de estado
        if (ARGLF.available() > 0) {
            nuevoEstado = ARGLF.read();
            if (nuevoEstado != estado) {
                estado = nuevoEstado;
                return; // Salir de la función para permitir cambios de estado
            }
        }
        delay(50); // Pequeño retraso para evitar sobrecargar el loop
    }
}




void arcoiris() {
    int numPixels = strip.numPixels();
    int delayTime = 20; // Tiempo entre cambios
    int hue = 0; // Inicializa el matiz

    while (true) {
        // Establece el color para cada LED
        for (int i = 0; i < numPixels; i++) {
            uint32_t color = strip.ColorHSV((hue + (i * 65536 / numPixels)) & 0xFFFF);
            strip.setPixelColor(i, color);
        }

        strip.show();
        hue += 256; // Aumenta el matiz para el movimiento del arcoíris
        delay(delayTime);

        // Verifica cambios de estado
        if (ARGLF.available() > 0) {
            nuevoEstado = ARGLF.read();
            if (nuevoEstado != estado) {
                estado = nuevoEstado;
                return; // Salir para permitir cambios de estado
            }
        }
    }
}



void latido() {
    uint32_t colors[] = {
        strip.Color(255, 0, 0),    // Rojo
        strip.Color(0, 255, 0),    // Verde
        strip.Color(0, 0, 255),    // Azul
        strip.Color(255, 255, 0),  // Amarillo
        strip.Color(255, 0, 255),  // Magenta
        strip.Color(0, 255, 255)   // Cian
    };

    int numColors = sizeof(colors) / sizeof(colors[0]);
    int delayTime = 100;

    while (true) {
        uint32_t color = colors[random(numColors)];

        // Incrementar brillo
        for (int brightness = 0; brightness <= 255; brightness += 5) {
            for (int i = 0; i < strip.numPixels(); i++) {
                int r = (color >> 16) & 0xFF;
                int g = (color >> 8) & 0xFF;
                int b = color & 0xFF;

                strip.setPixelColor(i, strip.Color(r * brightness / 255, g * brightness / 255, b * brightness / 255));
            }
            strip.show();
            delay(delayTime);

            // Verifica si hay un cambio de estado
            if (ARGLF.available() > 0) {
                nuevoEstado = ARGLF.read();
                if (nuevoEstado != estado) {
                    estado = nuevoEstado;
                    return;  // Salir de la función para permitir cambios de estado
                }
            }
        }

        // Disminuir brillo
        for (int brightness = 255; brightness >= 0; brightness -= 5) {
            for (int i = 0; i < strip.numPixels(); i++) {
                int r = (color >> 16) & 0xFF;
                int g = (color >> 8) & 0xFF;
                int b = color & 0xFF;

                strip.setPixelColor(i, strip.Color(r * brightness / 255, g * brightness / 255, b * brightness / 255));
            }
            strip.show();
            delay(delayTime);

            // Verifica si hay un cambio de estado
            if (ARGLF.available() > 0) {
                nuevoEstado = ARGLF.read();
                if (nuevoEstado != estado) {
                    estado = nuevoEstado;
                    return;  // Salir de la función para permitir cambios de estado
                }
            }
        }
    }
}




// Nuevo modo COMET
void cometa() {
  uint32_t cometColor = strip.Color(255, 255, 255);  // Color del cometa (blanco)
  int cometSize = 5;  // Tamaño del cometa
  int delayTime = 50; // Tiempo de retraso entre movimientos

  for (int pos = 0; pos < strip.numPixels() + cometSize; pos++) {
    strip.clear();  // Limpia la tira

    // Dibuja el cometa en la tira
    for (int i = 0; i < cometSize; i++) {
      int pixel = pos - i;
      if (pixel >= 0 && pixel < strip.numPixels()) {
        strip.setPixelColor(pixel, cometColor);
      }
    }

    strip.show();  // Muestra el cometa en la tira
    delay(delayTime);

    // Verifica si hay un cambio de estado
    if (ARGLF.available() > 0) {
      nuevoEstado = ARGLF.read();
      if (nuevoEstado != estado) {
        estado = nuevoEstado;
        return;  // Salir de la función para permitir cambios de estado
      }
    }
  }
}


void cielo() {
    int numPixels = strip.numPixels();
    int delayTime = 30; // Tiempo entre cambios
    float waveLength = 10; // Longitud de onda
    float amplitude = 127; // Amplitud del color

    while (true) {
        for (int i = 0; i < numPixels; i++) {
            float wave = sin((i + millis() / 100.0) / waveLength) * amplitude;
            uint32_t color = strip.Color((wave + amplitude) / 2, (wave + amplitude * 0.5) / 2, (wave + amplitude * 0.2) / 2);
            strip.setPixelColor(i, color);
        }
        strip.show();
        delay(delayTime);

        // Verifica cambios de estado
        if (ARGLF.available() > 0) {
            nuevoEstado = ARGLF.read();
            if (nuevoEstado != estado) {
                estado = nuevoEstado;
                return; // Salir para permitir cambios de estado
            }
        }
    }
}





void chispa() {
    int numPixels = strip.numPixels();
    int delayTime = 50; // Tiempo entre cambios
    float pulseSpeed = 0.05; // Velocidad del pulso
    int maxBrightness = 255; // Brillo máximo

    while (true) {
        // Crea un pulso de luz que se expande
        for (int brightness = 0; brightness < maxBrightness; brightness++) {
            for (int i = 0; i < numPixels; i++) {
                uint32_t color = strip.Color(
                    (sin((i + millis() * pulseSpeed) * 0.1) + 1) * brightness / 2,
                    (sin((i + millis() * pulseSpeed + 2) * 0.1) + 1) * brightness / 2,
                    (sin((i + millis() * pulseSpeed + 4) * 0.1) + 1) * brightness / 2
                );
                strip.setPixelColor(i, color);
            }
            strip.show();
            delay(delayTime);

            // Verifica cambios de estado
            if (ARGLF.available() > 0) {
                nuevoEstado = ARGLF.read();
                if (nuevoEstado != estado) {
                    estado = nuevoEstado;
                    return; // Salir para permitir cambios de estado
                }
            }
        }

        // Crea un pulso de luz que se contrae
        for (int brightness = maxBrightness; brightness >= 0; brightness--) {
            for (int i = 0; i < numPixels; i++) {
                uint32_t color = strip.Color(
                    (sin((i + millis() * pulseSpeed) * 0.1) + 1) * brightness / 2,
                    (sin((i + millis() * pulseSpeed + 2) * 0.1) + 1) * brightness / 2,
                    (sin((i + millis() * pulseSpeed + 4) * 0.1) + 1) * brightness / 2
                );
                strip.setPixelColor(i, color);
            }
            strip.show();
            delay(delayTime);

            // Verifica cambios de estado
            if (ARGLF.available() > 0) {
                nuevoEstado = ARGLF.read();
                if (nuevoEstado != estado) {
                    estado = nuevoEstado;
                    return; // Salir para permitir cambios de estado
                }
            }
        }
    }
}












void destellos() {
  int numPixels = strip.numPixels();
  int waveHeight = 128; // Altura de la ola
  int waveWidth = 20; // Ancho de la ola
  int delayTime = 50; // Velocidad de la ola

  // Almacena el valor de la fase actual para crear un ciclo continuo
  static int phase = 0;

  while (true) {
    for (int i = 0; i < numPixels; i++) {
      // Calcula la intensidad del color basado en la posición y la fase actual
      int brightness = (sin((i + phase) * 2 * PI / waveWidth) * waveHeight + waveHeight) / 2;
      strip.setPixelColor(i, strip.Color(brightness, 0, 255 - brightness)); // Gradiente de color
    }
    strip.show();
    delay(delayTime);
    
    // Incrementa la fase para el movimiento de la ola
    phase++;

    // Verifica cambios de estado
    if (ARGLF.available() > 0) {
      nuevoEstado = ARGLF.read();
      if (nuevoEstado != estado) {
        estado = nuevoEstado;
        return; // Salir para permitir cambios de estado
      }
    }
  }
}
