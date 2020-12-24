/*
 * Filename: datetimetools_int.h
 * Library: libdatetimetools
 *
 * Description: libdatetimetools is a library of useful functions to enable
 * users to perform date and time calculations. One of its primary uses is as a
 * component of solutions to computing chain-based scheduling, as in litigation
 * and project management. 
 *
 * Version: 1.0
 * Created: 12/19/2020 12:43:04
 * Last Modified: Wed Dec 23 16:01:48 2020
 *
 * Author: Thomas H. Vidal (THV), thomashvidal@gmail.com
 * Organization: Dark Matter Computing
 *
 * Copyright: (c) 2011-2020 - Thomas H. Vidal, Los Angeles, CA
 * SPDX-License-Identifier:  LGPL-3.0-only
 *
 * Usage: 
 * File Format: Presently the library usese CSV files to import various rules
 * such as holidays.
 * 
 * Restrictions: Presently, the library handles Unix encoded ASCII files fine,
 * but chokes on macOS and Windows files. Handling CR and CRLFs will be
 * addressed in the future.
 * 
 * Error Handling: Under development
 * 
 * Notes: this private header handles all the data type and file management
 * activities, which should be unnecessary to users of the library.
 */

#ifndef _DATETIMETOOLS_PVT_H_INCLUDED_
#define _DATETIMETOOLS_PVT_H_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "../include/datetools.h"

/*-----------------------------------------------------------------------------
 * PARAMETERIZED MACROS
 *----------------------------------------------------------------------------*/

#define SET_FLAG(x,y) ((x) |= (y))
    /* SET_BIT sets the bit in x corresponding to the event flag passed by
    the y parameter. */

#define CLEAR_FLAG(x,y) ((x) &= ~(y))
    /* CLEAR_BIT takes x and clears from x ALL the bits that are set in y. */

#define TEST_FLAG(x,y) ((x) & (y))
    /* TEST_BIT tests a single bit to see if it's "on." This will not test to
    see if MULTIPLE bits are set. */

#define TEST_MULTIPLEFLAGS(x,y) ((x) & ~(y))
    /* TEST_MULTIPLEFLAGS tests whether bits IN ADDITION TO the relevant bit
    are set */

#define CLEAR_ALLFLAGS(x) ((x) &= 0)
    /* CLEARS_ALLFLAGS zeros out the flag variable. */

#define ASCII2DECIMAL(x) ((x)-48)

    /* ASCII2DECIMAL: convert an ASCII character into a decimal. Macro does no
     * error checking (of course!), but anticipates an ascii character between
     * '0' ASCII 48) and 9 (ASCII 57).
     */


/*-----------------------------------------------------------------------------
 * Symbolic Constants: Holiday File Field Codes 
 *----------------------------------------------------------------------------*/

/*  Define field codes for the holiday CSV File */

const char* HF_MONTH     = "Month";
const char* HF_RTYPE     = "Rule Type";
const char* HF_RULE      = "Rule";
const char* HF_HOLIDAY   = "Holiday";
const char* HF_AUTHORITY = "Authority";

/*  Sizes and numbers of records and fields */

#define MAXRECORDLENGTH 500 /* Maximum Length (in characters of CSV File line,
                               each line consists of one record. */
#define MAXNUMFIELDS 25 /* Maxinum number of fields in CSV File */
#define MAXFIELDLEN 25 /* Maximum length (in chars) of name of field */

/*  Field and Text delimiters */

#define FDELIMITER ',' /* Field delimiter used in the CSV files */
#define TDELIMITER '\"' /* Text string delimiter used in the CSV files */
#define NEWLINE '\n' /*  Newline */
#define NULCHAR '\0' /* nul string */
#define EMPTYFIELD '\0'

/*  Token processing flags */

const unsigned char BEGIN_FIELD = (1<<0); /* Set when the first field delimiter is reached. */
const unsigned char BEGIN_TSTRING = (1<<1); /* Set when the first text delimiter is reached. */
const unsigned char END_TSTRING = (1<<2); /* Set when the end text delimter is reached. */
const unsigned char TOKEN_FOUND = (1<<7); /* Set when the token was found */

/* Macro definitions for error codes */

#define NOERROR 0
#define BADCOMMANDLINE 1
#define NULSTRING 2
#define NOFDELIM 3
#define NOTDELIM 4

/*-----------------------------------------------------------------------------
 * EXPORTED, BUT PRIVATE, DATA TYPES 
 *----------------------------------------------------------------------------*/

/* rulefile state */
struct RuleSet {
    FILE *rulefile;
    char headerfields[MAXNUMFIELDS][MAXFIELDLEN];
    int totalnumfields;
    enum {CLOSED, OPEN} openstatus;
};

/* The holiday rules are maintained in a chained hash table data structure.
 * The hash table is an array of linked lists. The array of nodes is indexed to
 * the months of the year.  Each pointer is the first node of a linked list of
 * HolidayNodes.  Each element holds the rules for the applicable month.  There
 * are 13 elements as the final element applies to the linked lists of rules
 * that apply to every month, like weekend rules.  The hash function is thus
 * the months of the year. 
 */

struct HolidayRule{
    int month;
    char ruletype;
        /* holds one of three possible values: 'w' for a weekend rule, 'r'
         * for a relative rule, and 'a' for an absolute rule
         */
    unsigned int wkday;
        /* the day of the wk to which the rule applies. if the rule is
         * relative.
         */
    int wknum;
        /* the week number to which the rule applies if the rule is
         * relative.
         */
    int day; /* the day of the holiday if the ruletype is 'a' */
    char holidayname[40]; /* the title of the holiday */
    char authority[100]; /* the statutory authority for the holiday */
};

struct HolidayNode { /* structure for linked list to hold court holidays */
    struct HolidayRule rule; 
    struct HolidayNode *nextrule; 
};

/* The hash table itself: an array to hold an individual holiday-node linked
list for each month of the year + the ALLMONTHS rules */
extern struct HolidayNode *holidayhashtable[13];

/*-----------------------------------------------------------------------------
 * Holiday Hashtable Handler Functions
 *----------------------------------------------------------------------------*/

void holiday_tbl_build(FILE *receivedrulefile,
                       struct HolidayNode *holidayhashtable[]);
void holiday_tbl_init(struct HolidayNode *holidayhashtable[]);
void holiday_rules_get_tokens(FILE *holidayrulefile,
                              struct HolidayNode *holidayhashtable[],
                              struct RuleSet *globalstate);
char *holiday_rules_tokenize(char *string, int *lasttoken);
void holiday_rules_processtoken(char *token, char *cur_field,
                                struct HolidayRule *newholiday);
void holiday_table_addrule(struct HolidayNode **elementhandle,
                                          struct HolidayRule *newrule);
void holiday_table_release(struct HolidayNode *holidayhashtable[]);

/*-----------------------------------------------------------------------------
 * Holiday Rule File Management 
 *----------------------------------------------------------------------------*/

int holiday_rules_validatefile(FILE *candidaterulefile_h);
void holiday_rules_getfields(FILE *holidayrulefile, struct RuleSet *globalstate);
void holiday_rules_resetfile(FILE *holidayrulefile);
int holiday_rules_closefile(FILE *holidayrulefile);

/*-----------------------------------------------------------------------------
 * Process Holiday Rules
 *----------------------------------------------------------------------------*/

int holiday_tbl_checkrule(struct DateTime *dt, struct HolidayNode *rulenode);

/*-----------------------------------------------------------------------------
 *  Error Handling
 *----------------------------------------------------------------------------*/

void errorprocessor(int error_code);

#endif /* _DATETIMETOOLS_PVT_H_INCLUDED_ */
