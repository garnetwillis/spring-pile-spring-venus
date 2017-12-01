// constants
constant int STATE_RESETTING = 0;
constant int STATE_INIT = 1;
constant int STATE_MOVING_UP = 2;
constant int STATE_PAUSED_UP = 3;
constant int STATE_MOVING_DOWN = 4;
constant int STATE_PAUSED_DOWN = 5;

constant int ON_OFF_PIN = 10;
constant int POLARITY_PIN = 11;

constant int RESET_DURATION = 20 * 1000;
constant int INIT_DURATION = 20 * 1000;

constant int MIN_PAUSE_DURATION = 5 * 1000;
constant int MAX_PAUSE_DURATION = 10 * 1000;

constant int MIN_MOVING_UP_DURATION = 5 * 1000;
constant int MAX_MOVING_UP_DURATION = 10 * 1000;
constant int MOVING_DOWN_DURATION_ADDITION = 1000;


// vars
int previousState = -1;
int currentState = -1;
int pauseDuration = 0;
int movingUpDuration = 0;
int movingDownDuration = 0;


// arduino methods
void setup() {
  // set pins
  pinMode(ON_OFF_PIN, OUTPUT);
  pinMode(POLARITY_PIN, OUTPUT);

  // start the process
  reset();
}

void loop() {
  // TODO?
}


// method definitions
void reset() {
  setState(STATE_RESETTING);

  // set to move cylinder down
  digitalWrite(ON_OFF_PIN, LOW);
  digitalWrite(POLARITY_PIN, LOW);

  // stay at this setting long enough to
  // ensure cylinder goes all the way down
  delay(RESET_DURATION);

  gotoNextState();
}

void init() {
  setState(STATE_INIT);

  // remain inactive for a short while
  delay(INIT_DURATION);

  gotoNextState();
}

void moveUp() {
  setState(STATE_MOVING_UP);

  // set pin for actuator to move cylinder upwards
  digitalWrite(POLARITY_PIN, LOW);

  // delay shortly to ensure polarity is switched
  // before powering actuator
  delay(10);

  // power actuator
  digitalWrite(ON_OFF_PIN, HIGH);

  // get new random duration
  setMovingUpDuration();
  delay(movingUpDuration);

  gotoNextState();
}

void moveDown() {
  setState(STATE_MOVING_UP);

  // set pin for actuator to move cylinder downwards
  digitalWrite(POLARITY_PIN, HIGH);

  // delay shortly to ensure polarity is switched
  // before powering actuator
  delay(10);

  // power actuator
  digitalWrite(ON_OFF_PIN, LOW);

  // get new random duration
  setMovingDownDuration();
  delay(movingDownDuration);

  gotoNextState();
}

void pause() {
  // set new state according to the previous state
  switch (previousState) {
    case STATE_MOVING_UP:
      setState(STATE_PAUSED_UP);
      break;

    case STATE_MOVING_DOWN:
      setState(STATE_PAUSED_DOWN);
      break;
  }

  // disable pin so actuator is off
  digitalWrite(ON_OFF_PIN, LOW);

  // get new random duration
  setPauseDuration();
  delay(pauseDuration);

  gotoNextState();
}


// state methods
void setState(int newState) {
  // quick exit if same state
  if (newState == currentState) {
    return;
  }

  // set new state
  previousState = currentState;
  currentState = newState;
}

void gotoNextState() {
  switch (currentState) {
    case STATE_RESETTING:
    case STATE_INIT:
    case STATE_MOVING_UP:
    case STATE_PAUSED_UP:
    case STATE_MOVING_DOWN:
      currentState++;
      break;

    case STATE_PAUSED_DOWN:
      currentState = STATE_MOVING_UP;
      break;
  }
}


// duration calculation methods
void setPauseDuration() {
  pauseDuration = random(MIN_PAUSE_DURATION, MAX_PAUSE_DURATION);
}

void setMovingUpDuration() {
  movingUpDuration = random(MIN_MOVING_UP_DURATION, MAX_MOVING_UP_DURATION);
}

void setMovingDownDuration() {
  movingDownDuration = movingUpDuration + MOVING_DOWN_DURATION_ADDITION;
}