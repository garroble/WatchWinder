#include "arduino_secrets.h"
/* 
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/d858aebb-4e63-4bf6-a066-30cd49a4e8f1 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  CloudCounter cl_winder_turns;
  CloudSchedule ctrl_start_time;
  bool cl_winder_status;
  bool ctrl_switch;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"
#include <AccelStepper.h>
#include <TimeLib.h>

#define WATCH_TURNS_PER_DAY 750

#define HALFSTEP  8
#define MOTOR_MAX_STEPS 4108
#define MOTOR_MAX_SPEED 5000.0
#define MOTOR_ACC 1000.0
#define motorPin1 7
#define motorPin2 8
#define motorPin3 9
#define motorPin4 10

/*
  Global variables
*/
bool led_blink = false;
bool winder_switch = false;
int motor_speed = 1000;
int motor_target = -MOTOR_MAX_STEPS;
const long led_interval = 500;         // Blink LED interval while scheduler on
long step_interval = 40000;            // Motor rotation interval
unsigned long led_prev_millis = 0;     // LED previous millis
unsigned long step_prev_millis = 0;    // Motor rotation previous millis
// NOTE: The sequence 1-3-2-4 is required for proper sequencing of 28BYJ-48
AccelStepper winder(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

/*
  Arduino Setup
*/
void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime <= 5000); ) { }

  Serial.println("\n\n");
  Serial.println("Watch Winder FW V1.0 Cloud");
  Serial.println("--------------------------");

  // Internal LEDs
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);

  // Initialize stepper.
  winder.setMaxSpeed(MOTOR_MAX_SPEED);
  winder.setAcceleration(MOTOR_ACC);
  winder.setSpeed(motor_speed);
  winder.moveTo(motor_target);
  
  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  // Initialize cloud variables
  cl_winder_status = false;
  cl_winder_turns = 0;

  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::CONNECT, doThisOnConnect);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::SYNC, doThisOnSync);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::DISCONNECT, doThisOnDisconnect);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

/*
  Arduino Loop
*/
void loop() {
  unsigned long currentMillis = millis();
  ArduinoCloud.update();

  /* Operation controlled/scheduled from Cloud */
  if (ctrl_start_time.isActive() or winder_switch) {
    cl_winder_status = true;

    // LED blink for debug
    if (currentMillis - led_prev_millis >= led_interval) {
      led_blink = !led_blink;
      led_prev_millis = currentMillis;
      digitalWrite(LEDB, (PinStatus) led_blink);
    }

    // Motor control for winder
    if (currentMillis - step_prev_millis >= step_interval) {
      step_prev_millis = currentMillis;
      if (winder.distanceToGo() == 0) {
        winder.enableOutputs();
        winder.setCurrentPosition(0);
        winder.moveTo(motor_target);
      }
      cl_winder_turns = cl_winder_turns++;
      Serial.print("Winder revolutions: ");
      Serial.println(cl_winder_turns);
    }
    if (winder.distanceToGo() != 0) {
      winder.run();  
    }
    else {
      winder.disableOutputs();
    }
  }
  else {
    cl_winder_status = false;
    cl_winder_turns = 0;
    digitalWrite(LEDB, LOW);
  }
}

/* Callback for connecting to Arduino Cloud */
void doThisOnConnect(){
  Serial.println("Board successfully connected to Arduino IoT Cloud");
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
}

/* Callback for sync with Arduino Cloud */
void doThisOnSync(){
  /* add your custom code here */
  Serial.println("Thing Properties synchronised");
}

/* Callback for disconnecting from Arduino Cloud */
void doThisOnDisconnect(){
  /* add your custom code here */
  Serial.println("Board disconnected from Arduino IoT Cloud");
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, LOW);
}

/*
  Since CtrlSwitch is READ_WRITE variable, onCtrlSwitchChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onCtrlSwitchChange()  {
  // Add your code here to act upon CtrlSwitch change
  Serial.println("General switch change");
  winder_switch = ctrl_switch;
}

/*
  Since CtrlStartTime is READ_WRITE variable, onCtrlStartTimeChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onCtrlStartTimeChange()  {
  // Add your code here to act upon CtrlStartTime change
  int program_len = ctrl_start_time.getCloudValue().len;
  step_interval = long((ctrl_start_time.getCloudValue().len*1000)/(WATCH_TURNS_PER_DAY+1));
  if (step_interval < 10000) {
    step_interval = 10000;
  }
  Serial.print("New program received. Duration: ");
  Serial.print(program_len);
  Serial.print("s. Interval: ");
  Serial.print(step_interval);
  Serial.println("ms.");
}

