#include <ps2.h>


/*
 * an arduino sketch to interface with a ps/2 mouse.
 * Also uses serial protocol to talk back to the host
 * and report what it finds.
 */

/*
 * Pin 5 is the mouse data pin, pin 6 is the clock pin
 * Feel free to use whatever pins are convenient.
 */
int posX=0;
int posY=0;
int led = 13;
int myX[] = { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40 };

int incX=0;
int incY=0;

boolean* picMatrix;

PS2 mouse(6, 5);

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

//Wait to get the ping from computer to begin receiving information
void wait_for_ping()
{
  while(true)
  {
    char val = ' ';
    
    if(Serial.available()){
      val = Serial.read();
      if(val == 'a'){ //If we got the ping, send back the ack
        Serial.println('b');
        delay(1000);
        return; 
      }
    }
  }
}

int receive_and_ack(){
  
  while(true)
  {
    return 0;
  }
  
  return -1;
}

void set_height_and_width(int* height, int* width) {
  
  char inChar;
  String imageWidth = "";
  String imageHeight = "";
  boolean gotWidth = false;
  boolean gotHeight = false;
  
  delay(1000);
  //Loop until we got width data
  while(!gotWidth) {
    //Loop until we get the imageWidth
    while(Serial.available()){
      inChar = Serial.read();
      imageWidth.concat(inChar);
      //Done reading data, break out of loop
      if(Serial.available() == 0) {
        gotWidth = true;
        break;
      }
    }
  }
  
  Serial.println(imageWidth);
  *width = imageWidth.toInt();
  //Wait until serial data is received
  delay(1000);
  
  //Loop until we got height data
  while(!gotHeight) {
    while(Serial.available()){
      inChar = Serial.read();
      imageHeight.concat(inChar);
      //Done reading data, break out of loop
      if(Serial.available() == 0) {
        gotHeight = true;
        break;
      }      
    } 
  }
  
  Serial.println(imageHeight);
  *height = imageHeight.toInt();
  
  
}

void set_matrix_position(int col, int row, int width, boolean* matrix, boolean value) {
  
  matrix[ row * width + col ] = value;  
}

boolean* setup_pic_mat()
{
  
  int imageHeight;
  int imageWidth;
  char value;
  boolean matValue;
  
  set_height_and_width(&imageHeight, &imageWidth);
  
  boolean pixMatrix[imageHeight * imageWidth];
  
  delay(1000);
  //Loop and get all of the boolean values for the image
  for(int i = 0; i < imageHeight; i++) {
    for(int j = 0; j < imageWidth; j++) {
      //Wait for data to be written
      delay(100);
      if(Serial.available()){
         value = Serial.read();
      }
      Serial.println(value);
      matValue = value == '1';
      set_matrix_position(j, i, imageWidth, pixMatrix, matValue);
    } 
  }
  
  for(int i = 0; i < imageHeight; i++) {
    for(int j = 0; j < imageWidth; j++) {
      if( pixMatrix[ i * imageWidth + j ]) {
        Serial.println('1'); 
      }else{
        Serial.println('0'); 
      }
    }
  }
  
  return pixMatrix;
  
}

void setup()
{
  pinMode(led, OUTPUT); 
  Serial.begin(9600);
  wait_for_ping();
  //Set up the picture Matrix
  picMatrix = setup_pic_mat();
  mouse_init();

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
  
  int interx= mx;
  int intery= my;
  

  /* get a reading from the mouse */
  mouse.write(0xeb);  // give me data!
  mouse.read();      // ignore ack
  mstat = mouse.read();
  mx = mouse.read();
  my = mouse.read();
  
  incX = mx-interx;
  incY = my-intery;
  
  posX= posX + incX;
  posY= posY + incY;

//  Serial.print(posX);
//  Serial.print(",");
//  Serial.print(posY);
//  Serial.println();
  
  int val = Serial.read();
  
//  if(val>0){
//    Serial.println(val);
//  }

//for (int i=0; i<10; i++)
//{
//  if (posX == myX[i])
//   {
//     digitalWrite(led, HIGH);
//     delay(10);
//   }
//  else
//  {
//    digitalWrite(led, LOW);
//    delay(10);
//  }
//}

  /* send the data back up */
//  Serial.print(mstat, BIN);
//  Serial.print("\tX=");
//  Serial.print(mx, DEC);
//  Serial.print("\tY=");
//  Serial.print(my, DEC);
//  Serial.println();



//  delay(20);  /* twiddle */
}
