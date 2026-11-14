/*
  ==============================================================================

    InfoButton.h
    Ramer Digital (2026) — Botão de Informações e Overlay Modal Integrado

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class InfoButton : public juce::Component, public juce::Button::Listener {
public:
  InfoButton();
  ~InfoButton() override = default;

  void setButtonPosition(int x, int y, int width, int height);
  void setOverlayOpen(bool open);
  bool isOverlayOpen() const { return isOpened; }

  void paint(juce::Graphics &g) override;
  void resized() override;
  bool hitTest(int x, int y) override;
  void mouseDown(const juce::MouseEvent &e) override;
  void mouseUp(const juce::MouseEvent &e) override;
  void mouseMove(const juce::MouseEvent &e) override;
  void mouseExit(const juce::MouseEvent &e) override;

  void buttonClicked(juce::Button *button) override;

private:
  juce::Rectangle<int> getPanelBounds() const;

  bool isOpened = false;
  bool isHovered = false;
  bool isDown = false;
  juce::Rectangle<int> buttonRect;

  juce::HyperlinkButton githubLink;
  juce::TextButton closeButton;
};
