/*
  ==============================================================================

    KnobLAF.cpp
    Ramer Digital (2026)

  ==============================================================================
*/

#include "KnobLAF.h"
#include <cmath>

KnobLAF::KnobLAF() {}

void KnobLAF::setImage(const juce::Image &inputImage) {
  img = inputImage;
}

void KnobLAF::drawRotarySlider(juce::Graphics &g, int x, int y, int width,
                               int height, float /*sliderPos*/,
                               float /*rotaryStartAngle*/,
                               float /*rotaryEndAngle*/,
                               juce::Slider &slider) {
  if (!img.isValid())
    return;

  const double rotation =
      (slider.getValue() - slider.getMinimum()) /
      (slider.getMaximum() - slider.getMinimum());

  const int frames = img.getHeight() / img.getWidth();
  const int frameId =
      juce::jlimit(0, frames - 1, (int)std::ceil(rotation * ((double)frames - 1.0)));

  const auto fWidth = static_cast<float>(width);
  const auto fHeight = static_cast<float>(height);
  const auto fX = static_cast<float>(x);
  const auto fY = static_cast<float>(y);
  const float radius = juce::jmin(fWidth / 2.0f, fHeight / 2.0f);
  const float centerX = fX + fWidth * 0.5f;
  const float centerY = fY + fHeight * 0.5f;
  const float rx = centerX - radius - 1.0f;
  const float ry = centerY - radius;

  g.drawImage(img, (int)rx, (int)ry, 2 * (int)radius, 2 * (int)radius, 0,
              frameId * img.getWidth(), img.getWidth(), img.getWidth());
}
