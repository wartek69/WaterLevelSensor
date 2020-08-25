#include <ESP8266WiFi.h>
#include <ThingsBoard.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

LiquidCrystal_I2C lcd(0x3F,16,2);

#define WIFI_AP "AP"
#define WIFI_PASSWORD "PASSWORD"

#define TOKEN "THINGSBOARDTOKEN"

int trigPin = 14;
int echoPin = 12;
int maxD = 500;
 
NewPing sonar(trigPin, echoPin, maxD);
//If you want to use a public ip forward the 1883 port for mqtt on your server
char thingsboardServer[] = "THINGSBOARDSERVER";

WiFiClient wifiClient;

ThingsBoard tb(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  delay(10);
  InitWiFi();
  lastSend = 0;
  //initialisatie lcd
  /*lcd.begin(16,2);
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(7,0);
  lcd.clear();*/
  Serial.println("Setup Done");                                                                            
}

void loop()
{
  if ( !tb.connected() ) {
    reconnect();
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off by making the voltage HIGH
  }
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH

  if ( millis() - lastSend > 60000L ) { // Update and send only after 1 minute
    Serial.println("A minute has gone by.");
    getSensorData();
    lastSend = millis();
    /*lcd.clear();
    lcd.print("Hello Alex");*/ 
  }

  tb.loop();
}

void getSensorData()
{
  Serial.println("Collecting sensor data...");
  int new_measurement = sonar.convert_cm(sonar.ping_median());
  Serial.print(new_measurement);
  Serial.println("cm");
  //TODO connect the sensor here
  // Check if any reads failed and exit early (to try again).
  if (isnan(new_measurement)) {
    Serial.println("Failed to read from Water Level sensor!");
    return;
  }

  Serial.println("Sending data to ThingsBoard:");
  Serial.print("Water level: ");
  Serial.print(new_measurement);
  Serial.print(" cm%\t");
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on by making the voltage LOW
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH

  tb.sendTelemetryFloat("water_level_cm", new_measurement);
}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}


void reconnect() {
  // Loop until we're reconnected
  while (!tb.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    if ( tb.connect(thingsboardServer, TOKEN) ) {
      Serial.println( "[DONE]" );

    } else {
      Serial.print( "[FAILED]" );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}
