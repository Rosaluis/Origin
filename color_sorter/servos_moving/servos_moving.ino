#include <Servo.h>
Servo door;
Servo channel;

void setup() {
  door.attach(6);
  channel.attach(7);
}

void set_channel(int pos) {
  switch (pos) {
  case 1:
    channel.write(0);
    break;
  case 2:
    channel.write(40);
    break;
  case 3:
    channel.write(80);
    break;
  case 4:
    channel.write(120);
    break;
  case 5:
    channel.write(160);
    break;

  default:
    channel.write(80);
    break;
  }
}



void loop() {
  
  door.write(150); // free input
  delay(1000);
  /*door.write(125); // middle position 
  delay(1000);*/
  door.write(90); // free output
  delay(1000);
  
  
  /*set_channel(1);
  delay(3000);
  set_channel(2);
  delay(3000);*/
  set_channel(3);
  delay(3000);
  /*set_channel(4);
  delay(3000);
  set_channel(5);
  delay(3000);*/

/*
void loop() {
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(15);
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(15);
  }
  */
}
