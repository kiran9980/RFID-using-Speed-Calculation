String uidString;
int count = 0; 

int n ;//The number of card you want to detect (optional)  
char input[12];//memory for storing 12 characters of ID


void setup()
{
  Serial.begin(9600);                                  // begin serial port with baud rate 9600bps
  delay(1000);
}

void loop()
{
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
    Serial.println(uidString);   //Please Note your RFID Number
    delay(50); 
  }
}


  
