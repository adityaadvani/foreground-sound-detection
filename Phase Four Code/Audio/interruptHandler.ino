int tops; //count overflow

void interruptSetup() {
  // Initializes Timer2 to throw an interrupt every 2mS.
  TCCR2A = 0x02;     // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
  TCCR2B = 0x07;     // DON'T FORCE COMPARE, 1024 PRESCALER
  OCR2A = 0x2E;      // SET THE TOP OF THE COUNT TO 46
  TIMSK2 = 0x02;     // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
  sei();     
  // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED
  tops = 0;
}

ISR(TIMER2_COMPA_vect){
   cli();
   
   tops++;
  // Serial.println(tops);
   
   if(tops >= 17){ // calls doFFT every 50ms 
      tops = 0;
      doFFT=true;
   }
      
   sei(); 
}
