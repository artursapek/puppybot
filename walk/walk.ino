#include <Servo.h> 

//#include <Leg.h>

using namespace std;

class Leg {


//  unsigned long lastTorso, lastThigh, lastKnee;
public:

  Servo hip;
  Servo thigh; 
  Servo knee;

  void setMuscles (int, int, int);
  void setPosition(int pos);
  

  // Given a float between 0.0 and 1.0
  // and a motor id,
  //   1 => hip
  //   2 => thigh
  //   3 => knee
  // return a degree count for that specific motor.
  int motorFloatToDeg(float pos, int motorID);

  // pos: 0.0-1.0
  float pos;
  // dir: 0 (forward) or 1 (reverse)
  int dir;

  void advance();

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
  void setAll(float);

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

int posOfRangeReverse(float pos, float max, float min) {
  float diff = max - min;
  return max - (pos * diff) + 0.5;
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
          deg = posOfRangeReverse(pos, 100., 50.);
          break;

        case 3:
          // BR
          // middle = 70
          deg = posOfRangeReverse(pos, 140., 80.);
          break;

        case 4:
          // BL
          deg = posOfRange(pos, 30., 90.);
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
          deg = posOfRangeReverse(pos, 90., 40.);
          break;

        case 2:
          // FR
          // middle = 70
          deg = posOfRange(pos, 10., 60.);
          break;

        case 3:
          // BR
          // middle = 70
          //deg = posOfRange(pos, 10., 90.);
          deg = 10;
          break;

        case 4:
          // BL
          // middle = 120
          //deg = posOfRangeReverse(pos, 10., 90.);
          deg = 90;
          break;
      }
      

      break;
    case 3:
      // Knee
      // 0.0 = Straight down (80)
      // 1.0 = Bent back towards inside of knee (140)

      switch (id) {
        case 1:
          // FL
          deg = posOfRange(pos, 80., 150.);
          break;

        case 2:
          // FR
          deg = posOfRangeReverse(pos, 100., 30.);
          break;

        case 3:
          // BR
          deg = posOfRange(pos, 90., 120.);
          break;

        case 4:
          // BL
          deg = posOfRangeReverse(pos, 90., 60.);
          break;
      }
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
  
void Leg::setAll(float pos){
  setHip(pos);
  setThigh(pos);
  setKnee(pos);
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

void Leg::advance (){
  Serial.println(dir);
  Serial.println(pos);
  Serial.println("");
  switch (dir) {
    // Go up
    case 0:
      if (pos > 0.7) {
        dir = 1;
        pos = 0.7;
      } else {
        pos += 0.1;
      }
      setAll(pos);
      break;
    // Extend knee
    case 1:
      if (pos < 0.1) {
        dir = 2;
        pos = 0.7;
      } else {
        pos -= 0.1;
      }
      if (id == 1 || id == 2){
        setKnee(pos);
      }
      break;
    // Go back down
    case 2:
      if (pos < 0.1) {
        dir = 0;
        pos = 0.1;
      } else {
        pos -= 0.1;
      }
      setAll(pos);
      break;
      
  }
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

  fl.pos = 0.7;
  fr.pos = 0.0;
  bl.pos = 0.0;
  br.pos = 0.7;

  fl.dir = 2;
  fr.dir = 0;
  bl.dir = 0;
  br.dir = 2;

  fl.down();
  fr.down();
  bl.down();
  br.down();
}



// Dir 0 = reverse

int lastUpdate = 0;

void runOnMillis(int ms) {
  if ((ms - 30) > lastUpdate) {
    fl.advance();
    fr.advance();
    br.advance();
    bl.advance();

    lastUpdate = ms;
  }

}


void loop() {


  runOnMillis(millis());
  /*
  fl.down();
  fr.down();
  br.down();
  bl.down();
  */
}

