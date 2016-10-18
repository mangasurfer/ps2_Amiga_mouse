#include <ps2.h>

/*
 * an arduino sketch to interface a Amiga with a ps/2 mouse.
 * most of the code is from the remote amiga mouse project by https://github.com/mntmn
 *
 * xb and ya are ment to be pin 3,4 but soldering error they got switched 
 */
static int pin_xa = 2;
static int pin_xb = 4;
static int pin_ya = 3;
static int pin_yb = 5;
static int pin_lb = 6;
static int pin_rb = 7;
/*
 * Pin 10 is the mouse data pin, pin 9 is the clock pin
 * Feel free to use whatever pins are convenient.
 */
PS2 mouse(10, 9);

/*
 * initialize the mouse. Reset it, and place it into remote
 * mode, so we can get the encoder data on demand.
 */
void mouse_init()
{
  mouse.write(0xff);  // reset
  mouse.read();  // ack byte
  mouse.read();  // blank */
  mouse.read();  // blank */
  mouse.write(0xf0);  // remote mode
  mouse.read();  // ack
  delayMicroseconds(100);
}

void setup()
{
  Serial.begin(9600);
  
  pinMode(pin_xa, OUTPUT);
  pinMode(pin_xb, OUTPUT);
  pinMode(pin_ya, OUTPUT);
  pinMode(pin_yb, OUTPUT);
  pinMode(pin_lb, OUTPUT);
  pinMode(pin_rb, OUTPUT);
  
  mouse_init();
  digitalWrite(pin_lb,HIGH);
  digitalWrite(pin_rb,HIGH);
  
}
// amiga mouse code
static int xspeed = 0;
static int yspeed = 0;

void send_down() {  
  digitalWrite(pin_yb, HIGH);
  delay(yspeed);
  digitalWrite(pin_ya, HIGH);
  delay(yspeed);
  digitalWrite(pin_yb, LOW);
  delay(yspeed);
  digitalWrite(pin_ya, LOW);
  delay(yspeed);
}

void send_up() {
  digitalWrite(pin_ya, HIGH);
  delay(yspeed);
  digitalWrite(pin_yb, HIGH);
  delay(yspeed);
  digitalWrite(pin_ya, LOW);
  delay(yspeed);
  digitalWrite(pin_yb, LOW);
  delay(yspeed);
}

void send_left() {
  digitalWrite(pin_xa, HIGH);
  delay(yspeed);
  digitalWrite(pin_xb, HIGH);
  delay(yspeed);
  digitalWrite(pin_xa, LOW);
  delay(yspeed);
  digitalWrite(pin_xb, LOW);
  delay(yspeed);
}

void send_right() {
  digitalWrite(pin_xb, HIGH);
  delay(yspeed);
  digitalWrite(pin_xa, HIGH);
  delay(yspeed);
  digitalWrite(pin_xb, LOW);
  delay(yspeed);
  digitalWrite(pin_xa, LOW);
  delay(yspeed);
}
/*
 * get a reading from the mouse and report it back to the
 * host via the serial line.
 */
void loop()
{
  char mstat;
  char mx;
  char my;

  /* get a reading from the mouse */
  mouse.write(0xeb);  // give me data!
  mouse.read();      // ignore ack
  mstat = mouse.read();
  mx = mouse.read();
  my = mouse.read();

  /* send the data back up. what hell is it doing */
  Serial.print(mstat, BIN);
 Serial.print("\tX=");
  Serial.print(mx, DEC);
  Serial.print("\tY=");
 Serial.print(my, DEC);
  Serial.println();
//  delay(20);  /* twiddle */

if (my < 0) send_down();

if (my > 0) send_up();

if (mx > 0) send_left();

if (mx < 0) send_right();
  
 //mouse buttons kiss
 
if (mstat & 1){
digitalWrite(pin_lb,LOW);
}
else
{
  digitalWrite(pin_lb,HIGH);
}
if (mstat & 2){
digitalWrite(pin_rb,LOW);
}
else
{
  digitalWrite(pin_rb,HIGH);
}
}
