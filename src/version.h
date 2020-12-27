/*
 * Filename:  version.h
 * Library: libdatetimetools
 *
 * Version:  1.0.0
 * Created: 08/18/2011 14:24:55
 * Last Modified: Sat Dec 26 22:43:26 2020
 *
 * Author: Thomas H. Vidal (THV), thomashvidal@gmail.com
 * Organization: Dark Matter Computing
 *  
 * Copyright: (c) 2011-2020 - Thomas H. Vidal
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#ifndef VERSION_H
#define VERSION_H

	//Date Version Types
	static const char DATE[] = "25";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2020";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 0;
	static const long BUILD = 02;
	static const long REVISION = 00;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 000;
	#define RC_FILEVERSION 1,0,2,0
	#define RC_FILEVERSION_STRING "1, 0, 2, 0\0"
	static const char FULLVERSION_STRING[] = "1.0.2.0";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 00;
	
#endif //VERSION_H
