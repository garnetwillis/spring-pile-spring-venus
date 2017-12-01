// constants
const int STATE_RESETTING = 0;
const int STATE_INIT = 1;
const int STATE_MOVING_UP = 2;
const int STATE_PAUSED_UP = 3;
const int STATE_MOVING_DOWN = 4;
const int STATE_PAUSED_DOWN = 5;

const int ON_OFF_PIN = 10;
const int POLARITY_PIN = 11;

const int RESET_DURATION = 20 * 1000;
const int INIT_DURATION = 20 * 1000;

const int MIN_PAUSE_DURATION = 5 * 1000;
const int MAX_PAUSE_DURATION = 10 * 1000;

const int MIN_MOVING_UP_DURATION = 5 * 1000;
const int MAX_MOVING_UP_DURATION = 10 * 1000;
const int MOVING_DOWN_DURATION_ADDITION = 1000;


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

  // set initial state
  setState(STATE_RESETTING);
}

void loop() {
  switch (currentState) {
    case STATE_RESETTING:
      reset();
      break;

    case STATE_INIT:
      prepare();
      break;

    case STATE_MOVING_UP:
      moveUp();
      break;

    case STATE_PAUSED_UP:
      pause();
      break;

    case STATE_MOVING_DOWN:
      moveDown();
      break;

    case STATE_PAUSED_DOWN:
      pause();
      break;
  }

  gotoNextState();
}


// method definitions
void reset() {
  // set to move cylinder down
  digitalWrite(ON_OFF_PIN, LOW);
  digitalWrite(POLARITY_PIN, LOW);

  // stay at this setting long enough to
  // ensure cylinder goes all the way down
  delay(RESET_DURATION);
}

void prepare() {
  // remain inactive for a short while
  delay(INIT_DURATION);
}

void moveUp() {
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
}

void moveDown() {
  // set pin for actuator to move cylinder downwards
  digitalWrite(POLARITY_PIN, HIGH);

  // delay shortly to ensure polarity is switched
  // before powering actuator
  delay(10);

  // power actuator
  digitalWrite(ON_OFF_PIN, HIGH);

  // get new random duration
  setMovingDownDuration();
  delay(movingDownDuration);
}

void pause() {
  // disable pin so actuator is off
  digitalWrite(ON_OFF_PIN, LOW);

  // get new random duration
  setPauseDuration();
  delay(pauseDuration);
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
      setState(STATE_INIT);
      break;

    case STATE_INIT:
      setState(STATE_MOVING_UP);
      break;

    case STATE_MOVING_UP:
      setState(STATE_PAUSED_UP);
      break;

    case STATE_PAUSED_UP:
      setState(STATE_MOVING_DOWN);
      break;

    case STATE_MOVING_DOWN:
      setState(STATE_PAUSED_DOWN);
      break;

    case STATE_PAUSED_DOWN:
      setState(STATE_MOVING_UP);
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