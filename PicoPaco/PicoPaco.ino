//#include <util/atomic.h>  // For the ATOMIC_BLOCK macro
#include <OneWire.h>
#include <DallasTemperature.h>

#define ENCA 2  // WHITE
#define ENCB 3  // YELLOW
#define PWM 5   //Output
#define ONE_WIRE_BUS 4 //Digital 4
#define CURRENT 1

OneWire oneWire(ONE_WIRE_BUS);  
DallasTemperature sensors(&oneWire);


int serialInfo = 0;
int dutyCycle = 0;
int increment = 0;


int i = 100;p
int x = 1;

// globals
long prevT = 0;
int posPrev = 0;

volatile int pos = 0;
volatile int pos_i = 0;

volatile float velocity = 0;
//volatile float velocity_i = 0;
//volatile long prevT_i = 0;

float deriv = 0;
float eintegral = 0;
float eprev = 0;

float v1Filt = 0;
float v1Prev = 0;

float temp = 0;


void setup() {
  Serial.begin(9600);
  sensors.begin();

  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  pinMode(PWM, OUTPUT);

  PWMtimer_setup();

  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);
}

void loop() {

  // Send the command to get temperatures
  sensors.requestTemperatures(); 

  //print the temperature in Celsius
  //Serial.print("Temperature: ");
  float newtemp = sensors.getTempCByIndex(0);
  if (newtemp>0){
    temp = newtemp;
  }
  //Serial.print(newtemp);
  //Serial.print("C  |  ");

  float val = analogRead(CURRENT);  // read the input pin
  float current = (val - 516)*39/512; //CURRENT SENSOR

  //Serial.print("Current: ");
  //Serial.print(current);

  // serialInfo = Serial.parseInt();
  // if (serialInfo > 0) {
  //   dutyCycle = serialInfo;
  // }
  
  dutyCycle = i;
  if (i>150){
    x = -1;
  }
  if(i<52){
    x= 1;
  }
  i = i+x;
  delay(50);


//thsi workx right

  // ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
  //   pos = pos_i;
  // }

   // Compute velocity
  long currT = micros();
  float deltaT = ((float) (currT-prevT))/1.0e6;
  float velocity = (pos - posPrev)/deltaT;
  //Gearbox Correction
  velocity = velocity/19.2;

  posPrev = pos;
  prevT = currT;


  // Low-pass filter
  v1Filt = 0.854*v1Filt + 0.0728*velocity + 0.0728*v1Prev;
  v1Prev = velocity;

  //CONTROL ALGORITHM
  float vtarget = 30;

  // Constants
  float kp = 1.5;
  float kd = 0;
  float ki = 1;

  //error
  float error = vtarget - v1Filt;
  deriv = (v1Filt-v1Prev)/(deltaT);
  eintegral = eintegral + error*deltaT;
  
  float u = kp*error + kd*deriv + ki*eintegral;

  int pwr = ((int) fabs(u));//+dutyCycle;
  if(pwr > 150){
    pwr = 150;
  }

  //Motor Outputs
  //dutyCycle = map(pwr,0,40,100,150);//map(u,0,800,100,150);

  // Serial.print("Serial : ");
  //Serial.println(dutyCycle);
  // Serial.print("Velocity : ");
  // Serial.println(velocity);
  // Serial.print("Pos : ");
  // Serial.println(pos);

  // Serial.print(vtarget);
  // Serial.print(" ");
  // Serial.print(v1Filt);
  // Serial.println();
  // delay(1);

  Serial.print("  ");
  Serial.print(50);
  Serial.print(" ");
  Serial.print(v1Filt);
  Serial.print(" ");
  Serial.print(velocity);
  Serial.print(" ");
  Serial.print(0);
  Serial.println();
  delay(1);


  // Serial.print(150);
  // Serial.print(" ");
  // Serial.print(dutyCycle);
  // Serial.print(" ");
  // Serial.print(100);
  // Serial.println();
  // delay(1);

  eprev = error;
}


void readEncoder() {
  int b = digitalRead(ENCB);
  if(b>0){
    increment = 1;
  }
  else{
    increment = -1;
  }
  pos_i = pos_i + increment;
}

void PWMtimer_setup() {
  // Set up Timer 1 for 50Hz PWM
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 39999;            // 16MHz / (50Hz * 8) - 1
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS11);    // Prescaler 8
  TIMSK1 |= (1 << OCIE1A);  // Enable timer compare interrupt
}

ISR(TIMER1_COMPA_vect) {
  // Calculate duty cycle
  int pulseWidth = map(dutyCycle, 0, 4000, 0, OCR1A);

  // Generate PWM signal
  digitalWrite(PWM, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(PWM, LOW);
}