/*
 * Filename: errorhandler_pvt.h
 * Library: libdatetimetools
 *
 * Description: libdatetimetools is a library of useful functions to enable
 * users to perform date and time calculations. One of its primary uses is as a
 * component of solutions to computing chain-based scheduling, as in litigation
 * and project management. 
 *
 * Version: See version.h
 * Created: 01/11/2021 22:43:54
 * Last Modified: Tue Jan 12 19:57:04 2021
 *
 * Author: Thomas H. Vidal (THV), thomashvidal@gmail.com
 * Organization: Dark Matter Computing
 *
 * Copyright: (c) 2011-2020 - Thomas H. Vidal, Los Angeles, CA
 * SPDX-License-Identifier:  LGPL-3.0-only
 *
 * Usage: 
 * File Format: 
 * Restrictions: 
 * Error Handling: 
 * References: 
 * Notes: this private header handles all the data type and file management
 * activities, which should be unnecessary to users of the library.
 */


/*-----------------------------------------------------------------------------
 * Symbolic Constants 
 *----------------------------------------------------------------------------*/

enum DateTimeErrorCode {
    /* computation error codes */ 
    /* file i/o error codes */
    /* configuration error codes */
    /* memory error codes */
    /* UI error codes */

};

enum DateTimeWarningCode {
    
}

/*-----------------------------------------------------------------------------
 * Error Handling  
 *----------------------------------------------------------------------------*/

/* ----- Computation Error Handling ----- */

    /*  MARK: - Computation errors */


/* ----- File i/o Error Handling ----- */

    /* MARK: - File i/o */


/* ----- Configuration Error Handling ----- */

    /* MARK: - Configuration */


/* ----- Memory Error Handling ----- */

    /* MARK: - Memory */


/* ----- User Interface Error Handling ----- */

    /* MARK: - User Interface */


/*-----------------------------------------------------------------------------
 * Error Reporting  
 *----------------------------------------------------------------------------*/


/*
 * Name: errorcode_to_string
 *
 * Description: Gets the text of the error code message.
 *
 * Parameters: Takes an integer corresponding to a DateTimeErrorCode.
 *
 * Return: The string of the error code or a NULL pointer.
 * 
 */

const char *errorcode_to_string(int ErrCode);

/*
 * Name: warncode_to_string
 *
 * Description: Gets the text of the warning code message.
 *
 * Parameters: Takes an integer corresponding to a DateTimeWarnCode.
 *
 * Return: The string of the warning code or a NULL pointer.
 * 
 */

const char *warncode_to_string(int WarnCode);



/* ----- CLI Non-Interactive Mode Error Reporting ----- */

    /* MARK: CLI - Non-Interactive */

/* ----- CLI Interactive Mode Error Reporting ----- */

    /* MARK: - CLI Interactive */

/* ----- GUI Mode Error Reporting ----- */

    /*  MARK: - GUI */
