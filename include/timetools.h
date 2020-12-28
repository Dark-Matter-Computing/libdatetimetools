/*
 * Filename: timetools.h
 * Library: libdatetimetools
 *
 * Description: libdatetimetools is a library of useful functions to enable
 * users to perform date and time calculations. One of its primary uses is as a
 * component of solutions to computing chain-based scheduling, as in litigation
 * and project management. 
 *
 * Version: See version.h
 * Created: 08/20/2011 10:33:55
 * Last Modified: Sun Dec 27 15:04:46 2020
 *
 * Author: Thomas H. Vidal (THV), thomashvidal@gmail.com
 * Organization: Dark Matter Computing
 *  
 * Copyright: (c) 2011-2020 - Thomas H. Vidal
 * SPDX-License-Identifier: LGPL-3.0-only
 */

/* function prototypes */
/* void parsetime (char input[], int *h, int *m, int *s); not used right now*/
int summin (int h, int m, int s);
void min2time (int min, int *h, int *m, int *s);

