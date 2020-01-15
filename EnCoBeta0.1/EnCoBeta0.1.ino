#include<Servo.h>
//
// Пины enable моторов
//
#define EnFl 9
#define EnFr 8
#define EnBl 6
#define EnBr 7
//
// Направления моторов
//
#define FL1 30
#define FL2 32
#define FR1 26
#define FR2 24
#define BL1 44
#define BL2 42
#define BR1 50
#define BR2 48
int DiFR = 1, DiFL = 1, DiBR = 1, DiBL = 1;
//
//Пины enable драйверов
//
#define EnDf 28
#define EnDb 46
//
//Servo
//
#define S_Take 47// 40 140
#define S_Catch 49 //40 140
#define S_Throw 51 // 100
#define S_Door 45
#define S_I_don't_want_to_break_free 479 
Servo s_catch;
Servo s_keep;
Servo s_throw;
//
//Пид-Регулятор
//
//коэффиценты
int snsr[8];


float K_f = 0.001;
float K_p = 0;
float K_n = 0.5;

int SpeedL = 0;
int SpeedR = 0;
int NSpeedL = 200;
int NSpeedR = 200;
int last_f = 0;
int last_p = 0;
int level;
//
//!Глаза!
//
#define L1 A8
#define L2 A9
#define L3 A10
#define L4 A11
#define R4 A12
#define R3 A13
#define R2 A14
#define R1 A15
//
//Понты
//
#define StartButt 3 // кнопка для старта
#define Mistake 450 // показатель датчика если видит линию
#define MakeItLower 1 //понижение сигнала ошибки
#define Signal 8 // 
bool points[8]; // видет / не видет линию
void SetDirection(bool L , bool R){
  digitalWrite(FL1, L);
  digitalWrite(FL2,!L);
  digitalWrite(FR1, R);
  digitalWrite(FR2,!R);
  digitalWrite(BL1,!L);
  digitalWrite(BL2, L);
  digitalWrite(BR1, R);
  digitalWrite(BR2,!R);

}


void setup() {
  s_catch.attach(S_Catch);
  s_keep.attach(8);
  s_throw.attach(S_Throw);
  pinMode(FL1, OUTPUT);
  pinMode(FL2, OUTPUT);
  pinMode(FR1, OUTPUT);
  pinMode(FR2, OUTPUT);
  pinMode(BL1, OUTPUT);
  pinMode(BL2, OUTPUT);
  pinMode(BR1, OUTPUT);
  pinMode(BR2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(StartButt, INPUT_PULLUP);
  pinMode(L1, INPUT_PULLUP);
  pinMode(L2, INPUT_PULLUP);
  pinMode(L3, INPUT_PULLUP);
  pinMode(L4, INPUT_PULLUP);
  pinMode(R4, INPUT_PULLUP);
  pinMode(R3, INPUT_PULLUP);
  pinMode(R2, INPUT_PULLUP);
  pinMode(R1, INPUT_PULLUP);
  pinMode(EnFl,OUTPUT);
  pinMode(EnFr,OUTPUT);
  pinMode(EnBl,OUTPUT);
  pinMode(EnBr,OUTPUT);
  pinMode(Signal, OUTPUT);
  Serial.begin(9600);
  digitalWrite(EnDf, 1);
  digitalWrite(EnDb, 1);
  SetDirection(true, true);
  int val;
  while(digitalRead(StartButt)){
    val = analogRead(A0);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  s_keep.write(val);                  // sets the servo position according to the scaled value
  delay(15);     
  }
}

void Update(){
  digitalWrite(3, 1);
  snsr[0] = analogRead(L1);
  snsr[1] = analogRead(L2);
  snsr[2] = analogRead(L3);
  snsr[3] = analogRead(L4);
  snsr[4] = analogRead(R4);
  snsr[5] = analogRead(R3);
  snsr[6] = analogRead(R2);
  snsr[7] = analogRead(R1) - 10;
  digitalWrite(3, 0);
}


void Run(int measured){
  /*if(points[7] && points[6] && points[5]){
  SetDirection(true, false);
  analogWrite(EnFr,255);
  analogWrite(EnBr,255);
  analogWrite(EnFl,255);
  analogWrite(EnBl,255);
  delay(400);
  } else */if(points[0]){
    SetDirection(false, true);
    analogWrite(EnFr,255);
    analogWrite(EnBr,255);
    analogWrite(EnFl,255);
    analogWrite(EnBl,255);
    delay(50);
    return;
  }
  else if(points[6]){
    SetDirection(true, false);
    analogWrite(EnFr,255);
    analogWrite(EnBr,255);
    analogWrite(EnFl,255);
    analogWrite(EnBl,255);
    delay(50);
    return;
  }
  if(analogRead(R1)  > 900){
    SetDirection(false, false);
    analogWrite(EnFr,20);
    analogWrite(EnBr,20);
    analogWrite(EnFl,20);
    analogWrite(EnBl,20);
    delay(100);
    SetDirection(true, false);
    analogWrite(EnFr,255);
    analogWrite(EnBr,255);
    analogWrite(EnFl,255);
    analogWrite(EnBl,255);
    while(analogRead(R4) < 900){}
  }
  SetDirection(true, true);
  int deflect = 0 - measured;
  last_p = last_p + K_p * deflect;
  level = K_n * deflect + last_p + K_f * (deflect - last_f);
  last_f = deflect;
  analogWrite(EnFr,constrain(NSpeedR + level, 0, 255));
  analogWrite(EnBr,constrain(NSpeedR + level, 0, 255));
  analogWrite(EnFl,constrain(NSpeedL - level, 0, 255));
  analogWrite(EnBl,constrain(NSpeedL - level, 0, 255));
}

int DefMeasure(int sensors[8], int cntr) {
  cntr = constrain(cntr, -4, 4);
  int summ = 0;

  int negative = 0;
  int positive = 0;
  for (int i = 0 ; i < 8; i++) {
    summ += sensors[i];
  }
  int s_avg = summ / 8;

  for (int i = 0 ; i < 8; i++) {
    if (sensors[i] > s_avg) {
      points[i] = 1;


    } else {
      points[i] = 0;
    }


    Serial.print(points[i]);
  }
  Serial.print(' ');


  int dd = 0;
  for (int i = 7 ; i > 0 ; i--) {
    if (points[i] == 1 and i <= 4 + cntr) {
      if (constrain(i - 1, 0, 7) != constrain(i, 0, 7)) {
        dd = sensors[constrain(i - 1, 0, 7)] + sensors[constrain(i, 0, 7)];
      } else {
        dd = sensors[constrain(i, 0, 7)];
      }

      negative -=  constrain(4 + cntr - i , 0, 7) * Mistake + dd ;
    }

  }

  for (int i = 0 ; i < 8 ; i++) {
    if (points[i] == 1 and i >= 4 + cntr) {
      if (constrain(i + 1, 0, 7) != constrain(i, 0, 7)) {
        dd = sensors[constrain(i + 1, 0, 7)] + sensors[constrain(i, 0, 7)];
      } else {
        dd = sensors[constrain(i, 0, 7)];
      }

      positive += constrain(-4 + cntr + i , 0, 7)  * Mistake + dd ;
    }

  }
  //if_corner(points);
  Serial.println((positive + negative)/MakeItLower);
  return (positive + negative)/MakeItLower ;

}


void loop() {
  Update();
  Serial.print(analogRead(R1));
  Run(DefMeasure(snsr, 0));
}
