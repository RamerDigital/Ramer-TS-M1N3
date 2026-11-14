/*
  ==============================================================================

    PluginProcessor_UI_Mac.cpp
    Ramer-TS-M1N3 — macOS Desktop Interface (AU / VST3)
    Ramer Digital (2026) — Interface de Usuário Desktop

  ==============================================================================
*/

#include "PluginProcessor_UI_Mac.h"

//==============================================================================
PluginProcessor_UI_Mac::PluginProcessor_UI_Mac (TSM1N3AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Look and Feel para Knobs
    blackHexKnobLAF.setImage (juce::ImageCache::getFromMemory (BinaryData::knob2_png, BinaryData::knob2_pngSize));

    // Footswitch e LED
    addAndMakeVisible (odFootSw);
    odFootSw.setImages (
        juce::ImageCache::getFromMemory (BinaryData::footswitch_up_png, BinaryData::footswitch_up_pngSize),
        juce::ImageCache::getFromMemory (BinaryData::footswitch_down_png, BinaryData::footswitch_down_pngSize));
    odFootSw.addListener (this);

    addAndMakeVisible (odLED);
    odLED.setImages (
        juce::ImageCache::getFromMemory (BinaryData::led_red_off_png, BinaryData::led_red_off_pngSize),
        juce::ImageCache::getFromMemory (BinaryData::led_red_on_png, BinaryData::led_red_on_pngSize));
    odLED.addListener (this);

    gainSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.treeState, GAIN_ID, ampGainKnob);
    addAndMakeVisible (ampGainKnob);
    ampGainKnob.setLookAndFeel (&blackHexKnobLAF);
    ampGainKnob.addListener (this);
    ampGainKnob.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampGainKnob.setTextBoxStyle (juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampGainKnob.setDoubleClickReturnValue (true, 0.0);

    toneSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.treeState, TONE_ID, ampToneKnob);
    addAndMakeVisible (ampToneKnob);
    ampToneKnob.setLookAndFeel (&blackHexKnobLAF);
    ampToneKnob.addListener (this);
    ampToneKnob.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampToneKnob.setTextBoxStyle (juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampToneKnob.setDoubleClickReturnValue (true, 0.5);

    masterSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.treeState, MASTER_ID, ampMasterKnob);
    addAndMakeVisible (ampMasterKnob);
    ampMasterKnob.setLookAndFeel (&blackHexKnobLAF);
    ampMasterKnob.addListener (this);
    ampMasterKnob.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampMasterKnob.setTextBoxStyle (juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampMasterKnob.setDoubleClickReturnValue (true, 0.5);

    addAndMakeVisible (inputGainToggle);
    inputGainToggle.setLookAndFeel (&toggleSwitchLAF);
    inputGainToggle.setSliderStyle (juce::Slider::SliderStyle::LinearVertical);
    inputGainToggle.setRange (0, 2, 1);
    inputGainToggle.setTextBoxStyle (juce::Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    inputGainToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.treeState, INPUT_GAIN_ID, inputGainToggle);

    addAndMakeVisible (pushToggle);
    pushToggle.setLookAndFeel (&toggleSwitchLAF);
    pushToggle.setSliderStyle (juce::Slider::SliderStyle::LinearVertical);
    pushToggle.setRange (0, 1, 1);
    pushToggle.setTextBoxStyle (juce::Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    pushToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.treeState, PUSH_ID, pushToggle);

    // Botão e Overlay de Informações (Auto-contido)
    addAndMakeVisible (infoButton);

    resetImages();

    // Dimensão fixa desktop
    setSize (255, 375);
}

PluginProcessor_UI_Mac::~PluginProcessor_UI_Mac()
{
    ampGainKnob.setLookAndFeel (nullptr);
    ampToneKnob.setLookAndFeel (nullptr);
    ampMasterKnob.setLookAndFeel (nullptr);
    inputGainToggle.setLookAndFeel (nullptr);
    pushToggle.setLookAndFeel (nullptr);
}

void PluginProcessor_UI_Mac::paint (juce::Graphics& g)
{
    g.drawImage (background, getLocalBounds().toFloat());
    g.drawImage (logoImage, juce::Rectangle<float> (40.0f, 313.0f, 32.0f, 32.0f));

    // Tipografia macOS Desktop
    g.setFont (juce::FontOptions ("Noteworthy", 10.0f, juce::Font::plain));
    g.setColour (juce::Colours::black.withAlpha (0.8f));

    // Input Gain Toggle Labels (hi, mid, lo)
    g.drawText ("hi", 156, 95, 25, 10, juce::Justification::right, false);
    g.drawText ("mid", 156, 105, 25, 10, juce::Justification::right, false);
    g.drawText ("lo", 156, 115, 25, 10, juce::Justification::right, false);

    // Push Mod Toggle Labels (on, off)
    g.drawText ("on", 74, 95, 25, 10, juce::Justification::left, false);
    g.drawText ("off", 74, 115, 25, 10, juce::Justification::left, false);
}

void PluginProcessor_UI_Mac::resized()
{
    ampGainKnob.setBounds (15, 10, 94, 71);
    ampMasterKnob.setBounds (147, 10, 94, 71);
    ampToneKnob.setBounds (82, 68, 94, 71);

    inputGainToggle.setBounds (185, 92, 18, 37);
    pushToggle.setBounds (52, 92, 18, 37);

    odLED.setBounds (114, 236, 30, 30);
    odFootSw.setBounds (100, 274, 60, 60);

    infoButton.setBounds (getLocalBounds());
    infoButton.setButtonPosition (199, 329, 16, 16);
}

void PluginProcessor_UI_Mac::buttonClicked (juce::Button* button)
{
    if (button == odFootSw.getButton()) {
        if (audioProcessor.fw_state == 0)
            audioProcessor.fw_state = 1;
        else
            audioProcessor.fw_state = 0;

        resetImages();
    }
}

void PluginProcessor_UI_Mac::sliderValueChanged (juce::Slider* /*slider*/)
{
}

void PluginProcessor_UI_Mac::resetImages()
{
    const bool isOn = (audioProcessor.fw_state != 0);
    odFootSw.setState (isOn);
    odLED.setState (isOn);
}
