const int led = 5;
const int button = 4;
int temp = 0;
const int buzzer =  14;

void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT);
}

void loop() {
  temp = digitalRead(button);
     
     if (temp == HIGH) {
        digitalWrite(buzzer, HIGH);
        Serial.println("LED Turned ON");
        delay(1000);
       }
     else {
        digitalWrite(buzzer, LOW);
        Serial.println("LED Turned OFF");
        delay(1000);
       }
}
