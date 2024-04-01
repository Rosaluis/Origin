/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

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

void setup() {
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

void get_color(int r = 0, int g = 0, int b = 0) {
  //if ((r < 238 && r >= 218) && (g < 248 && g >= 218) && (b < 258 && b >= 238)) {
  if (r > 2500 && g > 2500 && b > 2500) {
    Serial.println("empty");
  }
  else if ((r > 100 && r < 500) && (g > 100 && g < 500) && (b > 100 && b < 400)) {
    Serial.println("white");
  }
  else if ((r > 600 && r < 1500) && (g > 1500 /*&& g < 1900*/) && (b > 1200 /*&& b < 1500*/)) {
    Serial.println("red"); // *
  }
  else if ((r > 1300 && r < 1500) && (g > 700 && g < 1100) && (b > 900 && b < 1200)) {
    Serial.println("green");
  }
  else if ((r > 700 && r < 1200) && (g > 500 && g < 1000) && (b > 500 && b < 1000)) {
    Serial.println("blue"); // *
  }
  else if ((r > 1300 && r < 1400) && (g > 1400 && g < 1500) && (b > 700 && b < 800)) {
    Serial.println("yellow");
  }

}

void loop() {
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  
   // Printing the RED (R) value
  Serial.print("R = ");
  Serial.print(redFrequency);
  delay(100);
  
  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);
  
  // Printing the GREEN (G) value  
  Serial.print(" G = ");
  Serial.print(greenFrequency);
  delay(100);
 
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);
  
  // Printing the BLUE (B) value 
  Serial.print(" B = ");
  Serial.println(blueFrequency);
  delay(100);

  get_color(redFrequency, greenFrequency, blueFrequency);


}

