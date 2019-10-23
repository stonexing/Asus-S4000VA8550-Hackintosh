/*
 *  Copyright (c) 2012 - 2013 EMlyDinEsH(OSXLatitude). All rights reserved.
 *  Copyright (c) 2018 hieplpvip
 *
 *  Asus Fn keys Driver v1.0.2 by hieplpvip for macOS 10.13
 *
 *  Credits: EMlyDinEsH(OSXLatitude) for initial driver
 *
 *  AsusFnKeys.cpp
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


#include "AsusFnKeys.h"


#if DEBUG
#define DEBUG_LOG(fmt, args...) IOLog(fmt, ## args)
#else
#define DEBUG_LOG(fmt, args...)
#endif

#ifdef DEBUG
/*
 * GUID parsing functions
 */
/**
 * wmi_parse_hexbyte - Convert a ASCII hex number to a byte
 * @src:  Pointer to at least 2 characters to convert.
 *
 * Convert a two character ASCII hex string to a number.
 *
 * Return:  0-255  Success, the byte was parsed correctly
 *          -1     Error, an invalid character was supplied
 */
int AsusFnKeys::wmi_parse_hexbyte(const UInt8 *src)
{
    unsigned int x; /* For correct wrapping */
    int h;
    
    /* high part */
    x = src[0];
    if (x - '0' <= '9' - '0') {
        h = x - '0';
    } else if (x - 'a' <= 'f' - 'a') {
        h = x - 'a' + 10;
    } else if (x - 'A' <= 'F' - 'A') {
        h = x - 'A' + 10;
    } else {
        return -1;
    }
    h <<= 4;
    
    /* low part */
    x = src[1];
    if (x - '0' <= '9' - '0')
        return h | (x - '0');
    if (x - 'a' <= 'f' - 'a')
        return h | (x - 'a' + 10);
    if (x - 'A' <= 'F' - 'A')
        return h | (x - 'A' + 10);
    return -1;
}

/**
 * wmi_swap_bytes - Rearrange GUID bytes to match GUID binary
 * @src:   Memory block holding binary GUID (16 bytes)
 * @dest:  Memory block to hold byte swapped binary GUID (16 bytes)
 *
 * Byte swap a binary GUID to match it's real GUID value
 */
void AsusFnKeys::wmi_swap_bytes(UInt8 *src, UInt8 *dest)
{
    int i;
    
    for (i = 0; i <= 3; i++)
        memcpy(dest + i, src + (3 - i), 1);
    
    for (i = 0; i <= 1; i++)
        memcpy(dest + 4 + i, src + (5 - i), 1);
    
    for (i = 0; i <= 1; i++)
        memcpy(dest + 6 + i, src + (7 - i), 1);
    
    memcpy(dest + 8, src + 8, 8);
}

/**
 * wmi_parse_guid - Convert GUID from ASCII to binary
 * @src:   36 char string of the form fa50ff2b-f2e8-45de-83fa-65417f2f49ba
 * @dest:  Memory block to hold binary GUID (16 bytes)
 *
 * N.B. The GUID need not be NULL terminated.
 *
 * Return:  'true'   @dest contains binary GUID
 *          'false'  @dest contents are undefined
 */
bool AsusFnKeys::wmi_parse_guid(const UInt8 *src, UInt8 *dest)
{
    static const int size[] = { 4, 2, 2, 2, 6 };
    int i, j, v;
    
    if (src[8]  != '-' || src[13] != '-' ||
        src[18] != '-' || src[23] != '-')
        return false;
    
    for (j = 0; j < 5; j++, src++) {
        for (i = 0; i < size[j]; i++, src += 2, *dest++ = v) {
            v = wmi_parse_hexbyte(src);
            if (v < 0)
                return false;
        }
    }
    
    return true;
}

/**
 * wmi_dump_wdg - dumps tables to dmesg
 * @src: guid_block *
 */
void AsusFnKeys::wmi_dump_wdg(struct guid_block *g)
{
    char guid_string[37];
    
    wmi_data2Str(g->guid, guid_string);
    DEBUG_LOG("%s:\n", guid_string);
    if (g->flags & ACPI_WMI_EVENT)
        DEBUG_LOG("\tnotify_value: %02X\n", g->notify_id);
    else
        DEBUG_LOG("\tobject_id: %c%c\n",g->object_id[0], g->object_id[1]);
    DEBUG_LOG("\tinstance_count: %d\n", g->instance_count);
    DEBUG_LOG("\tflags: %#x", g->flags);
    if (g->flags) {
        DEBUG_LOG(" ");
        if (g->flags & ACPI_WMI_EXPENSIVE)
            DEBUG_LOG("ACPI_WMI_EXPENSIVE ");
        if (g->flags & ACPI_WMI_METHOD)
            DEBUG_LOG("ACPI_WMI_METHOD ");
        if (g->flags & ACPI_WMI_STRING)
            DEBUG_LOG("ACPI_WMI_STRING ");
        if (g->flags & ACPI_WMI_EVENT)
            DEBUG_LOG("ACPI_WMI_EVENT ");
    }
}
#endif

/**
 * wmi_data2Str - converts binary guid to ascii guid
 *
 */
int AsusFnKeys::wmi_data2Str(const char *in, char *out)
{
    int i;
    
    for (i = 3; i >= 0; i--)
        out += snprintf(out, 3, "%02X", in[i] & 0xFF);
    
    out += snprintf(out, 2, "-");
    out += snprintf(out, 3, "%02X", in[5] & 0xFF);
    out += snprintf(out, 3, "%02X", in[4] & 0xFF);
    out += snprintf(out, 2, "-");
    out += snprintf(out, 3, "%02X", in[7] & 0xFF);
    out += snprintf(out, 3, "%02X", in[6] & 0xFF);
    out += snprintf(out, 2, "-");
    out += snprintf(out, 3, "%02X", in[8] & 0xFF);
    out += snprintf(out, 3, "%02X", in[9] & 0xFF);
    out += snprintf(out, 2, "-");
    
    for (i = 10; i <= 15; i++)
        out += snprintf(out, 3, "%02X", in[i] & 0xFF);
    
    *out = '\0';
    return 0;
}

/**
 * flagsToStr - converts binary flag to ascii flag
 *
 */
OSString * AsusFnKeys::flagsToStr(UInt8 flags)
{
    char str[80];
    char *pos = str;
    if (flags != 0)
    {
        if (flags & ACPI_WMI_EXPENSIVE)
        {
            strncpy(pos, "ACPI_WMI_EXPENSIVE ", 20);
            pos += strlen(pos);
        }
        if (flags & ACPI_WMI_METHOD)
        {
            strncpy(pos, "ACPI_WMI_METHOD ", 20);
            pos += strlen(pos);
            DEBUG_LOG("%s::WMI METHOD\n", getName());
        }
        if (flags & ACPI_WMI_STRING)
        {
            strncpy(pos, "ACPI_WMI_STRING ", 20);
            pos += strlen(pos);
        }
        if (flags & ACPI_WMI_EVENT)
        {
            strncpy(pos, "ACPI_WMI_EVENT ", 20);
            pos += strlen(pos);
            DEBUG_LOG("%s::WMI EVENT\n", getName());
        }
        //suppress the last trailing space
        str[strlen(str)] = 0;
    }
    else
    {
        str[0] = 0;
    }
    return (OSString::withCString(str));
}

/**
 * wmi_wdg2reg - adds the WDG structure to a dictionary
 *
 */
void AsusFnKeys::wmi_wdg2reg(struct guid_block *g, OSArray *array, OSArray *dataArray)
{
    char guid_string[37];
    char object_id_string[3];
    OSDictionary *dict = OSDictionary::withCapacity(6);
    
    wmi_data2Str(g->guid, guid_string);
    
    dict->setObject("UUID", OSString::withCString(guid_string));
    if (g->flags & ACPI_WMI_EVENT)
        dict->setObject("notify_value", OSNumber::withNumber(g->notify_id, 8));
    else
    {
        snprintf(object_id_string, 3, "%c%c", g->object_id[0], g->object_id[1]);
        dict->setObject("object_id", OSString::withCString(object_id_string));
    }
    dict->setObject("instance_count", OSNumber::withNumber(g->instance_count, 8));
    dict->setObject("flags", OSNumber::withNumber(g->flags, 8));
#if DEBUG
    dict->setObject("flags Str", flagsToStr(g->flags));
#endif
    if (g->flags == 0)
        dataArray->setObject(readDataBlock(object_id_string));
    
    
    array->setObject(dict);
}

OSDictionary * AsusFnKeys::readDataBlock(char *str)
{
    OSObject    *wqxx;
    OSData        *data = NULL;
    OSDictionary *dict;
    char name[5];
    
    snprintf(name, 5, "WQ%s", str);
    dict = OSDictionary::withCapacity(1);
    
    do
    {
        if (WMIDevice->evaluateObject(name, &wqxx) != kIOReturnSuccess)
        {
            IOLog("%s::No object of method %s\n", getName(), name);
            continue;
        }
        
        data = OSDynamicCast(OSData , wqxx);
        if (data == NULL){
            IOLog("%s::Cast error %s\n", getName(), name);
            continue;
        }
        dict->setObject(name, data);
    }
    while (false);
    return dict;
}

/*
 * Parse the _WDG method for the GUID data blocks
 */
int AsusFnKeys::parse_wdg(OSDictionary *dict)
{
    UInt32 i, total;
    OSObject    *wdg;
    OSData        *data;
    OSArray        *array, *dataArray;
    
    do
    {
        if (WMIDevice->evaluateObject("_WDG", &wdg) != kIOReturnSuccess)
        {
            IOLog("%s::No object of method _WDG\n", getName());
            continue;
        }
        
        data = OSDynamicCast(OSData , wdg);
        if (data == NULL){
            IOLog("%s::Cast error _WDG\n", getName());
            continue;
        }
        total = data->getLength() / sizeof(struct guid_block);
        array = OSArray::withCapacity(total);
        dataArray = OSArray::withCapacity(1);
        
        for (i = 0; i < total; i++) {
            wmi_wdg2reg((struct guid_block *) data->getBytesNoCopy(i * sizeof(struct guid_block), sizeof(struct guid_block)), array, dataArray);
        }
        setProperty("WDG", array);
        setProperty("DataBlocks", dataArray);
        data->release();
    }
    while (false);
    
    return 0;
}

#pragma mark -
#pragma mark IOService overloading
#pragma mark -

#define super IOService

OSDefineMetaClassAndStructors(AsusFnKeys, IOService)

const FnKeysKeyMap AsusFnKeys::keyMap[] = {
    {0x30, NX_KEYTYPE_SOUND_UP, "NX_KEYTYPE_SOUND_UP"},
    {0x31, NX_KEYTYPE_SOUND_DOWN, "NX_KEYTYPE_SOUND_DOWN"},
    {0x32, NX_KEYTYPE_MUTE, "NX_KEYTYPE_MUTE"},
    {0x61, NX_KEYTYPE_VIDMIRROR, "NX_KEYTYPE_VIDMIRROR"},
    {0x10, NX_KEYTYPE_BRIGHTNESS_UP, "NX_KEYTYPE_BRIGHTNESS_UP"},
    {0x20, NX_KEYTYPE_BRIGHTNESS_DOWN, "NX_KEYTYPE_BRIGHTNESS_DOWN"},
    // Media buttons bound to Asus events keys Down, Left and Right Arrows in full keyboard
    {0x40, NX_KEYTYPE_PREVIOUS, "NX_KEYTYPE_PREVIOUS"},
    {0x41, NX_KEYTYPE_NEXT, "NX_KEYTYPE_NEXT"},
    {0x45, NX_KEYTYPE_PLAY, "NX_KEYTYPE_PLAY"},
    // Media button bound to Asus events keys C, V and Space keys in compact keyboard
    {0x8A, NX_KEYTYPE_PREVIOUS, "NX_KEYTYPE_PREVIOUS"},
    {0x82, NX_KEYTYPE_NEXT, "NX_KEYTYPE_NEXT"},
    {0x5C, NX_KEYTYPE_PLAY, "NX_KEYTYPE_PLAY"},
    {0,0xFF,NULL}
};

bool AsusFnKeys::init(OSDictionary *dict)
{
    keybrdBLight16 = false;
    keybrdBLightLvl = 0; // Stating with Zero Level
    panelBrightnessLevel = 16; // Mac starts with level 16
    
    touchpadEnabled = true; // touch enabled by default on startup
    isALSenabled  = false;
    isPanelBackLightOn = true;
    hasKeybrdBLight = false;
    hasMediaButtons = true;
    
    isautoOff = false;
    autoOffEnable = true;
    
    _notificationServices = OSSet::withCapacity(1);
    
    kev.setVendorID("com.hieplpvip");
    kev.setEventCode(AsusFnKeysEventCode);
    
    bool result = super::init(dict);
    properties = dict;
    DEBUG_LOG("%s::Init\n", getName());
    return result;
}

void AsusFnKeys::free(void)
{
    DEBUG_LOG("%s::Free\n", getName());
    super::free();
}

IOService * AsusFnKeys::probe(IOService *provider, SInt32 *score)
{
    IOService * ret = NULL;
    OSObject * obj;
    OSString * name;
    IOACPIPlatformDevice *dev;
    do
    {
        if (!super::probe(provider, score))
            continue;
        
        dev = OSDynamicCast(IOACPIPlatformDevice, provider);
        if (NULL == dev)
            continue;
        
        dev->evaluateObject("_UID", &obj);
        
        name = OSDynamicCast(OSString, obj);
        if (NULL == name)
            continue;
        
        if (name->isEqualTo("ATK"))
        {
            *score +=20;
            ret = this;
        }
        name->release();
    }
    while (false);
    
    return (ret);
}

bool AsusFnKeys::start(IOService *provider)
{
    if(!provider || !super::start( provider ))
    {
        IOLog("%s::Error loading kext\n", getName());
        return false;
    }
    
    WMIDevice = (IOACPIPlatformDevice *) provider;
    
    IOLog("%s::Found WMI Device %s\n", getName(), WMIDevice->getName());
    
    _keyboardDevice = NULL;
    
    parse_wdg(properties);
    
    parseConfig();
    
    enableEvent();
    
    PMinit();
    registerPowerDriver(this, powerStateArray, kAsusFnKeysIOPMNumberPowerStates);
    provider->joinPMtree(this);
    
    this->registerService(0);
    
    OSDictionary * propertyMatch = propertyMatching(OSSymbol::withCString(kDeliverNotifications), OSBoolean::withBoolean(true));
    
    IOServiceMatchingNotificationHandler notificationHandler = OSMemberFunctionCast(IOServiceMatchingNotificationHandler, this, &AsusFnKeys::notificationHandler);
    
    //
    // Register notifications for availability of any IOService objects wanting to consume our message events
    //
    _publishNotify = addMatchingNotification(gIOFirstPublishNotification,
                                             propertyMatch,
                                             notificationHandler,
                                             this,
                                             0, 10000);
    
    _terminateNotify = addMatchingNotification(gIOTerminatedNotification,
                                               propertyMatch,
                                               notificationHandler,
                                               this,
                                               0, 10000);
    
    propertyMatch->release();
    
    _workLoop = getWorkLoop();
    if (!_workLoop){
        DEBUG_LOG("%s::Failed to get workloop!\n", getName());
        return false;
    }
    _workLoop->retain();
    
    command_gate = IOCommandGate::commandGate(this);
    if (!command_gate) {
        return false;
    }
    _workLoop->addEventSource(command_gate);
    
    if (autoOffEnable && hasKeybrdBLight)
    {
        resetTimer();
        keytime += 1000000000; // small hack to avoid auto off on start
        
        _autoOffTimer = IOTimerEventSource::timerEventSource(this, OSMemberFunctionCast(IOTimerEventSource::Action, this, &AsusFnKeys::autoOffTimer));
        _workLoop->addEventSource(_autoOffTimer);
        _autoOffTimer->setTimeoutMS(500);
        
        if (!_autoOffTimer)
            return false;
    }
    
    return true;
}

void AsusFnKeys::stop(IOService *provider)
{
    DEBUG_LOG("%s::Stop\n", getName());
    
    if (_autoOffTimer){
        _autoOffTimer->cancelTimeout();
    }
    OSSafeReleaseNULL(_autoOffTimer);
    
    _workLoop->removeEventSource(command_gate);
    OSSafeReleaseNULL(command_gate);
    OSSafeReleaseNULL(_workLoop);
    
    disableEvent();
    PMstop();
    
    _publishNotify->remove();
    _terminateNotify->remove();
    OSSafeReleaseNULL(_publishNotify);
    OSSafeReleaseNULL(_terminateNotify);
    _notificationServices->flushCollection();
    OSSafeReleaseNULL(_notificationServices);
    
    super::stop(provider);
    return;
}

IOReturn AsusFnKeys::setPowerState(unsigned long powerStateOrdinal, IOService *whatDevice)
{
    if (whatDevice != this)
        return IOPMAckImplied;
    
    if (!powerStateOrdinal)
    {
        DEBUG_LOG("%s::Going to sleep\n", getName());
        if (_autoOffTimer){
            _autoOffTimer->cancelTimeout();
        }
        OSSafeReleaseNULL(_autoOffTimer);
    }
    else
    {
        DEBUG_LOG("%s::Woke up from sleep\n", getName());
        IOSleep(1000);
        _keyboardDevice->keyPressed(0);
        
        // Restore keyboard backlight
        if(hasKeybrdBLight && keybrdBLightLvl >= 0)
        {
            setKeyboardBackLight(keybrdBLightLvl);
            DEBUG_LOG("%s::Restore keyboard backlight %d\n", getName(), keybrdBLightLvl);
        }
        
        if (autoOffEnable && hasKeybrdBLight)
        {
            resetTimer();
            
            if (_autoOffTimer){
                _autoOffTimer->cancelTimeout();
            }
            OSSafeReleaseNULL(_autoOffTimer);
            
            _autoOffTimer = IOTimerEventSource::timerEventSource(this, OSMemberFunctionCast(IOTimerEventSource::Action, this, &AsusFnKeys::autoOffTimer));
            _workLoop->addEventSource(_autoOffTimer);
            _autoOffTimer->setTimeoutMS(500);
        }
    }
    
    return IOPMAckImplied;
}

#pragma mark -
#pragma mark AsusFnKeys Methods
#pragma mark -

void AsusFnKeys::parseConfig()
{
    // Detect keyboard backlight support
    if (WMIDevice->validateObject("SKBL") == kIOReturnSuccess && WMIDevice->validateObject("GKBL") == kIOReturnSuccess)
    {
        hasKeybrdBLight = true;
        // Detect keyboard backlight levels
        if (WMIDevice->validateObject("KBPW") == kIOReturnSuccess)
            keybrdBLight16 = true;
        else
            keybrdBLight16 = false;
        IOLog("%s::Keyboard backlight is supported with %d levels\n", getName(), keybrdBLight16?16:4);
    }
    else
    {
        hasKeybrdBLight = false;
        DEBUG_LOG("%s::Keyboard backlight is not supported\n", getName());
    }
    
    // Detect ALS sensor
    if (WMIDevice->validateObject("ALSC") == kIOReturnSuccess && WMIDevice->validateObject("ALSS") == kIOReturnSuccess)
    {
        hasALSensor = true;
        IOLog("%s::Found ALS sensor\n", getName());
    }
    else
    {
        hasALSensor = false;
        DEBUG_LOG("%s::No ALS sensors were found\n", getName());
    }
    
    // Reading the prefereces from the plist file
    OSDictionary *Configuration;
    Configuration = OSDynamicCast(OSDictionary, getProperty("Preferences"));
    if (Configuration){
        OSNumber *tmpNumber = 0;
        OSBoolean *tmpBoolean = FALSE;
        
        OSIterator *iter = 0;
        const OSSymbol *dictKey = 0;
        
        iter = OSCollectionIterator::withCollection(Configuration);
        if (iter) {
            while ((dictKey = (const OSSymbol *)iter->getNextObject())) {
                tmpNumber = OSDynamicCast(OSNumber, Configuration->getObject(dictKey));
                tmpBoolean = OSDynamicCast(OSBoolean, Configuration->getObject(dictKey));
                
                const char *tmpStr = dictKey->getCStringNoCopy();
                
                if (tmpNumber) {
                    if(!strncmp(tmpStr, "KeyboardBLightLevelAtBoot", strlen(tmpStr)))
                        keybrdBLightLvl = tmpNumber->unsigned8BitValue();
                    
                    else if(!strncmp(tmpStr, "IdleKBacklightAutoOffTimeout", strlen(tmpStr)))
                        autoOffTimeout = tmpNumber->unsigned64BitValue() * 1000000;
                }
                
                if (tmpBoolean)
                {
                    if(!strncmp(tmpStr, "HasMediaButtons", strlen(tmpStr)))
                        hasMediaButtons = tmpBoolean->getValue();
                    
                    else if(!strncmp(tmpStr, "IdleKBacklightAutoOff", strlen(tmpStr)))
                        autoOffEnable = tmpBoolean->getValue();
                }
            }
        }
    }
}

IOReturn AsusFnKeys::message(UInt32 type, IOService * provider, void * argument)
{
    if (type == kKeyboardKeyPressTime || type == kKeyboardModifierKeyPressTime)
    {
        DEBUG_LOG("%s::keyPressed时间类型 = %llu\n", getName(), keytime);
        keytime = *((uint64_t*)argument);
        if (isautoOff && keybrdBLightLvl)
        {
            setKeyboardBackLight(keybrdBLightLvl);
            isautoOff = false;
        }
    }
    else if (type == kIOACPIMessageDeviceNotification)
    {
        DEBUG_LOG("%s::设备通知类型\n", getName());
        UInt32 event = *((UInt32 *) argument);
        OSObject * wed;
        
        OSNumber * number = OSNumber::withNumber(event,32);
        WMIDevice->evaluateObject("_WED", &wed, (OSObject**)&number,1);
        number->release();
        number = OSDynamicCast(OSNumber, wed);
        if (NULL == number)
        {
            // try a package
            OSArray * array = OSDynamicCast(OSArray, wed);
            if (NULL == array)
            {
                // try a buffer
                OSData * data = OSDynamicCast(OSData, wed);
                if ( (NULL == data) || (data->getLength() == 0))
                {
                    DEBUG_LOG("%s::Fail to cast _WED returned objet %s\n", getName(), wed->getMetaClass()->getClassName());
                    return kIOReturnError;
                }
                const char * bytes = (const char *) data->getBytesNoCopy();
                number = OSNumber::withNumber(bytes[0],32);
            }
            else
            {
                number = OSDynamicCast(OSNumber, array->getObject(0));
                if (NULL == number)
                {
                    DEBUG_LOG("%s::Fail to cast _WED returned 1st objet in array %s\n", getName(), array->getObject(0)->getMetaClass()->getClassName());
                    return kIOReturnError;
                }
            }
        }
        
        handleMessage(number->unsigned32BitValue());
    }
    else
    {
        UInt32 message = *((UInt32 *) argument);
        uint utype = uint(type);
        if(utype == 0xe3ff8065){ //截屏键PrintScreen调用
            handleMessage(0xC3);
            //system("/usr/sbin/screencapture -u -U -W /dev/null");
        }
        DEBUG_LOG("%s::Unexpected message: %u Type %x Provider %s \n", getName(), message, utype, provider->getName());
    }
    
    return kIOReturnSuccess;
}

void AsusFnKeys::autoOffTimer()
{
    uint64_t now_abs;
    clock_get_uptime(&now_abs);
    uint64_t now_ns;
    absolutetime_to_nanoseconds(now_abs, &now_ns);
    
    DEBUG_LOG("%s::autoOffTimer %llu\n", getName(), now_ns - keytime);
    if (now_ns - keytime > autoOffTimeout && !isautoOff)
    {
        keybrdBLightLvl = getKeyboardBackLight();
        if (keybrdBLightLvl>0) setKeyboardBackLight(0, false);
        isautoOff = true;
    }
    
    _autoOffTimer->setTimeoutMS(500);
}

void AsusFnKeys::resetTimer()
{
    uint64_t now_abs;
    clock_get_uptime(&now_abs);
    absolutetime_to_nanoseconds(now_abs, &keytime);
    isautoOff = false;
}

void AsusFnKeys::handleMessage(int code)
{
    loopCount = 0;
    bool show = false;
    
    resetTimer();
    
    // Processing the code
    DEBUG_LOG("%s::handleMessage(%d)\n", getName(), code);
    switch (code) {
        case 0x57: // AC disconnected
        case 0x58: // AC connected
            // ignore silently
            break;
            
        // Backlight
        case 0x33:// hardwired On
        case 0x34:// hardwired Off
        case 0x35:// Soft Event, Fn + F7
            if(isPanelBackLightOn)
            {
                code = NOTIFY_BRIGHTNESS_DOWN_MIN;
                loopCount = 16;
                
                // Read Panel brigthness value to restore later with backlight toggle
                readPanelBrightnessValue();
            }
            else
            {
                code = NOTIFY_BRIGHTNESS_UP_MIN;
                loopCount = panelBrightnessLevel;
            }
            
            isPanelBackLightOn = !isPanelBackLightOn;
            break;
            
        case 0x6B: // Fn + F9, Touchpad On/Off
            touchpadEnabled = !touchpadEnabled;
            if(touchpadEnabled)
            {
                setProperty("TouchpadEnabled", true);
                removeProperty("TouchpadDisabled");
                DEBUG_LOG("%s::Touchpad Enabled\n", getName());
            }
            else
            {
                removeProperty("TouchpadEnabled");
                setProperty("TouchpadDisabled", true);
                DEBUG_LOG("%s::Touchpad Disabled\n", getName());
            }
            
            // send to 3rd party drivers
            dispatchMessage(kKeyboardSetTouchStatus, &touchpadEnabled);
            break;
            
        case 0x5C: // Fn + Space bar, Processor Speedstepping changes
            
            /*params[0] = OSNumber::withNumber(4, 8);
             
             if(WMIDevice->evaluateInteger("PSTT", &res, params, 1))
             IOLog("%s::Processor speedstep Changed\n", getName());
             else
             IOLog("%s::Processor speedstep change failed %d\n", getName(), res);*/
            
            break;
            
        case 0x5E:
            kev.sendMessage(kevSleep, 0, 0);
            break;
            
        case 0x7A: // Fn + A, ALS Sensor
            if(hasALSensor)
            {
                isALSenabled = !isALSenabled;
                enableALS(isALSenabled);
            }
            break;
            
        case 0x7D: // Airplane mode
            kev.sendMessage(kevAirplaneMode, 0, 0);
            break;
            
        case 0xC6:
        case 0xC7: // ALS Notifcations
            if(hasALSensor)
            {
                UInt32 alsValue = 0;
                WMIDevice->evaluateInteger("ALSS", &alsValue, NULL, NULL);
                DEBUG_LOG("%s::ALS %d\n", getName(), alsValue);
            }
            break;
            
        case 0xC5: // Fn + F3, Decrease Keyboard Backlight
            if(hasKeybrdBLight)
            {
                if(keybrdBLightLvl>0)
                    keybrdBLightLvl--;
                else
                    keybrdBLightLvl = 0;
                show = true;
            }
            break;
            
        case 0xC4: // Fn + F4, Increase Keyboard Backlight
            if(hasKeybrdBLight)
            {
                if(keybrdBLight16)
                {
                    if(keybrdBLightLvl < 16)
                        keybrdBLightLvl++;
                    else
                        keybrdBLightLvl = 16;
                }
                else
                {
                    if(keybrdBLightLvl < 3)
                        keybrdBLightLvl++;
                    else
                        keybrdBLightLvl = 3;
                }
                show = true;
            }
            break;
            
        case 0xC3 : // 自定义截屏 printScreen按钮
            //system("/usr/sbin/screen* -u -U -W /dev/null");
            kev.sendMessage(keyPrintScreen, 0, 0);
            DEBUG_LOG("%s::发送截屏事件\n", getName());
            return;
            break;
            
        default:
            // Fn + F5, Panel Brightness Down
            if(code >= NOTIFY_BRIGHTNESS_DOWN_MIN && code<= NOTIFY_BRIGHTNESS_DOWN_MAX)
            {
                code = NOTIFY_BRIGHTNESS_DOWN_MIN;
                
                if(panelBrightnessLevel > 0)
                    panelBrightnessLevel--;
            }
            // Fn + F6, Panel Brightness Up
            else if(code >= NOTIFY_BRIGHTNESS_UP_MIN && code<= NOTIFY_BRIGHTNESS_UP_MAX)
            {
                code = NOTIFY_BRIGHTNESS_UP_MIN;
                
                panelBrightnessLevel++;
                if(panelBrightnessLevel>16)
                    panelBrightnessLevel = 16;
            }
            break;
    }
    
    DEBUG_LOG("%s::Received Key %d(0x%x)\n", getName(), code, code);
    
    if (hasKeybrdBLight)
        setKeyboardBackLight(keybrdBLightLvl, true, show);
    
    // Sending the code for the keyboard handler
    processFnKeyEvents(code, loopCount);
}

//
// Process Fn key event
//
void AsusFnKeys::processFnKeyEvents(int code, int bLoopCount)
{
    if(bLoopCount>0)
    {
        for (int j = 0; j < bLoopCount; j++)
            _keyboardDevice->keyPressed(code);
        DEBUG_LOG("%s::Loop Count %d, Dispatch Key %d(0x%x)\n", getName(), bLoopCount, code, code);
    }
    else
    {
        _keyboardDevice->keyPressed(code);
        DEBUG_LOG("%s::Dispatch Key %d(0x%x)\n", getName(), code, code);
    }
}

void AsusFnKeys::enableALS(bool state)
{
    OSObject * params[1];
    UInt32 res;
    params[0] = OSNumber::withNumber(state, 8);
    
    if(WMIDevice->evaluateInteger("ALSC", &res, params, 1) == kIOReturnSuccess)
        DEBUG_LOG("%s::ALS %s %d\n", getName(), state ? "enabled" : "disabled", res);
    else
        DEBUG_LOG("%s::Failed to call ALSC\n", getName());
}

UInt8 AsusFnKeys::getKeyboardBackLight()
{
    if (WMIDevice->validateObject("GKBL") != kIOReturnSuccess)
    {
        DEBUG_LOG("%s::Keyboard backlight not found\n", getName());
        return -1;
    }
    else
    {
        OSObject * params[1];
        UInt32 res;
        params[0] = OSNumber::withNumber(0ULL, 8);
        
        if (WMIDevice->evaluateInteger("GKBL", &res, params, 1) != kIOReturnSuccess)
        {
            DEBUG_LOG("%s::Failed to get keyboard backlight\n", getName());
            return -1;
        }
        
        return res;
    }
}

void AsusFnKeys::setKeyboardBackLight(UInt8 level, bool nvram, bool display)
{
    if (WMIDevice->validateObject("SKBL") != kIOReturnSuccess)
        DEBUG_LOG("%s::Keyboard backlight not found\n", getName());
    else
    {
        OSObject * params[1];
        OSObject * ret = NULL;
        params[0] = OSNumber::withNumber(level, sizeof(level)*8);
        
        if (WMIDevice->evaluateObject("SKBL", &ret, params, 1) != kIOReturnSuccess)
        {
            DEBUG_LOG("%s::Failed to set keyboard backlight\n", getName());
            return;
        }
        
        if (nvram) saveKBBacklightToNVRAM(level);
        
        if (display)
        {
            kev.sendMessage(kevKeyboardBacklight, level, keybrdBLight16?16:3);
            DEBUG_LOG("%s::Sent message to user space daemon\n", getName());
        }
        
        curKeybrdBlvl = level;
        
        setProperty("KeyboardBLightLevel", level, sizeof(level)*8);
    }
}

int AsusFnKeys::checkBacklightEntry()
{
    if(IORegistryEntry::fromPath(backlightEntry))
        return 1;
    else
    {
        DEBUG_LOG("%s::Failed to find backlight entry for %s\n", getName(), backlightEntry);
        return 0;
    }
}

int AsusFnKeys::findBacklightEntry()
{
    snprintf(backlightEntry, 1000, "IOService:/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/GFX0@2/AppleIntelFramebuffer@0/display0/AppleBacklightDisplay");
    if(checkBacklightEntry())
        return 1;
    
    snprintf(backlightEntry, 1000, "IOService:/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/IGPU@2/AppleIntelFramebuffer@0/display0/AppleBacklightDisplay");
    if (checkBacklightEntry())
        return 1;
    
    char deviceName[5][5] = {"PEG0", "PEGP", "PEGR", "P0P2", "IXVE"};
    for (int i = 0; i < 5; i++)
    {
        snprintf(backlightEntry, 1000, "IOService:/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/%s@1/IOPP/GFX0@0", deviceName[i]);
        snprintf(backlightEntry, 1000, "%s%s", backlightEntry, "/NVDA,Display-A@0/NVDA/display0/AppleBacklightDisplay");
        if (checkBacklightEntry())
            return 1;
        
        snprintf(backlightEntry, 1000, "IOService:/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/%s@3/IOPP/GFX0@0", deviceName[i]);
        snprintf(backlightEntry, 1000, "%s%s", backlightEntry, "/NVDA,Display-A@0/NVDATesla/display0/AppleBacklightDisplay");
        if (checkBacklightEntry())
            return 1;
        
        snprintf(backlightEntry, 1000, "IOService:/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/%s@10/IOPP/GFX0@0", deviceName[i]);
        snprintf(backlightEntry, 1000, "%s%s", backlightEntry, "/NVDA,Display-A@0/NVDATesla/display0/AppleBacklightDisplay");
        if (checkBacklightEntry())
            return 1;
        
        snprintf(backlightEntry, 1000, "IOService:/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/%s@1/IOPP/display@0", deviceName[i]);
        snprintf(backlightEntry, 1000, "%s%s", backlightEntry, "/NVDA,Display-A@0/NVDA/display0/AppleBacklightDisplay");
        if (checkBacklightEntry())
            return 1;
        
        snprintf(backlightEntry, 1000, "IOService:/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/%s@3/IOPP/display@0", deviceName[i]);
        snprintf(backlightEntry, 1000, "%s%s", backlightEntry, "/NVDA,Display-A@0/NVDATesla/display0/AppleBacklightDisplay");
        if (checkBacklightEntry())
            return 1;
        
        snprintf(backlightEntry, 1000, "IOService:/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/%s@10/IOPP/display@0", deviceName[i]);
        snprintf(backlightEntry, 1000, "%s%s", backlightEntry, "/NVDA,Display-A@0/NVDATesla/display0/AppleBacklightDisplay");
        if (checkBacklightEntry())
            return 1;
    }
    
    return 0;
}

void AsusFnKeys::readPanelBrightnessValue()
{
    //
    //Reading AppleBezel Values from Apple Backlight Panel driver for controlling the bezel levels
    //
    
    if(!findBacklightEntry())
    {
        DEBUG_LOG("%s::GPU device not found\n", getName());
        return;
    }
    
    IORegistryEntry *displayDeviceEntry = IORegistryEntry::fromPath(backlightEntry);
    
    if (displayDeviceEntry != NULL) {
        if(OSDictionary* ioDisplayParaDict = OSDynamicCast(OSDictionary, displayDeviceEntry->getProperty("IODisplayParameters")))
        {
            if(OSDictionary* brightnessDict = OSDynamicCast(OSDictionary, ioDisplayParaDict->getObject("brightness")))
            {
                if (OSNumber* brightnessValue = OSDynamicCast(OSNumber, brightnessDict->getObject("value")))
                {
                    panelBrightnessLevel = brightnessValue->unsigned32BitValue()/64;
                    DEBUG_LOG("%s::Panel brightness level from AppleBacklightDisplay: %d\n", getName(), brightnessValue->unsigned32BitValue());
                    DEBUG_LOG("%s::Read panel brightness level: %d\n", getName(), panelBrightnessLevel);
                }
                else
                    DEBUG_LOG("%s::Can't not read brightness value\n", getName());
            }
            else
                DEBUG_LOG("%s::Can't not find dictionary brightness\n", getName());
        }
        else
            DEBUG_LOG("%s::Can't not find dictionary IODisplayParameters\n", getName());
    }
}

void AsusFnKeys::saveKBBacklightToNVRAM(UInt8 level)
{
    if (IORegistryEntry* nvram = OSDynamicCast(IORegistryEntry, fromPath("/options", gIODTPlane)))
    {
        if (const OSSymbol* symbol = OSSymbol::withCString(kAsusKeyboardBacklight))
        {
            if (OSData* number = OSData::withBytes(&level, sizeof(level)))
            {
                if (!nvram->setProperty(symbol, number))
                    DEBUG_LOG("%s::nvram->setProperty failed\n", getName());
                number->release();
            }
            symbol->release();
        }
        nvram->release();
    }
}

UInt8 AsusFnKeys::readKBBacklightFromNVRAM(void)
{
    IORegistryEntry* nvram = IORegistryEntry::fromPath("/chosen/nvram", gIODTPlane);
    if (!nvram)
    {
        if (OSDictionary* matching = serviceMatching("IODTNVRAM"))
        {
            nvram = waitForMatchingService(matching, MS_TO_NS(15000));
            matching->release();
        }
    }
    UInt8 val = 100;
    if (nvram)
    {
        // need to serialize as getProperty on nvram does not work
        if (OSSerialize* serial = OSSerialize::withCapacity(0))
        {
            nvram->serializeProperties(serial);
            if (OSDictionary* props = OSDynamicCast(OSDictionary, OSUnserializeXML(serial->text())))
            {
                if (OSData* number = OSDynamicCast(OSData, props->getObject(kAsusKeyboardBacklight)))
                {
                    val = 0;
                    unsigned l = number->getLength();
                    if (l <= sizeof(val))
                        memcpy(&val, number->getBytesNoCopy(), l);
                    DEBUG_LOG("%s::Keyboard backlight value from NVRAM: %d\n", getName(), val);
                }
                else
                    IOLog("%s::Keyboard backlight value not found in NVRAM\n", getName());
                props->release();
            }
            serial->release();
        }
        nvram->release();
    }
    else
        IOLog("%s::NVRAM not available\n", getName());
    return val;
}

void AsusFnKeys::getDeviceStatus(const char * guid, UInt32 methodId, UInt32 deviceId, UInt32 *status)
{
    DEBUG_LOG("%s::getDeviceStatus()\n", getName());
    
    char method[5];
    OSObject * params[3];
    OSString *str;
    OSDictionary *dict = getDictByUUID(guid);
    if (NULL == dict)
        return;
    
    str = OSDynamicCast(OSString, dict->getObject("object_id"));
    if (NULL == str)
        return;
    
    snprintf(method, 5, "WM%s", str->getCStringNoCopy());
    
    params[0] = OSNumber::withNumber(0x00D,32);
    params[1] = OSNumber::withNumber(methodId,32);
    params[2] = OSNumber::withNumber(deviceId,32);
    
    WMIDevice->evaluateInteger(method, status, params, 3);
    
    params[0]->release();
    params[1]->release();
    params[2]->release();
    
    return;
}

void AsusFnKeys::setDeviceStatus(const char * guid, UInt32 methodId, UInt32 deviceId, UInt32 *status)
{
    DEBUG_LOG("%s::setDeviceStatus()\n", getName());
    
    char method[5];
    char buffer[8];
    OSObject * params[3];
    OSString *str;
    OSDictionary *dict = getDictByUUID(guid);
    if (NULL == dict)
        return;
    
    str = OSDynamicCast(OSString, dict->getObject("object_id"));
    if (NULL == str)
        return;
    
    snprintf(method, 5, "WM%s", str->getCStringNoCopy());
    
    memcpy(buffer, &deviceId, 4);
    memcpy(buffer+4, status, 4);
    
    params[0] = OSNumber::withNumber(0x00D,32);
    params[1] = OSNumber::withNumber(methodId,32);
    params[2] = OSData::withBytes(buffer, 8);
    
    *status = ~0;
    WMIDevice->evaluateInteger(method, status, params, 3);
    
    DEBUG_LOG("%s::setDeviceStatus Res = %x\n", getName(), (unsigned int)*status);
    
    params[0]->release();
    params[1]->release();
    params[2]->release();
    
    return;
}

void AsusFnKeys::setDevice(const char * guid, UInt32 methodId, UInt32 *status)
{
    DEBUG_LOG("%s::setDevice(%d)\n", getName(), (int)*status);
    
    char method[5];
    char buffer[4];
    OSObject * params[3];
    OSString *str;
    OSDictionary *dict = getDictByUUID(guid);
    if (NULL == dict)
        return;
    
    str = OSDynamicCast(OSString, dict->getObject("object_id"));
    if (NULL == str)
        return;
    
    snprintf(method, 5, "WM%s", str->getCStringNoCopy());
    
    memcpy(buffer, status, 4);
    
    params[0] = OSNumber::withNumber(0x00D,32);
    params[1] = OSNumber::withNumber(methodId,32);
    params[2] = OSData::withBytes(buffer, 8);
    
    *status = ~0;
    WMIDevice->evaluateInteger(method, status, params, 3);
    
    DEBUG_LOG("%s::setDevice Res = %x\n", getName(), (unsigned int)*status);
    
    params[0]->release();
    params[1]->release();
    params[2]->release();
    
    return;
}


OSDictionary* AsusFnKeys::getDictByUUID(const char * guid)
{
    UInt32 i;
    OSDictionary *dict = NULL;
    OSString *uuid;
    OSArray *array = OSDynamicCast(OSArray, properties->getObject("WDG"));
    if (NULL == array)
        return NULL;
    for (i=0; i<array->getCount(); i++) {
        dict = OSDynamicCast(OSDictionary, array->getObject(i));
        uuid = OSDynamicCast(OSString, dict->getObject("UUID"));
        if (uuid->isEqualTo(guid)){
            break;
        }
        
    }
    return dict;
}


IOReturn AsusFnKeys::enableFnKeyEvents(const char * guid, UInt32 methodId)
{
    //Asus WMI Specific Method Inside the DSDT
    //Calling the Asus Method INIT from the DSDT to enable the Hotkey Events
    WMIDevice->evaluateObject("INIT", NULL, NULL, NULL);
    
    return kIOReturnSuccess;
}



#pragma mark -
#pragma mark Event handling methods
#pragma mark -

void AsusFnKeys::enableEvent()
{
    if (enableFnKeyEvents(ASUS_WMI_MGMT_GUID, ASUS_WMI_METHODID_INIT) != kIOReturnSuccess)
        IOLog("Unable to enable events!!!\n");
    else
    {
        _keyboardDevice = new FnKeysHIKeyboardDevice;
        
        if (!_keyboardDevice             ||
            !_keyboardDevice->init()     ||
            !_keyboardDevice->attach(this))
        {
            _keyboardDevice->release();
            IOLog("%s::Error creating keyboardDevice\n", getName());
        }
        else
        {
            _keyboardDevice->setKeyMap(keyMap);
            _keyboardDevice->registerService();
            
            // Setting Touchpad state on startup
            setProperty("TouchpadEnabled", true);
            
            // Load keyboard backlight level from NVRAM
            UInt8 tmp = readKBBacklightFromNVRAM();
            if(tmp != 100)
                keybrdBLightLvl = tmp;
            
            if(!keybrdBLight16 && keybrdBLightLvl>3) keybrdBLightLvl=3;
            
            // Calling the keyboardBacklight Event for Setting the Backlight
            if(hasKeybrdBLight)
                setKeyboardBackLight(keybrdBLightLvl);
            
            curKeybrdBlvl = keybrdBLightLvl;
            
            if(hasALSensor)
            {
                isALSenabled = true;
                enableALS(isALSenabled);
                IOLog("%s::ALS turned on at boot\n", getName());
            }
            
            IOLog("%s::Asus Fn Hotkey Events Enabled\n", getName());
        }
    }
}

void AsusFnKeys::disableEvent()
{
    OSSafeReleaseNULL(_keyboardDevice);
}

#pragma mark -
#pragma mark Notification methods
#pragma mark -

void AsusFnKeys::notificationHandlerGated(IOService * newService, IONotifier * notifier)
{
    if (notifier == _publishNotify) {
        IOLog("%s::Notification consumer published: %s\n", getName(), newService->getName());
        _notificationServices->setObject(newService);
    }
    
    if (notifier == _terminateNotify) {
        IOLog("%s::Notification consumer terminated: %s\n", getName(), newService->getName());
        _notificationServices->removeObject(newService);
    }
}

bool AsusFnKeys::notificationHandler(void * refCon, IOService * newService, IONotifier * notifier)
{
    command_gate->runAction(OSMemberFunctionCast(IOCommandGate::Action, this, &AsusFnKeys::notificationHandlerGated), newService, notifier);
    return true;
}

void AsusFnKeys::dispatchMessageGated(int* message, void* data)
{
    OSCollectionIterator* i = OSCollectionIterator::withCollection(_notificationServices);
    
    if (i != NULL) {
        while (IOService* service = OSDynamicCast(IOService, i->getNextObject()))  {
            service->message(*message, this, data);
        }
        i->release();
    }
}

void AsusFnKeys::dispatchMessage(int message, void* data)
{
    command_gate->runAction(OSMemberFunctionCast(IOCommandGate::Action, this, &AsusFnKeys::dispatchMessageGated), &message, data);
}
