//https://wokwi.com/projects/458222820217118721
void setup() {

  DDRC = (1 << 6)-1;
  DDRB = ((1 << 5)-1) ^ 1;
  
  PORTB = 18;
  PORTC = 33;
  
  TCCR1A = 0;
  TCCR1B = 13;
  
  TIMSK1 = 2;
  TIMSK0 = 0; // built in arduino interrupts disabled
  OCR1A = 31249;
}

enum Phases {VERTICAL_GREEN, VERTICAL_BLINKING, VERTICAL_YELLOW_TORED, VERTICAL_RED,
             HORIZONTAL_YELLOW_TOGREEN, HORIZONTAL_GREEN, HORIZONTAL_BLINKING, HORIZONTAL_YELLOW_TORED, HORIZONTAL_RED,
             VERTICAL_YELLOW_TOGREEN};
volatile int counter = 0;

Phases phase = VERTICAL_BLINKING;

ISR(TIMER1_COMPA_vect){
  TCNT1 = 0;
  switch(phase){
    case VERTICAL_GREEN:
      OCR1A = 31249;
      PORTC ^= (1 << 3) | (1 << 4) | (1 << 5); // vertical red/yellow off green on
      PORTB ^= (1 << 1) | (1 << 3); // vertical ped red off, green on
      phase = VERTICAL_BLINKING;
      break;
    case VERTICAL_BLINKING:  
      if (counter < 7){
        OCR1A = 11718;
        PORTB ^= (1 << 1); // vertical ped green on/off, ends with off
        counter++;
      }
      else{
        phase = VERTICAL_YELLOW_TORED;
        PORTB ^= (1 << 3); // vertical ped red on
        OCR1A = 1;
        counter = 0;
      }
      break;
    case VERTICAL_YELLOW_TORED:
      
      PORTC ^= (1 << 4) | (1 << 5); // vertical yellow on, green off
      OCR1A = 7811;
      phase = VERTICAL_RED;
      break;
    case VERTICAL_RED:
      OCR1A = 32768;
      PORTC ^= (1 << 4) | (1 << 3); // vertical yello off, red on 
      phase = HORIZONTAL_YELLOW_TOGREEN;
      break;
    case HORIZONTAL_YELLOW_TOGREEN: // 2 is always the one where it becomes green
      OCR1A = 8192;
      PORTC ^= (1 << 1); // horizontal yellow on
      phase = HORIZONTAL_GREEN;
      break;
    case HORIZONTAL_GREEN:
      OCR1A = 31249;
      PORTC ^= 1 | (1 << 1) | (1 << 2); // horizontal red/yello off, green on
      PORTB ^= (1 << 4) | (1 << 2); // horizontal ped red off, green on

      phase = HORIZONTAL_BLINKING;
      break;
    case HORIZONTAL_BLINKING:
      if (counter < 7){
        OCR1A = 11718;
        PORTB ^= (1 << 2);
        counter++;
      }
      else{
        PORTB ^= (1 << 4); // horizontal ped red on
        OCR1A = 1;
        counter = 0;
        phase = HORIZONTAL_YELLOW_TORED;
      }
        
      break;
    case HORIZONTAL_YELLOW_TORED:
      PORTC ^= (1 << 2) | (1 << 1); // horizontal green off, yellow on 
      OCR1A = 7811;
      phase = HORIZONTAL_RED;
      break;
    case HORIZONTAL_RED:
      OCR1A = 31249;
      PORTC ^= (1 << 1) | 1; // horizontal yellow off, red on
      phase = VERTICAL_YELLOW_2;
      break;
    case VERTICAL_YELLOW_TOGREEN:
      OCR1A = 7811;
      PORTC ^= (1 << 4); // vertical yellow on
      phase = VERTICAL_GREEN;
      break;
  }
    

}
void loop(){}
