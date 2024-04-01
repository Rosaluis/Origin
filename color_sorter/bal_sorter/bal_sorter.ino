#include <Servo.h>
Servo door;
Servo channel;
int old_door = 0;
int old_arm = 0;
int pos = 0;

void setup() {
  door.attach(6);
  channel.attach(7);
  Serial.begin(115200);
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
    turn_arm(80);
    break;
  case 4:
    turn_arm(120);
    break;
  case 5:
    turn_arm(160);
    break;

  default:
    turn_arm(80);
    break;
  }
}

void loop() {
  load_ball();
  set_channel(1);
  set_door(3);

  load_ball();
  set_channel(4);
  set_door(3);
  
  load_ball();
  set_channel(2);
  set_door(3);

  load_ball();
  set_channel(5);
  set_door(3);

  load_ball();
  set_channel(3);
  set_door(3);
  
  }


