#include <Servo.h>

Servo servo1;
Servo servo2;

/*
 * Input pins for the color sensor & servos
 */
const int s0 = 2;
const int s1 = 3;
const int s2 = 4;
const int s3 = 5;
const int out = 10;
const int servo1_pin = 6;
const int servo2_pin = 7;

/*
 * Vars
 */
const int tot_colors = 5; // 4 colors + 1 empty "color"
const int rep = 8; // repeat xx color reads
const int d = 5;   // add range of XX to the read

volatile unsigned de = 1;  // delay servo movement

/*
 * Servo positions  
 * Set the positions in degree where the servo 
 * has to stop to drop the ball
 */
const int s1pos[] = { 120, 90, 50 ,0 ,180};
const int s2pos[] = { 160, 34, 58, 80, 104}; // first item = unknown color

int colors_found = 0;
unsigned long red, green, blue = 0;

typedef struct{
  int red;
  int green;
  int blue;
}  colors_type;

colors_type colors[tot_colors]; // length of array:  sizeof(colors)/sizeof(colors_type)


void setup() {

  
  Serial.begin(9600);
  servo1.attach(servo1_pin);
  servo2.attach(servo2_pin);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);

  initCalibration();
   
}
void loop(){

    load();
    pos();
    readColor();
    checkColor();

}

void initCalibration(){
  gopos(0,4);
  delay(1000);
  gopos(0,5);
  delay(1000);
  gopos(0,2);  // empty the loader for calibration
  eject();  //delay(500);
  pos();    
  delay(500);
  readColor();
 
  colors[0].red   = red;
  colors[0].green = green;
  colors[0].blue  = blue;
  
  //Serial.println("Empty: ");
  //Serial.println(String(red) + " "+ String(green) + " " + String(blue));
}

void checkColor(){
 
  for (int i = 0 ; i < 5 ; i++) {

    //  Serial.println(String(i) + ": "+ String(colors[i].red) + " " + String(colors[i].green) + " "+ String(colors[i].blue));
      
        if (  
              inRange(red,   colors[i].red   - d, colors[i].red   + d) &&
              inRange(green, colors[i].green - d, colors[i].green + d) &&
              inRange(blue,  colors[i].blue  - d, colors[i].blue  + d)   
           )
        {   
            // Color match!
          if (i==0) {
            delay(100);
          //  eject();
            return; 
          }

            //Serial.println(String(red) + " "+ String(green) + " " + String(blue));
            Serial.println("Color match " + String(i));
            gopos(i,2);       
            eject();
            return;
        }
  }
  
  if (colors_found < tot_colors){ // Color not found, adding color to the array
    
    colors_found++;
    Serial.print("New color found! Tot colors: ");
    Serial.println(colors_found);
    if (colors_found >= 5){
      gopos(0,2);
      eject();
      return;
    
    }
    gopos(colors_found,2 );
    eject();
    colors[colors_found].red   = red;
    colors[colors_found].green = green;
    colors[colors_found].blue  = blue;
    return;
  }
    gopos(0,2);
    eject();
  
    //initCalibration();
 //delay(100);
}
void readColor(){

  red = green = blue = 0;
  
  for (int i = 1; i <= rep ; i++) {
    
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
    red   += pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
    
    digitalWrite(s3, HIGH);
    blue += pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
    
    digitalWrite(s2, HIGH);
    green += pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
    
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
   
void load() {
  gopos(0,1);
  
  
}
void pos() {
  gopos(1,1);
}
void eject() {
  gopos(2,1);
}

bool inRange(int val, int minimum, int maximum) {
  return ((minimum <= val) && (val <= maximum));
}

void gopos(int p, int z){

  int r  = (z == 1) ? servo1.read() : servo2.read();  
  int go = (z == 1) ? s1pos[p] : s2pos[p];

  if (r == go ) return;
  
  if (go > r){

    for (int s = r; s <= go ;s++){
      if (z == 1)   servo1.write(s);
      if (z == 2)   servo2.write(s);
      delay(de);
    }
    
  }
  else{

      for (int s = r; s >= go ; s--){
        if (z == 1)   servo1.write(s);
        if (z == 2)   servo2.write(s);
        delay(de);
      } 
  }
  delay(90);
}
