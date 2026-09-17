#include <LiquidCrystal.h>

// ==================================================
// PIN DEFINITIONS
// ==================================================

#define FAN 6

#define TEMP_PIN A0
#define LIGHT_PIN A1
#define EQUIP_TEMP_PIN A2
#define BATTERY_PIN A3
#define PRESSURE_PIN A4

#define GREEN_LED 8
#define RED_LED 9
#define BUZZER 10

#define SOS_BUTTON 7
#define RESET_BUTTON 13


// ==================================================
// SAFETY THRESHOLDS
// ==================================================

#define WARNING_ALTITUDE 3000
#define CRITICAL_ALTITUDE 4000

#define LOW_LIGHT 300
#define WARNING_LIGHT 800

#define LOW_BATTERY 20
#define CRITICAL_BATTERY 10


// ==================================================
// LCD
// ==================================================

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


// ==================================================
// VARIABLES
// ==================================================

int safetyLevel = 0;

// 0 = NORMAL
// 1 = WARNING
// 2 = CRITICAL
// 3 = EMERGENCY

bool emergencyLatched = false;


// ==================================================
// SIMULATED GPS LOCATION
// ==================================================

float latitude = 34.1526;
float longitude = 77.5771;


// ==================================================
// SETUP
// ==================================================

void setup()
{
  Serial.begin(9600);

  pinMode(FAN, OUTPUT);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(SOS_BUTTON, INPUT_PULLUP);
  pinMode(RESET_BUTTON, INPUT_PULLUP);

  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("SIH26049");

  lcd.setCursor(0, 1);
  lcd.print("SYSTEM START");

  delay(2000);
}


// ==================================================
// LOOP
// ==================================================

void loop()
{

  // ==================================================
  // READ SENSORS
  // ==================================================

  // Ambient temperature
  int tempRaw = analogRead(TEMP_PIN);

  float ambientTemp =
    (tempRaw * 5.0 / 1023.0 - 0.5) * 100.0;


  // Equipment temperature
  int equipRaw = analogRead(EQUIP_TEMP_PIN);

  float equipmentTemp =
    (equipRaw * 5.0 / 1023.0 - 0.5) * 100.0;


  // Light
  int lightValue = analogRead(LIGHT_PIN);


  // Battery
  int batteryRaw = analogRead(BATTERY_PIN);

  int batteryPercent =
    map(batteryRaw, 0, 1023, 0, 100);


  // Simulated altitude
  int pressureRaw = analogRead(PRESSURE_PIN);

  int altitude =
    map(pressureRaw, 0, 1023, 0, 5000);


  // ==================================================
  // SOS BUTTON
  // ==================================================

  if (digitalRead(SOS_BUTTON) == LOW)
  {
    emergencyLatched = true;
  }


  // ==================================================
  // RESET BUTTON
  // ==================================================

  if (digitalRead(RESET_BUTTON) == LOW)
  {
    emergencyLatched = false;

    noTone(BUZZER);

    digitalWrite(FAN, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("SYSTEM RESET");

    lcd.setCursor(0, 1);
    lcd.print("RESUMING...");

    Serial.println();
    Serial.println("==============================");
    Serial.println("SYSTEM RESET");
    Serial.println("EMERGENCY CLEARED");
    Serial.println("SYSTEM RESUMING");
    Serial.println("==============================");

    delay(2000);
  }


  // ==================================================
  // EMERGENCY MODE
  // ==================================================

  if (emergencyLatched == true)
  {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);

    // Stop fan
    digitalWrite(FAN, LOW);

    // Emergency alarm
    tone(BUZZER, 2500);

    // LCD
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("SOS ALERT!");

    lcd.setCursor(0, 1);
    lcd.print("LOCATION SENT");


    // Serial emergency information
    Serial.println();
    Serial.println("================================");
    Serial.println("       !!! SOS ALERT !!!");
    Serial.println("================================");

    Serial.println("EMERGENCY BUTTON PRESSED");

    Serial.print("LATITUDE: ");
    Serial.println(latitude, 4);

    Serial.print("LONGITUDE: ");
    Serial.println(longitude, 4);

    Serial.println("LOCATION: LADAKH");
    Serial.println("GPS STATUS: SIMULATED");
    Serial.println("COMMUNICATION: ALERT SENT");
    Serial.println("SYSTEM STATUS: STOPPED");
    Serial.println("PRESS RESET");

    Serial.println("================================");

    delay(1000);

    return;
  }


  // ==================================================
  // SAFETY LEVEL
  // ==================================================

  safetyLevel = 0;


  // ---------------- CRITICAL ----------------

  if (equipmentTemp >= 60 ||
      altitude >= CRITICAL_ALTITUDE ||
      lightValue < LOW_LIGHT ||
      batteryPercent <= CRITICAL_BATTERY)
  {
    safetyLevel = 2;
  }


  // ---------------- WARNING ----------------

  else if (equipmentTemp >= 40 ||
           altitude >= WARNING_ALTITUDE ||
           lightValue < WARNING_LIGHT ||
           batteryPercent <= LOW_BATTERY)
  {
    safetyLevel = 1;
  }


  // ---------------- NORMAL ----------------

  else
  {
    safetyLevel = 0;
  }


  // ==================================================
  // CRITICAL CONDITION
  // ==================================================

  if (safetyLevel == 2)
  {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);


    // ---------------- SMART COOLING ----------------

    if (equipmentTemp >= 60)
    {
      digitalWrite(FAN, HIGH);
    }
    else
    {
      digitalWrite(FAN, LOW);
    }


    // ---------------- LCD MESSAGE ----------------

    lcd.clear();


    if (equipmentTemp >= 60)
    {
      lcd.setCursor(0, 0);
      lcd.print("TEMP CRITICAL");

      lcd.setCursor(0, 1);
      lcd.print("FAN: ON");
    }

    else if (batteryPercent <= CRITICAL_BATTERY)
    {
      lcd.setCursor(0, 0);
      lcd.print("BATTERY CRIT");

      lcd.setCursor(0, 1);
      lcd.print("BAT:");
      lcd.print(batteryPercent);
      lcd.print("%");
    }

    else if (altitude >= CRITICAL_ALTITUDE)
    {
      lcd.setCursor(0, 0);
      lcd.print("ALT CRITICAL");

      lcd.setCursor(0, 1);
      lcd.print(altitude);
      lcd.print(" m");
    }

    else if (lightValue < LOW_LIGHT)
    {
      lcd.setCursor(0, 0);
      lcd.print("LIGHT CRITICAL");

      lcd.setCursor(0, 1);
      lcd.print("LEVEL:");
      lcd.print(lightValue);
    }


    // ---------------- SMART BUZZER ----------------

    if (equipmentTemp >= 60)
    {
      // Overheating
      tone(BUZZER, 2000);

      delay(500);

      noTone(BUZZER);
    }

    else if (batteryPercent <= CRITICAL_BATTERY)
    {
      // Critical battery
      tone(BUZZER, 700);

      delay(200);

      noTone(BUZZER);

      delay(200);
    }

    else if (altitude >= CRITICAL_ALTITUDE)
    {
      // Double beep
      tone(BUZZER, 1500);

      delay(250);

      noTone(BUZZER);

      delay(150);

      tone(BUZZER, 1500);

      delay(250);

      noTone(BUZZER);
    }

    else if (lightValue < LOW_LIGHT)
    {
      // Low light
      tone(BUZZER, 1000);

      delay(500);

      noTone(BUZZER);
    }

    delay(500);
  }


  // ==================================================
  // WARNING CONDITION
  // ==================================================

  else if (safetyLevel == 1)
  {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);


    // ---------------- FAN ----------------

    if (equipmentTemp >= 40)
    {
      digitalWrite(FAN, HIGH);
    }
    else
    {
      digitalWrite(FAN, LOW);
    }


    // ---------------- LCD ----------------

    lcd.clear();


    if (equipmentTemp >= 40)
    {
      lcd.setCursor(0, 0);
      lcd.print("TEMP WARNING");

      lcd.setCursor(0, 1);
      lcd.print("FAN: ON");
    }

    else if (altitude >= WARNING_ALTITUDE)
    {
      lcd.setCursor(0, 0);
      lcd.print("ALT WARNING");

      lcd.setCursor(0, 1);
      lcd.print(altitude);
      lcd.print(" m");
    }

    else if (batteryPercent <= LOW_BATTERY)
    {
      lcd.setCursor(0, 0);
      lcd.print("LOW BATTERY");

      lcd.setCursor(0, 1);
      lcd.print("BAT:");
      lcd.print(batteryPercent);
      lcd.print("%");
    }

    else if (lightValue < WARNING_LIGHT)
    {
      lcd.setCursor(0, 0);
      lcd.print("LOW LIGHT");

      lcd.setCursor(0, 1);
      lcd.print("LEVEL:");
      lcd.print(lightValue);
    }


    // ---------------- WARNING BUZZER ----------------

    tone(BUZZER, 1000);

    delay(500);

    noTone(BUZZER);

    delay(500);
  }


  // ==================================================
  // NORMAL CONDITION
  // ==================================================

  else
  {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);

    digitalWrite(FAN, LOW);

    noTone(BUZZER);

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("SYSTEM NORMAL");

    lcd.setCursor(0, 1);
    lcd.print("ALL OK");

    delay(1000);
  }


  // ==================================================
  // LCD SCREEN 1
  // TEMPERATURE
  // ==================================================

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("AMB:");
  lcd.print(ambientTemp, 1);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("EQP:");
  lcd.print(equipmentTemp, 1);
  lcd.print("C");

  delay(1500);


  // ==================================================
  // LCD SCREEN 2
  // BATTERY + ALTITUDE
  // ==================================================

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("BAT:");
  lcd.print(batteryPercent);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("ALT:");
  lcd.print(altitude);
  lcd.print("m");

  delay(1500);


  // ==================================================
  // LCD SCREEN 3
  // LIGHT
  // ==================================================

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("LIGHT:");
  lcd.print(lightValue);

  lcd.setCursor(0, 1);


  if (lightValue >= WARNING_LIGHT)
  {
    lcd.print("LIGHT NORMAL");
  }

  else if (lightValue >= LOW_LIGHT)
  {
    lcd.print("LIGHT WARNING");
  }

  else
  {
    lcd.print("LIGHT CRITICAL");
  }

  delay(1500);


  // ==================================================
  // SERIAL DATA LOGGER
  // ==================================================

  Serial.print("TIME: ");
  Serial.print(millis() / 1000);
  Serial.print("s | ");

  Serial.print("TEMP: ");
  Serial.print(ambientTemp, 1);
  Serial.print("C | ");

  Serial.print("EQUIP: ");
  Serial.print(equipmentTemp, 1);
  Serial.print("C | ");

  Serial.print("ALT: ");
  Serial.print(altitude);
  Serial.print("m | ");

  Serial.print("BAT: ");
  Serial.print(batteryPercent);
  Serial.print("% | ");

  Serial.print("LIGHT: ");
  Serial.print(lightValue);
  Serial.print(" | ");

  Serial.print("STATUS: ");


  if (safetyLevel == 2)
  {
    Serial.println("CRITICAL");
  }

  else if (safetyLevel == 1)
  {
    Serial.println("WARNING");
  }

  else
  {
    Serial.println("NORMAL");
  }


  // ==================================================
  // FAULT DETECTION
  // ==================================================

  Serial.println("----- FAULT CHECK -----");


  if (equipmentTemp >= 60)
  {
    Serial.println("FAULT: EQUIPMENT OVERHEATING");
  }

  else if (equipmentTemp >= 40)
  {
    Serial.println("WARNING: EQUIPMENT TEMPERATURE HIGH");
  }


  if (altitude >= CRITICAL_ALTITUDE)
  {
    Serial.println("FAULT: CRITICAL ALTITUDE");
  }

  else if (altitude >= WARNING_ALTITUDE)
  {
    Serial.println("WARNING: HIGH ALTITUDE");
  }


  if (batteryPercent <= CRITICAL_BATTERY)
  {
    Serial.println("FAULT: BATTERY CRITICALLY LOW");
  }

  else if (batteryPercent <= LOW_BATTERY)
  {
    Serial.println("WARNING: BATTERY LOW");
  }


  if (lightValue < LOW_LIGHT)
  {
    Serial.println("FAULT: LIGHT LEVEL TOO LOW");
  }

  else if (lightValue < WARNING_LIGHT)
  {
    Serial.println("WARNING: LOW LIGHT");
  }


  if (safetyLevel == 0)
  {
    Serial.println("NO FAULTS DETECTED");
  }


  Serial.println("-----------------------");

  delay(1000);
}
