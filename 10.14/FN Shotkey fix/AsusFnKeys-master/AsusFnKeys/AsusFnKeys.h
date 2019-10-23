/*
 *  Copyright (c) 2012 - 2013 EMlyDinEsH(OSXLatitude). All rights reserved.
 *  Copyright (c) 2018 hieplpvip
 *
 *
 *  AsusFnKeys.h
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

#ifndef _AsusFnKeys_h
#define _AsusFnKeys_h

#include <IOKit/hidsystem/ev_keymap.h>
#include <IOKit/pwr_mgt/IOPMPowerSource.h>
#include <IOKit/acpi/IOACPIPlatformDevice.h>
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/IOCommandGate.h>
#include <IOKit/IOService.h>
#include <IOKit/IONVRAM.h>
#include <IOKit/IOLib.h>
#include <sys/errno.h>
#include <mach/kern_return.h>
#include <sys/kern_control.h>
#include <libkern/OSTypes.h>

#include "FnKeysHIKeyboardDevice.h"
#include "KernEventServer.h"

//extern "C" {
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//}
//extern int     system(const char *) __DARWIN_ALIAS_C(system);


struct guid_block {
    char guid[16];
    union {
        char object_id[2];
        struct {
            unsigned char notify_id;
            unsigned char reserved;
        };
    };
    UInt8 instance_count;
    UInt8 flags;
};


/*
 * If the GUID data block is marked as expensive, we must enable and
 * explicitily disable data collection.
 */
#define ACPI_WMI_EXPENSIVE   0x1
#define ACPI_WMI_METHOD      0x2    /* GUID is a method */
#define ACPI_WMI_STRING      0x4    /* GUID takes & returns a string */
#define ACPI_WMI_EVENT       0x8    /* GUID is an event */
#define ASUS_WMI_MGMT_GUID      "97845ED0-4E6D-11DE-8A39-0800200C9A66"
#define ASUS_NB_WMI_EVENT_GUID  "0B3CBB35-E3C2-45ED-91C2-4C5A6D195D1C"

/* WMI Methods */
#define ASUS_WMI_METHODID_SPEC          0x43455053 /* BIOS SPECification */
#define ASUS_WMI_METHODID_SFBD          0x44424653 /* Set First Boot Device */
#define ASUS_WMI_METHODID_GLCD          0x44434C47 /* Get LCD status */
#define ASUS_WMI_METHODID_GPID          0x44495047 /* Get Panel ID?? (Resol) */
#define ASUS_WMI_METHODID_QMOD          0x444F4D51 /* Quiet MODe */
#define ASUS_WMI_METHODID_SPLV          0x4C425053 /* Set Panel Light Value */
#define ASUS_WMI_METHODID_SFUN          0x4E554653 /* FUNCtionalities */
#define ASUS_WMI_METHODID_SDSP          0x50534453 /* Set DiSPlay output */
#define ASUS_WMI_METHODID_GDSP          0x50534447 /* Get DiSPlay output */
#define ASUS_WMI_METHODID_DEVP          0x50564544 /* DEVice Policy */
#define ASUS_WMI_METHODID_OSVR          0x5256534F /* OS VeRsion */
#define ASUS_WMI_METHODID_DSTS          0x53544344 /* Device STatuS */
#define ASUS_WMI_METHODID_DSTS2         0x53545344 /* Device STatuS #2*/
#define ASUS_WMI_METHODID_BSTS          0x53545342 /* Bios STatuS ? */
#define ASUS_WMI_METHODID_DEVS          0x53564544 /* DEVice Set */
#define ASUS_WMI_METHODID_CFVS          0x53564643 /* CPU Frequency Volt Set */
#define ASUS_WMI_METHODID_KBFT          0x5446424B /* KeyBoard FilTer */
#define ASUS_WMI_METHODID_INIT          0x54494E49 /* INITialize */
#define ASUS_WMI_METHODID_HKEY          0x59454B48 /* Hot KEY ?? */

#define ASUS_WMI_UNSUPPORTED_METHOD     0xFFFFFFFE

/* Wireless */
#define ASUS_WMI_DEVID_HW_SWITCH        0x00010001
#define ASUS_WMI_DEVID_WIRELESS_LED     0x00010002
#define ASUS_WMI_DEVID_CWAP             0x00010003
#define ASUS_WMI_DEVID_WLAN             0x00010011
#define ASUS_WMI_DEVID_BLUETOOTH        0x00010013
#define ASUS_WMI_DEVID_GPS              0x00010015
#define ASUS_WMI_DEVID_WIMAX            0x00010017
#define ASUS_WMI_DEVID_WWAN3G           0x00010019
#define ASUS_WMI_DEVID_UWB              0x00010021

/* Leds */
/* 0x000200XX and 0x000400XX */
#define ASUS_WMI_DEVID_LED1             0x00020011
#define ASUS_WMI_DEVID_LED2             0x00020012
#define ASUS_WMI_DEVID_LED3             0x00020013
#define ASUS_WMI_DEVID_LED4             0x00020014
#define ASUS_WMI_DEVID_LED5             0x00020015
#define ASUS_WMI_DEVID_LED6             0x00020016

/* Backlight and Brightness */
#define ASUS_WMI_DEVID_BACKLIGHT        0x00050011
#define ASUS_WMI_DEVID_BRIGHTNESS       0x00050012
#define ASUS_WMI_DEVID_KBD_BACKLIGHT    0x00050021
#define ASUS_WMI_DEVID_LIGHT_SENSOR     0x00050022 /* ?? */

/* Misc */
#define ASUS_WMI_DEVID_CAMERA           0x00060013

/* Storage */
#define ASUS_WMI_DEVID_CARDREADER       0x00080013

/* Input */
#define ASUS_WMI_DEVID_TOUCHPAD         0x00100011
#define ASUS_WMI_DEVID_TOUCHPAD_LED     0x00100012

/* Fan, Thermal */
#define ASUS_WMI_DEVID_THERMAL_CTRL     0x00110011
#define ASUS_WMI_DEVID_FAN_CTRL         0x00110012

/* Power */
#define ASUS_WMI_DEVID_PROCESSOR_STATE  0x00120012

/* DSTS masks */
#define ASUS_WMI_DSTS_STATUS_BIT        0x00000001
#define ASUS_WMI_DSTS_UNKNOWN_BIT       0x00000002
#define ASUS_WMI_DSTS_PRESENCE_BIT      0x00010000
#define ASUS_WMI_DSTS_USER_BIT          0x00020000
#define ASUS_WMI_DSTS_BIOS_BIT          0x00040000
#define ASUS_WMI_DSTS_BRIGHTNESS_MASK   0x000000FF
#define ASUS_WMI_DSTS_MAX_BRIGTH_MASK   0x0000FF00


/*
 * <platform>/    - debugfs root directory
 *   dev_id      - current dev_id
 *   ctrl_param  - current ctrl_param
 *   method_id   - current method_id
 *   devs        - call DEVS(dev_id, ctrl_param) and print result
 *   dsts        - call DSTS(dev_id)  and print result
 *   call        - call method_id(dev_id, ctrl_param) and print result
 
 */

#define EEEPC_WMI_METHODID_SPEC 0x43455053
#define EEEPC_WMI_METHODID_DEVP 0x50564544
#define EEEPC_WMI_METHODID_DEVS    0x53564544
#define EEEPC_WMI_METHODID_DSTS    0x53544344
#define EEEPC_WMI_METHODID_CFVS    0x53564643

#define EEEPC_WMI_DEVID_BACKLIGHT    0x00050011
#define EEEPC_WMI_DEVID_BACKLIGHT2    0x00050012
#define EEEPC_WMI_DEVID_BLUETOOTH   0x00010013
#define EEEPC_WMI_DEVID_WIRELESS    0x00010011
#define EEEPC_WMI_DEVID_TRACKPAD    0x00100011

#define kIOPMPowerOff                       0
#define kAsusFnKeysIOPMNumberPowerStates     2
static IOPMPowerState powerStateArray[kAsusFnKeysIOPMNumberPowerStates] =
{
    { 1,kIOPMPowerOff,kIOPMPowerOff,kIOPMPowerOff,0,0,0,0,0,0,0,0 },
    { 1,kIOPMPowerOn,IOPMPowerOn,IOPMPowerOn,0,0,0,0,0,0,0,0 }
};

#define AsusFnKeysEventCode 0x8102

const UInt8 NOTIFY_BRIGHTNESS_UP_MIN = 0x10;
const UInt8 NOTIFY_BRIGHTNESS_UP_MAX = 0x1F;

const UInt8 NOTIFY_BRIGHTNESS_DOWN_MIN = 0x20;
const UInt8 NOTIFY_BRIGHTNESS_DOWN_MAX = 0x2F;

#define MS_TO_NS(ms) (1000ULL * 1000ULL * (ms))
#define kAsusKeyboardBacklight "asus-keyboard-backlight"

#define kDeliverNotifications "ASUSFN,deliverNotifications"
enum
{
    kKeyboardSetTouchStatus = iokit_vendor_specific_msg(100),       // set disable/enable touchpad (data is bool*)
    kKeyboardGetTouchStatus = iokit_vendor_specific_msg(101),       // get disable/enable touchpad (data is bool*)
    kKeyboardKeyPressTime = iokit_vendor_specific_msg(110),         // notify of timestamp a non-modifier key was pressed (data is uint64_t*)
    kKeyboardModifierKeyPressTime = iokit_vendor_specific_msg(111),  // notify of timestamp a key was pressed (data is uint64_t*)
};

enum
{
    kevKeyboardBacklight = 1,
    kevAirplaneMode = 2,
    kevSleep = 3,
    kevTouchpad = 4,
    keyPrintScreen = 5,
};

class AsusFnKeys : public IOService
{
    OSDeclareDefaultStructors(AsusFnKeys)
    
protected:
    IOACPIPlatformDevice * WMIDevice;
    FnKeysHIKeyboardDevice * _keyboardDevice;
    KernEventServer kev;
    
    OSDictionary * properties;
    
public:
    virtual IOReturn message(UInt32 type, IOService * provider, void * argument);
    
    // standard IOKit methods
    virtual bool       init(OSDictionary *dictionary = 0);
    virtual bool       start(IOService *provider);
    virtual void       stop(IOService *provider);
    virtual void       free(void);
    virtual IOService *probe(IOService *provider, SInt32 *score);
    
    //power management events
    virtual IOReturn    setPowerState(unsigned long powerStateOrdinal, IOService *policyMaker);
    
protected:
    OSDictionary* getDictByUUID(const char * guid);
    IOReturn enableFnKeyEvents(const char * guid, UInt32 methodID);
    
    void parseConfig();
    void enableEvent();
    void disableEvent();
    
    void handleMessage(int code);
    void processFnKeyEvents(int code, int bLoopCount);
    
    void enableALS(bool state);
    
    bool keybrdBLight16;
    UInt8 keybrdBLightLvl, curKeybrdBlvl;
    void saveKBBacklightToNVRAM(UInt8 level);
    UInt8 readKBBacklightFromNVRAM();
    UInt8 getKeyboardBackLight();
    void setKeyboardBackLight(UInt8 level, bool nvram = true, bool display = false);
    
    UInt32 panelBrightnessLevel;
    char backlightEntry[1000];
    int checkBacklightEntry();
    int findBacklightEntry();
    void readPanelBrightnessValue();
    
    void getDeviceStatus(const char * guid, UInt32 methodId, UInt32 deviceId, UInt32 *status);
    void setDeviceStatus(const char * guid, UInt32 methodId, UInt32 deviceId, UInt32 *status);
    void setDevice(const char * guid, UInt32 methodId, UInt32 *status);
    
    void notificationHandlerGated(IOService * newService, IONotifier * notifier);
    bool notificationHandler(void * refCon, IOService * newService, IONotifier * notifier);
    void dispatchMessageGated(int* message, void* data);
    void dispatchMessage(int message, void* data);
    
    static const FnKeysKeyMap keyMap[];
    
    bool   touchpadEnabled;
    bool   hasALSensor, isALSenabled;
    bool   isPanelBackLightOn;
    bool   hasMediaButtons, hasKeybrdBLight;
    int    loopCount;
    
    IOWorkLoop *_workLoop;
    IOTimerEventSource *_autoOffTimer;
    IOCommandGate* command_gate;
    
    void autoOffTimer();
    void resetTimer();
    bool isautoOff, autoOffEnable;
    uint64_t keytime = 0;
    uint64_t autoOffTimeout = 10000000000; // 10 seconds
    
    IONotifier* _publishNotify;
    IONotifier* _terminateNotify;
    OSSet* _notificationServices;
    
private:
    int parse_wdg(OSDictionary *dict);
    OSString *flagsToStr(UInt8 flags);
    void wmi_wdg2reg(struct guid_block *g, OSArray *array, OSArray *dataArray);
    OSDictionary * readDataBlock(char *str);
    
    //utilities
    int wmi_data2Str(const char *in, char *out);
#ifdef DEBUG
    bool wmi_parse_guid(const UInt8 *src, UInt8 *dest);
    void wmi_dump_wdg(struct guid_block *g);
    int wmi_parse_hexbyte(const UInt8 *src);
    void wmi_swap_bytes(UInt8 *src, UInt8 *dest);
#endif
    
};

#endif //_AsusFnKeys_h
