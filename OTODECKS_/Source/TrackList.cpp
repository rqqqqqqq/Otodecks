/*
  ==============================================================================

    TrackList.cpp
    Created: 11 Mar 2021 3:36:34pm
    Author:  FOO RUI QI

  ==============================================================================
*/

#include "TrackList.h"
#include <filesystem>

TrackList::TrackList(File _file) :file(_file),
                                  title(_file.getFileNameWithoutExtension()),
                                  URL(juce::URL{ _file })
{
    DBG("New track with title created: ""<< title");
}

bool TrackList::operator==(const String& other) const
{
    return title == other;
}

