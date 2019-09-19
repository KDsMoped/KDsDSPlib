/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class TestEffectAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    TestEffectAudioProcessorEditor (TestEffectAudioProcessor&);
    ~TestEffectAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TestEffectAudioProcessor& processor;
	Slider freqSlider;
	Slider qSlider;

public:
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> freqSliderAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> qSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestEffectAudioProcessorEditor)
};
