/*
  ==============================================================================

    Created: 14 Aug 2019
    Author:  Dominik Köhler

  ==============================================================================
*/

#pragma once

#include<math.h>

namespace kdsp {

    static float convertDBtoFloat(float dbValue)
    {
        return pow(10, dbValue / 20);
    }

    static float convertFloattoDB(float value)
    {
        return 20 * log(value);
    }

    /**
     Returns the corresponding frequency to a  MIDI-style note value
    */
    static float convertMidiToFreq(float midivalue)
    {
        return  440.0*pow(2.0,(midivalue - 69.f) * 1.f/12.f);
    }
}