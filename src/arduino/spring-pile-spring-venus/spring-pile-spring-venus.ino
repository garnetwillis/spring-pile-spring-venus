// constants
constant int STATE_RESETTING = 0;
constant int STATE_INIT = 1;
constant int STATE_MOVING_UP = 2;
constant int STATE_PAUSED_UP = 3;
constant int STATE_MOVING_DOWN = 4;
constant int STATE_PAUSED_DOWN = 5;

// TODO: add pin constants

// TODO: set proper durations

constant int RESET_DURATION = 30 * 1000;
constant int INIT_DURATION = 20 * 1000;

constant int MIN_PAUSE_DURATION = 10;
constant int MAX_PAUSE_DURATION = 100;

constant int MIN_MOVING_UP_DURATION = 10;
constant int MAX_MOVING_UP_DURATION = 100;
constant int MOVING_DOWN_DURATION_ADDITION = 100;


// vars
int previousState;
int currentState;
int pauseDuration = 0;
int movingUpDuration = 0;
int movingDownDuration = 0;


// arduino methods
void setup() {
  reset();
}

void loop() {
  // TODO?
}


// method definitions
void reset() {
  setState(STATE_RESETTING);

  // TODO: move peg down for a few seconds to ensure all is set
  // - send MOTOR_OUT_PIN value(s) to lower peg
  // - wait/while RESET_DURATION lasts
  // - trigger next state

  delay(RESET_DURATION);
  gotoNextState();
}

void init() {
  setState(STATE_INIT);
  delay(INIT_DURATION);
  gotoNextState();
}

void moveUp() {
  setState(STATE_MOVING_UP);

  // TODO: set pin(s) to move peg up

  setMovingDownDuration();
  delay(movingUpDuration);
  gotoNextState();
}

void moveDown() {
  setState(STATE_MOVING_UP);

  // TODO: set pin(s) to move peg down

  setMovingDownDuration();
  delay(movingDownDuration);
  gotoNextState();
}

void pause() {
  switch (previousState) {
    case STATE_MOVING_UP:
      setState(STATE_PAUSED_UP);
      break;

    case STATE_MOVING_DOWN:
      setState(STATE_PAUSED_DOWN);
      break;
  }

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