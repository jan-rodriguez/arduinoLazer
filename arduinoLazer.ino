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

int scale = 100;
int matrixWidth;
int matrixHeight;
boolean* picMatrix;

PS2 mouse(6, 5);

struct Rect {
  int top;
  int bottom;
  int left;
  int right;
};

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
  
  matrixWidth = imageWidth.toInt();
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
  matrixHeight = imageHeight.toInt();
  
  
}

void set_matrix_position(int col, int row, int width, boolean* matrix, boolean value) {
  
  matrix[ row * width + col ] = value;  
}

boolean get_matrix_position(int col, int row, int width, int height, boolean* matrix) {
  //Check bounds
  if(col >= 0 && col < width && row >= 0 && row < height) {
    return matrix[ row * width + col ];
  }
  
  return false;
  
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
      delay(50);
      if(Serial.available()){
         value = Serial.read();
      }
      //Serial.println(value);
      matValue = value == '1';
      set_matrix_position(j, i, imageWidth, pixMatrix, matValue);
    } 
  }
  
  
//  for(int i = 0; i < imageHeight; i++) {
//    for(int j = 0; j < imageWidth; j++) {
//      if( pixMatrix[ i * imageWidth + j ]) {
//        Serial.println('1'); 
//      }else{
//        Serial.println('0'); 
//      }
//    }
//  }
  
  return pixMatrix;
  
}

boolean within_rect(struct Rect rect, int x, int y){
  return x > rect.left && x < rect.right && y > rect.top && y > rect.bottom;
}

boolean* generate_mit() {
  int matHeight = 150;
  int matWidth = 200;
  
  int marg = 10;
  
  int xOffset = 20;
  int yOffset = 20;
  
  int barHeight = 80;
  int barWidth = 20;
  
  boolean picMat[matHeight * matWidth];
  
  Rect mBar1;
  mBar1.top = yOffset;
  mBar1.bottom = yOffset + barHeight;
  mBar1.left = xOffset;
  mBar1.right = xOffset + barWidth;
  
  Rect mBar2;
  mBar2.top = yOffset;
  mBar2.bottom = yOffset + ((3 * barHeight ) / 4);
  mBar2.left = (xOffset + barWidth + marg);
  mBar2.right = (xOffset + (2 * barWidth) + marg);
  
  Rect mBar3;
  mBar3.top = yOffset;
  mBar3.bottom = yOffset + barHeight;
  mBar3.left = (xOffset + barWidth + marg);
  mBar3.right = (xOffset + (2 * barWidth) + marg);

  Rect iDot;
  iDot.top = yOffset;
  iDot.bottom = yOffset + (barHeight / 5);
  iDot.left = xOffset + (3 * barWidth) + (3 * marg);
  iDot.right = xOffset + (4 * barWidth) + (3 * marg);

  Rect iBottom;
  iBottom.top = yOffset + (barHeight / 5) + marg;
  iBottom.bottom = yOffset + barHeight;
  iBottom.left = xOffset + (3 * barWidth) + (3 * marg);
  iBottom.right = xOffset + (4 * barWidth) + (3 * marg);

  Rect tTop;
  tTop.top = yOffset;
  tTop.bottom = yOffset + (barHeight / 5);
  tTop.left = xOffset + (4 * barWidth) + (4 * marg);
  tTop.right = xOffset + (6 * barWidth) + (4 * marg);

  Rect tBottom;
  tBottom.top = yOffset + (barHeight / 5) + marg;
  tBottom.bottom = yOffset + barHeight;
  tBottom.left = xOffset + (4 * barWidth) + (4 * marg);
  tBottom.right = xOffset + (5 * barWidth) + (4 * marg);

  
  for(int y = 0; y < matHeight; ++y){
    for(int x = 0; x < matWidth; ++x){
      //First bar in M
      if(within_rect(mBar1, x, y)) {
        set_matrix_position(x, y, matWidth, picMat, true);
      }
      //Second bar in M
      else if(within_rect(mBar2, x, y)){
          set_matrix_position(x, y, matWidth, picMat, true);
      }
      //Third bar in M
      else if(within_rect(mBar3, x, y)){
        set_matrix_position(x, y, matWidth, picMat, true);
      }
      //I dot
      else if(within_rect(iDot, x, y)){
        set_matrix_position(x, y, matWidth, picMat, true);
      }
      //Top of I
      else if(within_rect(iBottom, x, y)){
        set_matrix_position(x, y, matWidth, picMat, true);
      }
      //Top of T
      else if(within_rect(tTop, x, y)){
        set_matrix_position(x, y, matWidth, picMat, true);
      }
      //Rest of T
      else if(within_rect(tBottom, x, y)){
        set_matrix_position(x, y, matWidth, picMat, true);
      }
      else{ //default to false
      	set_matrix_position(x, y, matWidth, picMat, false);
      }
    
    }
  }
}

void setup()
{
  pinMode(led, OUTPUT); 
  Serial.begin(9600);
  //wait_for_ping();
  //Set up the picture Matrix
  //picMatrix = setup_pic_mat();
  picMatrix = generate_mit();
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

  Serial.print(posX / scale);
  Serial.print(",");
  Serial.print(posY / scale);
  Serial.println();
  
  int val = Serial.read();
  
  if(get_matrix_position(posX / scale, posY / scale, matrixWidth, matrixHeight, picMatrix)){
   
    digitalWrite(led,HIGH); 
    
  }else{
  
   digitalWrite(led, LOW); 
    
  }
  
}
