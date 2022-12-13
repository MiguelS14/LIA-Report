// Remote Variables

  #include <IRremote.h> // Includes the library for the remote.
  int receiver = 9; // Sets pin 9 as a variable named "receiver" for the remote.
  uint32_t Previous; 
  IRrecv irrecv(receiver); // create a new instance of "irrecv"
  decode_results results; // Decodes the IR signal results in the receiver from the remote.

// Line Tracker Sensors 

  int Line_Right; // integer for the right analog value reader in the line tracker module.
  int Line_Middle; // integer for the middle analog value reader in the line tracker module.
  int Line_Left; // integer for the left analog value reader in the line tracker module.

// Millis for Line Tracker "Search".

  unsigned long current_millis = millis();  // millis that will be used for search in line tracking module.
  unsigned long previous_millis; // saves the previous value to avoid delays.

// Ultrasonic variables pins and values.

  #define trigPin 13 // defines the output ultrasonic value pin.
  #define echoPin 12 // defines the input ultrasonic value pin.
  float duration, distance; // setting up variables for the duration before the wave is received and calculates the distance

// Servo motor

  #include <Servo.h> // adds servo motor library.
  Servo myservo;  // create servo object to control a servo.

// Modes to switch between functions (Line Tracker, Remote, and Ultrasonic).

  int mode = 0;

void setup(){

// Begins the serial to print values in the serial monitor IR remote and Analog values of Line Tracker Module values. 

   Serial.begin(9600); // to read line tracker in the serial monitor.

// Sets the pins (0 - 8) of the robot as an output.
 
  for (int i=0; i<9; i++){
  pinMode(i, OUTPUT);
  }
  digitalWrite(3,1); // sets the standby pin as on so robot would function.

// IR Remote

  irrecv.enableIRIn(); // Starts the receiver.

// Ultrasonic Sensor

  pinMode(trigPin, OUTPUT);// setting pin 10 as an output for trigPin
  pinMode(echoPin, INPUT);// setting pin 13 as an input for echoPin

// Servo Motor

  myservo.attach(10);  // attaches the servo on pin 10 to the servo object
}


// |-------------------- MOVEMENT FUNCTIONS --------------------|

// main function to be used in the movement functions to optimize code.

void move(bool a, int b, bool c, int d){ // bool for numbers between 0 and 1, int to hold the value inside the function in the loop

  analogWrite(5,b); // integer value for motor speed PWMA.
  analogWrite(6,d); // integer value for motor speed PWMB.
  digitalWrite(7,a); // boolean value (0 - 1) for  motor state of AIN1.
  digitalWrite(8,c); // boolean value (0 - 1) for  motor state of BIN1.

}

// function for the forward movement.

void Forward(int x){ // sets an integer inside called "x" to designate speed 0-255.
  move(1,x,1,x);
}

// function for the backward movement.

void Backward(int x){ // sets an integer inside called "x" to designate speed 0-255.
  move(0,x,0,x);
}

// function for the left movement.

void Left(int x){ // sets an integer inside called "x" to designate speed 0-255.
  move(1,x,0,x);
}

// function for the right movement.

void Right(int x){ // sets an integer inside called "x" to designate speed 0-255.
  move(0,x,1,x);
}

// function for the left forward movement.

void Left_Forward(int x){ // sets an integer inside called "x" to designate speed 0-255.
  move(1,x,1,x/2);
}

// function for the left backward movement.

void Left_Backward(int x){ // sets an integer inside called "x" to designate speed 0-255.
  move(0,x,0,x/2);
}

// function for the right forward movement.

void Right_Forward(int x){ // sets an integer inside called "x" to designate speed 0-255.
  move(1,x/2,1,x);
}

// function for the right backward movement.

void Right_Backward(int x){ // sets an integer inside called "x" to designate speed 0-255.
  move(0,x/2,0,x);
}

// function for stop.

void Stop(){
  move(0,0,0,0); // all motors are off.
}

// Ultrasonic sensor function to optimize "void loop()" code.

void Ultrasonic(){

  digitalWrite(trigPin, LOW); // sets trigger pin off.
  delayMicroseconds(2); // delays for 2 microseconds.
  digitalWrite(trigPin, HIGH);; // sets trigger pin on.
  delayMicroseconds(10); // delays for 10 microseconds.
  digitalWrite(trigPin, LOW);; // sets trigger pin off.
  duration = pulseIn(echoPin, HIGH);; // reads the value returned from pin 13 and stores it in duration(of wave).
  distance = (duration / 2) * 0.0343; // converts duration into distance in metric centimeters.
}

void loop() {

// Reads the analog values for the line tracker.

// Values gets printed in the serial monitor.

  Line_Right = analogRead(A0); // Right
  Line_Middle = analogRead(A1); // Middle
  Line_Left = analogRead(A2); // Left
  Serial.print("Right : ");
  Serial.print(Line_Right); // Line Tracker Right.
  Serial.print("        "); // Creates Space between values of Right and Middle.
  Serial.print("Middle : ");
  Serial.print(Line_Middle); // Line Tracker Middle
  Serial.print("        "); // Creates space between values of Middle and Left
  Serial.print("Left : ");
  Serial.println(Line_Left); // Line Tracker Left 

// if mode is 0 all functions must stop.

if(mode == 0){
    Stop();
  }
  
// IR receiver value reader and actuator.

if (irrecv.decode(&results)){ // have we received an IR signal?

  switch(results.value) // switch case for all the signal received by the IR receiver from the remote that will execute the statements below.

{

// Remote control mode

if(mode == 1){

  case 16736925: // Arrow-Up button is pressed in the IR remote.
  Forward(200);
                                     
break;

  case 16720605: // Left-Arrow button is pressed in the IR remote.
  Left(100);
                                           
break;

  case 16754775: // Arrow-Down button is pressed in the IR remote.
  Backward(200);
                                      
break;

  case 16761405: // Arrow-Right button is pressed in the IR remote.
  Right(100);                     
                      
break;
}

case 16712445: // if OK Button is pressed mode is set to 0 (Stop).
  mode = 0; // mode equals to 0
  
break;

  case 16738455: // if Button 1 is pressed mode is set to 1 (Remote Control).
  mode = 1; // mode equals to 1
                                            
break;

  case 16750695: // if Button 2 is pressed mode is set to 2 (Line Tracker).
  mode = 2; // mode equals to 2
                      
break;

  case 16756815: // if Button 3 is pressed mode is set to 3 (Ultrasonic).
  mode = 3; // mode equals to 3

break;                  
}

  Serial.println(results.value); // Prints the remote values in the serial monitor.
  irrecv.resume(); // receive the next value.

} 

// Line Tracker 

if(mode == 2){ // if Button 2 is received the mode that should be executed is Line Tracker Mode.

  current_millis = millis(); // Millis is used for the code to cancel the delay.

  Line_Right = analogRead(A0); // sets variable analog pin 'A0 as "Line_Right".
  Line_Left = analogRead(A2); // sets variable analog pin 'A2 as "Line_Left".
  Line_Middle = analogRead(A1); // sets variable analog pin 'A1 as "Line_Middle".

if(Line_Middle >50){ // If Middle line is reading more than 50 motor goes Forward.
  Forward(75); // used forward function above with an integer number of 75 for speed.
}

else if(Line_Right >50){ // If Right line is reading more than 50 motor goes Right to recenter robot.
  Right(250); // used right function above with an integer number of 250 for speed.

}

else if(Line_Left >50){ // If Left line is reading more than 50 motor goes Left to recenter robot.
  Left(250); // used left function above with an integer number of 250 for speed.

}

// Else statement used for when the robot does not read three of the statements above it will look for a line to track around it.

else{ 

  previous_millis = millis(); // previous_millis to keep previous value
  while((current_millis-previous_millis) <100){ // if all sensors are read.ing past a hundred it will be inside the while loop unless sensor reads something lower than 100
  current_millis = millis(); // curent_millis to read current value.

  Right(100); // used right function with a speed of 100.

  if(Line_Middle >50){ // to end the while loop if middle line reads lower than 50 it will go back to the first statement.

break;

}
}
}
}

// Ultrasonic and Servo Motor

else if(mode == 3){ // if mode 3 reads high it will execute the following statements:

  Forward(200); // used forward function with a speed of 200.
  myservo.write(90); // turns the servo motor to look for open area.        
  Ultrasonic(); // reads the value of the Ultrasonic sensor.
   
if (distance <= 25) { // checks the distance between the ultrasonic and an obstacle if it is less than 25 centimeters it executes the following statements:

  Stop(); // the robot stop to avoid collision
  myservo.write(180); // servo looks at the left side for ultrasonic to read values.
  delay(1000); // delay for 1 second after looking around.
  Ultrasonic(); // reads the value of the Ultrasonic sensor.
  myservo.write(90); // Servo motor goes back to default position.

if (distance <= 25) { // checks the distance between the ultrasonic and an obstacle if it is less than 25 centimeters it executes the following statements:

  myservo.write(0); // servo looks at the right side for ultrasonic to read values.
  delay(1000); // delay for 1 second after looking around.
  Ultrasonic(); // reads the value of the Ultrasonic sensor.
  myservo.write(90); // Servo motor goes back to default position.  

if (distance <= 25) { // checks the distance between the ultrasonic and an obstacle if it is less than 25 centimeters it executes the following statements:

  Left(100); // used left function with a duration of 1200 and a speed of 100 when right and left side has obstacles. 
  delay(1200); // duration 
}

else{

  Right(100); // used right function with a duration of 0.6 seconds and a speed of 100 when left side has obstacles to avoid collision.
  delay(600); // duration

}
}
else{

  Left(100); // used left function with a duration of 0.6 seconds and a speed of 100 when left side has obstacles to avoid collision.
  delay(600); // duration.

}
}
}
}


