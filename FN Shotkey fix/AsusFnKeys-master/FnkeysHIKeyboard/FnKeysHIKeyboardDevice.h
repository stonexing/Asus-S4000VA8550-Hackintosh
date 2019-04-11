/*
 *  Copyright (c) 2012 - 2013 EMlyDinEsH(OSXLatitude). All rights reserved.
 *  Copyright (c) 2018 hieplpvip
 *
 *
 *  FnKeysHIKeyboardDevice.h
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

#ifndef _FnKeysHIKeyboardDevice_h
#define _FnKeysHIKeyboardDevice_h

#include <IOKit/IOService.h>

typedef struct  {
    UInt16 in;
    UInt8 out;
    const char *description;
} FnKeysKeyMap;

class AsusFnKeys;

class FnKeysHIKeyboardDevice : public IOService
{
    OSDeclareDefaultStructors(FnKeysHIKeyboardDevice);
    
private:
    AsusFnKeys *FnKeys;
    
public:
    virtual bool attach(IOService * provider);
    virtual void detach(IOService * provider);
    
    void keyPressed(int code);
    
    const FnKeysKeyMap * keyMap;
    void setKeyMap(const FnKeysKeyMap * _keyMap);
    
};

#endif //_FnKeysHIKeyboardDevice_h
