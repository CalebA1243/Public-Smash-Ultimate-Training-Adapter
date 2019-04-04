#define frameLength 16
#define maxInputFrames 60

#include "Nintendo.h"
CGamecubeController controller(2); //sets RX0 on arduino to read data from controller
CGamecubeConsole console(3);       //sets TX1 on arduino to write data to console
Gamecube_Report_t gcc;             //structure for controller state
Gamecube_Data_t data;
Gamecube_Report_t* inputs;
struct{int8_t ax, ay, cx, cy; uint8_t l, r;}ini;
bool pressEscOption = 0, diRight = 0, sdi = 0, inputRecording = 0, recording = 0, playingRec = 0;
struct{bool dleft = 0, ddown = 0, dup = 0, dright = 0, start = 0, modeReset = 0; uint32_t frame = 0;}previous;
struct{uint32_t di = 0, esc = 0, record = 0;}msValues;
struct{int8_t xDI, yDI, xEsc, yEsc;}coord;
uint16_t diMode = 0, escMode = 0, recFrameNum = 0;
uint32_t currentTime = 0, currentFrame;

void trainMods(){
  modeSelect();

  switch (diMode) {
    case 1:
    leftRightDI();
    break;
    case 2:
    randomDI();
    break;
    case 3:
    setDI();
    break;
    case 4:
    setSDI();
    break;
  }

  if (escMode != 0) escOptionToggle();
  
  switch (escMode) {
    case 1: airDodge();
    break;
    case 2: jump();
    break;
    case 3: special();
    break;
    case 4: aerial();
    break; 
  }

  if (recording) recordInput();

  if (playingRec) playRecording();
}

void modeSelect() {
  if (!previous.dleft && gcc.dleft) {
    if (diMode < 4)
      diMode++;
    else
     diMode = 1;
     
    if (diMode == 3 || diMode == 4) assignDIDirection();
  }
  previous.dleft = gcc.dleft;
  
  if (!previous.dright && gcc.dright) {
    if (escMode < 4)
      escMode++;
    else
     escMode = 1;

    if (escMode == 3 || escMode == 4) assignEscDirection();
  }
  previous.dright = gcc.dright;

  if (!previous.ddown && gcc.ddown) {
    if (recording) endRecording();
    else recordSetup();
  }

  previous.ddown = gcc.ddown;

  if (!previous.dup && gcc.dup) {
    playingRec = 1;
  }

  previous.dup = gcc.dup;
  
  if (!previous.start && gcc.start) {
    if (diMode != 0 || escMode != 0 || recording || playingRec) {
      diMode = 0;
      escMode = 0;
      endRecording();
      endPlayback();
      previous.modeReset = 1;
    }
  }
  
//This is to make it so that start is only registered if you're not pressing it to reset modes
  if (previous.modeReset && previous.start && !gcc.start) previous.modeReset = 0;

  previous.start = gcc.start;
  
  gcc.start = gcc.start && !previous.modeReset; 
  gcc.dleft = 0;
  gcc.dright = 0;
  gcc.ddown = 0;
  gcc.dup = 0;

  
}

//void leftRightDI() {
//  currentTime = millis();
//  if (currentTime - msValues.di > frameLength) {
//    diRight = !diRight;
//    msValues.di = currentTime;
//  }
//
//  if (diRight) gcc.xAxis = 255;
//  else gcc.xAxis = -255;
//}

void leftRightDI() {
  currentFrame = micros() / 16667;
  if (currentFrame > previous.frame) {
    diRight = !diRight;
    previous.frame = currentFrame; 
  }

  if (diRight) gcc.xAxis = 255;
  else gcc.xAxis = -255;
}

void randomDI() {
  currentTime = millis();
  if (currentTime - msValues.di > frameLength) {
    gcc.xAxis = random(-255, 255);
    gcc.yAxis = random (-255, 255);
    msValues.di = currentTime;
  }
}

void setDI() {
  gcc.xAxis = coord.xDI; 
  gcc.yAxis = coord.yDI; 
}

void setSDI() {
  currentTime = millis();
  if (currentTime - msValues.di > frameLength) {
    sdi = !sdi;
    msValues.di = currentTime;
  }
  if (sdi) {
    gcc.xAxis = coord.xDI;
    gcc.yAxis = coord.yDI;
  }
}

void assignDIDirection() {
  coord.xDI = gcc.xAxis; 
  coord.yDI = gcc.yAxis; 
}

void assignEscDirection() {
  coord.xEsc = gcc.xAxis;
  coord.yEsc = gcc.yAxis;
}

void escOptionToggle() {
  currentTime = millis();
  if (currentTime - msValues.esc > frameLength) {
    pressEscOption = !pressEscOption;
    msValues.esc = currentTime;
  }
}

//void escOptionToggle() {
//  currentTime = micros();
//  currentFrame = currentTime / 16667;
//  if (currentFrame > previous.frame) {
//    pressEscOption = !pressEscOption;
//    previous.frame = currentFrame; 
//  }
//}

void airDodge() {
  if (pressEscOption) {
    gcc.right = 255;
  }
}

void jump() {
  if (pressEscOption) gcc.y = 1;
}

void special() {
  if (pressEscOption) {
    gcc.b = 1;
    gcc.xAxis = coord.xEsc;
    gcc.yAxis = coord.yEsc;
  }
}

void aerial() {
  if (pressEscOption) {
    gcc.a = 1;
    gcc.xAxis = coord.xEsc;
    gcc.yAxis = coord.yEsc;
  }
}

void recordSetup() {
  delete[] inputs;
  inputs = new Gamecube_Report_t[maxInputFrames];
  recording = 1;
}

void recordInput() {
  currentTime = millis();
  if (currentTime - msValues.record > frameLength) {
    inputs[recFrameNum] = gcc;
    msValues.record = currentTime;
    recFrameNum++;
  }

  if (recFrameNum >= maxInputFrames - 1) endRecording();
}

void endRecording() {
  recording = 0;
  recFrameNum = 0;
  msValues.record = 0;
}

void playRecording() {
  currentTime = millis();
  if (currentTime - msValues.record > frameLength) {
    gcc = inputs[recFrameNum];
    msValues.record = currentTime;
    recFrameNum++;
  }

  if (recFrameNum >= maxInputFrames) recFrameNum = 0;
}

void endPlayback() {
  playingRec = 0;
  recFrameNum = 0;
}

void setup(){
}

void loop(){
  controller.read(); //reads the controller
  data = defaultGamecubeData;               //this line is necessary for proper rumble
  gcc = controller.getReport();             //gets a report of the controller read
  trainMods();                          //implements all the mods (remove this line to unmod the controller)
  data.report = gcc; console.write(data);   //sends controller data to the console
  controller.setRumble(data.status.rumble); //allows for rumble
}
