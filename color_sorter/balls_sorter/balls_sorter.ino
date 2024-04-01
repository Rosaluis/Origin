#include <Servo.h>

// TCS230 or TCS3200 pins wiring to Arduino
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 10

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

Servo door;
Servo channel;
int old_door = 0;
int old_arm = 0;
int pos = 0;

unsigned long red, green, blue = 0;
int rep = 8;

typedef struct{
  int red;
  int green;
  int blue;
}  colors_type;



int color_found = 0;

void setup() {
  door.attach(6);
  channel.attach(7);

  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
   // Begins serial communication 
  Serial.begin(115200);
}

int get_red_fr() {
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, HIGH, 20000);
  
   // Printing the RED (R) value
  //Serial.print("R = ");
  //Serial.print(redFrequency);
  delay(100);
  return(redFrequency);
  }

int get_green_fr() {
  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, HIGH, 20000);
  
  // Printing the GREEN (G) value  
  //Serial.print(" G = ");
  //Serial.print(greenFrequency);
  delay(100);
  return(greenFrequency);
  }
 
int get_blue_fr() {
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, HIGH, 20000);
  
  // Printing the BLUE (B) value 
  //Serial.print(" B = ");
  //Serial.println(blueFrequency);
  delay(100);
  return(blueFrequency);
  }

void readColor(){

  red = green = blue = 0;
  
  for (int i = 1; i <= rep ; i++) {
    
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    red   += pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH);
    
    digitalWrite(S3, HIGH);
    blue += pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH);
    
    digitalWrite(S2, HIGH);
    green += pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH);
    
     delay(3);
  }
  //Serial.println(String(red) + " " + String(green) + " " + String(blue) + " ");
  
  red = red / rep;
  blue = blue / rep;
  green = green / rep;

  int sum = red + green + blue;

  red = red * 100 / sum;
  blue = blue * 100 / sum;
  green = green * 100/ sum;
  
  Serial.println(String(red) + " " + String(green) + " " + String(blue) + " ");
}
  
void choose_channel() {
  readColor();
  int rd;
  rd = get_red_fr();
  int gr;
  gr = get_green_fr();
  int bl;
  bl = get_blue_fr();

  if ((rd == 0) || (gr == 0) || (bl == 0)) {
    Serial.println("empty");
    set_channel(1);  // rameno v 0
  } else { // kdyz je kulicka vevnitr
    set_channel(4);
    readColor();











  }
}




void turn_door(int p) {
  door.write(p);
  delay(1000);
}

void turn_arm(int p) {
  if (p >= old_arm) {
    for (old_arm; pos <= p; pos += 1) {
      channel.write(pos);
      delay(10);
    }
  }
  else {
    for (old_arm; pos >= p; pos -= 1) {
      channel.write(pos);
      delay(10);
    }
  }
  delay(1000);
  old_arm = p;
}

void load_ball() {
  set_door(1);
  delay(500);
  set_door(2);
}

void set_door(int pos) {
  switch (pos) {
    case 2:             // closed
      turn_door(120);
      break;
    
    case 3:             // leave ball
      turn_door(90);
      break;
    
    case 1:             // load ball
      turn_door(150);
      break;

    default:
      turn_door(90);
      break;
  }

}

void set_channel(int pos) {
  switch (pos) {
  case 1:
    turn_arm(0);
    break;
  case 2:
    turn_arm(40);
    break;
  case 3:
    turn_arm(70);
    break;
  case 4:
    turn_arm(100);
    break;
  case 5:
    turn_arm(130);
    break;
  case 6:
    turn_arm(160);
    break;

  default:
    turn_arm(160);
    break;
  }
}

void loop() {
  load_ball();
  choose_channel();
  set_door(3);

  load_ball();
  choose_channel();
  set_door(3);
  
  
  }


