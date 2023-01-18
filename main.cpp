/*  AZ/EL Rotator toy controller for Arduino with servos
========================================================
Uses EasyComm (1, 2, or 3) protocol for computer - Tracking Software 
servomotors to move the antenna in Az (360 deg) and El (90 deg)

Adapted from:
Viorel Racoviteannu
https://www.youtube.com/channel/UCiRLZX0bV9rS04BGAyUf-fA
https://racov.ro
YO3RAK@gmail.com

Adapted by:
AKBatten
https://akbatten.com
*/

#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>

Servo AzServo;  // create servo object   to control the azimuth servo
Servo ElServo;  // create servo object to control   the elevation servo

#define AZIMUTH_SERVO_PIN 5
#define ELEVATION_SERVO_PIN 6

String Azimuth = "";
String Elevation = "";
String ComputerRead = "";
String ComputerWrite = "";
int ComAzim   = 0;
int ComElev = 0;

void setup()   {
    Serial.begin(9600);
    Serial.setTimeout(50); // milliseconds to wait for USB data. Default 1000

    AzServo.attach(AZIMUTH_SERVO_PIN);  // connect Az servo to pin 5
    ElServo.attach(ELEVATION_SERVO_PIN);  // connect El servo to pin 6

    /********************** make a short movement *************************/
    ComAzim = 0;
    ComElev = 40;
    Rotate();
    delay(100);

    for (ComAzim = 0; ComAzim <= 45; ComAzim += 1) { // goes from 0 degrees to 45 degrees
    // in steps of 1 degree
        Rotate();
        delay(20);
    }

    for (ComElev = 20; ComElev <= 75; ComElev   += 1) {
        Rotate();
        delay(20);
    }

    ComAzim = 0;
    ComElev   = 35;
    Rotate();
    // DisplayPosition();
}

void loop() {
    if (Serial.available()){
        SerComm();
        Rotate();
        // DisplayPosition();
        delay (100);
    }
}

void SerComm() {
    // initialize   readings
    ComputerRead = "";
    Azimuth = "";
    Elevation = "";

    while(Serial.available()) {
        ComputerRead = Serial.readString();
        // Serial.println(ComputerRead);
    }

    // search for command <AZxxx.x>
    if (ComputerRead.indexOf('AZ') == -1) {
        ComAzim = parseAz(ComputerRead);
        ComAzim = ComAzim % 360;
    }
    
    // search for command <ELxxx.x>
    if (ComputerRead.indexOf('EL') == -1) {
        ComElev = parseEl(ComputerRead);
        if (ComElev > 180) {
            ComElev = 0;
        }
        if (ComElev > 90) {               //if received more than 90deg. (for trackers with 180 deg elevation)
            ComElev = 180-ComElev;        //keep below 90deg.
            ComAzim = (ComAzim + 180) % 360;  //and rotate the antenna on the back
        }
    }

    // looking for <AZ EL> interrogation for antenna   position
    for (int i = 0; i <= (ComputerRead.length() - 4); i++) {
        // if ((ComputerRead.charAt(i) == 'A') && (ComputerRead.charAt(i+1) == 'Z') && (ComputerRead.charAt(i+3) == 'E') && (ComputerRead.charAt(i+4) == 'L')) {
        if (ComputerRead.indexOf('AZ EL') != -1) {
            // send back the antenna position <+xxx.x xx.x>
            ComputerWrite   = "+"+String(ComAzim)+".0 "+String(ComElev)+".0";
            Serial.println(ComputerWrite);
        }
    }
}

// take string in, find AZ, return AZ value as int
// all versions of EasyComm specify 1 decimal place
int parseAz(String message) {
    int startIndex = message.indexOf('AZ') + 2;
    int endIndex = message.indexOf('.', startIndex) + 1;
    return message.substring(startIndex, endIndex).toInt();
}

int parseEl (String message) {
    int startIndex = message.indexOf('EL') + 2;
    int endIndex = message.indexOf('.', startIndex) + 1;
    return message.substring(startIndex, endIndex).toInt();
}

void Rotate() {
    // rotate the servos to the coordinates received thru serial
    // because the servo rotates in the opposite direction, we have to reverse the movement

    if (ComElev > 90) {
        // keep it less than 90 deg off straight up & flip the antenna
        ComElev = 180 - ComElev;
        ComAzim = (180 + ComAzim) % 360;
    }
    if (ComAzim > 180) {
        // flip the antenna
        AzServo.write(360 - ComAzim);
        ElServo.write(180 - ComElev);
    }
    else {
        AzServo.write(180 - ComAzim);
        ElServo.write(ComElev);
    }
}

void DisplayPosition() {  
    display.clearDisplay();
    display.setCursor(0,28);
    display.print(ComAzim);
    display.print(" /   ");
    display.print(ComElev);
    display.display();
}