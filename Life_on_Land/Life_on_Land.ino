#include <DHT.h>

// ---------------- DHT11 ----------------
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---------------- SOIL SENSOR ----------------
#define SOIL_PIN 34

// ---------------- FLAME SENSOR ----------------
#define FLAME_PIN 27

// ---------------- SMOKE SENSOR ----------------
#define SMOKE_PIN 26

// ---------------- GSM UART ----------------
#define RXD2 16
#define TXD2 17

// ---------------- OUTPUT ALERT PINS ----------------
#define TEMP_ALERT 18
#define HUM_ALERT 19
#define BOTH_ALERT 21
#define SOIL_ALERT 22
#define FLAME_ALERT 23
#define SMOKE_ALERT 25
#define ANIMAL_ALERT 33

String phoneNumber = "+91**********"; // Your mobile number to send sms alert

// Alert flags
bool tempAlertSent = false;
bool humAlertSent = false;
bool bothAlertSent = false;
bool soilAlertSent = false;
bool flameAlertSent = false;
bool smokeAlertSent = false;
bool animalAlertSent = false;

String incomingData = "";

// ---------------- SMS FUNCTION ----------------
void sendSMS(String message)
{
  Serial.println("Sending SMS...");

  Serial2.println("AT+CMGF=1");
  delay(1000);

  Serial2.print("AT+CMGS=\"");
  Serial2.print(phoneNumber);
  Serial2.println("\"");
  delay(1000);

  Serial2.print(message);
  delay(500);

  Serial2.write(26);
  delay(5000);
}

void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  dht.begin();

  pinMode(TEMP_ALERT, OUTPUT);
  pinMode(HUM_ALERT, OUTPUT);
  pinMode(BOTH_ALERT, OUTPUT);
  pinMode(SOIL_ALERT, OUTPUT);
  pinMode(FLAME_ALERT, OUTPUT);
  pinMode(SMOKE_ALERT, OUTPUT);
  pinMode(ANIMAL_ALERT, OUTPUT);

  pinMode(FLAME_PIN, INPUT);
  pinMode(SMOKE_PIN, INPUT);

  Serial.println("Smart Farm Monitoring System Started");
}

void loop()
{

  // ---------------- READ YOLO DATA ----------------
  if (Serial.available())
  {
    incomingData = Serial.readStringUntil('\n');
    incomingData.trim();   // FIXED: removes \r or spaces

    if (incomingData == "ANIMAL")
    {
      digitalWrite(ANIMAL_ALERT, HIGH);

      if (!animalAlertSent)
      {
        sendSMS("Warning: Wild animal detected in farm!");
        animalAlertSent = true;
      }
    }

    else if (incomingData == "FIRE")
    {
      digitalWrite(FLAME_ALERT, HIGH);

      if (!flameAlertSent)
      {
        sendSMS("Emergency: Fire detected in farm!");
        flameAlertSent = true;
      }
    }

    else
    {
      digitalWrite(ANIMAL_ALERT, LOW);
      animalAlertSent = false;
    }
  }

  // ---------------- SENSOR READINGS ----------------

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  int soilValue = analogRead(SOIL_PIN);
  int flame = digitalRead(FLAME_PIN);
  int smoke = digitalRead(SMOKE_PIN);

  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("DHT Sensor Error");
    delay(2000);
    return;
  }

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print("C  ");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  ");

  Serial.print("Soil: ");
  Serial.print(soilValue);

  Serial.print(" Flame: ");
  Serial.print(flame);

  Serial.print(" Smoke: ");
  Serial.println(smoke);

  // ---------------- TEMPERATURE ----------------

  if (temperature > 30)
  {
    digitalWrite(TEMP_ALERT, HIGH);

    if (!tempAlertSent)
    {
      sendSMS("Alert: High temperature detected in farm!");
      tempAlertSent = true;
    }
  }
  else
  {
    digitalWrite(TEMP_ALERT, LOW);
    tempAlertSent = false;
  }

  // ---------------- HUMIDITY ----------------

  if (humidity > 70)
  {
    digitalWrite(HUM_ALERT, HIGH);

    if (!humAlertSent)
    {
      sendSMS("Alert: High humidity detected in farm!");
      humAlertSent = true;
    }
  }
  else
  {
    digitalWrite(HUM_ALERT, LOW);
    humAlertSent = false;
  }

  // ---------------- BOTH TEMP + HUM ----------------

  if (temperature > 30 && humidity > 70)
  {
    digitalWrite(BOTH_ALERT, HIGH);

    if (!bothAlertSent)
    {
      sendSMS("Alert: High temperature and humidity detected!");
      bothAlertSent = true;
    }
  }
  else
  {
    digitalWrite(BOTH_ALERT, LOW);
    bothAlertSent = false;
  }

  // ---------------- SOIL ----------------

  if (soilValue > 3000)
  {
    digitalWrite(SOIL_ALERT, HIGH);

    if (!soilAlertSent)
    {
      sendSMS("Alert: Soil moisture low. Irrigation required!");
      soilAlertSent = true;
    }
  }
  else
  {
    digitalWrite(SOIL_ALERT, LOW);
    soilAlertSent = false;
  }

  // ---------------- FLAME SENSOR ----------------

  if (flame == LOW)
  {
    digitalWrite(FLAME_ALERT, HIGH);

    if (!flameAlertSent)
    {
      sendSMS("Emergency: Flame detected!");
      flameAlertSent = true;
    }
  }
  else
  {
    digitalWrite(FLAME_ALERT, LOW);
    flameAlertSent = false;
  }

  // ---------------- SMOKE SENSOR ----------------

  if (smoke == LOW)
  {
    digitalWrite(SMOKE_ALERT, HIGH);

    if (!smokeAlertSent)
    {
      sendSMS("Warning: Smoke detected!");
      smokeAlertSent = true;
    }
  }
  else
  {
    digitalWrite(SMOKE_ALERT, LOW);
    smokeAlertSent = false;
  }

  delay(4000);
}