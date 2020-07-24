#include <Servo.h>

/*
 * Aldo Polanco
 * Punch-It
 * Note: some methods were initially class methods, but the program had trouble editing instance variables
 * from outside the class (even with class methods) unless the methods that edited them were not class methods
 * hence, methods that seem like they could be class methods ended up being global methods
 */

#define ENEMY_LS 36//enemy left punch servo
#define ENEMY_RS 34//enemy right punch servo
#define ENEMY_CS 35//enemy center servo
#define ENEMY_LS_DD 0//enemy left punch servo degrees in rest
#define ENEMY_RS_DD 70//enemy right punch servo in rest
#define ENEMY_CS_DD 20//enemy center servo in rest
#define ENEMY_HP1 7 //enemy health led 1
#define ENEMY_HP2 5 //enemy health led 2
#define ENEMY_HP3 4 //enemy health led 3
#define ENEMY_HP4 3 //enemy health led 4
#define ENEMY_HP5 2 //enemy health led 5
#define PLAYER_LS 27 //player left servo pin
#define PLAYER_RS 29 //player right servo pin
#define PLAYER_CS 25 //player center servo pin
#define PLAYER_LS_DD 70//player left punch servo degrees in rest
#define PLAYER_RS_DD 100//player right punch servo in rest
#define PLAYER_CS_DD 65//player center servo in rest
#define PLAYER_HP1 12 //PLAYER health led 1
#define PLAYER_HP2 11 //PLAYER health led 2
#define PLAYER_HP3 10 //PLAYER health led 3
#define PLAYER_HP4 9 //PLAYER health led 4
#define PLAYER_HP5 8 //PLAYER health led 5
#define JOYSTICK_Y A15//Joystick X direction (analog pin)
#define JOYSTICK_X A14//Joystick Y direction (analog pin)
#define A_BUTTON 44 //right button
#define B_BUTTON 45 //left button
#define READY_LED 30//Red timer led
#define SET_LED 31//Yellow timer led
#define GO_LED 32//Green timer led
#define BEEPER 23 //beeper pin


class Player
{
  public:
  Player(int healthPin1, int healthPin2, int healthPin3, int healthPin4, int healthPin5) { //class constructor
    health = 100; //health starts at zero
	  healthPins[0] = healthPin1; //list of health pins
    healthPins[1] = healthPin2; 
    healthPins[2] = healthPin3;
    healthPins[3] = healthPin4;
    healthPins[4] = healthPin5;
    lastMove = -1;
   
    }
  int getHealth() { 
    return health;
    }
  void changeHealth(int change) { 
	  health = health - change;
  }
  void resetHealth() {
    health = 100;
    }
  void setHealthPin(int index, int pin) { //debugging method
    healthPins[index] = pin;
    }
	void updateHealth() { //updates health LEDs according to health value
  Serial.println(healthPins[0]);
		switch (health)  {
			case 90 ... 100:
			for (int i = 0; i < 5; i++ ) {
				analogWrite(healthPins[i], 255);
        
			}
      break;
      case 80 ... 89:
      analogWrite(healthPins[4], 50);
      for (int i = 0; i < 4; i++ ) {
        analogWrite(healthPins[i], 255);
      }
      break;
      case 70 ... 79:
      analogWrite(healthPins[4], 0);
      for (int i = 0; i < 4; i++ ) {
        analogWrite(healthPins[i], 255);
      }
      break;
      case 60 ... 69:
      analogWrite(healthPins[4], 0);
      analogWrite(healthPins[3], 50);
      for (int i = 0; i < 3; i++ ) {
        analogWrite(healthPins[i], 255);
      }
      break;
      case 50 ... 59:
      analogWrite(healthPins[4], 0);
      analogWrite(healthPins[3], 0);
      for (int i = 0; i < 3; i++ ) {
        analogWrite(healthPins[i], 255);
      }
      break;
      case 40 ... 49:
      analogWrite(healthPins[4], 0);
      analogWrite(healthPins[3], 0);
      analogWrite(healthPins[2], 50);
      for (int i = 0; i < 2; i++ ) {
        analogWrite(healthPins[i], 255);
      }
      break;
      case 30 ... 39:
      analogWrite(healthPins[4], 0);
      analogWrite(healthPins[3], 0);
      analogWrite(healthPins[2], 0);
      for (int i = 0; i < 2; i++ ) {
        analogWrite(healthPins[i], 255);
      }
      break;
      case 20 ... 29:
      analogWrite(healthPins[4], 0);
      analogWrite(healthPins[3], 0);
      analogWrite(healthPins[2], 0);
      analogWrite(healthPins[1], 50);
      analogWrite(healthPins[0], 255);
      break;
      case 11 ... 19:
      analogWrite(healthPins[4], 0);
      analogWrite(healthPins[3], 0);
      analogWrite(healthPins[2], 0);
      analogWrite(healthPins[1], 0);
      analogWrite(healthPins[0], 255);
      
      break;
      case 1 ... 10:
      analogWrite(healthPins[4], 0);
      analogWrite(healthPins[3], 0);
      analogWrite(healthPins[2], 0);
      analogWrite(healthPins[1], 0);
      analogWrite(healthPins[0], 100);    
      break;
      case -30 ... 0:
      analogWrite(healthPins[4], 0);
      analogWrite(healthPins[3], 0);
      analogWrite(healthPins[2], 0);
      analogWrite(healthPins[1], 0);
      analogWrite(healthPins[0], 0);
      
		}
	}
  void resetMove() { //sets 
    lastMove = -1;
    }
  void setLastMove(int moves) {
    lastMove = moves;
    }
  int getLastMove(){ 
    return lastMove;
    }
    
  private:
  int health;
  int leftDegs;
  int rightDegs;
  int mainDegs;
  int lastMove;
  int healthPins[5];
} ;

Player user(PLAYER_HP1, PLAYER_HP2, PLAYER_HP3, PLAYER_HP4, PLAYER_HP5); //starts player objects
Player cpu(ENEMY_HP1, ENEMY_HP2, ENEMY_HP3, ENEMY_HP4, ENEMY_HP5);
Servo userLeft; //declares servos
Servo userRight;
Servo userCenter;
Servo cpuLeft;
Servo cpuRight;
Servo cpuCenter;
int timePass = 9999999;
int timeStart = 0;
int timeLights;
boolean start = false;
int ledList[] = {PLAYER_HP1, PLAYER_HP2, PLAYER_HP3, PLAYER_HP4, PLAYER_HP5, READY_LED, SET_LED, GO_LED, ENEMY_HP1, ENEMY_HP2, ENEMY_HP3, ENEMY_HP4, ENEMY_HP5};
int curLed = 0;

  void userMoveServo() { //moves servos according 
    switch (user.getLastMove()) {
      case -1: //User did nothing
      userLeft.write(120);
      userRight.write(45);
      break;
      case 0: //Left punch
      userLeft.write(25);
      userCenter.write(40);
      break;
      case 1: //Right punch
      userCenter.write(60);
      userRight.write(135);
      break;
      case 2: //Left dodge
      userCenter.write(80);
      break;
      case 3: //Right dodge
      userCenter.write(40);
      break;
      case 4: //Guard
      userLeft.write(35);
      userRight.write(145);
      break;
    }
  }

   void cpuMoveServo() { //moves servo according to cpu moves
    switch (cpu.getLastMove()) {
      case 0: //Left punch
      cpuLeft.write(25);
      cpuCenter.write(40);
      break;
      case 1: //Right punch
      cpuCenter.write(10);
      cpuRight.write(40);
      break;
      case 2: //Left dodge
      cpuCenter.write(0);
      break;
      case 3: //Right dodge
      cpuCenter.write(40);
      break;
      case 4: //Guard
      cpuLeft.write(25);
      cpuRight.write(40);
      break;
    }
  }



void postMove() { //Makes all the calls needed after one turn is over
  userMoveServo();
  cpuMoveServo();
	user.updateHealth();
	cpu.updateHealth();
  delay(2000);
  user.resetMove();
  cpu.resetMove();
  userLeft.write(PLAYER_LS_DD);
  userRight.write(PLAYER_RS_DD);
  userCenter.write(PLAYER_CS_DD);
  cpuLeft.write(ENEMY_LS_DD);
  cpuRight.write(ENEMY_RS_DD);
  cpuCenter.write(ENEMY_CS_DD);
  digitalWrite(READY_LED, LOW);
  digitalWrite(SET_LED, LOW);
  digitalWrite(GO_LED, LOW);
  delay(1000);
}

void compareMoves() { //compares user and cpu moves and adjusts health values accordingly
  
  int usermove = user.getLastMove();
  int cpumove = cpu.getLastMove();
  if (usermove == -1) {
    user.changeHealth(30);
  }
  else if (usermove == 0) {
    switch (cpumove) {
      case 1:
      user.changeHealth(25);
      cpu.changeHealth(25);
      break;
      case 2:
      user.changeHealth(10);
      break;
      case 3:
      cpu.changeHealth(25);
      break;
      case 4:
      cpu.changeHealth(10);
      break;
     }      
    }
  else if (usermove == 1) {
    switch (cpumove) {
      case 1:
      user.changeHealth(25);
      cpu.changeHealth(25);
      break;
      case 3:
      user.changeHealth(10);
      break;
      case 2:
      cpu.changeHealth(25);
      break;
      case 4:
      cpu.changeHealth(10);
      break;
     }      
    }
  else if (cpumove == 0) {
    switch (usermove) {
      case 2:
      cpu.changeHealth(10);
      break;
      case 3:
      user.changeHealth(25);
      break;
      case 4:
      user.changeHealth(10);
      break;
      }
    } 
  else if (cpumove == 1) {
    switch (usermove) {
      case 3:
      cpu.changeHealth(10);
      break;
      case 2:
      user.changeHealth(25);
      break;
      case 4:
      user.changeHealth(10);
      break;
      }
  }
  else {
    return;
    }
    
  
}

boolean checkWin() { //checks if one of the players have won
  if (user.getHealth() <= 0 || cpu.getHealth() <= 0 ) {
    return true;
    } else return false;
  }

void setUserMove() { //sets the user's move according to controlls
  if (digitalRead(B_BUTTON) == LOW) {
    user.setLastMove(0);
    }
  else if (digitalRead(A_BUTTON) == LOW) {
    user.setLastMove(1);
  }
  else if (analogRead(JOYSTICK_Y) > 600) {
    user.setLastMove(2);
  }
  else if (analogRead(JOYSTICK_Y) < 400) {
    user.setLastMove(3);
  }
  else if (analogRead(JOYSTICK_X) > 600) {
    user.setLastMove(4);
    }
  else {
    return;
    }
  }

 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(READY_LED, OUTPUT);
  pinMode(SET_LED, OUTPUT);
  pinMode(GO_LED, OUTPUT);
  pinMode(PLAYER_HP1, OUTPUT);
  pinMode(PLAYER_HP2, OUTPUT);
  pinMode(PLAYER_HP3, OUTPUT);
  pinMode(PLAYER_HP4, OUTPUT);
  pinMode(PLAYER_HP5, OUTPUT);
  pinMode(ENEMY_HP1, OUTPUT);
  pinMode(ENEMY_HP2, OUTPUT);
  pinMode(ENEMY_HP3, OUTPUT);
  pinMode(ENEMY_HP4, OUTPUT);
  pinMode(ENEMY_HP5, OUTPUT);
  userLeft.attach(PLAYER_LS);
  userRight.attach(PLAYER_RS);
  userCenter.attach(PLAYER_CS);
  cpuLeft.attach(ENEMY_LS);
  cpuRight.attach(ENEMY_RS);
  cpuCenter.attach(ENEMY_CS);
  userLeft.write(PLAYER_LS_DD);
  userRight.write(PLAYER_RS_DD);
  userCenter.write(PLAYER_CS_DD);
  cpuLeft.write(ENEMY_LS_DD);
  cpuRight.write(ENEMY_RS_DD);
  cpuCenter.write(ENEMY_CS_DD);
  

}

void loop() {
 while (digitalRead(A_BUTTON) == HIGH || digitalRead(B_BUTTON) == HIGH) { //start menu loop, waits until a button is pressed
  if (curLed%26 >= 13) { //shows a nice light show while it's not being played
    digitalWrite(ledList[curLed%13], LOW);
    delay(50);
    }
  else {
    digitalWrite(ledList[curLed%13], HIGH);
    delay(50);
    }
    curLed++;
  }
  delay(500); //game to start
  user.resetHealth(); //resets health and led's accordingly
  cpu.resetHealth();
  digitalWrite(READY_LED, LOW);
  digitalWrite(SET_LED, LOW);
  digitalWrite(GO_LED, LOW);
  user.updateHealth();
  cpu.updateHealth();
 while (!checkWin()) { //play loop, runs until winner exists
  timeStart = millis();
  while (timePass - timeStart <= 3000) { //single turn loop, checks for controls and lights red and yellow countdown led
    timePass = millis();
    switch (timePass - timeStart) {
      case 1501 ... 3000:
      digitalWrite(SET_LED, HIGH);
      case 0 ... 1500:
      digitalWrite(READY_LED, HIGH);
      }
      setUserMove();
      
    }

  cpu.setLastMove(random(0, 5)); //chooses the cpu's move
    if (user.getLastMove() == -1) { //if user didnt do anything, guard is down, opponent punches always
      cpu.setLastMove(random(0,2));
      }
    digitalWrite(GO_LED, HIGH); 
    compareMoves();
    postMove();
    
    
  }
  cpu.updateHealth();
  user.updateHealth();
  if (user.getHealth() > 0 && cpu.getHealth() <= 0) { //loop is over, check if user won and makes nice sounds + flashes health led
       tone(BEEPER, 500);
      delay(200);
      tone(BEEPER, 600);
      delay(200);
      tone(BEEPER, 700);
      delay(200);
      noTone(BEEPER);
    for (int i = 0; i < 10; i++) {
      analogWrite(PLAYER_HP1, 0);
      analogWrite(PLAYER_HP2, 0);
      analogWrite(PLAYER_HP3, 0);
      analogWrite(PLAYER_HP4, 0);
      analogWrite(PLAYER_HP5, 0);
      delay(500);
      analogWrite(PLAYER_HP1, 255);
      analogWrite(PLAYER_HP2, 255);
      analogWrite(PLAYER_HP3, 255);
      analogWrite(PLAYER_HP4, 255);
      analogWrite(PLAYER_HP5, 255);
      delay(500);
      } 
  } else if (cpu.getHealth() > 0 && user.getHealth() <= 0) { //user lost, ugly sounds and enemy leds flash
    tone(BEEPER, 700);
      delay(200);
      tone(BEEPER, 400);
      delay(200);
    for (int i = 0; i < 10; i++) {
      analogWrite(ENEMY_HP1, 0);
      analogWrite(ENEMY_HP2, 0);
      analogWrite(ENEMY_HP3, 0);
      analogWrite(ENEMY_HP4, 0);
      analogWrite(ENEMY_HP5, 0);
      delay(500);
      analogWrite(ENEMY_HP1, 255);
      analogWrite(ENEMY_HP2, 255);
      analogWrite(ENEMY_HP3, 255);
      analogWrite(ENEMY_HP4, 255);
      analogWrite(ENEMY_HP5, 255);
      delay(500);
      } 
  } else {
    for (int i = 0; i < 3; i++) { //tie (very unusual) , countdown leds flash and game starts over
      digitalWrite(READY_LED, LOW);
      delay(500);
      digitalWrite(SET_LED, LOW);
      delay(500);
      digitalWrite(GO_LED, LOW);
      delay(500);
      digitalWrite(READY_LED, HIGH);
      delay(500);
      digitalWrite(SET_LED, HIGH);
      delay(500);
      digitalWrite(GO_LED, HIGH);
      delay(500);
      }
    }
    user.resetHealth(); //restart health and start over
    cpu.resetHealth();
   
}
