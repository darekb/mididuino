#include <MidiClockPage.h>
#include <PitchEuclid.h>
#include <MNMWesenLivePatchSketch.h>

MNMWesenLivePatchSketch sketch;

void setup() {
  //  enableProfiling();
  initMNMTask();
  
  sketch.setup();
  GUI.setSketch(&sketch);
  
  initClockPage();
}

void loop() {
}
