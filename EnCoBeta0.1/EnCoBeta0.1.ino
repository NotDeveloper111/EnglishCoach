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
#define S_Take 47 
#define S_Catch 49 
#define S_Throw 51 
#define S_Door 45 
#define S_I_don't_want_to_break_free 479
//
//Пид-Регулятор
//
//коэффиценты
int snsr[8];
int K_f = 0;
int K_p = 0;
int K_n = 1;
int SpeedL = 0;
int SpeedR = 0;
int NSpeedL = 150;
int NSpeedR = 150;
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
void setup() { 
pinMode(FL1, INPUT_PULLUP);
pinMode(FL2, INPUT_PULLUP);
pinMode(FR1, INPUT_PULLUP);
pinMode(FR2, INPUT_PULLUP);
pinMode(BL1, INPUT_PULLUP);
pinMode(BL2, INPUT_PULLUP);
pinMode(BR1, INPUT_PULLUP);
pinMode(BR2, INPUT_PULLUP);

pinMode(L1, INPUT_PULLUP);
pinMode(L2, INPUT_PULLUP);
pinMode(L3, INPUT_PULLUP);
pinMode(L4, INPUT_PULLUP);
pinMode(R4, INPUT_PULLUP);
pinMode(R3, INPUT_PULLUP);
pinMode(R2, INPUT_PULLUP);
pinMode(R1, INPUT_PULLUP);
Serial.begin(9600);
digitalWrite(EnDf, 1);
digitalWrite(EnDb, 1);
SetDirection(true, true, true, true);
} 
void loop() { 
Run();

delay(500);
}
int Get(){
for(int i = 0;i < 8;i++){
  snsr[i] = analogRead(i + 62);
  }
  return snsr;
}
void SetDirection(bool FL, bool FR, bool BL, bool BR){
  
digitalWrite(FL1, FR);
digitalWrite(FL2, !FR);
digitalWrite(FR1, FL);
digitalWrite(FR2, !FL);
digitalWrite(BL1, BR);
digitalWrite(BL2, !BR);
digitalWrite(BR1, BL);
digitalWrite(BR2, !BL);
}
void Run(){
  int deflect = DefMeasure(Get(), 0);
  last_p = last_p + K_p * deflect;
  level = K_n * deflect + last_p + K_f * (deflect - last_f);
  last_f = deflect;
  analogWrite(EnFr,constrain(NSpeedR - level, 0, 255));
  analogWrite(EnBr,constrain(NSpeedR - level, 0, 255));
  analogWrite(EnFl,constrain(NSpeedL + level, 0, 255));
  analogWrite(EnBl,constrain(NSpeedL + level, 0, 255));  
}

int DefMeasure(int sensors[8], int cntr) {

  cntr = constrain(cntr, -4, 4);
  int summ = 0;
  bool points[8];
  int negative;
  int positive;
  for (int i = 0 ; i < 8; i++) {
    summ += sensors[i];
  }
  int s_avg = summ / 8;

  for (int i = 0 ; i < 8; i++) {
    if (sensors[i] > s_avg * 1.5) {
      points[i] = 1;
    } else {
      points[i] = 0;
    }
  }
  int dd = 0;
  for (int i = 8 ; i > 0 ; i--) {
    if (points[i] == 1 and i <= 8 + cntr) {
      if (constrain(i - 1, 0, 8) != constrain(i, 0, 8)) {
        dd = sensors[constrain(i - 1, 0, 8)] + sensors[constrain(i, 0, 8)];
      } else {
        dd = sensors[constrain(i, 0, 8)];
      }
      negative = 0 - (((constrain(4 + cntr - i , 0, 8)) * 450) + dd) ;
    }
  }
  for (int i = 0 ; i < 8 ; i++) {
    if (points[i] == 1 and i >= 4 + cntr) {
      if (constrain(i + 1, 0, 8) != constrain(i, 0, 8)) {
        dd = sensors[constrain(i + 1, 0, 8)] + sensors[constrain(i, 0, 8)];
      } else {
        dd = sensors[constrain(i, 0, 8)];
      }
      positive =  (((constrain(-4 + cntr + i , 0, 8)) * 450) + dd) ;
    }
  }
  return (positive + negative) ;
}
