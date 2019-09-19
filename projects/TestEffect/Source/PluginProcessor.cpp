/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TestEffectAudioProcessor::TestEffectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
	vtState(*this, nullptr, "Parameters", createParamLayout())
#endif
{
	vtState.addParameterListener("freq", this);
	vtState.addParameterListener("q", this);
}

TestEffectAudioProcessor::~TestEffectAudioProcessor()
{
	for (auto filter : filters)
	{
		if (filter != nullptr)
		{
			delete filter;
		}
	}
		
}

//==============================================================================

AudioProcessorValueTreeState::ParameterLayout TestEffectAudioProcessor::createParamLayout()
{
	std::vector<std::unique_ptr<RangedAudioParameter>> params;

	auto paramFreq = std::make_unique<AudioParameterFloat>("freq", "Frequency", NormalisableRange<float>(20.f, 20000.f, 1.f, 0.3f), 1000.f);
	params.push_back(std::move(paramFreq));

	auto paramQ = std::make_unique<AudioParameterFloat>("q", "Q", NormalisableRange<float>(0.f, 2.f, 0.01f, 1.f), 0.71f);
	params.push_back(std::move(paramQ));

	return { params.begin(), params.end() };
}

const String TestEffectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TestEffectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TestEffectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TestEffectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TestEffectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TestEffectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TestEffectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TestEffectAudioProcessor::setCurrentProgram (int index)
{
}

const String TestEffectAudioProcessor::getProgramName (int index)
{
    return {};
}

void TestEffectAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void TestEffectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	for (int i = 0; i < getTotalNumInputChannels(); ++i)
	{
		auto f = new kdsp::BiquadFilterDF2<double>(2);
		f->calcFilterCoeffs(kdsp::filterType::ZoelzerBandpass, getSampleRate(), *vtState.getRawParameterValue("freq"), *vtState.getRawParameterValue("q"));
		filters.push_back(f);
	}
}

void TestEffectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TestEffectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TestEffectAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

	// Clearing outputs
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	// Processing per channel
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
		
		for (int s = 0; s < buffer.getNumSamples(); ++s)
		{
			channelData[s] = filters[channel]->process(channelData[s]);
		}
    }
}

//==============================================================================
bool TestEffectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* TestEffectAudioProcessor::createEditor()
{
    return new TestEffectAudioProcessorEditor (*this);
}

//==============================================================================
void TestEffectAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TestEffectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void TestEffectAudioProcessor::parameterChanged(const String & parameterID, float newValue)
{
	if (parameterID == "freq")
	{
		for (auto filter : filters)
		{
			filter->calcFilterCoeffs(kdsp::filterType::ZoelzerBandpass, getSampleRate(), newValue, *vtState.getRawParameterValue("q"));
		}
	}

	if (parameterID == "q")
	{
		for (auto filter : filters)
		{
			filter->calcFilterCoeffs(kdsp::filterType::ZoelzerBandpass, getSampleRate(), *vtState.getRawParameterValue("freq"), newValue);
		}
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TestEffectAudioProcessor();
}
