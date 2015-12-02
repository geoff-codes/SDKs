/*
 *  CoreTelephonyDefines.h
 *  CFTelephony
 *
 *  Created by Jared Grubb on 7/26/06.
 *  Copyright 2010 Apple Inc. All rights reserved.
 *
 */

// Macros to export a symbol from the CoreTelephony library
#ifndef __CORETELEPHONY_DEFINES_H__
#define __CORETELEPHONY_DEFINES_H__

#ifdef __cplusplus
#define CORETELEPHONY_EXTERN   extern "C" __attribute__((visibility ("default")))
#else
#define CORETELEPHONY_EXTERN   extern __attribute__((visibility ("default")))
#endif

#define	CORETELEPHONY_EXTERN_CLASS	__attribute__((visibility("default")))

#endif
