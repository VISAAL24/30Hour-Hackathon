# MentorBitDHT11

Librería para la lectura de temperatura y humedad utilizando el sensor DHT11 en módulos compatibles con MentorBit.

## Descripción

La librería `MentorBitDHT11` facilita la lectura de datos de temperatura y humedad relativa desde el sensor DHT11 en módulos compatibles con MentorBit. Permite obtener mediciones precisas para aplicaciones de monitoreo ambiental, control climático y proyectos de domótica.

**Nota:** La librería depende de la librería `DHT.h`.

## Modo de Empleo

1.  **Instalación:**
    * Abre el IDE compatible con MentorBit.
    * Ve a "Herramientas" -> "Gestionar librerías..."
    * Busca "MentorBitDHT11" e instálala.
    * **Nota:** Asegúrate de que la librería `DHT.h` esté instalada.

2.  **Ejemplo básico:**

    ```c++
    #include <MentorBitDHT11.h>

    MentorBitDHT11 dht(2); // Sensor DHT11 conectado al pin 2

    void setup() {
      Serial.begin(9600);
      Serial.println("Sensor DHT11 inicializado.");
    }

    void loop() {
      float temperatura = dht.obtenerTemperatura();
      float humedad = dht.obtenerHumedad();

      if (isnan(temperatura) || isnan(humedad)) {
        Serial.println("Error al leer el sensor DHT11.");
        delay(2000); // Espera antes de reintentar la lectura
        return;
      }

      Serial.print("Temperatura: ");
      Serial.print(temperatura);
      Serial.println(" °C");

      Serial.print("Humedad: ");
      Serial.print(humedad);
      Serial.println(" %");

      delay(2000);
    }
    ```

## Constructor y Métodos Públicos

### Constructor

* `MentorBitDHT11(uint8_t pin = 0)`: Crea un objeto `MentorBitDHT11`.
    * `pin`: Número de pin al que está conectado el sensor DHT11. Si no se especifica, se asume que no está conectado a ningún pin inicialmente.

### Métodos

* `float obtenerTemperatura()`: Lee la temperatura en grados Celsius.
* `float obtenerHumedad()`: Lee la humedad relativa en porcentaje.

### Métodos Privados

* `void _reiniciarDHT()`: Re-inicializa el sensor DHT11.
