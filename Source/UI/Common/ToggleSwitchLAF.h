/*
  ==============================================================================

    ToggleSwitchLAF.h
    Ramer Digital (2026) — LookAndFeel para chave alavanca metálica vertical

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class ToggleSwitchLAF : public juce::LookAndFeel_V4 {
public:
  ToggleSwitchLAF() = default;
  ~ToggleSwitchLAF() override = default;

  void drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height,
                        float sliderPos, float minSliderPos, float maxSliderPos,
                        const juce::Slider::SliderStyle style,
                        juce::Slider &slider) override;
};
