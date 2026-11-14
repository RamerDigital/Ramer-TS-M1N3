/*
  ==============================================================================

    Led.cpp
    Ramer Digital (2026)

  ==============================================================================
*/

#include "Led.h"

Led::Led() {
  addAndMakeVisible(button);
}

void Led::setImages(const juce::Image &offImage,
                    const juce::Image &onImage) {
  offImg = offImage;
  onImg = onImage;
  updateVisuals();
}

void Led::setState(bool isOn) {
  isOnState = isOn;
  updateVisuals();
}

void Led::updateVisuals() {
  const auto &currentImg = (isOnState && onImg.isValid()) ? onImg : offImg;

  if (currentImg.isValid()) {
    button.setImages(false, true, true,
                     currentImg, 1.0f, juce::Colours::transparentWhite,
                     juce::Image(), 1.0f, juce::Colours::transparentWhite,
                     currentImg, 1.0f, juce::Colours::transparentWhite,
                     0.0f);
  }
}

void Led::resized() {
  button.setBounds(getLocalBounds());
}
