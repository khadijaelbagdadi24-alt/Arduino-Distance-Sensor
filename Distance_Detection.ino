#define GREEN_LED 8
#define YELLOW_LED 9
#define RED_LED 10
#define TRIG_PIN 11
#define ECHO_PIN 12
#define BUZZER_PIN 3  
const int DISTANCE_SEUIL_CM = 50; 
const int DUREE_FEU_ROUGE = 5000; 
void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  noTone(BUZZER_PIN); 

  Serial.begin(9600); 
  Serial.println("Arduino Smart Traffic System Started (Green ON).");
}
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); 
  long distance = duration / 58; 
  if (distance == 0) return 999; 
  return distance;
}
void trafficSequence() {
  if (digitalRead(GREEN_LED) == LOW) {
    return;
  }
  
  Serial.println("Starting sequence: Green -> Yellow -> Red.");
  for (int i = 0; i < 4; i++) {
    digitalWrite(GREEN_LED, LOW);
    delay(250);
    digitalWrite(GREEN_LED, HIGH);
    delay(250);
  }
  digitalWrite(GREEN_LED, LOW); 
  digitalWrite(YELLOW_LED, HIGH);
  delay(2000); 
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  Serial.println("RED Light ON + Buzzer.");
  unsigned long startTime = millis();
  while (millis() - startTime < DUREE_FEU_ROUGE) {
    tone(BUZZER_PIN, 1000); 
    delay(100); 
  }
  noTone(BUZZER_PIN); 
  digitalWrite(YELLOW_LED, HIGH);
  delay(2000); 
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
}
void loop() {
  long distance = getDistance();
  bool objectDetected = distance <= DISTANCE_SEUIL_CM;
  if (objectDetected && digitalRead(GREEN_LED) == HIGH) {
    
    Serial.print("Object detected at ");
    Serial.print(distance);
    Serial.println(" cm.");

    trafficSequence();
    
  } 
  else if (!objectDetected) {
    if (digitalRead(RED_LED) == LOW && digitalRead(YELLOW_LED) == LOW) {
       if (digitalRead(GREEN_LED) == LOW) {
           digitalWrite(GREEN_LED, HIGH);
           Serial.println("Path clear. Returning to Green.");
       }
    }
  }
  delay(100); 
}
