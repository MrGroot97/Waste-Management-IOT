#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <LiquidCrystal.h>
#define trigPin 5
#define echoPin 4
const int rs=2 ,en =16 ,d4 =15 ,d5 =13 ,d6 =12 ,d7 =14;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Set these to run example.
#define FIREBASE_HOST "fir-iot-616d2.firebaseio.com"
#define FIREBASE_AUTH "r45zhF3N9crN2su2HClAVyRCC55ARj0ONCMkJnWQ"
#define WIFI_SSID "androidone"
#define WIFI_PASSWORD "standalone"

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.print("Hello");

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println("trewq");
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
//    pinMode(irpin,INPUT);
}


void loop() {
   long duration, distance;
   int count = 0;
//   int fillDustbin;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  Serial.println(distance);
//  fillDustbin=digitalRead(irpin);
//  Serial.println(fillDustbin);

//out of range Case .................
  if (distance >= 40 || distance <= 0){
    Serial.println("Out of range");
        Firebase.setString("message", "Out of Range");
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
    }
    delay(1000);  
  }
//  in range case ######################################
  else {
    Serial.print(distance);
    Serial.println(" cm");
     Firebase.setFloat("Distance", distance);
     if(distance <40 && distance >=30){
     Firebase.setString("message", "Khali Hai 75%");
     }
     if(distance <30 && distance >=20){
     Firebase.setString("message", "Khali Hai 50%");
     }
     if(distance <20 && distance >=10){
     Firebase.setString("message", "Khali Hai 25%");
     }
     if(distance <10 && distance>0){
     Firebase.setString("message", "Bhar Chuka hai");
     }
    if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
    }
    delay(500);
  Serial.print("message: ");
  Serial.println(Firebase.getString("message"));
  delay(1000);
 }

    // append a new value to /logs
  String name = Firebase.pushInt("DustbinData", count++);
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(1000);
   
}
