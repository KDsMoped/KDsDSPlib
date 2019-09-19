/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TestEffectAudioProcessorEditor::TestEffectAudioProcessorEditor (TestEffectAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
	setSize(400, 300);

	freqSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	freqSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
	freqSlider.setNormalisableRange(NormalisableRange<double>(
			processor.vtState.getParameterRange("freq").start,
			processor.vtState.getParameterRange("freq").end,
			processor.vtState.getParameterRange("freq").interval,
			processor.vtState.getParameterRange("freq").skew
		)
	);
	addAndMakeVisible(freqSlider);
	freqSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.vtState, "freq", freqSlider);

	qSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	qSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
	qSlider.setNormalisableRange(NormalisableRange<double>(
		processor.vtState.getParameterRange("q").start,
		processor.vtState.getParameterRange("q").end,
		processor.vtState.getParameterRange("q").interval
		)
	);
	addAndMakeVisible(qSlider);
	qSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.vtState, "q", qSlider);
}

TestEffectAudioProcessorEditor::~TestEffectAudioProcessorEditor()
{
}

//==============================================================================
void TestEffectAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Filter Test", getLocalBounds(), Justification::centredTop, 1);
}

void TestEffectAudioProcessorEditor::resized()
{
	Rectangle<int> sliderBounds(100, 100);
	sliderBounds.setCentre(getBounds().getCentre());
	sliderBounds.translate(-100, 0);
	freqSlider.setBounds(sliderBounds);

	sliderBounds.setCentre(getBounds().getCentre());
	sliderBounds.translate(100, 0);
	qSlider.setBounds(sliderBounds);
}
