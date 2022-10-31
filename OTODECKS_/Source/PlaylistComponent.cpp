/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 3 Mar 2021 2:46:12pm
    Author:  FOO RUI QI

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "DJAudioPlayer.h"
#include "Tracklist.h"
#include <fStream>

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,
                                     DeckGUI* _deckGUI2,
                                     DJAudioPlayer* _playerForParsingMetaData
                                     ) : deckGUI1(_deckGUI1),
                                         deckGUI2(_deckGUI2),
                                         playerForParsingMetaData(_playerForParsingMetaData)
{
    //to add header and column into tablecomponent
    tableComponent.getHeader().addColumn("Track title", 1, 400);
    tableComponent.getHeader().addColumn("Track Length", 2, 200);
    tableComponent.getHeader().addColumn("Delete Track", 3, 200);
    tableComponent.setModel(this);

    //add components
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(searchTracks);
    addAndMakeVisible(addToFirstDeck);
    addAndMakeVisible(addToSecondDeck);

    //create button listeners
    loadButton.addListener(this);
    addToFirstDeck.addListener(this);
    addToSecondDeck.addListener(this);

    //searchField configuration
    searchTracks.setTextToShowWhenEmpty("Search for Track Title", juce::Colours::paleturquoise);
    searchTracks.setJustification(Justification::centred);

    //searchField.getText() is parameter for searchLibrary function
    searchTracks.onReturnKey = [this] {searchInLibrary(searchTracks.getText()); }; 

    loadLibraryWhenExiting();
}

PlaylistComponent::~PlaylistComponent()
{
    //execute when closing the application to save tracks
    saveLibraryWhenExiting();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    //fills the component's background and draws 

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);  
}

void PlaylistComponent::resized()
{
    //set bounds of for child components that your component contains

    tableComponent.setBounds(0, 0, getWidth(), getHeight());
    loadButton.setBounds(0, getWidth()/4, getWidth()/4, getHeight()/4);
    searchTracks.setBounds(getWidth() / 4, getWidth() / 4, getWidth() / 4, getHeight() / 4);
    addToFirstDeck.setBounds(getWidth() / 2, getWidth() / 4, getWidth() / 4, getHeight() / 4);
    addToSecondDeck.setBounds(getWidth() / 4*3, getWidth() / 4, getWidth() / 4, getHeight() / 4);
}

int PlaylistComponent::getNumRows() 
{
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(Colours::orange);
    }
    else
    {
        g.fillAll(Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    if (columnId == 1) 
    {
        g.drawText(trackTitles[rowNumber].title,
            2, 0,
            width - 4, height,
            Justification::centredLeft,
            true);
    }
    if (columnId == 2) 
    {
        g.drawText(trackTitles[rowNumber].length,
            2, 0,
            width - 4, height,
            Justification::centredLeft,
            true);
    }
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected,
    Component* existingComponentToUpdate)
    //creates text button within cells of column id 3
{
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "DELETE" };
            String id{std::to_string(rowNumber)};
            //convert int to string
            btn->setComponentID(id);
            //call setComponentID on the button
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &loadButton)
    {
        DBG("Load button was clicked");
        loadTrackToLibrary();
        tableComponent.updateContent();
    }
    else if (button == &addToFirstDeck)
    {
        DBG("Add to first deck button clicked");
        loadIntoDeck(deckGUI1);
    }
    else if (button == &addToSecondDeck)
    {
        DBG("Add to second deck button clicked");
        loadIntoDeck(deckGUI2);
    }
    else 
    {
        //convert JUCE string to standard string, then convert into int
        int id = std::stoi(button->getComponentID().toStdString());
        deleteFromTracks(id);
        tableComponent.updateContent();
    }
}


void PlaylistComponent::loadTrackToLibrary()
{
    //initialise file chooser
    FileChooser chooser{ "Select files" };
    if (chooser.browseForMultipleFilesToOpen())
    {
        for (const File& file : chooser.getResults())
        {
            juce::String trackTitle{ file.getFileNameWithoutExtension() };
            if (!trackIsInPlaylist(trackTitle))
            {
                //check newly created class tracklist 
                TrackList newTrack{ file }; 
                juce::URL audioURL{ file };
                newTrack.length = getTrackLength(audioURL);
                trackTitles.push_back(newTrack);
                DBG("loaded track: " << newTrack.title);
            }
            else 
            {
                //display info message
                AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
                    "Load information: ",
                    trackTitle + " has already been loaded",
                    "OK",
                    nullptr
                );
            }
        }
    }
}

bool PlaylistComponent::trackIsInPlaylist(juce::String fileName) {

    return (std::find(trackTitles.begin(), trackTitles.end(), fileName) != trackTitles.end());
}

String PlaylistComponent::getTrackLength(juce::URL audioURL)
{
    playerForParsingMetaData->loadURL(audioURL);
    double inSeconds{ playerForParsingMetaData->getLength() };

    //call function to change from sec to min and secs
    String minutesAndSeconds{ fromSecondsToMinutes(inSeconds) };
    return minutesAndSeconds;
}

String PlaylistComponent::fromSecondsToMinutes(double seconds)
{
    //find seconds and minutes and make into string
    int secondsRounded{ int(round(seconds)) };
    String second{ std::to_string(secondsRounded / 60) };
    String minute{ std::to_string(secondsRounded / 60) };

    if (second.length() < 2)//if seconds is 1 digit or less
    {
        //add '0' to seconds until seconds is length 2
        second = second.paddedLeft('0', 2);
    }
    return String{ minute + ":" + second };
}

void PlaylistComponent::searchInLibrary(String textSearch)
{
    DBG("Search in library for: " << textSearch);
    if (textSearch != "") 
    {
        int rowNumber = whereInTracks(textSearch);
        tableComponent.selectRow(rowNumber);
    }
    else 
    {
        tableComponent.deselectAllRows();
    }  
}

int PlaylistComponent::whereInTracks(juce::String textSearch)
{
    auto it = find_if(trackTitles.begin(), trackTitles.end(),
        [&textSearch](const TrackList& obj) {return obj.title.contains(textSearch); });
        int i = -1;

    if (it != trackTitles.end())
    {
        i = std::distance(trackTitles.begin(), it);
    }
    return i;
}

void PlaylistComponent::loadIntoDeck(DeckGUI* deckGUI)
{
    int selectedRow{ tableComponent.getSelectedRow() };
    if (selectedRow != -1)
    {
        DBG("Now adding: " << trackTitles[selectedRow].title << " to deck");
        deckGUI->loadFile(trackTitles[selectedRow].URL);
    }
}

void PlaylistComponent::saveLibraryWhenExiting()
{
    //create .csv to save library
    std::ofstream mylibrary("my-library.csv");

    //save library to file
    for (TrackList& t : trackTitles)
    {
        mylibrary << t.file.getFullPathName() << "," << t.length << "\n";
    }
}

void PlaylistComponent::loadLibraryWhenExiting()
{
    //create input stream from saved library
    std::ifstream mylibrary("my-library.csv");
    std::string filepath;
    std::string trackLength;

    //read data, line by line
    if (mylibrary.is_open())
    {
        while (getline(mylibrary, filepath, ','))
        {
            juce::File file{ filepath };
            TrackList newtrack{ file };

            std::getline(mylibrary, trackLength);
            newtrack.length = trackLength;
            trackTitles.push_back(newtrack);
        }
    }
    mylibrary.close();
}

void PlaylistComponent::deleteFromTracks(int id)
{
    trackTitles.erase(trackTitles.begin() + id);
}