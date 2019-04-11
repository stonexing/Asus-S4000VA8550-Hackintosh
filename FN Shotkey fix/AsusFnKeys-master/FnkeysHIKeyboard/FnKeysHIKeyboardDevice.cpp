/*
 *  Copyright (c) 2012 - 2013 EMlyDinEsH(OSXLatitude). All rights reserved.
 *  Copyright (c) 2018 hieplpvip
 *
 *
 *  FnKeysHIKeyboardDevice.cpp
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


#include "FnKeysHIKeyboardDevice.h"
#include "AsusFnKeys.h"

#if DEBUG
#define DEBUG_LOG(fmt, args...) IOLog(fmt, ## args)
#else
#define DEBUG_LOG(fmt, args...)
#endif

#define super IOService
OSDefineMetaClassAndStructors(FnKeysHIKeyboardDevice, IOService);


bool FnKeysHIKeyboardDevice::attach(IOService * provider )
{
    if (!super::attach(provider))  return false;
    
    FnKeys = OSDynamicCast(AsusFnKeys ,provider);
    if (NULL == FnKeys)
	    return false;
    
    FnKeys->retain();
    
    return true;
}

void FnKeysHIKeyboardDevice::detach(IOService * provider )
{
    OSSafeReleaseNULL(FnKeys);
    super::detach(provider);
}

void FnKeysHIKeyboardDevice::keyPressed(int code)
{
    int i = 0, out;
    do
    {
	    if (keyMap[i].description == NULL && keyMap[i].in == 0 && keyMap[i].out == 0xFF)
	    {
            DEBUG_LOG("%s::Unknown key %02X i=%d\n", getName(), code, i);
    	    break;
	    }
	    if (keyMap[i].in == code)
	    {
            DEBUG_LOG("%s::Key Pressed %02X i=%d\n", getName(), code, i);
    	    out = keyMap[i].out;
    	    messageClients(kIOACPIMessageDeviceNotification, &out);
    	    break;
	    }
	    i++;
    }
    while (true);    
}


void FnKeysHIKeyboardDevice::setKeyMap(const FnKeysKeyMap *_keyMap)
{
    int i = 0;
    keyMap = _keyMap;
    OSDictionary *dict = OSDictionary::withCapacity(10);
    DEBUG_LOG("%s::Setting key %02X i=%d\n", getName(), keyMap[i].in, i);
    do
    {
	    if (keyMap[i].description == NULL && keyMap[i].in == 0 && keyMap[i].out == 0xFF)
    	    break;
        DEBUG_LOG("%s::Setting key %02X i=%d\n", getName(), keyMap[i].in, i);
	    dict->setObject(keyMap[i].description, OSNumber::withNumber(keyMap[i].in,8));
	    i++;
    }
    while (true);
    
    setProperty("KeyMap", dict);
}
