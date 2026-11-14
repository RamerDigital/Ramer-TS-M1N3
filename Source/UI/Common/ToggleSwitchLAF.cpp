/*
  ==============================================================================

    ToggleSwitchLAF.cpp
    Ramer Digital (2026)

  ==============================================================================
*/

#include "ToggleSwitchLAF.h"

void ToggleSwitchLAF::drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height,
                                      float sliderPos, float /*minSliderPos*/, float /*maxSliderPos*/,
                                      const juce::Slider::SliderStyle /*style*/,
                                      juce::Slider &/*slider*/) {
  auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();
  auto centerX = bounds.getCentreX();

  auto trackWidth = juce::jmax(4.0f, bounds.getWidth() * 0.24f);
  auto track = bounds.withSizeKeepingCentre(trackWidth, bounds.getHeight() * 0.75f);
  
  // Track Slot (Trilho escuro)
  g.setColour(juce::Colour(0xFF101214));
  g.fillRoundedRectangle(track, trackWidth * 0.5f);
  g.setColour(juce::Colours::white.withAlpha(0.15f));
  g.drawRoundedRectangle(track, trackWidth * 0.5f, 1.0f);

  // Chave metálica
  auto handleHeight = juce::jmax(10.0f, bounds.getHeight() * 0.22f);
  auto handleWidth = juce::jmax(20.0f, bounds.getWidth() * 0.85f);
  auto handleY = sliderPos - handleHeight * 0.5f;

  juce::Rectangle<float> handleRect(centerX - handleWidth * 0.5f, handleY,
                                    handleWidth, handleHeight);

  juce::ColourGradient grad(juce::Colour(0xFFFFFFFF), handleRect.getX(),
                            handleRect.getY(), juce::Colour(0xFF6E7278),
                            handleRect.getX(), handleRect.getBottom(), false);
  g.setGradientFill(grad);
  g.fillRoundedRectangle(handleRect, 3.0f);

  g.setColour(juce::Colours::white.withAlpha(0.8f));
  g.drawHorizontalLine((int)handleRect.getY() + 1,
                       handleRect.getX() + 1.5f,
                       handleRect.getRight() - 1.5f);

  g.setColour(juce::Colour(0xFF22252A));
  g.drawRoundedRectangle(handleRect, 3.0f, 1.0f);
}
