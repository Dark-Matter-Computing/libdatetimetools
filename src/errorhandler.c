/*
 * Filename: errorhandler.c
 * Library: libdatetimetools
 *
 * Description: libdatetimetools is a library of useful functions to enable
 * users to perform date and time calculations. One of its primary uses is as a
 * component of solutions to computing chain-based scheduling, as in litigation
 * and project management. 
 *
 * Version: See VERSION
 * Created: 01/11/2021 23:04:50
 * Last Modified: Fri Feb  5 18:07:00 2021
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
 * Author: Thomas H. Vidal (THV), thomashvidal@gmail.com
 * Notes: 
 */


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

const char *errorcode_to_string(int ErrCode)
{
    switch (ErrCode) {
        case /* ERRORCODE */:
            return "/*  to be inserted */";
            break;
        case /* ERRORCODE */:
            return "/*  to be inserted */";
            break;
        default:
            /* do nothing */
            break;
    }

    return NULL;
}

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
{
    switch (WarnCode) {
        case /* WARNCODE */:
            return "/*  to be inserted */";
            break;
        case /* WARNCODE */:
            return "/*  to be inserted */";
            break;
        default:
            /* do nothing */
            break;
    }

    return NULL;
}

/* ----- CLI Non-Interactive Mode Error Reporting ----- */

    /* MARK: - CLI - Non-Interactive */

/* ----- CLI Interactive Mode Error Reporting ----- */

    /* MARK: - CLI Interactive */

/* ----- GUI Mode Error Reporting ----- */

    /*  MARK: - GUI */
