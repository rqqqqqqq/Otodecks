/*
  ==============================================================================

    DeckGUI.h
    Created: 3 Mar 2021 1:13:44pm
    Author:  FOO RUI QI

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

using namespace juce;

//==============================================================================
/*
*/
class DeckGUI : public Component,
                public Button::Listener,
                public Slider::Listener,
                public FileDragAndDropTarget,
                public Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
        AudioFormatManager & formatManagerToUse,
        AudioThumbnailCache & cacheToUse);
    ~DeckGUI();

    void paint(Graphics&) override;
    void resized() override;

    //implement button listener for buttons to perform function
    void buttonClicked(Button *) override;

    //implement slider listener for sliders to perform function
    void sliderValueChanged(Slider * slider) override;

    //implement FileDragAndDropTarget allowing files to be dropped into deck
    bool isInterestedInFileDrag(const StringArray & files) override;
    void filesDropped(const StringArray & files, int x, int y) override;

    //implement timer to track length of track played
    void timerCallback() override;

    //to load track files into deck
    void loadFile(URL audioURL);

    //allow tracks to be replayed
    void replayTrack(Button* button, Slider* slider);

private:
    int id;
    friend class PlaylistComponent;
    DJAudioPlayer* player;

    //create objects
    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "STOP" };
    TextButton loadButton{ "LOAD" };
    TextButton replayButton{ "REPLAY" };

    Slider volumeSlider;
    Slider speedSlider;
    Slider positionSlider;

    Label volumeLabel;
    Label speedLabel;
    Label positionLabel;

    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
