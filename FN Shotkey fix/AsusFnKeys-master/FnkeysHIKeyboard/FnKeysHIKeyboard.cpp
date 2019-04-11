/*
 *  Copyright (c) 2012 - 2013 EMlyDinEsH(OSXLatitude). All rights reserved.
 *  Copyright (c) 2018 hieplpvip
 *
 *
 *  FnKeysHIDKeyboard.cpp
 *  
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <IOKit/acpi/IOACPIPlatformDevice.h>
#include <IOKit/hidsystem/ev_keymap.h>
#include "FnKeysHIKeyboard.h"

#if DEBUG
#define DEBUG_LOG(fmt, args...) IOLog(fmt, ## args)
#else
#define DEBUG_LOG(fmt, args...)
#endif

#define super IOHIKeyboard
OSDefineMetaClassAndStructors(FnKeysHIKeyboard, IOHIKeyboard)


#pragma mark -
#pragma mark IOService override
#pragma mark -

bool FnKeysHIKeyboard::init(OSDictionary *dictionary)
{
    return super::init(dictionary);
}

bool FnKeysHIKeyboard::start(IOService *provider)
{
    if(!provider || !super::start( provider ))
    {
        IOLog("%s::Failed to load..\n", getName());
        return false;
    }
    
    Device = (FnKeysHIKeyboardDevice *) provider;
    clock_get_system_microtime(&lastEventSecs,&lastEventMicrosecs);
    
    setProperty("Product", "Fn Keys Keyboard Driver for Asus");
    
    return true;
}

void FnKeysHIKeyboard::stop(IOService *provider)
{
    super::stop(provider);
}


void FnKeysHIKeyboard::free(void)
{
    super::free();
}

/*
 * Receive hotkey event (only down) and send keyboard down and up event
 * Limits the rate of event send to HID stack, otherwise the system slow down and the sound/sun bezel lags.
 */
IOReturn FnKeysHIKeyboard::message(UInt32 type, IOService * provider, void * argument)
{
    if (type == kIOACPIMessageDeviceNotification)
    {
        UInt32 code = *((UInt32 *) argument);
        
        
        AbsoluteTime now;
        clock_get_uptime((uint64_t *)(&now));
        dispatchKeyboardEvent(code,
                              /*direction*/ true,
                              /*timeStamp*/ now);
        
        clock_get_uptime((uint64_t *)(&now));
        dispatchKeyboardEvent(code,
                              /*direction*/ false,
                              /*timeStamp*/ now);
    }
    else
    {
        DEBUG_LOG("%s::Unexpected message: %u Type %x Provider %s \n", getName(), *((UInt32 *) argument), uint(type), provider->getName());
    }
    return kIOReturnSuccess;
}

#pragma mark -
#pragma mark IOHIKeyboard override
#pragma mark -

//====================================================================================================
// defaultKeymapOfLength - IOHIKeyboard override
// This allows us to associate the scancodes we choose with the special
// keys we are interested in posting later. This gives us auto-repeats for free. Kewl.
//====================================================================================================
const unsigned char * FnKeysHIKeyboard::defaultKeymapOfLength( UInt32 * length )
{
    static const unsigned char ConsumerKeyMap[] =
    {
        // The first 16 bits are always read first, to determine if the rest of
        // the keymap is in shorts (16 bits) or bytes (8 bits). If the first 16 bits
        // equals 0, data is in bytes; if first 16 bits equal 1, data is in shorts.
        
        0x00,0x00,        // data is in bytes
        
        // The next value is the number of modifier keys. We have none in our driver.
        
        0x00,
        
        // The next value is number of key definitions. We have none in our driver.
        
        0x00,
        
        // The next value is number of of sequence definitions there are. We have none.
        
        0x00,
        
        // The next value is the number of special keys. We use these.
        
        NX_NUMSPECIALKEYS,
        
        // Special Key          SCANCODE
        //-----------------------------------------------------------        
        
        NX_KEYTYPE_SOUND_UP,            NX_KEYTYPE_SOUND_UP,
        NX_KEYTYPE_SOUND_DOWN,            NX_KEYTYPE_SOUND_DOWN,
        NX_KEYTYPE_BRIGHTNESS_UP,        NX_KEYTYPE_BRIGHTNESS_UP,
        NX_KEYTYPE_BRIGHTNESS_DOWN,        NX_KEYTYPE_BRIGHTNESS_DOWN,
        NX_KEYTYPE_CAPS_LOCK,            NX_KEYTYPE_CAPS_LOCK,
        NX_KEYTYPE_HELP,                NX_KEYTYPE_HELP,
        NX_POWER_KEY,                    NX_POWER_KEY,
        NX_KEYTYPE_MUTE,                NX_KEYTYPE_MUTE,
        NX_UP_ARROW_KEY,                NX_UP_ARROW_KEY,
        NX_DOWN_ARROW_KEY,                NX_DOWN_ARROW_KEY,
        NX_KEYTYPE_NUM_LOCK,            NX_KEYTYPE_NUM_LOCK,
        NX_KEYTYPE_CONTRAST_UP,            NX_KEYTYPE_CONTRAST_UP,
        NX_KEYTYPE_CONTRAST_DOWN,        NX_KEYTYPE_CONTRAST_DOWN,
        NX_KEYTYPE_LAUNCH_PANEL,        NX_KEYTYPE_LAUNCH_PANEL,
        NX_KEYTYPE_EJECT,                NX_KEYTYPE_EJECT,
        NX_KEYTYPE_VIDMIRROR,            NX_KEYTYPE_VIDMIRROR,
        NX_KEYTYPE_PLAY,                NX_KEYTYPE_PLAY,
        NX_KEYTYPE_NEXT,                NX_KEYTYPE_NEXT,
        NX_KEYTYPE_PREVIOUS,            NX_KEYTYPE_PREVIOUS,
        NX_KEYTYPE_FAST,                NX_KEYTYPE_FAST,
        NX_KEYTYPE_REWIND,                NX_KEYTYPE_REWIND,
        NX_KEYTYPE_ILLUMINATION_UP,        NX_KEYTYPE_ILLUMINATION_UP,
        NX_KEYTYPE_ILLUMINATION_DOWN,    NX_KEYTYPE_ILLUMINATION_DOWN,
        NX_KEYTYPE_ILLUMINATION_TOGGLE,    NX_KEYTYPE_ILLUMINATION_TOGGLE
        
    };
    
    if(length) *length = sizeof(ConsumerKeyMap);
    
    return(ConsumerKeyMap );
}
