/*
  ==============================================================================

    PluginProcessor.h
    Original work Copyright (c) 2021-2023 Keith Bloemer (GuitarML)
    Modified by Ramer Digital (2026) — Apple Silicon macOS DSP & Resampling
    Licensed under the GNU General Public License v3.0 (GPLv3)

  ==============================================================================
*/
#pragma once

#include <nlohmann/json.hpp>
#include "RTNeuralLSTM.h"
#include <Accelerate/Accelerate.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include <dsp/ResamplingContainer/ResamplingContainer.h>

#define GAIN_ID "drive"
#define GAIN_NAME "Drive"
#define MASTER_ID "level"
#define MASTER_NAME "Level"
#define TONE_ID "tone"
#define TONE_NAME "Tone"
#define INPUT_GAIN_ID "input_gain"
#define INPUT_GAIN_NAME "Input Gain"
#define PUSH_ID "push"
#define PUSH_NAME "Push"


//==============================================================================
class TSM1N3AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    TSM1N3AudioProcessor();
    ~TSM1N3AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // Pedal/amp states
    std::atomic<int> fw_state { 1 }; // 0 = off, 1 = on

    bool lstm_state = true;

    RT_LSTM LSTM;
    RT_LSTM LSTM2;

    juce::AudioProcessorValueTreeState treeState;

private:
    std::atomic<float>* driveParam = nullptr;
    std::atomic<float>* toneParam = nullptr;
    std::atomic<float>* masterParam = nullptr;
    std::atomic<float>* inputGainParam = nullptr;
    std::atomic<float>* pushParam = nullptr;

    float previousMasterValue = 0.5f;
    bool previousPushActive = false;
    bool needsResampling = false;

    ::dsp::ResamplingContainer<float, 2> resampler { 48000.0 };
    std::vector<float> monoRightScratch;

    // Parametric EQ (Push Mod) centered at 200 Hz (+2.0 dB)
    juce::dsp::IIR::Filter<float> pushFilterL;
    juce::dsp::IIR::Filter<float> pushFilterR;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TSM1N3AudioProcessor)
};
