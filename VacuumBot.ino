#include <etherShield.h>
#include <ETHER_28J60.h>
#define ON      HIGH
#define OFF     LOW
#define RELAY  A4


static uint8_t mac[6] = {
  0x54, 0x55, 0x58, 0x10, 0x00, 0x24};  
static uint8_t ip[4] = {
  192, 168, 1, 55}; 
static uint16_t port = 80; 

ETHER_28J60 client;
//M1
const int in1_d1 = 3;
const int in2_d1 = 4;
//M2
const int in3_d1 = 5;
const int in4_d1 = 6;


//M3
const int in1_d2 = 7;
const int in2_d2 = 8;
//M4
const int in3_d2 = 9;
const int in4_d2 = A1;

const int BatPort = A0;
const int Vaccume = A2;

void Forward()
{
  //M1
  digitalWrite(in1_d1, 1);
  digitalWrite(in2_d1, 0);
  //M2
  digitalWrite(in3_d1, 1);
  digitalWrite(in4_d1, 0);
  //M3
  digitalWrite(in1_d2, 0);
  digitalWrite(in2_d2, 1);
  //M4
  digitalWrite(in3_d2, 0);
  digitalWrite(in4_d2, 1);
}

void Reward()
{
  //M1
  digitalWrite(in1_d1, 0);
  digitalWrite(in2_d1, 1);
  //M2
  digitalWrite(in3_d1, 0);
  digitalWrite(in4_d1, 1);
  //M3
  digitalWrite(in1_d2, 1);
  digitalWrite(in2_d2, 0);
  //M4
  digitalWrite(in3_d2, 1);
  digitalWrite(in4_d2, 0);
}

void TurnLeft()
{
  //M1
  digitalWrite(in1_d1, 1);
  digitalWrite(in2_d1, 0);
  //M2
  digitalWrite(in3_d1, 0);
  digitalWrite(in4_d1, 1);
  //M3
  digitalWrite(in1_d2, 1);
  digitalWrite(in2_d2, 0);
  //M4
  digitalWrite(in3_d2, 0);
  digitalWrite(in4_d2, 1);
}

void TurnRight()
{
  //M1
  digitalWrite(in1_d1, 0);
  digitalWrite(in2_d1, 1);
  //M2
  digitalWrite(in3_d1, 1);
  digitalWrite(in4_d1, 0);
  //M3
  digitalWrite(in1_d2, 0);
  digitalWrite(in2_d2, 1);
  //M4
  digitalWrite(in3_d2, 1);
  digitalWrite(in4_d2, 0);
}

void StopMoter()
{
  //M1
  digitalWrite(in1_d1, LOW);
  digitalWrite(in2_d1, LOW);
  //M2
  digitalWrite(in3_d1, LOW);
  digitalWrite(in4_d1, LOW);
  //M3
  digitalWrite(in1_d2, LOW);
  digitalWrite(in2_d2, LOW);
  //M4
  digitalWrite(in3_d2, LOW);
  digitalWrite(in4_d2, LOW);
}

void setup() {
  client.setup(mac, ip, port);
  //pinMode(RELAY, OUTPUT);
  //digitalWrite(RELAY,HIGH);
  pinMode(in1_d1,OUTPUT);
  pinMode(in2_d1,OUTPUT);
  pinMode(in3_d1,OUTPUT);
  pinMode(in4_d1,OUTPUT);
  pinMode(in1_d2,OUTPUT);
  pinMode(in2_d2,OUTPUT);
  pinMode(in3_d2,OUTPUT);
  pinMode(in4_d2,OUTPUT);

  pinMode(Vaccume,OUTPUT);
  digitalWrite(Vaccume,HIGH);

  StopMoter();
  Serial.begin(9600);
}

byte TypeCmd = 0;
int DelCmd = 0;
float VBat = 0.0;
int vbat;

int TimeCnt = 0;

void loop() {

  /// Check VBat ///
  VBat = ((analogRead(BatPort)/1024.0)*5.0)*10.9990;  // 12.0Volt
  VBat = ((VBat-11.36)/0.13)*10.0;        // 12Volt
  vbat=VBat;
  String s = String(vbat)+"%";
  char chrvolt[s.length()];
  s.toCharArray(chrvolt, s.length());

  //check battery voltage every 100 loop
  if(TimeCnt<100){
    TimeCnt++;
  }
  else
  {
    TimeCnt = 0;
    Serial.println( String(vbat) +"%");
  }

  /// Net Control ///
  char* params;
  if (params = client.serviceRequest()) {    
    // custom buttons
    //client.print("<h2>SFE Ethernet Relay Control</h2></br>");
    //client.print("<input type=submit value='LED1 ON' style=width:100px;height:45px onClick=location.href='/?LED=ON'>&nbsp&nbsp&nbsp");
    //client.print("<input type=submit value='LED1 OFF' style=width:100px;height:45px onClick=location.href='/?LED=OFF'><br/>");

    client.print("<h2>");
    client.print(chrvolt);
    client.print("</h2></br>");

    if (strcmp(params, "?H0") ==0){ 
      TypeCmd = 1; 
      Serial.println("Forward"); 
    }    // Forward  
    else if(strcmp(params, "?H1") ==0){ 
      TypeCmd = 2; 
      Serial.println("Left"); 
    }   // Left
    else if(strcmp(params, "?H2") ==0){ 
      TypeCmd = 0; 
      Serial.println("Stop"); 
    }   // Stop
    else if(strcmp(params, "?H3") ==0){ 
      TypeCmd = 3; 
      Serial.println("Right"); 
    }  // Right 
    else if(strcmp(params, "?H4") ==0){ 
      TypeCmd = 4; 
      Serial.println("Reward"); 
    } // Reward
    else if(strcmp(params, "?V0") ==0){ 
      digitalWrite(Vaccume,LOW); 
      Serial.println("On Vaccume"); 
    }    // On Vaccume
    else if(strcmp(params, "?V1") ==0){ 
      digitalWrite(Vaccume,HIGH);
      Serial.println("Off Vaccume"); 
    }   // Off Vaccume

    /*
    if (strcmp(params, "?LED=ON") == 0) {
     digitalWrite(RELAY, ON);
     }
     if (strcmp(params, "?LED=OFF") == 0) {
     digitalWrite(RELAY, OFF);
     }
     */
    client.respond();
    DelCmd = 0;
    //delay(10);
    //client.setup(mac, ip, port);
  }
  ///
  //////////////////// Control Moter ///////////////////////////
  if(TypeCmd==1)           // Forward
  {
    Forward();
  }
  else if(TypeCmd==2)      // Turn Left
  {
    TurnLeft();
  }
  else if(TypeCmd==3)      // Turn Right
  {
    TurnRight();
  }
  else if(TypeCmd==4)      // Reward
  {
    Reward();
  }
  else{
    StopMoter();
  }       // Stop
  //////////////////////////////////////////////////////////////
}

