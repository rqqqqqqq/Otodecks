/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 3 Mar 2021 1:14:02pm
    Author:  FOO RUI QI

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse,
    AudioThumbnailCache& cacheToUse) :
    audioThumb(1000, formatManagerToUse, cacheToUse),
    fileLoaded(false),
    position(0)
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(Graphics& g)
{
    // fills the component's background and draws 

    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);   
    g.setColour(Colours::mediumorchid);

    if (fileLoaded)
    {
        audioThumb.drawChannel(g,
            getLocalBounds(),
            0,
            audioThumb.getTotalLength(),
            0,
            1.0f
        );

        g.setColour(Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
    }
    else
    {
        g.setFont(20.0f);
        g.drawText("File is not loaded", getLocalBounds(),
            Justification::centred, true);   
    }
}

void WaveformDisplay::resized()
{

}

void WaveformDisplay::loadURL(URL audioURL)
{
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));

    if (fileLoaded)
    {
        std::cout << "waveform display loaded! " << std::endl;
        repaint();
    }
    else 
    {
        std::cout << "waveform display not loaded! " << std::endl;
    }
}

void WaveformDisplay::setRelativePosition(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    std::cout << "change for waveform display received! " << std::endl;
    repaint();
}