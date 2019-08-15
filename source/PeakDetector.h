/*
  ==============================================================================

    Created: 14 Aug 2019
    Author:  Dominik Köhler

  ==============================================================================
*/

#pragma once

namespace kdsp {

    class PeakDetector {
    public:
        PeakDetector() {}
        ~PeakDetector() {}
        
        float calcEnvelope(float sample, float timeConstant, int sr) {
            float x = sample;
            float attack = 0.1f;
            float release = timeConstant;
            float dt = 1.f / sr;
            float tc = log10(0.01f);
            
            // Calculate rect, AT and RT values
            float rect = fabs(x);
            float at = 1.f - exp((-2.2f * dt) / (attack / 1000.f));
            float rt = 1.f - exp((-2.2f * dt) / (release / 1000.f));
            
            // Writing the output value before calculation starts imitates the one sample delay
            float y = vc;
            
            // Calculate the envelope value for attack and release case
            if (rect > vc) {
                vc = (1.f - at) * vc + at * rect;
            }
            else {
                vc = (1.f - rt) * vc;
            }
            
            return y;
        }
        
        void flushVC() {
            vc = 0.f;
        }
        
    private:
        float vc;
    };
}
