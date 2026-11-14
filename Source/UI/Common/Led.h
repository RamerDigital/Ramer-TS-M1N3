/*
  ==============================================================================

    Led.h
    Ramer Digital (2026) — Componente de LED Indicador de Status

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class Led : public juce::Component {
public:
  Led();
  ~Led() override = default;

  void setImages(const juce::Image &offImage, const juce::Image &onImage);
  void setState(bool isOn);
  bool getState() const { return isOnState; }

  void addListener(juce::Button::Listener *l) { button.addListener(l); }
  void removeListener(juce::Button::Listener *l) { button.removeListener(l); }
  juce::Button *getButton() { return &button; }

  void resized() override;

private:
  juce::ImageButton button;
  juce::Image offImg;
  juce::Image onImg;
  bool isOnState = false;

  void updateVisuals();
};
