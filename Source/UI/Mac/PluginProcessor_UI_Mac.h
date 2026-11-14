/*
  ==============================================================================

    PluginProcessor_UI_Mac.h
    Ramer-TS-M1N3 — macOS Desktop Interface (AU / VST3)
    Ramer Digital (2026) — Interface de Usuário Desktop

  ==============================================================================
*/

#pragma once

#include "../../PluginProcessor.h"
#include "../Common/Footswitch.h"
#include "../Common/InfoButton.h"
#include "../Common/KnobLAF.h"
#include "../Common/Led.h"
#include "../Common/ToggleSwitchLAF.h"

//==============================================================================
class PluginProcessor_UI_Mac : public juce::AudioProcessorEditor,
                               private juce::Button::Listener,
                               private juce::Slider::Listener {
public:
  PluginProcessor_UI_Mac(TSM1N3AudioProcessor &);
  ~PluginProcessor_UI_Mac() override;

  void paint(juce::Graphics &) override;
  void resized() override;
  void resetImages();

  KnobLAF blackHexKnobLAF;
  ToggleSwitchLAF toggleSwitchLAF;

  Footswitch odFootSw;
  Led odLED;

  InfoButton infoButton;

private:
  TSM1N3AudioProcessor &audioProcessor;

  juce::Image background =
      juce::ImageCache::getFromMemory(BinaryData::ts_background_black_jpg,
                                      BinaryData::ts_background_black_jpgSize);
  juce::Image logoImage =
      juce::ImageCache::getFromMemory(BinaryData::logo_png, BinaryData::logo_pngSize);

  juce::Slider ampGainKnob;
  juce::Slider ampMasterKnob;
  juce::Slider ampToneKnob;
  juce::Slider inputGainToggle;
  juce::Slider pushToggle;

  void buttonClicked(juce::Button *button) override;
  void sliderValueChanged(juce::Slider *slider) override;

public:
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      gainSliderAttach;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      toneSliderAttach;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      masterSliderAttach;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      inputGainToggleAttach;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      pushToggleAttach;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor_UI_Mac)
};
