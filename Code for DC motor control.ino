#include <IRremote.hpp>



int RECV_PIN = 8; // IR Receiver - Arduino Pin Number 8
int pwmPin = 7; // Arduino Pin Number 7 to the Base of the Transistor
int pwmValue;

IRrecv irrecv(RECV_PIN);

decode_results results;
#define enA 9
#define in1 6
#define in2 7
#define button 4

int rotDirection = 0;
int pressed = false;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(button, INPUT);
  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
    Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode( pwmPin, OUTPUT);  
  pwmValue = 0; // Starts the program with turned off motor

}

void loop() {
  int potValue = analogRead(A0); // Read potentiometer value
  int pwmOutput = map(potValue, 0, 1023, 0 , 255); // Map the potentiometer value from 0 to 255
  analogWrite(enA, pwmOutput); // Send PWM signal to L298N Enable pin

  // Read button - Debounce
  if (digitalRead(button) == true) {
    pressed = !pressed;
  }
  while (digitalRead(button) == true);
  delay(20);

  // If button is pressed - change rotation direction
  if (pressed == true  & rotDirection == 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    rotDirection = 1;
    delay(20);
  }
  // If button is pressed - change rotation direction
  if (pressed == false & rotDirection == 1) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    rotDirection = 0;
    delay(20);
  }


    if (irrecv.decode(&results)) {   
    
    analogWrite(pwmPin, pwmValue);
  
    if (results.value == 0xAFAF8374) { // PLAY Button
        pwmValue = 255; // 100% Duty Cycle | Max Speed
        }
    if (results.value == 0x98519C65) { // STOP Button
        pwmValue = 0; // 0% Duty Cycke | Turned off
        }
    if (results.value == 0x93F1BA08) { // FORWARD Button
        if(pwmValue <= 245){  
        pwmValue = pwmValue + 10; // Increases the Duty Cycle of the PWM Signal
        delay(20);     
      }   
    }
    if (results.value == 0x71D086FF) { // BACKWARD Button
      if(pwmValue >= 20){
        pwmValue = pwmValue - 10; // Decreases the Duty Cycle of the PWM Signal
        delay(20);    
      }
    }   
  Serial.print(pwmValue);
  Serial.print(" ");
  Serial.println(results.value, HEX);
   
  irrecv.resume(); // Receive the next value
  }
  delay(100);
}
