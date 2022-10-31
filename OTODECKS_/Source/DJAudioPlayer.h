/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 3 Mar 2021 1:13:23pm
    Author:  FOO RUI QI

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

using namespace juce;

class DJAudioPlayer : public AudioSource {
public:

    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    //tells memory and transport source to prepare for playing
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    //retrieves and plays the next block of audio data
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

    //release resources that are no longer required
    void releaseResources() override;

    //load audio URL 
    void loadURL(URL audioURL);

    //get length of track in seconds
    double getLength();

    //tells audio player to start/stop playing resource
    void start();
    void stop();

    //get the relative position of the playhead
    double getRelativePosition();

    //set ratio for volume level
    void setGain(double gain);

    //set ratio for speed of track
    void setSpeed(double ratio);

    //set ratio for position of the playhead
    void setPosition(double posInSecs);
    void setRelativePosition(double pos);

private:
    AudioTransportSource transportSource;
    ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
};