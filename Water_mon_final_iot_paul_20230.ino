#define BLYNK_TEMPLATE_ID "TMPL65aTEtSf"
#define BLYNK_TEMPLATE_NAME "Water Level"
#define BLYNK_AUTH_TOKEN "Q_vWmil-NJ_2ZUCx2CJvn-0jt0ei4p-I"
#define BLYNK_PRINT Serial
#define TINY_GSM_MODEM_SIM800
#include <TinyGPSPlus.h>
#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>
#include <LiquidCrystal.h>


///#define BLYNK_TEMPLATE_ID "TMPL65aTEtSf"
//#define BLYNK_TEMPLATE_NAME "Water Level"
//#define BLYNK_AUTH_TOKEN "Q_vWmil-NJ_2ZUCx2CJvn-0jt0ei4p-I"


HardwareSerial sim(1);
TinyGsm modem (sim);

char auth[] = "Q_vWmil-NJ_2ZUCx2CJvn-0jt0ei4p-I";
char apn[]  = "web.gprs.mtnnigeria.net";
char user[] = "";
char pass[] = "";

BlynkTimer timer;
int pointIndex = 1; 
LiquidCrystal lcd(22,21,2,4,23,19);


// ultrasonic sensor trigger pin on arduino
const int Trig= 5; //33
// ultrasonic sensor echo pin on arduino
 
const int Echo = 18; //32
// pump pin

float VolumeOfWater =0.00;
float waterLevel;

// Interval before pluse is recieved from the sensor
long Interval;


void setup(){

  Serial.begin(9600);  
  lcd.begin(16, 2); 
  lcd.setCursor (0,0);
  lcd.print(" Water level");
  lcd.setCursor (0,1);
  lcd.print("IoT System");   
  
  delay(2000);
  
  pinMode(Trig, OUTPUT);
  // Sets trigger pin as input on the arduino
  pinMode(Echo, INPUT); 

  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);

  // recieve pulse time from the ultasonic sensor
  Interval = pulseIn(Echo, HIGH);

  VolumeOfWater = ((Interval * 0.034) / 2);
  waterLevel = map(VolumeOfWater,32,0,0,100);    
  
  LCD(waterLevel);
  
  

  sim.begin(9600, SERIAL_8N1,16,17);
  
  Blynk.begin (auth, modem, apn, user, pass);
  

  delay(3000);
  }
  

void loop()
{  
   sendToBlynk();
   Blynk.run();
   timer.run();   

}

void LCD(float depth){ 
  lcd.setCursor (0,0);
  lcd.print(" Water level");
  lcd.setCursor (0,1);
  lcd.print("Percentage:");
  lcd.setCursor (11,1);
  lcd.print(depth);
  delay(100);
}

void sendToBlynk()
{
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);

  // recieve pulse time from the ultasonic sensor
  Interval = pulseIn(Echo, HIGH);

  VolumeOfWater = ((Interval * 0.034) / 2);
  waterLevel = map(VolumeOfWater,36,0,0,100);    
  Serial.println(VolumeOfWater);
  
 LCD(waterLevel);
 Blynk.virtualWrite(V0, VolumeOfWater);
 Blynk.virtualWrite(V1, waterLevel); 

   if(waterLevel < 20){
    Blynk.logEvent("water_level_low", String("Attention: Water level in the container is low (below 20% capacity). Please refill as soon as possible.") + waterLevel);
      
  }
  if(waterLevel == 50){
   
    Blynk.logEvent("water_level_medium", String("Attention: The container is half-full (at 50% capacity). Consider refilling soon.") + waterLevel);
  
    }

  if(waterLevel > 90){

    Blynk.logEvent("water_level_full", String("Attention: The container is almost full ( at 90% capacity). Please stop filling it to avoid overflows.") + waterLevel);
  
    
  }
 

}  
  

