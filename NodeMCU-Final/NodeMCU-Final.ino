#include <Arduino.h>
#include <ESP8266WiFi.h> //import for wifi functionality
#include <WebSocketsServer.h> //import for websocket

const char *ssid =  "Left";   //Wifi SSID (Name)
const char *pass =  "123456789"; //wifi password

// UltraSonic Sensor - 1
const int trigP = 2;  //D4 Or GPIO-2 of nodemcu
const int echoP = 0;  //D3 Or GPIO-0 of nodemcu

// UltraSonic Sensor - 2
const int trigPin = 15;  //D8 Or GPIO-15 of nodemcu
const int echoPin = 13;  //D7 Or GPIO-13 of nodemcu

const int buzzer =  14;
int temp = 0;
const int button = 4;

long duration, durationTwo;
int distance, distanceTwo;

#define ledpin 2 //defining the OUTPUT pin for LED

WebSocketsServer webSocket = WebSocketsServer(81); //websocket init with port 81

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  //webscket event method
  String cmd = "";
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("Websocket is disconnected");
      //case when Websocket is disconnected
      break;
    case WStype_CONNECTED: {
        //wcase when websocket is connected
        Serial.println("Websocket is connected");
        Serial.println(webSocket.remoteIP(num).toString());
        webSocket.sendTXT(num, "connected");
      }
      break;
    case WStype_TEXT:
      cmd = "";
      for (int i = 0; i < length; i++) {
        cmd = cmd + (char) payload[i];
      } //merging payload to single string
      Serial.println(cmd);

      if (cmd == "poweron") { //when command from app is "poweron"
        digitalWrite(ledpin, HIGH);   //make ledpin output to HIGH
      } else if (cmd == "poweroff") {
        digitalWrite(ledpin, LOW);    //make ledpin output to LOW on 'pweroff' command.
      }

      webSocket.sendTXT(num, cmd);
      //send response to mobile, if command is "poweron" then response will be "poweron:success"
      //this response can be used to track down the success of command in mobile app.
      break;
    case WStype_FRAGMENT_TEXT_START:
      break;
    case WStype_FRAGMENT_BIN_START:
      break;
    case WStype_BIN:
      hexdump(payload, length);
      break;
    default:
      break;
  }
}

void setup() {
  Serial.println("Connecting to wifi");

  IPAddress apIP(192, 168, 0, 1);   //Static IP for wifi gateway
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); //set Static IP gateway on NodeMCU
  WiFi.softAP(ssid, pass); //turn on WIFI
  webSocket.begin(); //websocket Begin
  webSocket.onEvent(webSocketEvent); //set Event for websocket
  Serial.println("Websocket is started");
  
  pinMode(button, INPUT);
  pinMode(ledpin, OUTPUT); //set ledpin (D2) as OUTPUT pin
  pinMode(trigP, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoP, INPUT);   // Sets the echoPin as an Input
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);   // Sets the echoPin as an Input
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600); //serial start
}

void loop() {
  webSocket.loop(); //keep this line on loop method
  temp = digitalRead(button);
  digitalWrite(trigP, LOW);   // Makes trigPin low
  digitalWrite(trigPin, LOW);   // Makes trigPin low
  delayMicroseconds(2);       // 2 micro second delay

  digitalWrite(trigP, HIGH);  // tigPin high
  digitalWrite(trigPin, HIGH);  // tigPin high
  digitalWrite(trigP, LOW);   // trigPin low
  digitalWrite(trigPin, LOW);   // trigPin low
  delayMicroseconds(10);      // trigPin high for 10 micro seconds

  duration = pulseIn(echoP, HIGH);   //Read echo pin, time in microseconds
  durationTwo = pulseIn(echoPin, HIGH);   //Read echo pin, time in microseconds
  distance = duration * 0.034 / 2;   //Calculating actual/real distance
  distanceTwo = durationTwo * 0.034 / 2;   //Calculating actual/real distance

  Serial.print("Distance(cm)[1] = ");        //Output distance on arduino serial monitor
  Serial.println(distance);
  Serial.print("Distance(cm)[2]= ");        //Output distance on arduino serial monitor
  Serial.println(distanceTwo);
  if (temp == HIGH) {
    Serial.println("Button Turned ON");
    delay(1000);
  }
  else {
    Serial.println("Button Turned OFF");
    delay(1000);
  }
  if (distance <= 15) {
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
  }
  //  delay(3000);                        //Pause for 3 seconds and start measuring distance again
}
