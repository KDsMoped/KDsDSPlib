/*
  ==============================================================================

    Created: 14 Aug 2019
    Author:  Dominik Köhler

  ==============================================================================
*/

#pragma once

#include "Helpers.h"
#include "OverlapAddBuffer.h"
#include "RingBuffer.h"
//#include "FFT.h"
#include "DelayLine.h"


namespace dsphelpers{

	static void convertSamplerate(int currentSamplerate, int targetSamplerate, std::vector<double> inputData, std::vector<double> &outputData, int &outputLength) {
		
		//if (targetSamplerate >= currentSamplerate) {
			double factor = (double)targetSamplerate / (double)currentSamplerate;
			int inputLength = inputData.size();
			outputLength = helpers::nextPowOfTwo(inputLength*factor);
			outputData.resize(outputLength, 0);
		
			int inputIndex = 0;
			int outputIndex = 0;
			double floatingIndex = 0.0f;
			double ratio = 0;
		
			while ((inputIndex + 1) < inputLength) {
				//Linear interpolieren
				outputData[outputIndex] = inputData[inputIndex] * (1.0f - ratio) + inputData[inputIndex + 1] * ratio;
				outputData[outputIndex] *= (1 / factor);

				//Berechnungen fuer naechste Runde.
				outputIndex++;
				floatingIndex = outputIndex * (1 / factor);
				inputIndex = (int)floatingIndex;
				ratio = floatingIndex - inputIndex;
			}
		//}
	}

	static float convertDbToFloat(float value) {
		return pow(10, value / 20);
	}

	static float convertFloatToDb(float value) {
		return 20 * log(value);
	}

} //namespace dsphelpers
