
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define WIFI_SSID "Kirankumar"   //Enter Your Wi-Fi Name
#define WIFI_PASSWORD "46yur1fvfa"  //Enter Your Wi-Fi Password

#define NTP_OFFSET  19800 // In seconds 
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "1.asia.pool.ntp.org"


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);
String formattedDate;
String dayStamp;
String timeStamp;

String uidString;

int count = 0; 

String Log;
String Name;//user name
String Number;//user number

int n ;//The number of card you want to detect (optional)  

String Time1;
String Time2;

String Date1;
String Date2;

int Flag=1;

unsigned long t1=0;
unsigned long t2=0; 

float velocity;

char input[12];//memory for storing 12 characters of ID


void setup()
{
  Serial.begin(9600);                                  // begin serial port with baud rate 9600bps
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  Serial2.begin(9600, SERIAL_8N1, 16, 17);   //For RFID
  Serial1.begin(9600, SERIAL_8N1, 25, 26);   //For GSM
  gsmtest();   //GSM Test Message   
}

void loop()
{
  timeClient.update();
  String formattedTime = timeClient.getFormattedTime();
  formattedDate = timeClient.getFormattedDate();
  //  Serial.println(formattedDate);
  //  Serial.println(formattedTime);
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  Serial.println(dayStamp);
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.println(timeStamp);
  delay(1000);



  if(Serial1.available())
  {
    count = 0;
    while(Serial1.available() && count < 12)          // Read 12 characters and store them in input array
    {
      input[count] = Serial1.read();
      count++;
      delay(5);
    }
    Serial.println(input);                             // Print RFID tag number       
    uidString = String(input[0]+input[1]+input[2]+input[3]+input[4]+input[5]+input[6]+input[7]+input[8]+input[9]+input[10]+input[11]);
    Serial.println(uidString);
    
     //Before Enter "688" Check the RFID Tag Number//
     //Here 688 means our RFID tag number//
     //For RFID Number checking use RFID test code//
     
    if(uidString=="688" && Flag==1)   
    {
      Name="First";//user name
      Number="KA07";//user number
      Time1=timeStamp;
      Date1=dayStamp;
      Flag=2;
      t1=millis();
      Serial.println("1st RFID entered at"+ String(Time1));
    }

    else if(uidString=="688" && Flag==2)
    {
      Name="Second";//user name
      Number="KA07";//user number
      Time2=timeStamp;
      Date2=dayStamp;
      Flag=1;
      t2=millis();
      Timecalculate();
      Serial.println("1st RFID exit at"+ String(Time2));
    }
  }
}




void Timecalculate()
{
  velocity=t2-t1;
  velocity=velocity/1000;//convert millisecond to second
  velocity=(5.0/velocity);//v=d/t
  velocity=velocity*3600;//multiply by seconds per hr
  velocity=velocity/1000;//division by meters per Km
  Serial.print(velocity);
  Serial.println(" Km/hr ");
  speeddetection();
  delay(500);  
}



void speeddetection()
{
  if(velocity > 2)
  {
    Serial.print("KA07 Speed Detected");
    SentSMS();    
  }
}


void gsmtest()
{
  Serial2.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial2.println("AT+CMGS=\"+91xxxxxxxxxx\"\r"); // Replace x with mobile number
  delay(1000);
  Serial2.println("This is test SMS from ESP32");// The SMS text you want to send
  Serial.println("sms sent");
  delay(4000);
  Serial2.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void SentSMS()
{

  Serial2.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial2.println("AT+CMGS=\"+91xxxxxxxxxx\"\r"); // Replace x with mobile number
  delay(1000);
  Serial2.println("Your Vehicle" + String(Number)+"Speed Detected at Kolar at"  + String(Date2) + String(Time2) );// The SMS text you want to send
  Serial.println("sms sent to vehicle");
  delay(4000);
  Serial2.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}


  
