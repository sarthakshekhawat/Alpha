#include<Wire.h>
#include<Servo.h>
#include "dht.h"
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(50, 51, 5, 4, 3, 2);

/*define all pins here for easy reference for new addition of functions*/
unsigned long time1,time2;
Servo A,B;

int angleA=90, angleB=90;
const int motor11 = 22;
const int motor12 = 24;
const int motor21 = 26;
const int motor22 = 28;

const int trigPin = 39; //sonar pin
const int echoPin = 38; //sonar pin
long duration;
int distance;

const int dht_apin = A0;  //temp sensor pin
dht DHT;  //temp sensor

byte x=0;

void setup()
{
  Serial.begin(9600);
   Wire.begin();
  pinMode(22,OUTPUT);
  pinMode(24,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(28,OUTPUT);
  pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
   A.attach(12);      // attach the signal pin of servo to pin12 of arduino
  B.attach(13);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  lcd.display();
  
  A.write(90);
  B.write(90);
}

int path[30];



int Sonar()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  return (distance);
}

void stop_()
{

  digitalWrite(22,LOW);
   digitalWrite(24,LOW);
    digitalWrite(26,LOW);
     digitalWrite(28,LOW);
}


void right_()
{
   digitalWrite(22,HIGH);
   digitalWrite(26,LOW);
    digitalWrite(24,LOW);
     digitalWrite(28,HIGH);
     delay(500);
     stop_();
}


void left_()
{
   digitalWrite(26,HIGH);
   digitalWrite(22,LOW);
    digitalWrite(28,LOW);
     digitalWrite(24,HIGH);
     delay(500);
     stop_();
}


void move_()
{
    digitalWrite(22,HIGH);
   digitalWrite(26,HIGH);
    digitalWrite(24,LOW);
     digitalWrite(28,LOW);
  int f = Sonar();
 if(f<=20)
 stop_();
     
}

void _back()
{

   digitalWrite(24,HIGH);
   digitalWrite(28,HIGH);
    digitalWrite(22,LOW);
     digitalWrite(26,LOW);
     
}

void move_2()
{
    digitalWrite(22,HIGH);
   digitalWrite(26,HIGH);
    digitalWrite(24,LOW);
     digitalWrite(28,LOW);
     delay(2000);
      stop_();
     
}

void _back2()
{

   digitalWrite(24,HIGH);
   digitalWrite(28,HIGH);
    digitalWrite(22,LOW);
     digitalWrite(26,LOW);
     delay(2000);
     stop_();
     
}

void A_down()
{
  if(angleA==90) return;
  angleA-=15;
  A.write(angleA);
}

void A_up()
{
  if(angleA==180) return;
  angleA+=15;
  A.write(angleA);
}

void B_hold()
{
  if(angleB==90) return;
  angleB-=15;
  B.write(angleB);
}

void B_release()
{
if(angleB==180) return;
  angleB+=15;
  B.write(angleB);
}

void dance()
{

     x=1;
     Wire.beginTransmission(8); // transmit to device #8
     Wire.write(x);              // sends one byte
     x=3;
     Wire.write(x);
     Wire.endTransmission();    // stop transmitting
  left_();
  delay(600);
  right_();
  delay(600);
  left_();
  delay(300);
  right_();
  delay(550);
  move_();
  delay(300);
  _back();
  delay(300);
  stop_();
}

void path_()
{
    int f = Sonar();
 if(f<=10)
 stop_();
  int j=0;
  while(1)
  {
    if(Serial.available())
     {
      String datas=Serial.readString();
      if(datas=="*move#"||datas=="Move"||datas=="move"||datas=="Move"||datas == "forward"||datas=="*forward#")
        {
         x=1;
         Wire.beginTransmission(8); // transmit to device #8
         Wire.write(x);              // sends one byte
         x=3;
         Wire.write(x);
         Wire.endTransmission();    // stop transmitting
            move_2();
            path[j++]=1;
        }
        
        else if(datas=="*back#"||datas=="back")
          {
            x=1;
                Wire.beginTransmission(8); // transmit to device #8
             Wire.write(x);              // sends one byte
             x=4;
            Wire.write(x);
            Wire.endTransmission();    // stop transmitting
            _back2();
            path[j++]=2;
          }
          
          else if(datas=="*left#"||datas=="left")
          {
            x=1;
             Wire.beginTransmission(8); // transmit to device #8
            Wire.write(x);              // sends one byte
             x=1;
           Wire.write(x);
           Wire.endTransmission();    // stop transmitting
            left_();
            path[j++]=3;
          }
          
           else if(datas =="*right#"||datas =="right")
          {
            x=1;
            Wire.beginTransmission(8); // transmit to device #8
            Wire.write(x);              // sends one byte
             x=2;
            Wire.write(x);
            Wire.endTransmission();    // stop transmitting
            right_();
            path[j++]=4;
          }

          else if(datas=="*return#"||datas=="return")
          {
            j=j-1;
            break;
        }
     }
  }
  
  for(int i=0;i<=j;i++)
    {
      if(path[i]==3) path[i]=4;
      if(path[i]==4) path[i]=3;
    }
    
    right_();
    right_();
    
    for(int i=j;i>=0;i--)
    {
      if(path[i]==1) move_2();
      else if(path[i]==2) _back2();
      else if(path[i]==3) left_();
      else if(path[i]==4) right_();
    }
}




void add()
{
  
  
   String j,k;
   while(Serial.available()<=0);
   if(Serial.available()>0)
   
   j=Serial.readString();
   int q=0;
   char z[j.length()];
    for(int a=1;a<j.length();a++)
   {
    if(j[a]!='#')
    z[q++]=j[a];
   }
   z[q]='\0';
   Serial.print(z);
   int g=atoi(z);
   Serial.print(g);
   while(Serial.available()<=0);
   j=Serial.readString();
   char y[j.length()];
   q=0;
    for(int a=1;a<j.length();a++)
   {
    if(j[a]!='#')
    y[q++]=j[a];
   }
   y[q]='\0';
   Serial.print(y);
  int h=atoi(y);
   Serial.print(h);
   int f=g+h;
   int t=10;
   int s[30];
   int count=0;
   
   while(1)
   {  count++;
      if((int)(f/t)==0) {s[count++]=f%t; break;}
      else s[count++]=f%t; t=t*10;  
   }
   count = count-1;
   byte c=count;
   
   Serial.println(f);
   //byte myarray[2];
   //myarray[0]= (f>>8) & 0xFF;
   //myarray[1]=f & 0xFF;
   lcd.clear();
   lcd.print(f);
   byte m=f;
   x=9;
    Wire.beginTransmission(8); // transmit to device #8
   Wire.write(x);              // sends one byte
   Wire.write(m);              // sends one byte
   Wire.endTransmission();    // stop transmitting */
  // Wire.beginTransmission(8); // transmit to device #8
   //Wire.write(m);              // sends one byte
   //Wire.endTransmission();    // stop transmitting
/*      Wire.beginTransmission(8); // transmit to device #8
    for(int v=count;v>=0;v--)
    {
      byte y=s[v];
       Wire.write(y);
    }
     Wire.endTransmission();    // stop transmitting*/
   
   
}

double humid()
{
  DHT.read11(dht_apin);
  return DHT.humidity;
}

double temp()
{
  DHT.read11(dht_apin);
  return DHT.temperature;
}



void teach()
{
  lcd.clear();
  lcd.print("Tell This");
   x=11;
  Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=3;
      Wire.write(x);
      Wire.endTransmission();    // stop transmitting
      lcd.clear();
      lcd.print("APPLE");
      String p,q;
      while(Serial.available()<=0);
      if(Serial.available())
      p=Serial.readString();
      if(p!="exit"&&p!="*exit#")
      {
        if(p=="Apple"||p=="*Apple#")
          {x=11;
            Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=1;
      Wire.write(x);
      Wire.endTransmission();
            
          }
          else
           {x=11;
            Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=2;
      Wire.write(x);
      Wire.endTransmission();
            
          }
      }
       lcd.clear();
  lcd.print("Tell This");
   x=11;
  Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=3;
      Wire.write(x);
      Wire.endTransmission();    // stop transmitting
      lcd.clear();
      lcd.print("BALL");
      
      while(Serial.available()<=0);
      if(Serial.available())
      p=Serial.readString();
      if(p!="exit"&&p!="*exit#")
      {
        if(p=="ball"||p=="*ball#")
          {x=11;
          lcd.clear();
      lcd.print("CORRECT");
            Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=1;
      Wire.write(x);
      Wire.endTransmission();
            
          }
          else
           {x=11;
           lcd.clear();
      lcd.print("WRONG");
            Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=2;
      Wire.write(x);
      Wire.endTransmission();
            
          }
      }
}

void loop()
{
  //lcd.clear();
  time2=millis();
  if(time2==time1)
  {
    x=10;
  Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=1;
      Wire.write(x);
      Wire.endTransmission();    // stop transmitting
  }
   /*int f = Sonar();
 if(f<=22)
 {stop_();
 Serial.println(f);
 x=3;
  Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=1;
      Wire.write(x);
      Wire.endTransmission();    // stop transmitting
 
 }
// Serial.println(f);*/
  while(Serial.available())
  {
   String datas=Serial.readString();
   Serial.println(datas);
    if(datas == "*move#"||datas == "*more#"||datas=="*forward#"||datas =="*move ahead#"||datas=="*chalo#"||datas == "move"||datas == "more"||datas=="forward"||datas =="move ahead")
    {
      x=1;
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=3;
      Wire.write(x);
      Wire.endTransmission();    // stop transmitting
 move_();

    }
     else if(datas == "*back#"||datas== "*move back#"||datas == "back"||datas== "move back")
    {
      x=1;
        Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=4;
      Wire.write(x);
      Wire.endTransmission();    // stop transmitting
 _back();
    }
    else if(datas == "*stop#"||datas =="*ruko#"||datas == "stop"||datas =="ruko")
      {
        stop_();
    }

    else if(datas =="*left#"||datas == "*list#"||datas =="left"||datas == "list")
      {
        x=1;
        Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=1;
       Wire.write(x);              // sends one byte
      Wire.endTransmission();    // stop transmitting
      
        left_();
      }
       else if(datas =="*right#"||datas =="right")
      {
        x=1;
        Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=2;
       Wire.write(x);              // sends one byte
       Wire.endTransmission();    // stop transmitting
        right_();
      }
  else if(datas=="*dance#"||datas=="dance")
  {
    dance();
  }
      else if(datas=="*Parth#"||datas=="*path#"||datas=="Parth"||datas=="path")
      {
        path_();
      }
      else if(datas=="*temperature#"||datas=="temperature"||datas=="what is the temperature"||datas=="*what is the temperature#")
      {
        double z=temp();
        Serial.println("Temperature: ");
        Serial.println(z);
        lcd.clear();
        int f=z;
        String my=String(f);
        lcd.print(z);
        lcd.print("  Degree"); 
         Wire.beginTransmission(8); // transmit to device #8
         x=2;
      Wire.write(x);              // sends one byte
      x=(byte)z;
       Wire.write(x);              // sends one byte
       Wire.endTransmission();    // stop transmitting
      }
      else if(datas == "*humidity#"||datas == "humidity")
      {
       // humid();
         double z=humid();
        Serial.println("Humidity: ");
        Serial.println(z);
        lcd.clear();
        lcd.print(z);
         Wire.beginTransmission(8); // transmit to device #8
         x=5;
      Wire.write(x);              // sends one byte
      x=(byte)z;
   
       Wire.write(x);              // sends one byte
       Wire.endTransmission();    // stop transmitting
      }
      
      else if(datas == "*add#"||datas == "add")
      {
        add();
      }
     /*  else if(datas == "*multiply#")
      {
        multiply();
      }*/

       else if(datas == "*music#"||datas == "music")
       {
        x=7;
         Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=1;
      Wire.write(x);
      Wire.endTransmission();    // stop transmitting
       }
       else if(datas == "*what is your name#"||datas== "*name#"||datas=="*Who are you#"||datas == "what is your name"||datas== "name"||datas=="Who are you")
      {
        x=6;
        lcd.clear();
        lcd.print("I am Alpha");
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=1;
      Wire.write(x);
      Wire.endTransmission();    // stop transmitting
      }
        else if(datas == "*hi#"||datas== "*hello#"||datas == "hi"||datas== "hello")
      {
        x=6;
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=2;
      Wire.write(x);
      Wire.endTransmission();    // stop transmitting
      }
      else if(datas =="*up#"||datas =="up")
      {
        A_up();
      }
       else if(datas =="*down#"||datas =="down")
      {
        A_down();
      }
       else if(datas =="*hold#"||datas =="hold")
      {
        B_hold();
      }
       else if(datas =="*release#"||datas =="release")
      {
        B_release();
      }
      else if(datas=="*set alarm#"||datas=="set alarm")
      {
        time1=millis();
        while(Serial.available()<0);
        unsigned long delta;
        String v;
        v=Serial.readString();
        String g,h;
        int b=0;
        for(int i=0;i<v.length();i++)
        {
          
          if(v[i]==32) break;
          b++;
        }
        g=v.substring(1,b);
        char n[5];
        int cc=0;
        for(int cc=0;cc<g.length()-1;cc++)
        n[cc]=g[cc+1];
        n[cc]='\0';
        char w[8];
        int z=0;
        for(int i=b+1;i<v.length();i++)
        {
          w[z++]=v[i];
        }
        w[z]='\0';
        int t=atoi(n);
        if(w=="minute"||w=="minutes")
        {
          time1=time1+t*60*1000;
        }
        else if(w=="hour"||w=="hours")
        {
          time1=time1+t*60*1000*60;
        }
      }
      else if(datas == "*I am sick#"||datas == "I am sick"||datas=="i am sick")
      {
         x=6;
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=3;
      Wire.write(x);
      Wire.endTransmission();    // stop transmitting
      }
       else if(datas == "*I broke up#"||datas == "I broke up"||datas == "i broke up"||datas == "*i broke up#")
      {
         x=6;
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=4;
      Wire.write(x);
      Wire.endTransmission();    // stop transmitting
      }
      else if(datas == "*I like you#"||datas=="*I love you#"||datas == "I like you"||datas=="I love you"||datas == "*I like you#"||datas=="*I love you#"||datas == "i like you"||datas=="i love you")
      {
         x=6;
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=5;
      Wire.write(x);
      Wire.endTransmission();    // stop transmitting
      }
       else if(datas == "*Give your introduction#"||datas=="*introduce yourself#"||datas=="*introduce#"||datas == "give your introduction"||datas=="introduce yourself"||datas=="introduce")
      {
         x=6;
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=6;
      Wire.write(x);
      Wire.endTransmission();    // stop transmitting
      }
      
      else if(datas=="*teach#"||datas=="teach")
      {
       teach();
      }
      else
      {
        x=8;
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(x);              // sends one byte
      x=1;
      Wire.write(x);
      Wire.endTransmission();    // stop transmitting
      }
      
      
   }
}


