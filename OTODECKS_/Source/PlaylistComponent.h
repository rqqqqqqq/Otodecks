/*
  ==============================================================================

    PlaylistComponent.h
    Created: 3 Mar 2021 2:46:12pm
    Author:  FOO RUI QI

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "PlaylistComponent.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "Tracklist.h"

//==============================================================================

class PlaylistComponent  : public juce::Component,
                           public TableListBoxModel,
                           public Button::Listener
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1, DeckGUI* _deckGUI2, DJAudioPlayer*_playerForParsingMetaData);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    //get number of tracks in playlist
    int getNumRows () override;

    //fill row with color
    void paintRowBackground (Graphics&,
        int rowNumber,
        int width,
        int height, 
        bool rowIsSelected) override;

    //inserts text to cell based on columnId
    void paintCell(Graphics&,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected) override;

    //creates text button within cells of column id 3
    Component * refreshComponentForCell (int rowNumber,
        int columnId,
        bool isRowSelected,
        Component *existingComponentToUpdate) override; 

    //implement button listener for buttons to perform function
    void buttonClicked(Button* button) override;
    
    //add files to library
    void loadTrackToLibrary();

    //check if tracks are already loaded into playlist
    bool trackIsInPlaylist(String fileName);

    //get length of track
    String getTrackLength(URL audioURL);

    //parses meta data of track
    String fromSecondsToMinutes(double seconds);
    
    //search for files in library
    void searchInLibrary(String textSearch);
    
    //search for title using keyword
    int whereInTracks(String textSearch);
    
    //load track from playlist into either deck
    void loadIntoDeck(DeckGUI* deckGUI);

    //execute when closing the application to save tracks
    void saveLibraryWhenExiting();
    
    //execute when closing the application to load tracks
    void loadLibraryWhenExiting();
    
    //delete track from playlist using id
    void deleteFromTracks(int id);

private:

    TableListBox tableComponent;

    //store data in track list variable here as vector
    std::vector<TrackList> trackTitles; 

    //declare buttons
    TextButton loadButton{ "LOAD TRACKS" };
    TextEditor searchTracks;
    TextButton addToFirstDeck{ "ADD TO FIRST DECK" };
    TextButton addToSecondDeck{ "ADD TO SECOND DECK" };
    
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    DJAudioPlayer* playerForParsingMetaData;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
