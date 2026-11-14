/*
  ==============================================================================

    KnobLAF.h
    Ramer Digital (2026) — LookAndFeel para Knobs Rotativos com Filmstrip

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class KnobLAF : public juce::LookAndFeel_V4 {
public:
  KnobLAF();
  ~KnobLAF() override = default;

  void setImage(const juce::Image &inputImage);
  void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
                        float sliderPos, float rotaryStartAngle,
                        float rotaryEndAngle, juce::Slider &slider) override;

private:
  juce::Image img;
};
