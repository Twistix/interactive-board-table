// resolution de la cam : 128 x 96 pixels
// angles de detection theoriques: 33 horizontal, 23 vertical
// couleurs fils : rouge = +5V, noir = GROUND, vert = SCL, jaune = SDA
// champ de vision horizontal 37.5°
// champ de vision vertical 29°

#include <Wire.h>

int IRsensorAddress = 0xB0;
//int IRsensorAddress = 0x58;
int slaveAddress;

int i;
unsigned long time;
double a, b, c, delta, result1, result2;
int Ix[4];
int Iy[4];
double x1,x2,x3,x4,x5,x6,x7,y1,y2,y3,y4,y5,y6,y7;
int angles[4][2];
double u,v;

void Write_2bytes(byte d1, byte d2)
{
    Wire.beginTransmission(slaveAddress);
    Wire.write(d1); Wire.write(d2);
    Wire.endTransmission();
}

void reading(int slaveAddress, int *Ix, int *Iy)
{
  byte data_buf[16];
  int i;
  int s;
  
  //IR sensor read
    Wire.beginTransmission(slaveAddress);
    Wire.write(0x36);
    Wire.endTransmission();

    Wire.requestFrom(slaveAddress, 16);        // Request the 2 byte heading (MSB comes first)
    for (i=0;i<16;i++) { data_buf[i]=0; }
    
    i=0;
    while(Wire.available() && i < 16) {
        data_buf[i] = Wire.read();
        i++;
    }

    Ix[0] = data_buf[1];
    Iy[0] = data_buf[2];
    s   = data_buf[3];
    Ix[0] += (s & 0x30) <<4;
    Iy[0] += (s & 0xC0) <<2;

    Ix[1] = data_buf[4];
    Iy[1] = data_buf[5];
    s   = data_buf[6];
    Ix[1] += (s & 0x30) <<4;
    Iy[1] += (s & 0xC0) <<2;

    Ix[2] = data_buf[7];
    Iy[2] = data_buf[8];
    s   = data_buf[9];
    Ix[2] += (s & 0x30) <<4;
    Iy[2] += (s & 0xC0) <<2;

    Ix[3] = data_buf[10];
    Iy[3] = data_buf[11];
    s   = data_buf[12];
    Ix[3] += (s & 0x30) <<4;
    Iy[3] += (s & 0xC0) <<2;
}


void setup()
{
    slaveAddress = IRsensorAddress >> 1;   // This results in 0x21 as the address to pass to TWI
    Serial.begin(9600);
    Wire.begin();
    // IR sensor initialize
    Write_2bytes(0x30,0x01); delay(10);
    Write_2bytes(0x30,0x08); delay(10);
    Write_2bytes(0x06,0x90); delay(10);
    Write_2bytes(0x08,0xC0); delay(10);
    Write_2bytes(0x1A,0x40); delay(10);
    Write_2bytes(0x33,0x33); delay(10);
    delay(100);

    // Calibration :
    //face camera : 0 = haut droite, 1 = haut gauche, 2 = bas gauche, 3 = bas droite
    i=0;
    Serial.print("Angle ");
    Serial.println(i);
    while (true) {
      reading(slaveAddress, Ix, Iy);
      if (Ix[0] != 1023) {
        Ix[0] = 1023-Ix[0];
        angles[i][0]=Ix[0];
        angles[i][1]=Iy[0];
        Serial.print("Ok : (");
        Serial.print(angles[i][0]);
        Serial.print(",");
        Serial.print(angles[i][1]);
        Serial.println(")");
        Serial.println();
        i+=1;
        if (i==4) {
          break;
        }
        time=millis();
        while ((millis()-time)<2000) {
          delay(50);
        }
        Serial.print("Angle ");
        Serial.println(i);
      }
    }
    x1 = angles[0][0];
    x2 = angles[1][0];
    x3 = angles[2][0];
    x4 = angles[3][0];
    y1 = angles[0][1];
    y2 = angles[1][1];
    y3 = angles[2][1];
    y4 = angles[3][1];
    Serial.println("Calibration finie");
}


void loop()
{
    reading(slaveAddress, Ix, Iy);

    i=0;
    Ix[i] = 1023-Ix[i];
    
    if (Ix[i]!=0) {
      x5 = Ix[i];
      y5 = Iy[i];
  
      a = x1*(y3-y4)-x2*(y3-y4)-(x3-x4)*(y1-y2);
      b = -x1*(y3-2*y4+y5)-x2*(y4-y5)+x3*(y1-y5)+x4*(-2*y1+y2+y5)+x5*(y1-y2+y3-y4);
      c = -x1*(y4-y5)+x4*(y1-y5)-x5*(y1-y4);
      
      delta = pow(b,2) - 4 * a * c;
      
      if (delta==0) { 
        result1 = result2 = (-b) / (2 * a);
      } 
      else if (delta>0) {
        delta = sqrt(delta);
        result1 = (-b + delta) / (2 * a);
        result2 = (-b - delta) / (2 * a);
      }
      else {
      }
      
      u = abs(result2);
      
      x6 = x4 + u*(x3 - x4);
      y6 = y4 + u*(y3 - y4);
      x7 = x1 + u*(x2 - x1);
      y7 = y1 + u*(y2 - y1);
  
      if (abs(x7-x6)>abs(y7-y6)) {      
        v = abs(x5-x7) / abs(x6-x7);
      }
      else {
        v = abs(y5-y7) / abs(y6-y7);
      }
      
      Serial.print( u );
      Serial.print(",");
      Serial.print( v );
      Serial.print(",");
      Serial.println("");

      //delay(1000);
    }
    else {
      u = 2;
      v = 2;
    }
    
    //Serial.println("");
    delay(1);
}
