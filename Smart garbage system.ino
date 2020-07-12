#include<SoftwareSerial.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
String ssid     = "Simulator Wifi";  // SSID to connect to
String password = ""; // Our virtual wifi has no password (so dont do your banking stuff on this network)
String host     = "api.thingspeak.com";
const int httpPort   = 80;
const String uri     = "/update?api_key=ZU3CXLQBBEWRMDDX&field1=";
int inches = 0;

float cm = 0;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

int setupESP8266(void) {
 // Start our ESP8266 Serial Communication
  Serial.begin(115200);   // Serial connection over USB to computer
  Serial.println("AT");   // Serial connection on Tx / Rx port to ESP8266
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 1;
    
  // Connect to 123D Circuits Simulator Wifi
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 2;
  
  // Open TCP connection to the host:
  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
  delay(50);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 3;
  
  return 0;
}

void anydata(void) {
  
   float cm = 0.01723 * readUltrasonicDistance(7,7);
    int percent = map(cm,335.8,2.4,0,100);
  Serial.println(percent);
  //inches = (cm / 2.54);
  
  // Construct our HTTP call
  String httpPacket = "GET " + uri + String(percent)+" HTTP/1.1\r\nHost: " + host + "\r\n\r\n";;
  int length = httpPacket.length();
  
  // Send our message length8);
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  delay(100); // Wait a little for the ESP to respond if (!Serial.find(">")) return -1;

  // Send our http request
  Serial.print(httpPacket);
  delay(100); // Wait a little for the ESP to respond
  if (!Serial.find("SEND OK\r\n")) return;
  
  
}
 int cms = map(cm,335.8,2.4,0,100);

void setup()
{
  Serial.begin(9600);
  pinMode(7, INPUT);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("INMOVIDU !!!");
  
  setupESP8266();

}

void loop()
{
  // measure the ping time in cm
  lcd.clear();
   float cm = 0.01723 * readUltrasonicDistance(7,7);
    int percent = map(cm,335.8,2.4,0,100);
  //lcd.print("Cms:");
  //lcd.setCursor(6,1);
  //lcd.print(cms);
  lcd.setCursor(0,1);
  lcd.print(percent);
  lcd.setCursor(3,3);
  lcd.print("% is full");
  anydata();
  delay(10000);
  // Wait for 100 millisecond(s)
}
