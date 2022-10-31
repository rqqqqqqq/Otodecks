/*
  ==============================================================================

    WaveformDisplay.h
    Created: 3 Mar 2021 1:14:02pm
    Author:  FOO RUI QI

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

using namespace juce;

//==============================================================================
/*
*/
class WaveformDisplay : public Component,
    public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager& formatManagerToUse,
                    AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay();

    void paint(Graphics&) override;
    void resized() override;


    //load track files to create waveform
    void loadURL(URL audioURL);

    //set the relative position of the playhead
    void setRelativePosition(double pos);

    //changes waveform display when new track is loaded
    void changeListenerCallback(ChangeBroadcaster* source) override;


private:
    AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
