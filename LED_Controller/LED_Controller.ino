
/**
 *  License: GNU GENERAL PUBLIC LICENSE
 * 
 *  Arduino class to control LED array
 *  Two modes:
 *  0 - Brightness relative to potentiometer value
 *  1 - Fade speed relative to potentiometer value
 *  
 */

/* Components */
const int potPin = A1;  // Potentiometer
const int pushButton = 1; // Mode Button
const int ledPin = 0; // LED Array / controller

/* Fade States */
const int FadeUp = -1;
const int FadeDown = 1;


/* Input States */
int butState = 0;
/* Button already held down? - For single press button rather than hold switch */
bool butDown = false;

/* Variables */
int mode = 0; // Current control mode
int potInput = 0; // Inpunt reading of potentiometer        
int fadeDirection = FadeUp; // Direction of fade
int brightness = 0;    // LED Brightness
int fadeAmount = 1;    // Fade speed

/* Initialise Components */
void setup() {
  pinMode(pushButton, INPUT);
}

void loop() {

  /* Update button states */
  butState = digitalRead(pushButton);
  /* Button is released */
  if(butDown && butState == LOW){butDown = false;}
  /* Simple button press for mode momentary switch */
  if(!butDown && butState == HIGH){ 
    mode++; // Change mode
    if (mode > 1){ // Curerntly only two modes
      mode = 0;
    }
    butDown = true; // Only change once on push down
  }

  
  /* Update analog in value */
  potInput = analogRead(potPin);

  /* Mode switch */
  switch(mode){
    case 0: // Brightness relative to potentiometer value
      brightness = map(potInput, 0, 1023, 0, 255); // Map brightness to pot value
      break;
      
    case 1: // Fade speed relative to potentiometer value
      fadeAmount =  map(potInput, 0, 1023, 0,255); // Map fade speed to pot value
      /* Calculate brightness */
      switch(fadeDirection){
        case FadeUp: // Fading up
          brightness = brightness + fadeAmount;
          if(brightness >= 255) {brightness = 255; fadeDirection = FadeDown;} // Max brightness - change direction
          break;
        case FadeDown:// Fading down
          brightness = brightness - fadeAmount;
          if(brightness <= 0) {brightness = 0; fadeDirection = FadeUp;}  // Min brightness - change direction
          break;
      }
      break;
  }
  
  /* Set brightness */
  analogWrite(ledPin, brightness);
   /* Debounce 30ms */
   delay (30);
}
