
/*
  Turbidity control with arduino
     --Author:@CSTAO
  A
*/

//parameters
float voltage_Threshold=600; //measured OD=0.153 in LB medium
int pumpTime = 20; //s
int loopTime = 60; //s
int HZ_stimulate = 4000;
int HZ_stable = 300;




//
int microS=(1000000/HZ_stimulate)/2;
int loopS=(1000000/HZ_stable)/2;
int pwm_loop = HZ_stimulate;
int one_loop = HZ_stable;
int i;
int j;
int a;
int b;
int c;
int count =0;
int sensor_all =0;

void setup(){
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(13,OUTPUT);
}

void loop(){
  for(c = 0;c<=10000;c++){
    sensor_all += analogRead(A0);
    count+=1;
  }
  float sensor = sensor_all/count;
  float turbidity_voltage = sensor;
  sensor_all=0;
  count=0;
  
  Serial.println(turbidity_voltage);
  if(turbidity_voltage < voltage_Threshold){
    
    for(a = 0;a<=pumpTime;a++){
      micro_pwm(pwm_loop,microS);
    }
  }

  for(b = 0;b<=loopTime;b++){
    micro_pwm(one_loop,loopS);
  }
}




void micro_pwm(int all_loop, int half_wave){
  for(i = 0;i<=all_loop;i++){
  digitalWrite(13, HIGH);
  delayMicroseconds(half_wave);
  digitalWrite(13, LOW);
  delayMicroseconds(half_wave);
  }
}
