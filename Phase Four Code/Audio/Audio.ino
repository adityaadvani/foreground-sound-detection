#define LOG_OUT 1 
#define FFT_N 128 
#include <FFT.h> 

static unsigned long t1; //system time 
int count; //bin set counter
int ticks; //LED lapsed time counter
int past[9][64]; //averaging buffer

boolean doFFT; //interrupt flag


void setup() {
  doFFT = false;
  pinMode(13,OUTPUT); //digital signal pin for LED
  Serial.begin(115200); //fast serial communication
  ticks = 0; 
  count = 0; 

  
  interruptSetup(); //function to set interrupt with required settings
}

void loop() {
  //if interrupt invoked
  if(doFFT){
    doFFT = false;
    t1 = micros();
   // Serial.println(t1);
    ticks++;
    count = 0;

    //read 128 values
    for (int i = 0 ; i < 256 ; i += 2) { 
      fft_input[i] = analogRead(A0); 
      fft_input[i+1] = 0;
    }

    //do fft
    fft_window();
    fft_reorder();
    fft_run(); 
    fft_mag_log(); 

    
    //check all 64 bins
    for(int i = 0; i < 64; i++){
        //Serial.print(fft_log_out[i]);
        
        //threshold = 15, if crossed, activate bin
        if(((fft_log_out[i] - past[0][i]) > 15) || ((past[0][i] - fft_log_out[i]) > 15)){
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

    //if 10 or more bins signify foreground sound presence
    if(count > 10){
      ticks = 0;
      //activate LED
      digitalWrite(13,HIGH);
      //event occured
    }

    //if no forefround sound event in last 3 rounds, deactivate LED
    if(ticks > 2){
      digitalWrite(13,LOW);
      //event dissolved
    } 
  }
}



