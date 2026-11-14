/*
  ==============================================================================

    Footswitch.h
    Ramer Digital (2026) — Componente de Footswitch Metálico (Stomp Switch)

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class Footswitch : public juce::Component {
public:
  Footswitch();
  ~Footswitch() override = default;

  void setImages(const juce::Image &upImage, const juce::Image &downImage);
  void setState(bool isOn);
  bool getState() const { return isOnState; }

  void addListener(juce::Button::Listener *l) { button.addListener(l); }
  void removeListener(juce::Button::Listener *l) { button.removeListener(l); }
  juce::Button *getButton() { return &button; }

  void resized() override;

private:
  juce::ImageButton button;
  juce::Image upImg;
  juce::Image downImg;
  bool isOnState = false;

  void updateVisuals();
};
