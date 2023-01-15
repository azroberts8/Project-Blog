// Control pin 11 LED brightness with brightness variable
// Modified ISR to reduce flicker for values below 50%
// Brightness range: 0 (dim) - 100 (max)
// Cycle period: ~100Hz

char brightness = 100;
char count = 1;
char ticks = 0;

void setup() {
  DDRB |= (1 << PORTB3);
  cli(); // stop interrupts

  // set timer0 interrupt at 12.8KHz
  TCCR0A = 0b00000010;
  TCCR0B = 0b00000010; // 8 prescaler
  TCNT0 = 0; // set timer to 0
  OCR0A = 157;
  TIMSK0 |= (1 << OCIE0A); // enable timer compare interrupt

  sei(); // resume interrupts
}

ISR(TIMER0_COMPA_vect) {
  if(count == 100) {
    count = 0;
    ticks = 0;
  }

  if((ticks * 100) / count < brightness) {
    PORTB |= (1 << PORTB3);
    ticks++;
  } else {
    PORTB &= ~(1 << PORTB3);
  }
  count++;
}

void loop() {

}