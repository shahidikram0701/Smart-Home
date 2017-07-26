#include <Servo.h>
#include <Password.h>
#include <Key.h>
#include <Keypad.h>
const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad

//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols]= 
{
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {9,8,7,6}; //Rows 0 to 3
byte colPins[numCols]= {5,4,3,2}; //Columns 0 to 3

//initializes an instance of the Keypad class
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);


Password password = Password("1234");
Servo myservo;

int ledPin = 10;                // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
int tempPin = 1;
#define TEMP_MAX 25
float temp_avg[100] = {0};
int val_t;
int temp_n = 0;
int temp_count = 0;

/* accelerometer 
const int xpin = A5;                  // x-axis of the accelerometer
const int ypin = A4;                  // y-axis
const int zpin = A3;                  // z-axis (only on 3-axis models)*/
int smoke = A5;
void setup()
{
  pinMode(12, OUTPUT);
  myservo.attach(13);
   pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  pinMode(smoke, INPUT);
Serial.begin(9600);
}
int pos;
//If key is pressed, this key is stored in 'keypressed' variable
//If key is not equal to 'NO_KEY', then this key is printed out
//if count=17, then count is reset back to 0 (this means no key is pressed during the whole keypad scan process
void loop()
{
  /* --------------------- KEYPAD ------------------*/
char key = myKeypad.getKey();
if (key != NO_KEY)
{
Serial.print(key);
if (key == '#') {
  if (password.evaluate())
  {  
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  } else {
 digitalWrite(12, HIGH);
    
  }
  password.reset();
} else if (key == '*') {
  digitalWrite(12,LOW);
} else {

  password.append(key);
}
}
/*--------------------------------------------------*/
/*----------------------PIR-------------------------*/
val = digitalRead(inputPin);  // read input value

  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
  /*------------------------------------------------*/
  /*----------------------TEMP-------------------------*/
  val_t = analogRead(tempPin);
/*float mv = ( val_t/1024.0)*5000; 
float cel = mv/10;
if (temp_n < TEMP_MAX) {
  temp_avg[temp_n] = cel;
  temp_n++;
} else {
  temp_n--;
  float avg_temp = 0.0;
  for (int i = temp_n; i >= 0; i--)avg_temp += temp_avg[i];
  avg_temp /= temp_n;
  if (avg_temp >= 39.0) {

      //digitalWrite(12,HIGH);
      Serial.print("AVG TEMPRATURE = ");
      Serial.print(avg_temp);
      Serial.print("*C");
      Serial.println();
      //delay(1000);
  }else
  {
    Serial.print("avg temp = ");
      Serial.print(avg_temp);
      Serial.print("*C");
      Serial.println();
      
  }
}*/
float mv = ( val_t/1024.0)*5000; 
float cel = mv/10;
float farh = (cel*9)/5 + 32;
 if(cel > 50){
    temp_count ++;
 }
 else{
  if(temp_count < 5){
    temp_count = 0;
    digitalWrite(12, LOW);
  }
 }
 if(temp_count > 10){
  digitalWrite(12, HIGH);
 }
 else{
  digitalWrite(12, LOW);
 }
 
    Serial.print("TEMPRATURE = ");
    Serial.print(cel);
    Serial.print("*C");
    Serial.println();
    delay(1000);
  
  /*------------------------------------------------*/
  /*----------------------ACCELEROMETER-------------------------
  // print the sensor values:
  Serial.print(analogRead(xpin));
  // print a tab between values:
  Serial.print("\t");
  Serial.print(analogRead(ypin));
  // print a tab between values:
  Serial.print("\t");
  Serial.print(analogRead(zpin));
  Serial.println();
  // delay before next reading:
  delay(100);*/
  
  /*------------------------------------------------*/
  /*----------------------SMOKE--------------*/
  int smoke_count = 0;
 int analogSensor = analogRead(smoke );
  if(analogSensor > 650){
    digitalWrite(12, HIGH);
    Serial.print("Pin A0: ");
    Serial.println(analogSensor);
    //delay(100);
  }
  if(analogSensor < 650){
    digitalWrite(12, LOW);
  }
}
