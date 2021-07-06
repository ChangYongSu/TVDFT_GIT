//=============================================================================
// Copyright Langis Pitre 1998
// You may do whatever you want with this code, as long as you include this
// copyright notice in your implementation files.
//
// If you wish to add new classes to this collection, feel free to do so.
// But please send me your code so that I can update the collection.
//
// Comments and bug reports: lpitre@sympatico.ca
//
//=============================================================================
//=============================================================================
//                         CMixerBase
//                         CMixerFader
//                         CMixerSwitch
//                         CMixerNumber
//                         CMixerPeakMeter
//
// This collection of classes encapsulates part of the audio mixer services
// which control the routing of audio lines to a destination device for playing
// or recording, or to control volume and other audio effects.
//
// For more information on each of these classes, see their respective header
// and implementation files.
//
// It takes quite a bit of work just to get a simple volume control to work
// in an application. With these classes, all it takes is a few lines of code
// to get a control up and running.
// For example: the CMixerFader class is derived from CSliderCtrl, so it takes
// a slider control resource, and 3 lines of code (plus the include directive!)
// to have a fully functional volume control in a dialog box.
//
// Also supplied is a utility function you can use to determine the capabilities
// of your sound card and the available audio services (see MixerInfo.h).
//
// To use these classes, add this file and the source files to your project,
// include this file where necessary, and link with winmm.lib
//                                        -------------------
//=============================================================================
#ifndef _LANGIS_PITRE_MIXER_CLASSES_H_
#define _LANGIS_PITRE_MIXER_CLASSES_H_

#include "MixerBase.h"
#include "MixerFader.h"
#include "MixerSwitch.h"
#include "MixerNumber.h"
#include "MixerPeakMeter.h"


#endif // _LANGIS_PITRE_MIXER_CLASSES_H_

//===========================================================================
