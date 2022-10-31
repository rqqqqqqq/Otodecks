/*
  ==============================================================================

    TrackList.h
    Created: 11 Mar 2021 3:36:34pm
    Author:  FOO RUI QI

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class TrackList
{
public:
    //create object
    TrackList(File _file);

    //object contains track file, track title and url
    File file;
    String title;
    URL URL;
    
    //return title of track
    bool operator==(const String& other) const;
    String length;
};