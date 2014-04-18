## Open a serial connection with Arduino.

from time import sleep
import serial
import Image


#Continuously pings the serial until it receives an acknowledgement back
def sendPing(serial):

  while True:

    serial.write(str("a"))

    sleep(.1);

    if ser.inWaiting():
      #Got ack back, stop for a second for arduino to catch back up and start sending picture info
      if ser.readline().rstrip() == 'b':
        sleep(1)
        return;

def sendAndAck(data):
  return

def waitAndReturnSerialData():

  stillWaiting = True
  returnVal = ""

  #Loop while until we get data back from the arduino
  while(stillWaiting):
    sleep(.1)
    #Read data and exit loop
    if(ser.inWaiting()):
      returnVal = ser.readline().rstrip()
      stillWaiting = False

  return returnVal

def sendPictureInfo(image):

  pix = image.load()

  imageWidth = str(image.size[0])
  imageHeight = str(image.size[1])

  ser.write(imageWidth)

  print "Wrote width ", imageWidth
  #Get the response from arduino
  returnVal = waitAndReturnSerialData()

  print "Got width ", returnVal

  #Wait until all serial stuff is done
  sleep(.1)

  ser.write(imageHeight)

  print "Write height ", imageHeight

  returnVal = waitAndReturnSerialData()

  print "Got height ", returnVal


  #Iterate through all of the pixels in the image
  for x in range(image.size[0]):
    for y in range(image.size[1]):
      if pix[x, y] == 0:
        ser.write('1')
      else:
        ser.write('0')
      print waitAndReturnSerialData(),
    print '\n'
  print '---------------------------\n'
  for x in range(image.size[0]):
    for y in range(image.size[1]):
      print waitAndReturnSerialData(),
    print '\n'

  return 


im = Image.open("checkers.gif")

ser = serial.Serial("/dev/ttyACM0", 9600)   # open serial port that Arduino is using

# Reminder to close the connection when finished
if(ser.isOpen()):

  sendPing(ser)
  sendPictureInfo(im);

ser.close();

print "closed"
