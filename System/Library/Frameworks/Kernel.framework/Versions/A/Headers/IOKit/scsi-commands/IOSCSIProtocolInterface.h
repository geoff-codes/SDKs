/*
 * Copyright (c) 1998-2001 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef _IOKIT_IO_SCSI_PROTOCOL_INTERFACE_H_
#define _IOKIT_IO_SCSI_PROTOCOL_INTERFACE_H_

#include <IOKit/storage/IOStorageDeviceCharacteristics.h>

//
//	SCSI Device Characteristics - 	Defined between SCSI Application Layer and
//									SCSI Protocol Layer only.
//

// This key is used to define SCSI Device Characteristics for a particular device.
// It is the key for the dictionary containing the keys of characteristics. These keys
// are only defined between the SCSI Protocol Layer and the SCSI Applicaiton Layer. Some
// properties may be copied from this dictionary to the more generic Device Characteristics
// or Protocol Characteristics dictionaries.
#define kIOPropertySCSIDeviceCharacteristicsKey		"SCSI Device Characteristics"

// This key is used to define a default INQUIRY length to issue to the device. The
// value is a UInt32 corresponding to the number of bytes to request in the INQUIRY
// command.
#define kIOPropertySCSIInquiryLengthKey				"Inquiry Length"

// This key is used to indicate that the device is known to be a manual ejectable media
// device.  This property overrides all of the driver checks for determining this capability.
// This property is a string, although if it exists it should always be true.
#define kIOPropertySCSIManualEjectKey				"Manual Eject"


#if defined(KERNEL) && defined(__cplusplus)


#include <IOKit/IOLib.h>
#include <IOKit/IOService.h>

#include <IOKit/scsi-commands/SCSITask.h>
#include <IOKit/IOCommandGate.h>
#include <kern/thread.h>

// SCSI Protocol Interface Device Notification values
enum
{
	kSCSIProtocolNotification_DeviceRemoved			= 0x69000010,
	kSCSIProtocolNotification_VerifyDeviceState		= 0x69000020
};


typedef UInt32 SCSIProtocolFeature;
// SCSI Protocol Features
enum
{
	
	// kSCSIProtocolFeature_ACA:
	// Not yet used.
	kSCSIProtocolFeature_ACA								= 1,
	
	// kSCSIProtocolFeature_CPUInDiskMode:
	// Used to determine if the SCSI Protocol Services Driver supports
	// a CPU which is in target disk mode.
	kSCSIProtocolFeature_CPUInDiskMode						= 2,
	
	// kSCSIProtocolFeature_ProtocolSpecificPolling:
	// Used to determine if the SCSI Protocol Services Driver supports
	// protocol specific polling for media. This is used for low-power
	// polling specifically on ATAPI buses.
	kSCSIProtocolFeature_ProtocolSpecificPolling			= 3,
	
	// kSCSIProtocolFeature_ProtocolSpecificSleepCommand:
	// Used to determine if the SCSI Protocol Services Driver supports
	// protocol specific sleep commands to a drive. This is used for
	// sleeping drives specifically on ATAPI buses.
	kSCSIProtocolFeature_ProtocolSpecificSleepCommand		= 4,
	
	// kSCSIProtocolFeature_GetMaximumLogicalUnitNumber:
	// If the SCSI Protocol Services Driver supports logical units, it will
	// report the maximum addressable ID that it supports in the UInt32 pointer
	// that is passed in as the serviceValue. If only one unit is supported,
	// the driver should return false for this query. 
	kSCSIProtocolFeature_GetMaximumLogicalUnitNumber		= 5,
	
	// kSCSIProtocolFeature_MaximumReadBlockTransferCount and 
	// kSCSIProtocolFeature_MaximumWriteBlockTransferCount: 
	// If the SCSI Protocol Services Driver has a maximum number of
	// blocks that can be transfered in either a read or write request,
	// it will return true to appropriate query and return the block count 
	// in the UInt32 pointer that is passed in as the serviceValue. 
	kSCSIProtocolFeature_MaximumReadBlockTransferCount		= 6,
	kSCSIProtocolFeature_MaximumWriteBlockTransferCount		= 7
	
};


// This class defines the public SCSI Protocol Layer API for any class that
// provides Protocol services or needs to provide the Protocol Service API
// for passing service requests to a Protocol Service driver. 
class IOSCSIProtocolInterface : public IOService
{
	
	OSDeclareAbstractStructors ( IOSCSIProtocolInterface )

public:
	
	virtual bool	start 	( IOService * provider );
	virtual void	free 	( void );
	
	// ------ User Client Support ------

	// The GetUserClientExclusivityState method is called by the user client
	// to determine if any user client is holding exclusive access at the
	// current time.
	virtual bool 		GetUserClientExclusivityState ( void );

	// The SetUserClientExclusivityState method is called by the user client
	// to set the exclusive access mode.
	virtual IOReturn	SetUserClientExclusivityState ( IOService * userClient, bool state );

	// ------ Power Management Support ------
	
	// The initialPowerStateForDomainState() method is called by the power manager
	// to ask us what state we should be in based on the power flags of our parent
	// in the power tree.
	virtual UInt32		initialPowerStateForDomainState ( IOPMPowerFlags flags );
	
	// The setPowerState method is called by the power manager to tell us
	// to transition to a different power state denoted as powerStateOrdinal.
	// The whichDevice field is not pertinent to us since the driver is both
	// the "policy maker" for the drive, and the "policy implementor" for the
	// drive.
	virtual IOReturn 	setPowerState ( UInt32 powerStateOrdinal, IOService * whichDevice );
	
	// The IsPowerManagementIntialized method can be called to determine whether or not
	// power management has been successfully initialized.
	virtual bool		IsPowerManagementIntialized ( void );
	
	// The CheckPowerState method is called by clients of the object before
	// issuing an I/O request. If the device is not ready to handle such
	// requests, it gives the driver a chance to block the thread until the
	// device is ready.
	virtual void 		CheckPowerState ( void );

	// -- Functionality available to the SCSI Application Layer	--
    // The ExecuteCommand method will take a SCSI Task and transport
    // it across the physical wire(s) to the device
    virtual void    	ExecuteCommand ( SCSITaskIdentifier request ) = 0;
    
    // The AbortCommand method will abort the indicated SCSI Task,
    // if it is possible and the task has not already completed.
    virtual SCSIServiceResponse    AbortCommand ( SCSITaskIdentifier request ) = 0;
    
	// The IsProtocolServiceSupported will return true if the specified
	// feature is supported by the protocol layer.  If the service has a value that must be
	// returned, it will be returned in the serviceValue output parameter.
	virtual bool		IsProtocolServiceSupported ( 
								SCSIProtocolFeature 	feature, 
								void * 					serviceValue ) = 0;
	
	// The HandleProtocolServiceFeature will return true if the specified feature could
	// be handled properly by the protocol layer.  If the feature has a value that must be
	// sent as a parameter, it will be sent in the serviceValue parameter.
	virtual bool		HandleProtocolServiceFeature ( 
								SCSIProtocolFeature 	feature, 
								void * 					serviceValue ) = 0;
	
protected:
    // Reserve space for future expansion.
    struct IOSCSIProtocolInterfaceExpansionData { };
    IOSCSIProtocolInterfaceExpansionData *fIOSCSIProtocolInterfaceReserved;
	
	// ------ Power Management Support ------
	
	thread_call_t		fPowerManagementThread;
	IOCommandGate *		fCommandGate;
	UInt32				fCurrentPowerState;
	UInt32				fProposedPowerState;
	bool				fPowerTransitionInProgress;
	bool				fPowerAckInProgress;
    bool				fPowerManagementInitialized;
	    
    // The InitializePowerManagement method is called to initialize power management.
	// The default implementation calls PMinit() and joinPMtree but doesn't register
	// the device for power managment. That is left to other layers to do.	
	virtual void		InitializePowerManagement ( IOService * provider );

	// The GetInitialPowerState method is called once, right after InitializePowerManagement()
	// in order to determine what state the device is initially in at startup time (usually
	// the highest power mode).
	virtual UInt32		GetInitialPowerState ( void ) = 0;

    // The current implementation of the Power Manager requires that leaf nodes
    // are PMstop'ed first. Moreover, if you call PMstop on a parent node first
    // it will result in a kernel panic. We use finalize to call PMstop so we get
    // "top down" ordering. However, the termination thread also needs to be
    // synced with the power thread so we have to wait for any outstanding power
    // transitions to complete before processing the finalize.
    virtual bool		finalize ( IOOptionBits options );

	// The sHandleSetPowerState method is a static function used as C->C++ glue
	// for going behind the command gate.
	static IOReturn 	sHandleSetPowerState ( IOSCSIProtocolInterface * self, UInt32 powerStateOrdinal );

    // The sGetPowerTransistionInProgress method is a static function used as
    // C->C++ glue for getting the command gate.
    static bool sGetPowerTransistionInProgress ( IOSCSIProtocolInterface * 	self );
   
	// The HandleSetPowerState method is called by the glue code and is on the
	// serialized side of the command gate. This allows us to touch any member
	// variables as necessary without any multi-threading issues.
	virtual void		HandleSetPowerState ( UInt32 powerStateOrdinal );
	
	// The sPowerManagement method is a static C-function which is called using
	// mach's thread_call API. It guarantees us a thread of execution which is
	// different than the power management thread and the workloop thread on which
	// we can issue commands to the device synchronously or asynchronously without
	// worrying about deadlocks. It calls through to HandlePowerChange, which is
	// a state machine used to direct power management.
	static void			sPowerManagement ( thread_call_param_t whichDevice );
	
	// The HandlePowerChange method is pure virtual and is left to each protocol or
	// application layer driver to implement. It is guaranteed to be called on its
	// own thread of execution and can make synchronous or asynchronous calls.
	virtual void		HandlePowerChange ( void ) = 0;
		
	// The sHandleCheckPowerState method is a static function used as C->C++ glue
	// for going behind the command gate.
	static void			sHandleCheckPowerState ( IOSCSIProtocolInterface * self );

	// The HandleCheckPowerState (void) method is on the serialized side of the command
	// gate and can change member variables safely without multi-threading issues.
	// It's main purpose is to call the superclass' HandleCheckPowerState ( UInt32 maxPowerState )
	// with the max power state the class registered with.
	virtual void		HandleCheckPowerState ( void ) = 0;

	// The HandleCheckPowerState ( UInt32 maxPowerState ) method is called by
	// the subclasses and is passed the maxPowerState number given to the power
	// manager at initialization time. This guarantees the threads block until that
	// power state has been achieved.
	void 				HandleCheckPowerState ( UInt32 maxPowerState );

	// The TicklePowerManager ( void ) method is called by CheckPowerState and
	// sends an activity tickle to the power manager so that the idle timer is
	// reset.
	virtual void		TicklePowerManager ( void ) = 0;

	// The TicklePowerManager ( UInt32 maxPowerState ) method is a convenience
	// function which can be called by subclasses in TicklePowerManager (void)
	// in order to tell the power manager to reset idle timer or bring the device
	// into the requested state. It returns whatever is returned by activityTickle
	// (true if device is in the requested state, false if it is not).
	bool				TicklePowerManager ( UInt32 maxPowerState );

	// ------ User Client Support ------

	bool				fUserClientExclusiveControlled;
	IOService *			fUserClient;
	
	// The sGetUserClientExclusivityState method is a static function used as C->C++ glue
	// for going behind the command gate.
	static void 		sGetUserClientExclusivityState ( IOSCSIProtocolInterface * self, bool * state );
	static void		 	sSetUserClientExclusivityState ( IOSCSIProtocolInterface * self, IOReturn * result, IOService * userClient, bool state );
	
	virtual bool 		HandleGetUserClientExclusivityState ( void );
	virtual IOReturn 	HandleSetUserClientExclusivityState ( IOService * userClient, bool state );

private:
	// Space reserved for future expansion.
    OSMetaClassDeclareReservedUnused( IOSCSIProtocolInterface, 1 );
    OSMetaClassDeclareReservedUnused( IOSCSIProtocolInterface, 2 );
    OSMetaClassDeclareReservedUnused( IOSCSIProtocolInterface, 3 );
    OSMetaClassDeclareReservedUnused( IOSCSIProtocolInterface, 4 );
    OSMetaClassDeclareReservedUnused( IOSCSIProtocolInterface, 5 );
    OSMetaClassDeclareReservedUnused( IOSCSIProtocolInterface, 6 );
    OSMetaClassDeclareReservedUnused( IOSCSIProtocolInterface, 7 );
    OSMetaClassDeclareReservedUnused( IOSCSIProtocolInterface, 8 );
    OSMetaClassDeclareReservedUnused( IOSCSIProtocolInterface, 9 );
    OSMetaClassDeclareReservedUnused( IOSCSIProtocolInterface, 10 );
    OSMetaClassDeclareReservedUnused( IOSCSIProtocolInterface, 11 );
    OSMetaClassDeclareReservedUnused( IOSCSIProtocolInterface, 12 );
    OSMetaClassDeclareReservedUnused( IOSCSIProtocolInterface, 13 );
    OSMetaClassDeclareReservedUnused( IOSCSIProtocolInterface, 14 );
    OSMetaClassDeclareReservedUnused( IOSCSIProtocolInterface, 15 );
    OSMetaClassDeclareReservedUnused( IOSCSIProtocolInterface, 16 );
	
};

#endif	/* defined(KERNEL) && defined(__cplusplus) */

#endif	/* _IOKIT_IO_SCSI_PROTOCOL_INTERFACE_H_ */