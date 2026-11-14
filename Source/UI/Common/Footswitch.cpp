/*
  ==============================================================================

    Footswitch.cpp
    Ramer Digital (2026)

  ==============================================================================
*/

#include "Footswitch.h"

Footswitch::Footswitch() {
  addAndMakeVisible(button);
}

void Footswitch::setImages(const juce::Image &upImage,
                           const juce::Image &downImage) {
  upImg = upImage;
  downImg = downImage;
  updateVisuals();
}

void Footswitch::setState(bool isOn) {
  isOnState = isOn;
  updateVisuals();
}

void Footswitch::updateVisuals() {
  const auto &currentImg = (isOnState && downImg.isValid()) ? downImg : upImg;

  if (currentImg.isValid()) {
    button.setImages(false, true, true,
                     currentImg, 1.0f, juce::Colours::transparentWhite,
                     juce::Image(), 1.0f, juce::Colours::transparentWhite,
                     currentImg, 1.0f, juce::Colours::transparentWhite,
                     0.0f);
  }
}

void Footswitch::resized() {
  button.setBounds(getLocalBounds());
}
