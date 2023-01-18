// Control pin 11 LED brightness with brightness variable
// Modified ISR to reduce flicker for values below 50%
// Brightness range: 0 (off) - 255 (max)
// Cycle period: ~120Hz

unsigned char brightness = 10;
unsigned char count = 1;
unsigned char ticks = 0;

void setup() {
  DDRB |= (1 << PORTB3);
  cli(); // stop interrupts

  // set timer0 interrupt at ~30.7KHz
  TCCR0A = 0b00000010;
  TCCR0B = 0b00000010; // 8 prescaler
  TCNT0 = 0; // set timer to 0
  OCR0A = 64;
  TIMSK0 |= (1 << OCIE0A); // enable timer0 compare interrupt

  sei(); // resume interrupts
}

ISR(TIMER0_COMPA_vect) {
  if((ticks * 255) / count < brightness) {
    PORTB |= (1 << PORTB3);
    ticks++;
  } else PORTB &= ~(1 << PORTB3);

  if(count == 255) {
    count = 1;
    ticks = 0;
  } else count++;
}

void loop() {

}