/*
  ==============================================================================

    InfoButton.cpp
    Ramer Digital (2026) — Botão de Informações e Overlay Modal Integrado

  ==============================================================================
*/

#include "InfoButton.h"

InfoButton::InfoButton() {
  githubLink.setButtonText("https://github.com/ramerdigital/Ramer-TS-M1N3");
  githubLink.setURL(juce::URL("https://github.com/ramerdigital/Ramer-TS-M1N3"));
  githubLink.setFont(juce::FontOptions(9.5f, juce::Font::plain), false);
  githubLink.setJustificationType(juce::Justification::centredLeft);
  githubLink.setColour(juce::HyperlinkButton::textColourId,
                       juce::Colour(0xFF4DA6FF));
  addChildComponent(githubLink);

  closeButton.setButtonText("Close");
  closeButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFF2A2D34));
  closeButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
  closeButton.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
  closeButton.addListener(this);
  addChildComponent(closeButton);
}

juce::Rectangle<int> InfoButton::getPanelBounds() const {
  auto bounds = getLocalBounds();
  int maxW = juce::jmin(bounds.getWidth() - 20, 260);
  int maxH = juce::jmin(bounds.getHeight() - 20, 340);
  int x = (bounds.getWidth() - maxW) / 2;
  int y = (bounds.getHeight() - maxH) / 2;
  return juce::Rectangle<int>(x, y, maxW, maxH);
}

void InfoButton::setButtonPosition(int x, int y, int width, int height) {
  buttonRect = juce::Rectangle<int>(x, y, width, height);
  repaint();
}

void InfoButton::setOverlayOpen(bool open) {
  isOpened = open;
  githubLink.setVisible(open);
  closeButton.setVisible(open);
  if (open)
    toFront(true);
  repaint();
}

void InfoButton::paint(juce::Graphics &g) {
  // 1. Desenho do Overlay Modal quando aberto
  if (isOpened) {
    g.fillAll(juce::Colours::black.withAlpha(0.82f));

    auto panelRect = getPanelBounds();

    g.setColour(juce::Colour(0xFF1B1E24));
    g.fillRoundedRectangle(panelRect.toFloat(), 12.0f);

    g.setColour(juce::Colours::white.withAlpha(0.18f));
    g.drawRoundedRectangle(panelRect.toFloat(), 12.0f, 1.0f);

    int textX = panelRect.getX() + 16;
    int textW = panelRect.getWidth() - 32;
    int currentY = panelRect.getY() + 18;

    // Header: Nome e Versão
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    g.drawSingleLineText("Ramer-TS-M1N3", textX, currentY);
    currentY += 15;

    g.setColour(juce::Colour(0xFFAAAAAA));
    g.setFont(juce::FontOptions(10.5f, juce::Font::plain));
    g.drawSingleLineText("Version 1.0.0 (Build 2026.08)", textX, currentY);
    currentY += 14;

    g.setColour(juce::Colours::white.withAlpha(0.12f));
    g.fillRect(textX, currentY, textW, 1);
    currentY += 12;

    // Seção de Desenvolvimento
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(11.0f, juce::Font::bold));
    g.drawSingleLineText("Developed by:", textX, currentY);
    currentY += 14;

    g.setColour(juce::Colour(0xFFCCCCCC));
    g.setFont(juce::FontOptions(10.0f, juce::Font::plain));
    g.drawSingleLineText("Ramer Digital (2026)", textX, currentY);
    currentY += 13;
    g.drawSingleLineText("Neural modeling by GuitarML", textX, currentY);
    currentY += 16;

    g.setColour(juce::Colours::white.withAlpha(0.12f));
    g.fillRect(textX, currentY, textW, 1);
    currentY += 12;

    // Seção Open Source
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(11.0f, juce::Font::bold));
    g.drawSingleLineText("Open Source Notice:", textX, currentY);
    currentY += 14;

    g.setColour(juce::Colour(0xFFAAAAAA));
    g.setFont(juce::FontOptions(9.5f, juce::Font::plain));
    g.drawSingleLineText("- JUCE Framework (GPLv3)", textX, currentY);
    currentY += 12;
    g.drawSingleLineText("- RTNeural (BSD 3-Clause) - J. Chowdhury", textX, currentY);
    currentY += 12;
    g.drawSingleLineText("- AudioDSPTools (MIT) - S. Atkinson", textX, currentY);
    currentY += 12;
    g.drawSingleLineText("- Eigen 3 (MPL 2.0) / xsimd (BSD 3-Clause)", textX, currentY);
    currentY += 12;
    g.drawSingleLineText("- nlohmann/json (MIT) - N. Lohmann", textX, currentY);
    currentY += 16;

    g.setColour(juce::Colours::white.withAlpha(0.12f));
    g.fillRect(textX, currentY, textW, 1);
    currentY += 12;

    // Source Code Repository Link
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(11.0f, juce::Font::bold));
    g.drawSingleLineText("Source Code & Repository:", textX, currentY);
  }

  // 2. Desenho do Botão Circular [?] — estritamente quando o modal NÃO estiver aberto
  if (!isOpened && !buttonRect.isEmpty()) {
    auto bFloat = buttonRect.toFloat();

    if (isDown)
      g.setColour(juce::Colours::white.withAlpha(0.35f));
    else if (isHovered)
      g.setColour(juce::Colours::white.withAlpha(0.22f));
    else
      g.setColour(juce::Colours::white.withAlpha(0.12f));

    g.fillEllipse(bFloat);
    g.setColour(juce::Colours::white.withAlpha(0.45f));
    g.drawEllipse(bFloat, 1.2f);

    g.setColour(juce::Colours::white.withAlpha(0.9f));
    g.setFont(juce::FontOptions(bFloat.getHeight() * 0.58f, juce::Font::bold));
    g.drawText("?", bFloat, juce::Justification::centred);
  }
}

void InfoButton::resized() {
  auto panelRect = getPanelBounds();
  int textX = panelRect.getX() + 16;
  int textW = panelRect.getWidth() - 32;

  int githubY = panelRect.getY() + 210;
  githubLink.setBounds(textX, githubY, textW, 16);

  closeButton.setBounds(panelRect.getCentreX() - 45,
                        panelRect.getBottom() - 34, 90, 24);
}

bool InfoButton::hitTest(int x, int y) {
  if (isOpened)
    return true;

  return buttonRect.contains(x, y);
}

void InfoButton::mouseDown(const juce::MouseEvent &e) {
  if (!isOpened) {
    if (buttonRect.contains(e.getPosition())) {
      isDown = true;
      repaint();
    }
  } else {
    auto panelRect = getPanelBounds();
    if (!panelRect.contains(e.getPosition())) {
      setOverlayOpen(false);
    }
  }
}

void InfoButton::mouseUp(const juce::MouseEvent &e) {
  if (!isOpened && isDown) {
    isDown = false;
    if (buttonRect.contains(e.getPosition())) {
      setOverlayOpen(true);
    } else {
      repaint();
    }
  }
}

void InfoButton::mouseMove(const juce::MouseEvent &e) {
  if (!isOpened) {
    bool hovered = buttonRect.contains(e.getPosition());
    if (hovered != isHovered) {
      isHovered = hovered;
      repaint();
    }
  }
}

void InfoButton::mouseExit(const juce::MouseEvent &) {
  if (isHovered || isDown) {
    isHovered = false;
    isDown = false;
    repaint();
  }
}

void InfoButton::buttonClicked(juce::Button *button) {
  if (button == &closeButton) {
    setOverlayOpen(false);
  }
}
