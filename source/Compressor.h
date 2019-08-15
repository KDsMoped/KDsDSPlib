/*
  ==============================================================================

    Created: 14 Aug 2019
    Author:  Dominik Köhler

  ==============================================================================
*/

#pragma once

#include "PeakDetector.h"
#include "DSPUtilities.h"

namespace kdsp {

    class Compressor {
    public:
        Compressor() 
        {
            peakDetector = std::make_unique<PeakDetector>();
        }
        ~Compressor() {}
        
        float calcGain(float sample, float threshold, float ratio, float release, int sr)
        {
            // Set slope variable
            float cs = 1.f - (1.f / ratio);
            
            // Envelope detector
            float env = peakDetector->calcEnvelope(sample, release, sr);
            
            // Convert Envelope to logarithmic value
            float envDB = convertFloattoDB(env);
            
            // Compute gain; Threshold and detection values in dB
            float y = cs * (threshold - envDB);
            
            // Clamp; this allows ratios of 1:1 to still operate
            y = fminf(0.f, y);
            
            // Convert to linear values
            return convertDBtoFloat(y);
        }
        
        void flushDetector()
        {
            peakDetector->flushVC();
        }
        
    private:
        std::unique_ptr<PeakDetector> peakDetector;
    };
}
