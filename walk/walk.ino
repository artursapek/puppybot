#include <Servo.h> 

//#include <Leg.h>

using namespace std;

class Leg {

  Servo hip;
  Servo thigh; 
  Servo knee;


//  unsigned long lastTorso, lastThigh, lastKnee;
public:
  void setMuscles (int, int, int);
  void setPosition(int pos);
  

  // Given a float between 0.0 and 1.0
  // and a motor id,
  //   1 => hip
  //   2 => thigh
  //   3 => knee
  // return a degree count for that specific motor.
  int motorFloatToDeg(float pos, int motorID);

  // 1: fl
  // 2: fr
  // 3: br
  // 4: bl
  int id;

  void back();
  void up();
  void down();

  void read();

  void setHip(float);
  void setThigh(float);
  void setKnee(float);

  bool isOnLeftSide();
  bool isInFront();

};

  /*
void Leg::up(){
  // Serial.println(knee.read());
  int static counter;
  int static pos[6] =  {
    20, 40,    30, 60,    40, 50,    80, 120  };
    posMax = sizeof(pos) / 2;

  if (millis() - lastKnee > 60){
    ++counter;
    if (counter == 6) counter = 0;     

    knee.write(pos[counter]);
    delay(100);
    thigh.write(100); 

    lastKnee  = millis();     
  }
}
  */


int posOfRange(float pos, float min, float max) {
  // Helper method
  // Example:
  // posOfRange(0.2, 100, 200);
  // => 120
  float diff = max - min;
  return min + (pos * diff) + 0.5;
}


int Leg::motorFloatToDeg(float pos, int motorID){
  float deg;

  switch (motorID) {
    case 1:
      // Hip
      switch (id) {
        case 1:
          // FL
          // middle = 90
          deg = posOfRange(pos, 60., 120.);
          break;

        case 2:
          // FR
          // middle = 70
          deg = posOfRange(1.0 - pos, 40., 90.);
          break;

        case 3:
          // BR
          // middle = 70
          deg = posOfRange(1.0 - pos, 60., 120.);
          break;

        case 4:
          // BL
          deg = posOfRange(pos, 40., 90.);
          break;
      }

      break;
    case 2:
      // Thigh
      // 0.0 = Straight down
      // 1.0 = Up
      switch (id) {
        case 1:
          // FL
          // middle = 120
          deg = posOfRange(1.0 - pos, 40., 110.);
          break;

        case 2:
          // FR
          // middle = 70
          deg = posOfRange(pos, 10., 110.);
          break;

        case 3:
          // BR
          // middle = 70
          deg = posOfRange(pos, 10., 110.);
          break;

        case 4:
          // BL
          // middle = 120
          deg = posOfRange(1.0 - pos, 40., 110.);
          break;
      }
      

      break;
    case 3:
      // Knee
      // Same for both sides. They are all glued on the same way cuz
      // 0.0 = Straight down (80)
      // 1.0 = Bent back towards inside of knee (140)
      deg = posOfRange(pos, 60., 140.);
      break;
  }
  return (int) (deg + 0.5);
}

bool Leg::isOnLeftSide(){
  return (id == 1 || id == 4);
}
  
bool Leg::isInFront(){
  return (id == 1 || id == 2);
}

void Leg::setHip(float pos){
  hip.write(motorFloatToDeg(pos, 1));
}

void Leg::setThigh(float pos){
  thigh.write(motorFloatToDeg(pos, 2));
}
  
void Leg::setKnee(float pos){
  knee.write(motorFloatToDeg(pos, 3));
}
  
  
void Leg::back(){
  setHip(0.0);
  setThigh(0.0);
  setKnee(0.0);
}

void Leg::up(){
  setHip(1.0);
  setThigh(1.0);
  setKnee(1.0);
}

void Leg::down(){
  setHip(0.5);
  setThigh(0.0);
  setKnee(0.0);
}

void Leg::read(){
  Serial.print("read");
  Serial.print(id);
  Serial.print("\n");
  Serial.print("hip: ");
  Serial.print(hip.read());
  Serial.print("\n");
  Serial.print("thigh: ");
  Serial.print(thigh.read());
  Serial.print("\n");
  Serial.print("knee: ");
  Serial.print(knee.read());
  Serial.print("\n");
}




void Leg::setMuscles (int hipPin, int thighPin, int kneePin) { 
  // Set up the motors
  hip.attach(hipPin);
  thigh.attach(thighPin);
  knee.attach(kneePin);

  hip.write(100);
  thigh.write(40);
  knee.write(80);
}


// Front left leg
const int flHip =   6;
const int flThigh = 7;
const int flKnee =  5;
Leg fl;

// Frong right
const int frHip =   35;
const int frThigh = 31;
const int frKnee =  33;
Leg fr;

// Back left
const int blHip =   9;
const int blThigh = 10;
const int blKnee =  8;
Leg bl;

// Back right
const int brHip =   47;
const int brThigh = 43;
const int brKnee =  45;
Leg br;


void setup() { 

  Serial.begin(9600);

  fl.setMuscles(flHip, flThigh, flKnee);
  fr.setMuscles(frHip, frThigh, frKnee);
  bl.setMuscles(blHip, blThigh, blKnee);
  br.setMuscles(brHip, brThigh, brKnee);

  fl.id = 1;
  fr.id = 2;
  bl.id = 4;
  br.id = 3;
} 


void loop() {


  

  //fr.straight();
  //bl.straight();
  //br.straight();

  //fr.up();
  //bl.up();
  //br.up();

  fl.setHip(0.0);
  fr.setHip(0.0);
  bl.setHip(0.0);
  br.setHip(0.0);

  fl.setThigh(0.0);
  fr.setThigh(0.0);
  bl.setThigh(0.0);
  br.setThigh(0.0);

  delay(3000);

  fl.setHip(0.5);
  fr.setHip(0.5);
  bl.setHip(0.5);
  br.setHip(0.5);

  delay(3000);

  fl.setHip(1.0);
  fr.setHip(1.0);
  bl.setHip(1.0);
  br.setHip(1.0);
  delay(3000);

  /*
  delay(2000);

  fl.setHip(0.0);
  fr.setHip(0.0);
  bl.setHip(0.0);
  br.setHip(0.0);

  delay(2000);

  fl.setHip(1.0);
  fr.setHip(1.0);
  bl.setHip(1.0);
  br.setHip(1.0);
  */

  /*
  fl.up();
  fr.up();
  bl.up();
  br.up();

  delay(1000);

  fl.down();
  fr.down();
  bl.down();
  br.down();

  delay(1000);
  */



  //myLeg.up();
//  delay(4000);

 // myLeg.down();
//  delay(4000);


  /*
  
   knee.write(10);
   delay(10);
   
   thigh.write(100);
   
   
   delay(100);
   
   // Shift leg forward  
   torso.write(20);
   
   
   
   
   delay(500);
   
   
   // Knee up
   
   knee.write(100);
   delay(10);
   thigh.write(10);
   delay(400);
   torso.write(100);
   
   
   
   
   delay(150);
   
   
   */
}

