
#define clockpin 13 // CI
#define enablepin 10 // EI
#define latchpin 9 // LI
#define datapin 11 // DI

//number of modules in the chain
#define NumLEDs 6 
 
int LEDChannels[NumLEDs][3] = {0};
int SB_CommandMode;
int SB_RedCommand;
int SB_GreenCommand;
int SB_BlueCommand;
 
void setup()
{
  SerialUSB.begin(38400);
   
   pinMode(datapin, OUTPUT);
   pinMode(latchpin, OUTPUT);
   pinMode(enablepin, OUTPUT);
   pinMode(clockpin, OUTPUT);
   SPCR = (1<<SPE)|(1<<MSTR)|(0<<SPR1)|(0<<SPR0);
   digitalWrite(latchpin, LOW);
   digitalWrite(enablepin, LOW);

}

//sample code from macetech
void SB_SendPacket() {
  
   if (SB_CommandMode == B01) {
     SB_RedCommand = 127;
     SB_GreenCommand = 127;
     SB_BlueCommand = 127;
    }
 
    SPDR = SB_CommandMode << 6 | SB_BlueCommand>>4;
    while(!(SPSR & (1<<SPIF)));
    SPDR = SB_BlueCommand<<4 | SB_RedCommand>>6;
    while(!(SPSR & (1<<SPIF)));
    SPDR = SB_RedCommand << 2 | SB_GreenCommand>>8;
    while(!(SPSR & (1<<SPIF)));
    SPDR = SB_GreenCommand;
    while(!(SPSR & (1<<SPIF)));
}

//sample code from macetech
void WriteLEDArray() {
 
    SB_CommandMode = B00; // Write to PWM control registers
    for (int h = 0;h<NumLEDs;h++) {
    SB_RedCommand = LEDChannels[h][0];
    SB_GreenCommand = LEDChannels[h][1];
    SB_BlueCommand = LEDChannels[h][2];
    SB_SendPacket();
    }
 
    delayMicroseconds(15);
    digitalWrite(latchpin,HIGH); // latch data into registers
    delayMicroseconds(15);
    digitalWrite(latchpin,LOW);
 
    SB_CommandMode = B01; // Write to current control registers
    for (int z = 0; z < NumLEDs; z++) SB_SendPacket();
    delayMicroseconds(15);
    digitalWrite(latchpin,HIGH); // latch data into registers
    delayMicroseconds(15);
    digitalWrite(latchpin,LOW); 
}
 

int incomingatmo[255];
int buffer=0;
int channels;
int i=0;
int BluePin=11;
int GreenPin=9;
int RedPin=10;
int laststatus=0;
int lastbyte=0;
int average=0;
int j=0;
int channel=0;
int x=0;

//order is reversed, first byte is last module in the chain
//0=sum
//1=left
//2=right
//3=top
//4=bottom
byte channelorder[6]={2,2,3,3,1,1};

// old gammatable for atmega's own pwm 0-255
//byte gammatable[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,5,5,5,5,5,6,6,6,7,7,7,7,8,8,8,9,9,9,10,10,10,11,11,12,12,12,13,13,14,14,14,15,15,16,16,17,17,18,18,19,19,20,20,21,22,22,23,23,24,25,25,26,26,27,28,28,29,30,31,31,32,33,33,34,35,36,36,37,38,39,39,40,41,42,43,44,45,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,68,69,70,71,72,73,75,76,77,78,79,81,82,83,85,86,87,88,90,91,93,94,95,97,98,99,101,102,104,105,107,108,110,111,113,114,116,117,119,121,122,124,125,127,129,130,132,134,135,137,139,141,142,144,146,148,150,151,153,155,157,159,161,162,164,166,168,170,172,174,176,178,180,182,184,186,189,191,193,195,197,199,201,203,206,208,210,212,215,217,219,221,224,226,228,231,233,235,238,240,243,245,247,250,252,255};

int gammatable[]={0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,3,3,3,4,4,4,5,5,6,6,6,7,7,8,8,9,10,10,11,12,12,13,14,14,15,16,17,18,19,19,20,21,22,23,24,26,27,28,29,30,31,33,34,35,36,38,39,41,42,44,45,47,48,50,52,53,55,57,58,60,62,64,66,68,70,72,74,76,78,80,82,85,87,89,92,94,96,99,101,104,106,109,112,114,117,120,123,125,128,131,134,137,140,143,146,149,153,156,159,162,166,169,172,176,179,183,187,190,194,198,201,205,209,213,217,221,225,229,233,237,241,246,250,254,258,263,267,272,276,281,286,290,295,300,305,310,314,319,324,329,335,340,345,350,355,361,366,372,377,383,388,394,400,405,411,417,423,429,435,441,447,453,459,465,472,478,484,491,497,504,510,517,524,530,537,544,551,558,565,572,579,586,593,601,608,615,623,630,638,645,653,661,668,676,684,692,700,708,716,724,732,740,749,757,765,774,782,791,800,808,817,826,835,844,853,862,871,880,889,898,908,917,926,936,945,955,965,974,984,994,1004,1014,1023};
const int numReadings = 1;

 int bluereadings[numReadings];      
 int blueindex = 0;                 
 int bluetotal = 0;                  
 int redreadings[numReadings];     
 int redindex = 0;                 
 int redtotal = 0;                 
 int greenreadings[numReadings];    
 int greenindex = 0;               
 int greentotal = 0;                


void outputpwm(int curchannel)
{
  int blue=gammatable[incomingatmo[(channelorder[curchannel]*3)+3]];
  int red=gammatable[incomingatmo[(channelorder[curchannel]*3)+1]];
  int green=gammatable[incomingatmo[(channelorder[curchannel]*3)+2]];
  
   bluetotal= bluetotal - bluereadings[blueindex];
  redtotal= redtotal - redreadings[redindex]; 
  greentotal= greentotal - greenreadings[greenindex]; 
 
   bluereadings[blueindex] =  blue;
   redreadings[redindex] =  red;
   greenreadings[greenindex] =  green;
 
   bluetotal= bluetotal + bluereadings[blueindex];  
   redtotal= redtotal + redreadings[redindex]; 
   greentotal= greentotal + greenreadings[greenindex]; 
   // advance to the next position in the array:  
   blueindex = blueindex + 1;  
   redindex = redindex + 1;
   greenindex = greenindex + 1;

   if (blueindex >= numReadings)              
     blueindex = 0;   
    if (redindex >= numReadings)              
       redindex = 0;  
    if (greenindex >= numReadings)              
      greenindex = 0;       

   // calculate the average:   
    average = redtotal / numReadings; 
     LEDChannels[curchannel][0]=average;
   average = greentotal / numReadings; 
  LEDChannels[curchannel][1]=average;
  
   average = bluetotal / numReadings; 
   LEDChannels[curchannel][2]=average;  
  }

void loop()
{
  if(SerialUSB.available()>0)
  {
    buffer = SerialUSB.read();
   if(buffer==0xff)
   {
     while(SerialUSB.available()==0)
     { }
     buffer=SerialUSB.read();
     if(buffer==0x00)
     {
       while(SerialUSB.available()==0)
     { }
       buffer=SerialUSB.read();
       if(buffer==0x00)
         {
           for(i=0;i<16;i++)
            {
              while(SerialUSB.available()==0)
             { }
              incomingatmo[i]=SerialUSB.read();
   
             }
            for(channel=0;channel<NumLEDs;channel++)
             {           
                outputpwm(channel);
             }
             WriteLEDArray();
         }
       }    
     }
   }
   else
   {
     WriteLEDArray();
     delay(5);
   }
}
