/*
  ==============================================================================

    DeckGUI.cpp
    Created: 3 Mar 2021 1:13:44pm
    Author:  FOO RUI QI

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
        AudioFormatManager & formatManagerToUse,
        AudioThumbnailCache & cacheToUse
    ) : player(_player),
        waveformDisplay(formatManagerToUse, cacheToUse)
{
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(replayButton);

    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(positionSlider);

    addAndMakeVisible(volumeLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(positionLabel);

    addAndMakeVisible(waveformDisplay);


    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    replayButton.addListener(this);

    volumeSlider.addListener(this);
    speedSlider.addListener(this);
    positionSlider.addListener(this);

    volumeSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    speedSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    positionSlider.setSliderStyle(Slider::SliderStyle::Rotary);

    volumeSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    positionSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);

    volumeSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 10.0);
    positionSlider.setRange(0.0, 1.0);


    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint(Graphics& g)
{
    //fills the component's background and draws 

    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.setJustificationType(Justification::centred);
    volumeLabel.setColour(Label::textColourId, Colours::white);
    volumeLabel.setFont(Font(12.0f, Font::bold));
    volumeLabel.attachToComponent(&volumeSlider, false);

    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setJustificationType(Justification::centred);
    speedLabel.setColour(Label::textColourId, Colours::white);
    speedLabel.setFont(Font(12.0f, Font::bold));
    speedLabel.attachToComponent(&speedSlider, false);

    positionLabel.setText("Position", juce::dontSendNotification);
    positionLabel.setJustificationType(Justification::centred);
    positionLabel.setColour(Label::textColourId, Colours::white);
    positionLabel.setFont(Font(12.0f, Font::bold));
    positionLabel.attachToComponent(&positionSlider, false);
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::darkturquoise);
}

void DeckGUI::resized()
{
    //resizing graphics implemented in paint     
    //set bounds of for child components that your component contains

    double rowH = getHeight() / 8;

    playButton.setBounds(0, 0, getWidth()/2, rowH);
    stopButton.setBounds(getWidth() / 2, 0, getWidth()/2, rowH);

    volumeSlider.setBounds(0, rowH*2, getWidth()/3, rowH * 3);
    speedSlider.setBounds(getWidth()/3, rowH*2, getWidth()/3, rowH * 3);
    positionSlider.setBounds(getWidth()/3*2, rowH*2, getWidth()/3, rowH * 3);

    waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH * 2);

    loadButton.setBounds(0, rowH * 7, getWidth() / 2, rowH);
    replayButton.setBounds(getWidth() / 2, rowH * 7, getWidth() / 2, rowH);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
    if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();
    }
    if (button == &loadButton)
    {
        FileChooser chooser{ "Choose a file" };
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(URL{ chooser.getResult() });
            waveformDisplay.loadURL(URL{chooser.getResult()});
        }
    }
    if (button == &replayButton)
    {
        replayTrack(&replayButton, &positionSlider);
        player->start();
    }
}

void DeckGUI::sliderValueChanged(Slider * slider)
{
    if (slider == &volumeSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }

    if (slider == &positionSlider)
    {
        player->setRelativePosition(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const StringArray &files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

void DeckGUI::filesDropped(const StringArray &files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped" << std::endl;
    if (files.size() == 1)
    {
        player->loadURL(URL{File{files[0]}});
    }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setRelativePosition(player->getRelativePosition());
}

void DeckGUI::loadFile(URL audioURL)
{
    DBG("DeckGUI::loadFile called");
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}

void DeckGUI::replayTrack(Button* button, Slider* slider)
{
    slider->setValue(0);
}