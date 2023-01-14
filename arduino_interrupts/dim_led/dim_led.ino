// Control pin 11 LED brightness with brightness variable
// Brightness range: 0 (dim) - 127 (max)
// Cycle period: ~100Hz

char brightness = 100;
char count = 0;
bool state = 0;

void setup() {
  DDRB |= (1 << PORTB3);
  cli(); // stop interrupts

  // set timer0 interrupt at 12.8KHz
  TCCR0A = 0b00000010;
  TCCR0B = 0b00000010; // 8 prescaler
  TCNT0 = 0; // set timer to 0
  OCR0A = 155;
  TIMSK0 |= (1 << OCIE0A); // enable timer compare interrupt

  sei(); // resume interrupts
}

ISR(TIMER0_COMPA_vect) {
  if(count <= brightness && !state) {
    // turn on
    PORTB |= (1 << PORTB3);
    state = 1;
  } else if(count > brightness && state) {
    // turn off
    PORTB &= ~(1 << PORTB3);
    state = 0;
  }
  count = (count + 1) % 128;
}

void loop() {

}