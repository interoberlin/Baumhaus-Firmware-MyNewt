/**
 * @file
 * In this file the developer can select which
 * variant of target hardware should be compiled for.
 * This information will be used e.g. in pinout.h
 * to configure pin numbers according to that hardware.
 */

#ifndef PLATFORM_H
#define PLATFORM_H

/*
 * https://github.com/interoberlin/nrfduino
 */
//#define PLATFORM_NRFDUINO

/*
 * https://github.com/interoberlin/Baumhaus-Floor-Adapter
 */
//#define PLATFORM_ADAPTER_SMD
#define PLATFORM_ADAPTER_TH

#endif
