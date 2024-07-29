/*
 * Track Rotation Test
 * Tell motor to spin at arbitrary speed 
 * until given number of pulses is counted,
 * indicating wheel has reached target location.
 * Target distance originally converted to pulses, 
 * so desired pulses is known.
 */

//======Encoders======
const byte esPin = 3;

int ppr = 20;

volatile long pulseCount = 0; 

double ppcm = 0.0;

//======Motor Driver======
const byte mEnablePin = 6,
  mSigPin = 7;

//======Mobile Platform======
double wheelDiam = 6.0;

//======Circle======
double piApprox = 3.14159,
  rads = 57.2958; // radians to deg conversion
  
void setup() 
{
  Serial.begin(9600);
  
  while(!Serial);

  displayMessage("Starting TrackRotationTest.ino");

  initMotionControl();

  Serial.print("pulseCount before driving: ");
  Serial.println(pulseCount);

  driveDistance(20.0);
}

void displayMessage(String msg)
{
  Serial.print("Message: '");
  Serial.print(msg);
  Serial.println("'\n");
}

// Construct a motion controller
void initMotionControl()
{
  startEncoderChannel();
  
  startMotorChannel();

  stopMoving();

  displayMessage("Motion control initialized.");
}

void startEncoderChannel()
{
  attachInterrupt(digitalPinToInterrupt(esPin), updateEsVel, CHANGE);

  displayMessage("Encoder channel started.");
}

void startMotorChannel()
{
  /* Start Motor Channel */
  pinMode(mEnablePin, OUTPUT);
  pinMode(mSigPin, OUTPUT);

  displayMessage("Motor channel started.");
}

void stopMoving()
{
  digitalWrite(mEnablePin, LOW);

  displayMessage("Stopped moving.");
}

void driveDistance(double targetDistance)
{
  long mPulses;
  
  pulseCount = 0;

  mPulses = (long) convertCmToPulses(targetDistance);

  Serial.print("Pulses: ");
  Serial.println(mPulses);

  Serial.print("pulseCount before while loop: ");
  Serial.println(pulseCount);
  
  while(pulseCount < mPulses)
  {
    Serial.print("pulseCount in while loop: ");
    Serial.println(pulseCount);
    
    analogWrite(mEnablePin, 100); // CHANGE: optimize velocity

    //delay(3000);
  }

  stopMoving();
}

double convertCmToPulses(double cm)
{
  double wheelCircumf,
    ratioPulsesPerCm,
    pulses;

  wheelCircumf = piApprox * wheelDiam;

  Serial.print("wheelCircumf: ");
  Serial.println(wheelCircumf);

  ratioPulsesPerCm = ppr / wheelCircumf;

  Serial.print("ratioPulsesPerCm: ");
  Serial.println(ratioPulsesPerCm);

  pulses = cm * ratioPulsesPerCm;

  Serial.print("Pulses Returned: ");
  Serial.println(pulses);

  return pulses;
}

void loop() {}

//======Interrupt Service Routines======

void updateEsVel()
{
  pulseCount++;
}
