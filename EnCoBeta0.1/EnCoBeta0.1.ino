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


bool K_f = 0.1;
bool K_p = 0;
bool K_n = 0.001;

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
#define StartButt 3
#define Mistake 450
#define MakeItLower 20
#define Signal 8
void setup() {
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
  SetDirection(true, true, true, true);
  while(digitalRead(StartButt)){}
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
  snsr[7] = analogRead(R1 - 15);
  digitalWrite(3, 0);
}
void SetDirection(bool FL, bool FR, bool BL, bool BR){
  digitalWrite(FL1, FR);
  digitalWrite(FL2, !FR);
  digitalWrite(FR1, FL);
  digitalWrite(FR2, !FL);
  digitalWrite(BL1, !BR);
  digitalWrite(BL2, BR);
  digitalWrite(BR1, BL);
  digitalWrite(BR2, !BL);
}

void Run(int measured){
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
  bool points[8];
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

      negative -= pow(2.7, constrain(4 + cntr - i , 0, 7) * constrain(4 + cntr - i , 0, 7)) * Mistake + dd ;
    }

  }

  for (int i = 0 ; i < 8 ; i++) {
    if (points[i] == 1 and i >= 4 + cntr) {
      if (constrain(i + 1, 0, 7) != constrain(i, 0, 7)) {
        dd = sensors[constrain(i + 1, 0, 7)] + sensors[constrain(i, 0, 7)];
      } else {
        dd = sensors[constrain(i, 0, 7)];
      }
     
       positive += pow(2.7,constrain(-4 + cntr + i , 0, 7) * constrain(4 + cntr - i , 0, 7) ) * Mistake + dd ;}

  }
  Serial.println((positive + negative)/MakeItLower);
  return (positive + negative)/MakeItLower ;
}

void loop() {
  Update();
  Run(DefMeasure(snsr, 0));
}
