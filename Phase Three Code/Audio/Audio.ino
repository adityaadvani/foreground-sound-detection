#define LOG_OUT 1 
#define FFT_N 128 
#include <FFT.h> 

static unsigned long t1;
int count;
int ticks;
int past[9][64];
int ledPin = 13;

void setup() {
  Serial.begin(115200);
  ticks = 0;
  count = 0; 
  pinMode(ledPin,OUTPUT);
}

void loop() {
  while(1){
    t1 = micros();
    //Serial.println(t1);
    ticks++;
    count = 0;
    for (int i = 0 ; i < 256 ; i += 2) { 
      fft_input[i] = analogRead(A0);
      //Serial.println(fft_input[i]);
      fft_input[i+1] = 0;
    }

    fft_window();
    fft_reorder();
    fft_run(); 
    fft_mag_log(); 

    
    
    for(int i = 0; i < 64; i++){
        //Serial.print(fft_log_out[i]);
        if(((fft_log_out[i] - past[0][i]) > 20) || ((past[0][i] - fft_log_out[i]) > 20)){
            //Serial.print("*");
            count++;
        }
        //Serial.print(",");
      
    }
    //Serial.println();
     
      
    //print average
    for(int i = 0; i < 64; i++){
      Serial.print(past[0][i]);
      Serial.print(",");
    }
    Serial.println();

    //move back 1 position
    for(int i = 0; i < 8 ; i++){
      for(int j = 0 ; j < 64 ; j++){
        past[i][j] = past[i+1][j];
      }
    }

    //get new reading
    for(int i = 0; i < 64 ; i++){
      past[8][i] = fft_log_out[i];
    }

    //compute new moving average sum
    for(int i = 1; i < 9 ; i++){
      for(int j = 0 ; j < 64 ; j++){
        past[0][j] += past[i][j];
      }
    }

    //compute new moving average
    for(int i = 0; i < 64 ; i++){
      past[0][i] /= 9;
    }

    if(count > 8){
      ticks = 0;
      digitalWrite(ledPin,HIGH);
      //event occured
    }

    if(ticks > 2){
      digitalWrite(ledPin,LOW);
      //event dissolved
    }
  
  }
}
