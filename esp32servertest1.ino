 
#include <WiFi.h>
#include <WebServer.h>
//#include <Robojax_L298N_DC_motor.h>
#include<Servo.h>

WebServer server(80);
Servo grip,shoulder,elbow,wrist;
int gripos=45,shoulderpos=45,elbowpos=45,wristpos=45;


/*
// motor 1 settings
#define CHA 0
#define ENA 19 
*/
// this pin must be PWM enabled pin if Arduino board is used
#define IN1 26
#define IN2 25

// motor 2 settings
#define IN3 33
#define IN4 32
/*
#define ENB 4// this pin must be PWM enabled pin if Arduino board is used
#define CHB 1

const int CCW = 2; // do not change
const int CW  = 1; // do not change
*/
/*
#define motor1 1 // do not change
#define motor2 2 // do not change
*/
String prev="";

//Robojax_L298N_DC_motor robot(IN1, IN2, ENA, CHA,  IN3, IN4, ENB, CHB);

void gripopen()
{
  Serial.println("open");
  gripos+=5;
  if(gripos>180)
  {
    gripos=180;
  }
  grip.write(gripos);
  
}

void gripclose()
{
  gripos-=5;
  Serial.println("close");

  if(gripos<0)
  {
    gripos=0;
  }
  grip.write(gripos);
  
}

void wristleft()
{
  wristpos-=5;
  Serial.println("wristleft");
  if(wristpos<0)
  {
    wristpos=0;
  }
  wrist.write(wristpos);
}

void wristright()
{
  Serial.println("wristright");
  wristpos+=5;
  if(wristpos>180)
  {
    wristpos=180;
  }
  wrist.write(wristpos);
}

void elbowup()
{
  Serial.println("elbow up");
  elbowpos+=5;
  if(elbowpos>180)
  {
    elbowpos=180;
  }
  elbow.write(elbowpos);
}

void elbowdown()
{
  Serial.println("elbow down");
  elbowpos-=5;
  if(elbowpos<0)
  {
    elbowpos=0;
  }
  elbow.write(elbowpos);
}

void shoulderup()
{
  Serial.println("shoulder up");
  shoulderpos+=5;
  if(shoulderpos>180)
  {
    shoulderpos=180;
  }
  shoulder.write(shoulderpos);
}

void shoulderdown()
{
  Serial.println("shoulder down");
  shoulderpos-=5;
  if(shoulderpos<0)
  {
    shoulderpos=180;
  }
  shoulder.write(shoulderpos);
}

void forward()
{
  /*
    Serial.println("forward");
    robot.rotate(motor1,100,CW);
    robot.rotate(motor2,100,CCW);
  */
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
Serial.println("end reached f");
  
}

void backward()
{
  /*
    Serial.println("backward");
        robot.rotate(motor1,100,CCW);
    robot.rotate(motor2,100,CW);
    */
    digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  Serial.println("end reached b");
  
}

void right()
{
  /*
    Serial.println("left");
        robot.rotate(motor1,100,CW);
    robot.rotate(motor2,100,CW);
    */

  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);

Serial.println("end reached l");
  
}

void left()
{
  /*
    Serial.println("right");
      robot.rotate(motor1,100,CCW);
    robot.rotate(motor2,100,CCW);
    */

  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);

Serial.println("end reached r");
  
}

void stopbot()
{
  Serial.println("stopbot");
  //robot.brake(1);
  //robot.brake(2);
  prev="";
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  Serial.println("end reached s");
}

void reset()
{
  grip.write(45);
  shoulder.write(45);
  elbow.write(45);
  wrist.write(45);
}

void handleRoot() {
  server.send(200, "text/plain", "Ready");
}

void handleGet() {
  if (server.hasArg("data")) {
    String data = server.arg("data");
    if(data=="gripper_open")
    {
      gripopen();
    }
    else if(data=="reset")
    {
      Serial.println("Reset");
      reset();
    }
    else if(data=="Stop")
    {
      stopbot();
    }
    else if(data=="Forward")
    {
     // forward();
      if(prev!=data)
      {
        forward();
        prev=data;
      }
    }
    else if(data=="Backward")
    {
     // backward();
      if(prev!=data)
      {
        backward();
        prev=data;
      }
    }
    else if(data=="Left")
    {
      //left();
      if(prev!=data)
      {
        left();
        prev=data;
      }
    }
    else if(data=="Right")
    {
      //right();
      if(prev!=data)
      {
        right();
        prev=data;
      }
    }
    else if(data=="gripper_close")
    {
      gripclose();
    }
    else if(data=="elbow_up")
    {
      elbowup();
    }
    else if(data=="elbow_down")
    {
      elbowdown();
    }
    else if(data=="shoulder_up")
    {
      shoulderup();
    }
    else if(data=="shoulder_down")
    {
      shoulderdown();
    }
    else if(data=="Wrist_left")
    {
      wristleft();
    }
    else if(data=="Wrist_right")
    {
      wristright();
    }
    Serial.println("Data: " + data);
   
  }
  server.send(200, "text/plain", "Data Received");
}

void handlePost() {
    if (server.hasArg("data")) {
    String data = server.arg("data");
    Serial.println("Data: " + data);
  }
 // server.send(200, "text/plain", "Data Received");
  server.send(200, "text/plain", "Processing Data");
}

void handleUpload() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    Serial.println("Receiving data:");
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    Serial.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    server.send(200, "text/plain", "Data: ");
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.softAP("Dynamo");
  grip.attach(13);
  shoulder.attach(12);
  elbow.attach(14);
  wrist.attach(27);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  
  grip.write(20);
  shoulder.write(45);
  elbow.write(45);
  wrist.write(45);
    digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);

  // motordriver pins
  //robot.begin();
  
  server.on("/", handleRoot);
  server.on("/get", HTTP_GET, handleGet);
  server.on("/pot", HTTP_POST,handlePost);
  server.begin();
}

void loop() {
  server.handleClient();
}
