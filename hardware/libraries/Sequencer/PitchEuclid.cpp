#include <Platform.h>
#include "PitchEuclid.h"


const scale_t *PitchEuclid::scales[] = {
  &ionianScale,
  &aeolianScale,

  &harmonicMinorScale,
  &melodicMinorScale,
  &lydianDominantScale,

  &wholeToneScale,
  &wholeHalfStepScale,
  &halfWholeStepScale,

  &bluesScale,
  &majorPentatonicScale,
  &minorPentatonicScale,
  &suspendedPentatonicScale,
  &inSenScale,

  &majorBebopScale,
  &dominantBebopScale,
  &minorBebopScale,

  &majorArp,
  &minorArp,
  &majorMaj7Arp,
  &majorMin7Arp,
  &minorMin7Arp,
};

PitchEuclid::PitchEuclid(scale_t *scale) : track(3, 8, 0) {
  if (scale) {
    currentScale = scale;
  } else {
    currentScale = (scale_t *)scales[0];
  }
  octaves = 0;
  noteLength = 1;
	
  pitches_len = 0;
  pitches_idx = 0;
  setPitchLength(4);
  mdTrack = 0;
  muted = false;
  octaves = 0;
}

void PitchEuclid::setup() {
  MidiClock.addOn16Callback(this, (midi_clock_callback_ptr_t)&PitchEuclid::on16Callback);
}

void PitchEuclid::setPitchLength(uint8_t len)  {
  pitches_len = len;
  randomizePitches();
}

void PitchEuclid::randomizePitches()  {
  for (uint8_t i = 0; i < pitches_len; i++) {
    pitches[i] = randomScalePitch(currentScale, octaves);
  }
}

void PitchEuclid::on16Callback(uint32_t counter)  {
  static uint8_t lastPitch = 255;
  static uint8_t lastLength = 0;

  if (lastLength > 0) {
    lastLength--;
  }
  
  if (lastPitch <= 127) {
    if ((noteLength == 0) || (lastLength == 0)) {
      MidiUart.sendNoteOff(mdTrack, lastPitch, 0);
      lastPitch = 255;
    }
  }

  /* if noteLength == 0 then the pitch euclid is disabled */
  if (noteLength == 0) {
    return;
  }
 	
  if (track.isHit(MidiClock.div16th_counter)) {
    uint8_t pitch = basePitch + pitches[pitches_idx];
    if (lastPitch <= 127) {
      MidiUart.sendNoteOff(mdTrack, lastPitch, 0);
      lastPitch = 255;
    }

    if (pitch <= 127) {
      if (!muted) {
	MidiUart.sendNoteOn(mdTrack, pitch, 100);
	lastLength = noteLength;
	lastPitch = pitch;
      }
    }
    pitches_idx = (pitches_idx + 1) % pitches_len;
  }
}
