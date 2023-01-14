// Blink an LED once per second using timer interrupt
// Using timer 1 because it stores 16 bit values

boolean toggle = 0;

void setup() {
  pinMode(11, OUTPUT);

  cli(); // stop interrupts

  // set timer1 interrupt at 1Hz
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0; // initialize counter value to 0

  // set compare match register for 2Hz increments
  OCR1A = 7811; // 16,000,000 / (1024 * 2) - 1
 
  TCCR1B |= (1 << WGM12); // turn on CTC mode

  TCCR1B |= (1 << CS12) | (1 << CS10); // set CS10 and CS12 bits for 1024 prescaler

  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt

  sei(); // allow interrupts
}

// timer1 interrupt 1Hz toggle LED 9
ISR(TIMER1_COMPA_vect) {
  if(toggle) {
    digitalWrite(11, HIGH);
    toggle = 0;
  } else {
    digitalWrite(11, LOW);
    toggle = 1;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
