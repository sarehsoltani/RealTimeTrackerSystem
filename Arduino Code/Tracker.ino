/********************************************************************************************************************
* Bsc Final Project
* Real Time Tracking system
* Sareh Soltani Nejad, Email: sare.soltani74@gmail.com / sare1996@aut.ac.ir
* 13/june/2019
*********************************************************************************************************************/

#include<SoftwareSerial.h>
extern uint8_t SmallFont[];

#define rxPin 11
#define txPin 10

SoftwareSerial mySerial(txPin, rxPin);

const char url[] = "http://103.216.62.79/api/device/add/";
char response[200];
char latitude[15];
char longitude[15];
char altitude[16];
char date[24];
char TTFF[3];
char satellites[3];
char speedOTG[10];
char course[15];

void setup(){
  mySerial.begin(9600);
  Serial.begin(9600); 
  Serial.println("Starting...");
  power_on();
  start_GPS();
  while (sendATcommand("AT+CREG?", "+CREG: 0,1", 2000) == 0);
  sendATcommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", "OK", 2000);
  sendATcommand("AT+SAPBR=3,1,\"APN\",\"mtnirancell\"", "OK", 2000);
  sendATcommand("AT+SAPBR=3,1,\"USER\",\"\"", "OK", 2000);
  sendATcommand("AT+SAPBR=3,1,\"PWD\",\"\"", "OK", 2000);
  // gets the GPRS bearer
  sendATcommand("AT+SAPBR=0,1", "OK", 20000);
  while (sendATcommand("AT+SAPBR=1,1", "OK", 20000) == 0){
    delay(5000);
  }
}

void loop(){
  // gets GPS data
  get_GPS();
  // sends GPS data to the script
  send_HTTP();   
  //sendNMEALocation("989164450465",frame);
  delay(12000);
}

void power_on(){
  uint8_t answer=0;
  // checks if the module is started
  answer = sendATcommand("AT", "OK", 2000);
  if (answer == 0){
    // waits for an answer from the module
    while(answer == 0){  
    // Send AT every two seconds and wait for the answer   
      answer = sendATcommand("AT", "OK", 2000);    
    }
  }
  Serial.println("Power on");
}

int8_t start_GPS(){
  // starts the GPS
  while(sendATcommand("AT+CGNSPWR=1", "OK", 2000)==0);
  while(sendATcommand("AT+CGPSRST=0", "OK", 2000)==0);
  // waits for fix GPS
  while((( 
      sendATcommand("AT+CGPSSTATUS?", "2D Fix", 5000) || 
      sendATcommand("AT+CGPSSTATUS?", "3D Fix", 5000)) == 0 ) );
  return 1;
}

int8_t get_GPS(){   
  int8_t answer;
  char * auxChar;
  // request Basic string
  sendATcommand("AT+CGPSINF=0", "O", 8000); 
  auxChar = strstr(response, "+CGPSINF:");
  if (auxChar != NULL){
    // Parses the string 
    memset(longitude, '\0', 15);
    memset(latitude, '\0', 15);
    memset(altitude, '\0', 16);
    memset(date, '\0', 24);
    memset(TTFF, '\0', 3);
    memset(satellites, '\0', 3);
    memset(speedOTG, '\0', 10);
    memset(course, '\0', 15);  
    strcpy (response, auxChar);
    Serial.println(response);
      
    strtok(response, ",");
    strcpy(latitude,strtok(NULL, ",")); // Gets longitude
    strcpy(longitude,strtok(NULL, ",")); // Gets latitude
    strcpy(altitude,strtok(NULL, ",")); // Gets altitude    
    strcpy(date,strtok(NULL, ",")); // Gets date
    strcpy(TTFF,strtok(NULL, ","));  
    strcpy(satellites,strtok(NULL, ",")); // Gets satellites
    strcpy(speedOTG,strtok(NULL, ",")); // Gets speed over ground. Unit is knots.
    strcpy(course,strtok(NULL, "\r")); // Gets course
    convert2Degrees(latitude);
    convert2Degrees(longitude);
    answer = 1;
  }
  else
    answer = 0;
  return answer;
}

void sendNMEALocation(char * cellPhoneNumber, char * message) { 
  char ctrlZString[2];  
  char sendSMSString[100];    
  // Started sendNMEALocation.
  memset(ctrlZString, '\0', 2);
  ctrlZString[0] = 26;    
  memset(sendSMSString, '\0', 100); 
  sprintf(sendSMSString,"AT+CMGS=\"%s\"",cellPhoneNumber);            
  // request Basic string
  sendATcommand(sendSMSString, ">", 2000);
  mySerial.println(message);
  sendATcommand(ctrlZString, "OK", 6000); 
  //Ended sendNMEALocation.    
} 

int8_t send_HTTP(){
  int8_t answer;
  char aux_str[200];
  char frame[200];
  // Initializes HTTP service
  answer = sendATcommand("AT+HTTPINIT", "OK", 10000);
  if (answer == 1){
    // Sets CID parameter
    answer = sendATcommand("AT+HTTPPARA=\"CID\",1", "OK", 5000);
    if (answer == 1){
      // Sets url 
      memset(aux_str, '\0', 200);
      sprintf(aux_str, "AT+HTTPPARA=\"URL\",\"%s", url);
      mySerial.print(aux_str);
      Serial.println(aux_str);
      sprintf(frame, "?visor=false&lat=%s&lon=%s&alt=%s&date=%s&TTFF=%s&sat=%s&speedOTG=%s&course=%s", latitude, longitude, altitude, date, TTFF, satellites, speedOTG, course);
      Serial.println(frame);
      mySerial.print(frame);          
      answer = sendATcommand("\"", "OK", 5000);
      if (answer == 1){
        // Starts GET action
        answer = sendATcommand("AT+HTTPACTION=0", "+HTTPACTION: 0,200", 30000);
        if (answer == 1){
          Serial.println(F("Done!"));
        }
        else{
          Serial.println(F("Error getting url"));
        }
      }
      else{
        Serial.println(F("Error setting the url"));
      }
    }
    else{
      Serial.println(F("Error setting the CID"));
    }    
  }
  else{
    Serial.println(F("Error initializating"));
  }

   sendATcommand("AT+HTTPTERM", "OK", 5000);
   return answer;
}

int8_t sendATcommand(char* ATcommand, char* expected_answer1, unsigned int timeout){
  uint8_t x=0,  answer=0;
  unsigned long previous;
  char readVar[200];
  char * auxChar;

  memset(response, '\0', 200);    // Initialize the string
  memset(readVar, '\0', 200);    // Initialize the string

  while( mySerial.available() > 0) mySerial.read();    // Clean the input buffer
  while( Serial.available() > 0) Serial.read();    // Clean the input buffer
 
  mySerial.write(ATcommand);    // Send the AT command 
  mySerial.write("\r\n\r\n");    // Send enter
 
  Serial.println(ATcommand);
    
  x = 0;
  previous = millis();
  // this loop waits for the answer
  do{
     if(mySerial.available() != 0){    
       readVar[x] = mySerial.read();
       x++;
       // check if the desired answer is in the response of the module
       auxChar = strstr(readVar, expected_answer1);
       if (auxChar != NULL){
        if( strstr(readVar, "+CGPSINF:") == NULL)
          strcpy (response, auxChar);
        else
          strcpy (response, readVar);
        answer = 1;
        }
     }
     // Waits for the asnwer with time out
    }
    while((answer == 0) && ((millis() - previous) < timeout));  
    if(auxChar == NULL)
      Serial.println(readVar);
    return answer;
}

void convert2Degrees(char* input){
  char res[40];
  float deg;
  float minutes;
  float seconds;
  char aux[10] ="";
  //latitude format: DDmm.mmmm'
    
  // get 'degrees' from input parameter
  aux[0] = input[0];
  aux[1] = input[1];
  aux[2] = '\0';
  
  // convert string to integer and add it to final float variable
  deg = atoi(aux);
  // get 'minutes' from input parameter
  for ( int i=0; i<7; i++ ){
    aux[i] = input[i+2];
  }
  aux[7] = '\0';
  // convert string to integer and add it to final float variable
  minutes = atoi(aux);
    
  // get 'seconds' from input parameter
  for ( int i=0; i<2; i++ ){
    aux[i] = aux[i+3];
  }
  aux[3] = '\0';
  seconds = atoi(aux);
    
  // add minutes to degrees
  deg = deg + minutes/60 + seconds/3600;
  dtostrf(deg, 2, 10, res);
  strncpy(input, res,9);
}
