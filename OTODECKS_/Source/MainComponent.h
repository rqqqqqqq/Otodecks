#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

using namespace juce;


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    
    //tells memory and transport source to prepare for playing
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    //retrieves and plays the next block of audio data
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

    //release resources that are no longer required
    void releaseResources() override;

    //==============================================================================
    void paint(Graphics& g) override;
    void resized() override;

private:
    //==============================================================================

    AudioFormatManager formatManager;
    MixerAudioSource mixerSource;
    AudioThumbnailCache thumbCache{ 100 };

    DJAudioPlayer playerForParsingMetaData{ formatManager };

    DJAudioPlayer player1{ formatManager };
    DeckGUI deckGUI1{ &player1, formatManager, thumbCache };

    DJAudioPlayer player2{ formatManager };
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache };

    PlaylistComponent playlistComponent{ &deckGUI1, &deckGUI2, &playerForParsingMetaData };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

