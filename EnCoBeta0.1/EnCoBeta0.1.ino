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
int deflect = 0;
int CPt = 0, CPr = 0, CPt = 0;
void setup() { 
pinMode(FL1, INPUT_PULLUP);
pinMode(FL2, INPUT_PULLUP);
pinMode(FR1, INPUT_PULLUP);
pinMode(FR2, INPUT_PULLUP);
pinMode(BL1, INPUT_PULLUP);
pinMode(BL2, INPUT_PULLUP);
pinMode(BR1, INPUT_PULLUP);
pinMode(BR2, INPUT_PULLUP);

} 
void loop() { 
deflect = DefMeasure();
digitalWrite(FL1, 0);
digitalWrite(FL2, 1);
digitalWrite(FR1, 0);
digitalWrite(FR2, 1);
digitalWrite(BL1, 0);
digitalWrite(BL2, 1);
digitalWrite(BR1, 0);
digitalWrite(BR2, 1);
analogWrite(EnFl,100);
analogWrite(EnFr,100);
analogWrite(EnBl,100);
analogWrite(EnBr,100);



}

int DefMeasure(){

  return 0;
}
