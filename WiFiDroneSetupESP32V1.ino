/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.
  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1
     OR
     Run raw TCP "GET /[code]" PuTTY terminal with 192.168.4.1 as IP address and 80 as port
  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
  
  Remote control for quadcopter using esp32
 * by Matt Prater  02Dec2021
 * mprater@uat.edu
 * 
 * DoFs used
 * Up/Down                  Chan3       Left stick ver    servoPin3   10
 * Right/Left               Chan1       Right stick horz  servoPin1   8
 * Forward/Backward         Chan2       Right stick vert  servoPin2   9
 * Clockwise/counterwise    Chan4       Left stick horz   servoPin4   11
 * 
 * may wish to use 1250 min and 1750 max to start
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <ESP32_Servo.h>

#define servoPin1 13
#define servoPin2 12
#define servoPin3 14
#define servoPin4 27
// Set these to your desired credentials.
const char *ssid = "profPrater";
const char *password = "1234";

WiFiServer server(80);

int steadyLvl = 1500;
int minLvl = 1500;
int maxLvl = 1500;


Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;


  
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");

  myservo1.attach(servoPin1, 1000, 2000); 
myservo2.attach(servoPin2, 1000, 2000); 
myservo3.attach(servoPin3, 1000, 2000); 
myservo4.attach(servoPin4, 1000, 2000); 


  hover();

  
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.println("<a href=\"/U\"><button>Up</button></a><br><br>");
            
            client.println("<a href=\"/F\"><button>Forward</button></a><br><br>");
            
            client.print("<a href=\"/L\"><button>Left</button></a><br><br>");
            client.print("<a href=\"/S\"><button>hover</button></a><br><br>");
            client.println("<a href=\"/R\"><button>Right</button></a><br><br>");
            
            client.println("<a href=\"/B\"><button>Backward</button></a><br><br>");
            
            client.println("<a href=\"/D\"><button>Down</button></a><br><br>");
            client.println();

            client.print("<a href=\"/CC\"><button>Counterclockwise</button></a><br><br>");
            client.println("<a href=\"/W\"><button>clockwise</button></a><br><br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        
        // Check to see if the client request 
        if (currentLine.endsWith("GET /U")) {
          up();               
        }
        if (currentLine.endsWith("GET /F")) {
          forward();               
        }
        if (currentLine.endsWith("GET /L")) {
          left();               
        }
        if (currentLine.endsWith("GET /S")) {
          hover();                
        }
        if (currentLine.endsWith("GET /R")) {
          right();                
        }
        if (currentLine.endsWith("GET /B")) {
          backward();                
        }
        if (currentLine.endsWith("GET /D")) {
          down();                
        }
        if (currentLine.endsWith("GET /CC")) {
          counterclockwise();                
        }
        if (currentLine.endsWith("GET /W")) {
          clockwise();                
        }
      }
    }
    // close the connection:
    
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

void NoseDown(){
     
    myservo1.writeMicroseconds(steadyLvl);
    myservo2.writeMicroseconds(maxLvl);
    myservo3.writeMicroseconds(steadyLvl);
    myservo4.writeMicroseconds(steadyLvl);

    
}

void throttleDown(){
    
    myservo1.writeMicroseconds(steadyLvl);
    myservo2.writeMicroseconds(steadyLvl);
    myservo3.writeMicroseconds(minLvl);
    myservo4.writeMicroseconds(steadyLvl);

    
}

void throttleUp(){
 
    myservo1.writeMicroseconds(steadyLvl);
    myservo2.writeMicroseconds(steadyLvl);
    myservo3.writeMicroseconds(maxLvl);
    myservo4.writeMicroseconds(steadyLvl);

    
}

void rollLeft(){
    
    myservo1.writeMicroseconds(steadyLvl);
    myservo2.writeMicroseconds(minLvl);
    myservo3.writeMicroseconds(steadyLvl);
    myservo4.writeMicroseconds(steadyLvl);

    
}

void yawRight(){
    
    myservo1.writeMicroseconds(steadyLvl);
    myservo2.writeMicroseconds(steadyLvl);
    myservo3.writeMicroseconds(steadyLvl);
    myservo4.writeMicroseconds(maxLvl);

    
}

void rangeTest(){
    
    myservo1.writeMicroseconds(maxLvl);
    myservo2.writeMicroseconds(maxLvl);
    myservo3.writeMicroseconds(maxLvl);
    myservo4.writeMicroseconds(maxLvl);
  delay(1000);
   myservo1.writeMicroseconds(maxLvl);
    myservo2.writeMicroseconds(minLvl);
    myservo3.writeMicroseconds(maxLvl);
    myservo4.writeMicroseconds(minLvl);
    delay(1000);
   myservo1.writeMicroseconds(minLvl);
    myservo2.writeMicroseconds(minLvl);
    myservo3.writeMicroseconds(minLvl);
    myservo4.writeMicroseconds(minLvl);
    delay(1000);
   myservo1.writeMicroseconds(minLvl);
    myservo2.writeMicroseconds(maxLvl);
    myservo3.writeMicroseconds(minLvl);
    myservo4.writeMicroseconds(maxLvl);
    delay(1000);
     myservo1.writeMicroseconds(1500);
    myservo2.writeMicroseconds(1500);
    myservo3.writeMicroseconds(1500);
    myservo4.writeMicroseconds(1500);
    Serial.println("test complete");
}





void hover(){
    myservo1.writeMicroseconds(1500);
    myservo2.writeMicroseconds(1500);
    myservo3.writeMicroseconds(1500);
    myservo4.writeMicroseconds(1500);

    
}
