#include "MD.h"
#include "MDRandomizer.h"

const uint32_t MDRandomizerClass::paramSelectMask[13]  = {
    // SELECT_FILTER
    _BV32(MODEL_FLTF) | _BV32(MODEL_FLTW) | _BV32(MODEL_FLTQ), 

    // SELECT_AMD
    _BV32(MODEL_AMD) | _BV32(MODEL_AMF), 

    // SELECT_EQ
    _BV32(MODEL_EQF) | _BV32(MODEL_EQG), 

    // SELECT_EFFECT
    _BV32(MODEL_AMD) | _BV32(MODEL_AMF) | _BV32(MODEL_EQF) | _BV32(MODEL_EQG) |
      _BV32(MODEL_FLTF) | _BV32(MODEL_FLTW) | _BV32(MODEL_FLTQ), 

    // SELECT_LOWSYN
    _BV32(MODEL_P5) | _BV32(MODEL_P6) | _BV32(MODEL_P7) | _BV32(MODEL_P8),

    // SELECT_UPSYN
    _BV32(MODEL_P2) | _BV32(MODEL_P3) | _BV32(MODEL_P4),

    // SELECT_SYN
    _BV32(MODEL_P2) | _BV32(MODEL_P3) | _BV32(MODEL_P4) | _BV32(MODEL_P5) |
      _BV32(MODEL_P6) | _BV32(MODEL_P7) | _BV32(MODEL_P8),

    // SELECT_LFO  
    _BV32(MODEL_LFOS) | _BV32(MODEL_LFOD) | _BV32(MODEL_LFOM),

    // SELECT_SENDS
    _BV32(MODEL_DEL) | _BV32(MODEL_REV),

    // SELECT_DIST
    _BV32(MODEL_SRR) | _BV32(MODEL_DIST),

    // SELECT FX_LOWSYN
    _BV32(MODEL_AMD) | _BV32(MODEL_AMF) | _BV32(MODEL_EQF) | _BV32(MODEL_EQG) |
      _BV32(MODEL_FLTF) | _BV32(MODEL_FLTW) | _BV32(MODEL_FLTQ) |
      _BV32(MODEL_P5) | _BV32(MODEL_P6) | _BV32(MODEL_P7) | _BV32(MODEL_P8),

    // SELECT_FX_SYN
    _BV32(MODEL_AMD) | _BV32(MODEL_AMF) | _BV32(MODEL_EQF) | _BV32(MODEL_EQG) |
      _BV32(MODEL_FLTF) | _BV32(MODEL_FLTW) | _BV32(MODEL_FLTQ) |
      _BV32(MODEL_P2) | _BV32(MODEL_P3) | _BV32(MODEL_P4) |
      _BV32(MODEL_P5) | _BV32(MODEL_P6) | _BV32(MODEL_P7) | _BV32(MODEL_P8),

    // SELECT_ALL
    _BV32(MODEL_AMD) | _BV32(MODEL_AMF) | _BV32(MODEL_EQF) | _BV32(MODEL_EQG) |
      _BV32(MODEL_FLTF) | _BV32(MODEL_FLTW) | _BV32(MODEL_FLTQ) | _BV32(MODEL_SRR) |
      _BV32(MODEL_P1) | _BV32(MODEL_P2) | _BV32(MODEL_P3) | _BV32(MODEL_P4) |
      _BV32(MODEL_P5) | _BV32(MODEL_P6) | _BV32(MODEL_P7) | _BV32(MODEL_P8) |
      _BV32(MODEL_DIST) | _BV32(MODEL_VOL) | _BV32(MODEL_PAN) | _BV32(MODEL_DEL) |
      _BV32(MODEL_REV) | _BV32(MODEL_LFOS) | _BV32(MODEL_LFOD) | _BV32(MODEL_LFOM)
};

const char *MDRandomizerClass::selectNames[13] = {
    "FILTER",
    "AMD   ",
    "EQ    ",
    "EFFECT",
    "LOWSYN",
    "UP SYN",
    "SYN   ",
    "LFO   ",
    "SENDS ",
    "DIST  ",
    "FX LOW",
    "FX SYN",
    "ALL   "
  };

void MDRandomizerClass::setTrack(uint8_t _track) {
    track = _track;
    undoStack.reset();
  }

void MDRandomizerClass::randomize(int amount, uint8_t mask) {
    uint32_t trackMask = paramSelectMask[mask];

    if (amount == 0)
      return;
    undoStack.push(&MD.kit.machines[track].params);

    for (uint8_t i = 0; i < 24; i++) {
      if (IS_BIT_SET32(trackMask, i)) {
        int param = MD.kit.machines[track].params[i];
        int rand = random(-amount, amount);
        param += rand;
        if (param > 127) 
          param = 127;
        if (param < 0)
          param = 0;
        MD.kit.machines[track].params[i] = param;
        MD.setTrackParam(track, i, param);
      }
    }
  }

void MDRandomizerClass::undo() {
    GUI.setLine(GUI.LINE1);
    if (undoStack.pop(&MD.kit.machines[track].params)) {
      GUI.flash_p_string_fill(PSTR("UNDO"));
      for (uint8_t i = 0; i < 24; i++) {
        MD.setTrackParam(track, i, MD.kit.machines[track].params[i]);
      }
    } 
    else {
      GUI.flash_p_string_fill(PSTR("UNDO XXX"));
    }
  }

void MDRandomizerClass::loadKit() {
  m_memcpy(origParams, MD.kit.machines[track].params, sizeof(origParams));
}
