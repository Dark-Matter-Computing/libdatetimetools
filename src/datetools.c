/*
 * Filename: datetools.c
 * Library: libdatetimetools
 *
 * FOR DESCRIPTION AND OTHER DETAILS, PLEASE SEE THE DATETOOLS.H AND 
 * DATETIMETOOLS_PVT.H header files.
 *
 * Version: See version.h
 * Created: 08/18/2011 14:24:15
 * Last Modified: Sat Jan  2 12:00:20 2021
 *
 * Author: Thomas H. Vidal (THV), thomashvidal@gmail.com
 * Organization: Dark Matter Computing
 *
 * Copyright: (c) 2011-2020 - Thomas H. Vidal, Los Angeles, CA
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "datetimetools_pvt.h"

/*-----------------------------------------------------------------------------
 * GLOBAL LIBRARY DATA TYPES 
 *----------------------------------------------------------------------------*/

struct HolidayNode *holidayhashtable[13];
struct RuleSet activerules_h;

/*-----------------------------------------------------------------------------
 * Holidy Hashtable Handler Functions
 *----------------------------------------------------------------------------*/   
int holiday_rules_open(const char *receivedrulefilename, int close_on_success) 
{
    FILE *holidayrulefile;
    int valid_file = 0;

    holidayrulefile = fopen(receivedrulefilename, "r"); 
    if (holidayrulefile == NULL)
    {
        printf("Cannot open HOLIDAY_FILE:%s\n", receivedrulefilename);
        exit(8);
    }

    if ((valid_file = holiday_rules_validatefile(holidayrulefile))) {
        holiday_rules_getfields(holidayrulefile, &activerules_h);
        holiday_tbl_build(holidayrulefile, holidayhashtable);
    }
    else
        /* address failure */;
    
    if (close_on_success == 1) {
        activerules_h.openstatus=CLOSED;
        holiday_rules_closefile(holidayrulefile);
    } else {
        activerules_h.openstatus=OPEN;
    }
    return 1;
}

void holiday_tbl_build(FILE *receivedrulefile,
                       struct HolidayNode *holidayhashtable[])
{
    holiday_tbl_init(holidayhashtable);
    holiday_rules_get_tokens(receivedrulefile, holidayhashtable,
                             &activerules_h);
}

void holiday_tbl_init(struct HolidayNode *holidayhashtable[])
{
    int monthctr; /* counter to loop through months */

    for(monthctr = 0; monthctr < TTLMONTHS; monthctr++)
    {
        holidayhashtable[monthctr] = NULL;
    }
    return;
}

/* 
 * Description:  Extract holiday-rule tokens from a record and populate the
 * holiday hash table with the parsed rules.
 *
 * Parameters:  A string containing a record filled with input fields used to
 * populate the hash table; a string containing the list of field names, and a
 * pointer to the holiday hash table. 
 *
 * Returns:  Nothing.
 * Algorithm:  
 * References:  
 * Notes:  
 */

void holiday_rules_get_tokens(FILE *holidayrulefile,
                              struct HolidayNode *holidayhashtable[],
                              struct RuleSet *globalstate)
{

    char tokenbuf[MAXRECORDLENGTH]; 
    char *cur_token = NULL;
    int cur_field = 0;
    int lasttoken = 0;
    struct HolidayRule newholiday;

    while (fgets(tokenbuf, sizeof(tokenbuf), holidayrulefile) != NULL) {
        do {
            cur_token = holiday_rules_tokenize(tokenbuf, &lasttoken);
            holiday_rules_parse_token(cur_token,
                                       globalstate->headerfields[cur_field],
                                       &newholiday);
            if (cur_field < (globalstate->totalnumfields-1)) {
                cur_field++;
             } else {
                cur_field = 0; /* Reset once we reach the end of the fields */
             }
        } while (!lasttoken);
        holiday_table_addrule(&holidayhashtable[newholiday.month], &newholiday);
        lasttoken=0;
    }
}

/*
 * Parameters:  Takes a character string, a field delimiter, and a text string
 * delimiter.
 *
 * Returns:  a pointer to a string containing the token or a null pointer.
 *
 * Algorithm:  The function first clears a set of token status flags.  Then the
 * function checks to determine whether this is the first time it was called
 * with a particular string.  If so, cur_char and prevpsn are set to the
 * beginnigng of the string.  Otherwise, cur_car is set to prevpsn.
 *
 * Next, the function starts a while loop that iterates until the flag
 * TOKEN_FOUND has been set.  As the loop iterates, cur_char is advanced along
 * the string.  The guts of the while loop is a case statement that parses
 * the tokens.
 *
 *
 * Notes: On the first call of the function on a particular record the user
 * must pass the string containing the record to tokenize.  On subsequent
 * calls, only a null string should be passed.  The function sets up a static
 * pointer to the next token in the record.
 */

char * holiday_rules_tokenize(char *string, int *lasttoken)
{
    /* FIXME Include handling for DOS/UNIX/MAC file endings.
     * So far this just handles UNIX endings. 
     */

    unsigned char flags = 0x0; /* clear the flags. */
    char *cur_char; /* character pointer to cycle through the string */
    char *tokenptr = NULL; /* Pointer to current token in the record
                           string */

    static char *prevpsn = NULL; /* Previous position in the string from the last
                             call to this function. */

    if (prevpsn == NULL) { /* If this is the first time the string is processed */
        cur_char = prevpsn = string; /* point to the beginning of the string */
        SET_FLAG(flags, BEGIN_FIELD); /* Set the BEGIN_FIELD flag because the
                                          first field does not lead off with a
                                         delimiter. */
    } else { /* On subsequent calls, start at the previous position. */
        cur_char = prevpsn;
    }

    while (TEST_FLAG(flags, TOKEN_FOUND) == 0) {

        switch (*cur_char) {
            case FDELIMITER:
                /* If this is the first field delimiter found in the string,
                 * it is likely the start of a field.  If it's a subsequent
                 * one, it could be a character used within the text string. */

                if (TEST_FLAG(flags,BEGIN_FIELD) == 0 &&
                        TEST_FLAG(flags, BEGIN_TSTRING) == 0) {
                    
                    /* this is the first field delimiter found and we are not
                     * in a text string, so advance cur_char and set the 
                     * BEGIN_FIELD flag. */

                    cur_char++; /* point to the next char after fdelim */
                    SET_FLAG(flags,BEGIN_FIELD);
                } else if (TEST_FLAG(flags, BEGIN_TSTRING) != 0) {
                    /* We are in the middle of a text field, so ignore the 
                     * field delimiter. */
                    cur_char++;
                } else {
                    /* There are two field delimiters back-to-back, which
                     * indicates and empty field. */
                    *cur_char = NULCHAR;
                    return (tokenptr = cur_char); /* field does not contain data */
                }
                break;
            case TDELIMITER:
                if (TEST_FLAG(flags, BEGIN_FIELD) == 0) {
                    /* If we are not inside a field, but we have reached a
                     * text delimiter, the file is not properly formatted. */
                    errorprocessor(NOFDELIM);
                } else if (TEST_FLAG(flags, BEGIN_TSTRING) == 0) {
                    *cur_char = '\0'; /* terminate the string */
                    cur_char++; /* cur_char now points to first char of
                                   token. */
                    SET_FLAG(flags, BEGIN_TSTRING);
                    tokenptr = cur_char; /* Set the tokenpointer to the begin of
                                            the token. */
                    cur_char++;
                } else { /* we are at the end of the token */
                    *cur_char = '\0'; /* terminate the token string */
                    SET_FLAG(flags, TOKEN_FOUND);
                    CLEAR_FLAG(flags, BEGIN_FIELD);
                    prevpsn = cur_char+1; /* point prevpsn to the next
                                      field delimter or end of tokenbuffer */
                    if (*prevpsn == NEWLINE) { /* see if we are at the end of
                                                * tokenbuff
                                                */
                        *lasttoken = 1;
                        prevpsn = NULL;
                    }
                    return tokenptr;
                }
                break;
            case NEWLINE:
                if (string == NULL) {
                    if (TEST_FLAG(flags, BEGIN_FIELD) != 0)
                        errorprocessor(NOTDELIM);
                    else if (TEST_FLAG(flags, BEGIN_TSTRING))
                        errorprocessor(NOTDELIM);
                    else
                        return NULL;
                } else {
                    errorprocessor(NULSTRING);
                }
                break;
            case NULCHAR:
                if (string == NULL) {
                    if (TEST_FLAG(flags, BEGIN_FIELD) != 0)
                        errorprocessor(NOTDELIM);
                    else if (TEST_FLAG(flags, BEGIN_TSTRING))
                        errorprocessor(NOTDELIM);
                } else {
                    errorprocessor(NULSTRING);
                }
                break;
            default:
                if(TEST_FLAG(flags, BEGIN_TSTRING) == 0) {
                    errorprocessor(NOTDELIM);
                } else {
                    cur_char++;
                }
                break;
        }
    }

return tokenptr;
}		/* -----  end of function holiday_rules_tokenize  ----- */

void holiday_rules_parse_token(char *token, char *cur_field,
                                struct HolidayRule *newholiday)
{
    char *currentchar = token;
    int idx = 0;

    if (*token == NULCHAR) {
        /* THIS SIGNALS AN EMPTY FIELD */
    }


    if (strcmp(cur_field, HF_MONTH) == 0) {
        /*  Analyze the field to determine the month */
        if (*currentchar == '0') {
            /* if currentchar = '0' then the month is september or earlier */
            currentchar++; /* read next character */

            /* TODO (Thomas#1#): Add error processing in case the month
            is not listed as a number betweeen 1 and 12. */

            newholiday->month = (ASCII2DECIMAL(*currentchar));
        } else {
            currentchar++; /* read next character of filed */
            newholiday->month = (10 + (ASCII2DECIMAL(*currentchar)));
			/* add ten reflecting the first character read and
			 * convert the second ASCII character to a number
			 * between 0 and 9.
			 * */

            /* TODO (Thomas#1#): Add error processing in case the first
            digit of the month month is != '1'. */
        }
    } else if (strcmp(cur_field, HF_RTYPE) == 0) {
        newholiday->ruletype = *currentchar; /* ruletype is a single character */
    } else if (strcmp(cur_field, HF_RULE) == 0) {
        switch (newholiday->ruletype) {
            case 'w':   /* Weekend Rules */
                         /* fall through */
            case 'W':
                 newholiday->wkday = ASCII2DECIMAL(*currentchar);
                 currentchar += 2; /* get rid of the dash */
                 newholiday->wknum = ASCII2DECIMAL(*currentchar);
                 newholiday->day = 0; /* TODO use symbolic constant */
                 break;
            case 'a':   /* Absolute Rules */
                        /* fall through */
            case 'A':
                newholiday->wkday = 999; /* change this to a symbolic const  */
                newholiday->wknum = 999; /* change this to a symbolic const  */
                if (*currentchar == '0') { /* the day is less than 10 */
                    currentchar++;
                    newholiday->day = ASCII2DECIMAL(*currentchar);
                } else { /* day is greater than 10 */
                   newholiday->day = ASCII2DECIMAL(*currentchar);
                   newholiday->day = (newholiday->day * 10) + 
                       ASCII2DECIMAL(*++currentchar);
               }
               break;
            case 'r':   /* Relative Rules */
                        /* fall through */
            case 'R':
                        newholiday->wkday = ASCII2DECIMAL(*currentchar);
                        currentchar++; /* get rid of the dash */
                        newholiday->wknum = ASCII2DECIMAL(*++currentchar);
                        break;
            case 'x':   /* rule has not been populated yet
                            error to be handled */
            default:
                        /* TODO (Thomas#1#): Add error processing in case the
                            rule is not in the proper format. */
                        break;
                }
    } else if (strcmp(cur_field, HF_HOLIDAY) == 0) {
        while(*currentchar) {
            newholiday->holidayname[idx] = *currentchar;
            currentchar++;
            idx++; 
        }
        newholiday->holidayname[idx] = '\0';    
    }  else if (strcmp(cur_field, HF_AUTHORITY) == 0) {
        while(*currentchar) {
            newholiday->authority[idx] = *currentchar;
            currentchar++;
            idx++; 
        }
        newholiday->authority[idx] = '\0';    
    }  else {
	    /* Error field name not defined */
    } 
    return;
}

void holiday_table_addrule(struct HolidayNode **elementhandle,
                                    struct HolidayRule *newrule)
{
    struct HolidayNode *newnode; /* pointer to new holiday table  node */

    newnode = (struct HolidayNode*) malloc(sizeof(struct HolidayNode)); /* creates a new node */

    /* copy the data into the new node */
    newnode->rule.month = newrule->month;
    newnode->rule.ruletype = newrule->ruletype;
    newnode->rule.wkday  = newrule->wkday;
    newnode->rule.wknum = newrule->wknum;
    newnode->rule.day = newrule->day;
    strcpy(newnode->rule.holidayname, newrule->holidayname);
    strcpy(newnode->rule.authority, newrule->authority);

    if (*elementhandle == NULL) {
        (*elementhandle) = newnode; /* makes the new node the first node */
        newnode->nextrule = NULL; /* newnode is the end of the list. */
    } else {
        newnode->nextrule = (*elementhandle)->nextrule;
        (*elementhandle)->nextrule = newnode; /* inserts new node as first node */
    }

    return;
}

void holiday_table_release(struct HolidayNode *holidayhashtable[])
{
    struct HolidayNode *tempnode;
    int monthctr; /* counter to loop through months */

    for(monthctr = 0; monthctr < TTLMONTHS; monthctr++)
    {
        while (holidayhashtable[monthctr] != NULL)
            /* checks to see if list is empty */
        {
            tempnode = holidayhashtable[monthctr];
            /* sets a temporary pointer to the first node so we don't
                lose it. */
            holidayhashtable[monthctr] = holidayhashtable[monthctr]->nextrule;
            /* makes the next node the new first node */
            free(tempnode); /* frees the memory allocated to the former first
                                node */
        }
    }
    return;
}

/*-----------------------------------------------------------------------------
 * Holidy Rule File Management 
 *----------------------------------------------------------------------------*/

int holiday_rules_validatefile(FILE *candidaterulefile_h)
{
    /* TODO is fields used for anyting? should i create a global rulesset file
     * where these fields can be saved?
     */
    char tokenbuf[MAXRECORDLENGTH]; /* buffer to read the file tokenbuf */
    char *name = NULL; 
    char *vers = NULL;
    int index = 0; 

    /* read first line of file */
    name = fgets(tokenbuf, sizeof(tokenbuf), candidaterulefile_h); 

    if (name == NULL) { /* file is empty */
        return -1; /* TODO change this to return a meaningful errorcode */
    } else {
        name = &tokenbuf[0];
        index = 1;
        while (tokenbuf[index] != '\n') {
            if (tokenbuf[index] == FDELIMITER) {
                tokenbuf[index] = '\0'; /* file is CSV, to covert field delimiter
                                          to end of string character */
                if (vers == NULL) {
                    vers = &tokenbuf[index+1];
                }
            }
            index++; 
            if ((tokenbuf[index] == FDELIMITER) &&
                    (tokenbuf[index+1] == FDELIMITER))
                tokenbuf[index] = '\0';
        }
        /* can't test these earlier b/c we first have to NUL term the strings */
        if(strcmp(name, "Court Holiday Rules File") != 0) 
           return -1; /* TODO change this to return a meaningful errorcode
                       *  "ERROR: This is not a holiday rules file"
                       */
        if(strcmp(vers, "V1.0") != 0) 
           return -1; /* TODO change this to return a meaningful errorcode
                       *  "ERROR: This is not the correct version
                       */
    }
    return 1;
}

void holiday_rules_getfields(FILE *holidayrulefile, struct RuleSet *globalstate)
{
    char fieldnames[MAXRECORDLENGTH]; 
    unsigned char flags = 0x0; /* clear the flags. */
    int i = 0;
    char curfield[MAXRECORDLENGTH]; 
    char *curchar = curfield;
    int endoffields = 0;
    int fieldindex = 0; 

    /*  Read the field names from the file */
    fgets(fieldnames, sizeof(fieldnames), holidayrulefile);
    globalstate->totalnumfields = 0;
    do {
        *curchar = fieldnames[i];
        switch (*curchar){
        case TDELIMITER:
            if (TEST_FLAG(flags, BEGIN_TSTRING) == 0) {
                SET_FLAG(flags, BEGIN_TSTRING);
                i++; /* discard delimiter */
            } else { /* we are at the end of the token */
                CLEAR_FLAG(flags, BEGIN_TSTRING);
                *curchar = NULCHAR;
                strcpy(globalstate->headerfields[fieldindex], curfield);
                fieldindex++;
                i++;
                curchar++;
                curchar = curfield;
                globalstate->totalnumfields++;
            }
            break;
        case FDELIMITER:
            i++;
            break;
        case NEWLINE:
            endoffields = 1;
            break;
        case NULCHAR:
          errorprocessor(NULSTRING);
          break;
        default:
            i++;
            curchar++;
            break;
      }

    } while (!endoffields);

    for (fieldindex++; fieldindex < MAXNUMFIELDS; fieldindex++){
        globalstate->headerfields[fieldindex][0] = NULCHAR;
    }
    return;
}

void holiday_rules_resetfile(FILE *holidayrulefile)
{ /* returns the file pointer to the beginning of the file. */
  fseek(holidayrulefile, 0L, SEEK_SET);
  clearerr(holidayrulefile);

  return;
}		/* -----  end of function resetfile  ----- */

int holiday_rules_closefile(FILE *holidayrulefile)
{
    if( fclose(holidayrulefile) == EOF ) { /* close input file   */
    fprintf (stderr, "Couldn't close holiday rule file; %s\n",
	         strerror(errno) );
    exit (EXIT_FAILURE);
    }
    return 0;
}		/* -----  end of function closefile  ----- */

/*-----------------------------------------------------------------------------
 * Process Holiday Rules
 *----------------------------------------------------------------------------*/

/*
 * File Format: Comma Separated Values.  Fields: Type, Rule, Holiday, Source.
 * The rule field is organized as Month-DayOfWeek-Number.  The "number"
 * sub-field of the rule can contain (1) the week number e.g., 4 for the 4th
 * week, (2) the word "all" for every week- day of the month (e.g., all
 * Saturdays), (3) "last" for the last week-day (e.g., last Monday), or (4)
 * "first" for the first week-day (e.g., first Tuesday).
 */

int holiday_tbl_checkrule(struct DateTime *dt, struct HolidayNode *rulenode)
{

    struct HolidayNode *rulecheck;
    
    rulecheck = rulenode;
    while (rulecheck != NULL) {
        switch (rulecheck->rule.ruletype)
            {
                case 'a': /* fall through */
                case 'A':
                    if (rulecheck->rule.day == dt->day)
                        return 1;
                    break;
                case 'r': /* fall through */
                case 'R':
                    if (rulecheck->rule.wkday == dt->day_of_week)
                    {
                        /* previous line tests to see if day of week matches. */
                        if ((rulecheck->rule.wknum == LASTWEEK)
                             && islastxdom(dt))
                        {
                            return 1;
                        }
                        else if (dt->day >=
                                ((rulecheck->rule.wknum-1)*WEEKDAYS+1) &&
                                dt->day <= (rulecheck->rule.wknum*WEEKDAYS))
                        {
                            /* Prev. line tests to see if the day is in the
                            proper week. The formula "(wknum-1)*7+1" gets the
                            first day of the applicable week; "wknum*7"
                            calculates the last day of the applicable week.  */

                            return 1;
                        }
                    }
                    break;
                case 'w': /* fall through */
                case 'W':
                    if(rulecheck->rule.wkday == dt->day_of_week)
                        return 1;
                    break;
                default:
                    /* fall through */
                    break;
            }
        rulecheck = rulecheck->nextrule;
    }
    return 0;
}


/*-----------------------------------------------------------------------------
 * DAY OF WEEK FUNCTIONS
 *----------------------------------------------------------------------------*/

/*
 * Usage/Limitations: This function calculates the day of the week based on
 * Sakamoto's formula.  The formula is accurate for any date in the range
 *                    September 14, 1752 December 31, 9999
 *
 * Explanation: Sakamoto's code in C is as follows:
 *   int dow(int y, int m, int d)
 *      {
 *       static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
 *       y -= m < 3;
 *       return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
 *      }
 *
 * In the above, y, m and d are, respectively the year (e.g., 1988), month
 * (1-12) and day of the month (1-31).  The function returns 0 = Sunday,
 * 1 = Monday, etc.  As the algorithm performs no range checks on the
 * function arguments, unreasonable dates will produce erroneous results
 * or run-time errors. TODO: Add error checking.
 *
 * References. Devised by Tomohiko Sakamoto[1] in 1993.
 * Source. Text copied from Wikipedia.              
 *
 */

int derive_weekday(const struct DateTime *dt)
{
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
        /* This is some factor that gets applied to the formula depending on
         * what month is at issue. I'm not sure where it comes from
         */
 
    int year; 
    year = dt->year;
    year -= dt->month < 3;

    if ((dt->year > 9999) || (dt->year < 1752) || ((dt->year == 1752) &&
       (dt->month < 9)) || ((dt->year == 1752) && ((dt->month == 9) &&
        (dt->day < 14)))) {
        /* printf("#################################################\n");
        printf("## Temporary warning in func derive_weekday    ##\n");
        printf("## Date out of range of forumla to derive      ##\n");
        printf("## the week day. Date is %d/%d/%d (Y/M/D).     ##\n",
                dt->year, dt->month, dt->day);
        printf("## Formula is accurate between September 14,   ##\n");
        printf("## 1752 and December 31, 9999.                 ##\n");
        printf("#################################################\n"); */

        return - 1;
    } else {
        return ((year + year/4 - year/100 + year/400 +
            t[dt->month-1] + dt->day) % 7); 
    }
}

void set_weekday(struct DateTime *dt)
{
    dt->day_of_week = derive_weekday(dt);

    return;
}
/*-----------------------------------------------------------------------------
 *  DATE CALCULATIONS
 *----------------------------------------------------------------------------*/

int isweekend(struct DateTime *dt)
{
    /* check to see whether day_of_week has been initialized yet */
    /* if not set, call set_weekday rather than generate an error.  */
    if (dt->day_of_week == DAYNOTSET)
        set_weekday(dt);
    else if (dt->day_of_week < SUNDAY || dt->day_of_week > SATURDAY)
        set_weekday(dt);

    if ((dt->day_of_week == SATURDAY) || (dt->day_of_week == SUNDAY))
        return 1;
    else return 0;
}

int isleapyear(struct DateTime *dt)
{
    return (dt->year%4 == 0 && (dt->year%100 != 0 || dt->year%400 == 0));
}

/*
 * Name: jdncvrt
 *
 * Description: Converts a Gregorian calendar date to a Julian Day Number (JDN).
 *
 * Notes:   This algorithm was published onlne by Aesir Research.
 * The algorithm referenced several interim variables, but it was not clear
 * to me what the variables referenced.  Thus, I have retained the variable
 * names used by Aesir without change.  These  variables are not used in any
 * place other than the function. So I felt it acceptable to use the names
 * "as is." The algorithm is saved in the research subfolder titled "Julian Day
 * Number - Algorithm Forms.txt"
 *
 * The algorithm results in a Julian Day Number, when tested with Jan. 1,
 * 2000, of 2,451,544.  Wikipedia lists the JDN for that date as 2,454,545,
 * and the Julian Date Calculator at www.convertalot.com lists it as
 * 2,451,544.5. (I think the wikipedia entry may just be wrong.)
 *
 * I believe these differences are the result of the fact that pure Julian 
 * Day Numbers go from noon to noon, and account for the time of day.  The 
 * 0.5 difference does not appear to create any cumulative error that
 * spills over to other dates, so this does not present an issue for this
 * function or other functions that use the JDN.
 *
 * References: Algorithm was found at:
 * at http://mysite.verizon.net/aesir_research/date/jdalg2.htm. Aesir
 * research of Onset Massachusetts conducts research into AI, consumer
 * electronics, scientific instrumentations, optics, and mathematical
 * algorithms.
 *
 */

        /* FIXME (Thomas#1#): IMPORTANT - STREAMLINE THE AMOUNT OF FUNCTION
        CALLS TO JDNCNVRT.  PERHAPS INITIALIZE THIS WHEN A DATE IS CREATED TO
        SOME MAGIC NUMBER.  THAT WAY IF THE JDN HAS NOT BE CALCULATED THE
        FUNCTION WILL CALCULATE IT, OTHERWISE IT WILL **NOT** RE-CALCULATE IT.
        THIS SHOULD SAVE SOME CLOCK CYCLES!! */

        /* TODO (Thomas#4#): instead of having jdncnvrt return an int, consider
        having the function simply assign the JDN to the struct member data
        object jdn. */

int jdncnvrt(struct DateTime *dt)
{
    int m; /* m is a temporary variable for month. so we don't change month
                it's only used to calculate f. */
    int z; /* z is the adjusted year, the year starts at March to put leap days
            at the end of the year. */
    float f;

    if (dt->month >= 3)
        z = dt->year;
        else z = dt->year; /* FIXME looks like the if and else do the same thing. */
    m = dt->month;
    if (m < 3){
        m += 12;
        z = z -1;
    }

    f = floor(30.6*m-91.4); /* I'm not sure why, but this variable f is never
                             actually used.  I think it is becaue of the
                             different options that can be selected for the
                             algorithm. */

    return dt->day + (153 * m - 457) / 5 + 365 * z + (z / 4) -
        (z / 100) + (z / 400) + 1721118.5;


    /* dt->day + (153 * m - 457) / 5 + 365 * z + floor(((float)z/4)) -
        floor(((float)z/100)) + floor(((float)z/400)) + 1721118.5; */

}

/*
 * Name: jdn2greg
 *
 * Description: Converts a Julian Day Number to the Gregorian calendar date.
 *
 * Notes:   The algorithm referenced several interim variables, but it was
 * not clear to me what the variables referenced.  Thus, I have retained
 * the variable names used by Dr. Math without change. These variables are
 * not used in any place other than the function. So I felt it acceptable
 * to use the names "as is." The algorithms are stored in the research
 * subfolder titled "Dr Math re Julian to Calendar Date Conversion.txt"
 *
 * For some inexplicable reason, the formula when used exactly as
 * described by Dr. Math results in a calculation that is one day short of
 * the actual JDN resulting from the jdncvrt function.  Consequently, I
 * added a "+1" constant to the formula.
 *
 * I believe the difference may be a result of the use of the greatest
 * integer (the floor function in C) in his algorithm, but my use of
 * simple integer math.  I am not sure.  Needless to say, the algorithm
 * appears to produce accurate results using my correction factor.
 *
 * References: This algorithm was published onlne by "Ask Dr. Math" at
 * http://mathforum.org/library/drmath/view/51907.html. Dr. Math
 * referenced two algorithms: Jean Meeus's algorithm in "Astronomical
 * Formulae for Calculators"; or the Fliegel-Van Flandern algorithm given
 * in the Astronomical Almanac.  I used the Meeus version, which should
 * work for any positive Julian date.
 *
 */

void jdn2greg(int jdn, struct DateTime *calc_date)
{
    int a, b, c, d;
    int alpha;
    int z;
    float e, f, m;

    /* run the intermediate calculations */
    z = jdn+1; /* supposed to be +.5. I used 1 to account for rounding errors */
    f = (jdn+1)-z; /* supposed to be +.5. same as above */
    if (z < 2299161)
        a=z;
        else {
            alpha = ((z-1867216.25)/36524.25);
            a = z + 1 + alpha - (alpha/4);
        }
      b = a + 1524;
      c = (b-122.1)/365.25;
      d = 365.25*c;
      e = floor((b-d)/30.6001);

      /* Use the intermediate calculations to derive the day, month, year */

      calc_date->day = b - d - (30.6001*e) + f + 1;
            /* for the date calculation formula, the final "+1" is not normally
             * included. I use it b/c my result is always one day shy of
             * the correct date.  Perhaps it is because of the use of integer
             * math and because we drop the resulting "0.5" from the jdncnvrt
             * algorithm (again b/c of the integers)? Needless to say, it works.
             */
      
      if (e < 13.5)
        calc_date->month = e-1;
      else calc_date->month = e-13;
      m=calc_date->month;
      if (m >= 3)
        calc_date->year = c-4716;
        else calc_date->year = c - 4715; /* the formula used in the original
                                            algorithm had a constant of 4715 */

   return;
}

int date_difference(struct DateTime *date1, struct DateTime *date2)
{
    return jdncnvrt(date2) - jdncnvrt(date1);
}

void date_offset(struct DateTime *orig_date, struct DateTime *calc_date,
                  int numdays)
{

    jdn2greg ((jdncnvrt(orig_date) + numdays), calc_date);
    return;
}

/*
 * Description: calculates the number of courtdays between two dates. Court
 *   days exclude weekends and holidays.  So the offset does not count those 
 *   days.
 *
 * Algorithm: Function uses two nested while loops.  The outer one counts
 * up or down by numdays.  The inner while-loop cycles through each day
 * starting with the day after the original date.  It tests whether the day
 * falls on a holiday.  If the day falls on a holliday, the inner loop  is
 * incremented, but the outerloop is not. Thus, this function could be
 * modified to report back not only the resulting court day, but the number
 * of actual calendar days between the two dates.
 *
 * NOTE: This function counts forward and backward, so references to
 * incrementing can refer to decrementing; references to next day can refer
 * to prior day, as the case may be; day after can refer to day before.
 */


void courtday_offset(struct DateTime *orig_date, struct DateTime *calc_date,
                  int numdays)
{
    int tempday;
        /* since numdays can only be used to count court-days, tempday
         * is used to check ALL days between original date and end
         * date. numdays will only be incremented when tempday is NOT a holiday
         */

    int test; /* test vaariable to see if a date falls on a holiday or not */
    int fwd_back; /* variable to increment up or down depending on whether
                    we are moving forward or backward on the calendar */

    /* set tempday to the same JDN as orig_date */
    orig_date->jdn = jdncnvrt(orig_date);
    tempday = orig_date->jdn;

    /* If numdays == 0, then there is no need to count anything. */
    if(numdays == 0) {
        calc_date->month = orig_date->month;
        calc_date->day = orig_date->day;
        calc_date->year = orig_date->year;
        calc_date->jdn = orig_date->jdn;

        return;
    }
    /* set fwd_back to 1 or -1 depending on whether we are counting forward or
        backward. */
    if(numdays > 0)
            fwd_back = 1; /* count forward */
    else
        fwd_back = -1; /* count backward */


    /* loop through and count the days */
    while (numdays !=0) { /* while there are days to count */
        test = 1;
        while(test == 1) {
            tempday += fwd_back;
            /* tempday starts out == to the original date; we increment it
             * to point to the very next day. If tempday turns out to be a
             * holiday (see below), tempday is incremented at the beginning
             * of each loop iteration until it points to the next non-holiday
             * date. 
             */

            jdn2greg (tempday, calc_date); /* determine the new day */

            test = isholiday(calc_date); /* is the new day a holiday? */
        }
        numdays -= fwd_back; /* decrease numdays -- this means the function has
                                counted one non-holiday*/
    }
    return;
}

/*
 * Description: Counts court days between two dates.
 *
 * NOTE: This function counts forward and backward, so references to
 * incrementing can refer to decrementing; references to next day can refer
 * to prior day, as the case may be; day after can refer to day before.
 *
 * This function could be modified to report back not only the resulting court
 * day, but the number of actual calendar days between the two dates.
 */

int courtday_difference(struct DateTime *date1, struct DateTime *date2)
{
    struct DateTime testdate; /* structure to hold temporary dates for
                                intermediate testing */

    int test; /* test vaariable to see if a date falls on a holiday or not */

    int fwd_back; /* variable to increment up or down depending on whether
                    we are moving forward or backward on the calendar */

    int count = 0; /* the variable to store the date difference count */

    /* FIXME (Thomas#1#): This function does not work properly if the start
    date falls on a holiday.  MUST ADD ERROR HANDLING. */


    /* set tempday to the same JDN as orig_date */
    date1->jdn = jdncnvrt(date1);
    date2->jdn = jdncnvrt(date2);

    /* If numdays == 0, then there is no need to count anything. */
    if(date1->jdn == date2->jdn) {
        return 0;
    }

    testdate.jdn = date2->jdn; /* set testdate to date2, this is our starting
                                counting position ---> zero! */

    /* set fwd_back to 1 or -1 depending on whether we are counting forward or
        backward. */
    if(date1->jdn > testdate.jdn)
            fwd_back = 1; /* count forward */
    else
        fwd_back = -1; /* count backward */


    /* loop through and count the days */
    while (testdate.jdn != date1->jdn) { /* while there are days to count */
        test = 1;
        while(test == 1) {
            testdate.jdn += fwd_back;
                /* we increment testdate to point to the very next (or
                 * previous) day.  if testdate turns out to be a holiday (see
                 * below), testdate is incremented again at the beginning of
                 * the next loop iteration. The loop iterates until testdate
                 * points to a non-holiday date.
                 */

            jdn2greg (testdate.jdn, &testdate); /* determine the new day */

            test = isholiday(&testdate);
                /* is the new day a holiday? If so, don't count it and test the
                 * next (or previous) day
                 */
        }
        count -= fwd_back; /* testdate was not a holiday, so count it! */
    }
    return count;
}

/*
 * Description: calculates whether a particular date is in the last x day
 * of the month.  (E.g., the last Tuesday of February.)
 */

int islastxdom(struct DateTime *dt)
{
    struct DateTime tempdate; /* date struct used to store interim values */
    int daycount;
        /* Count how many days difference between the day of the week of the
         * last week of the first month to the laset x-day of the current month
         */

    if (dt->day < (WEEKDAYS*(MINNUMTTLWKS-1))) {
        /* If the date of month is less then 22 it cannot be the last week.*/
        return 0;
    }
    else {
        /* Set tempdate to first day of next month */
        tempdate.day = 1;
        if (dt->month == 12) {
            tempdate.month = 1;
            tempdate.year = dt->year + 1;
        }

        else {
            tempdate.month = dt->month + 1;
            tempdate.year = dt->year;
        }
        set_weekday(dt);
        set_weekday(&tempdate);
            /* calculate day of week of tempdate */
        jdncnvrt (&tempdate); /*calculate Julian Day Number of tempdate */

        if (tempdate.day_of_week > dt->day_of_week)
            daycount = (tempdate.day_of_week - dt->day_of_week) * -1;
        else
            daycount = (tempdate.day_of_week - dt->day_of_week + WEEKDAYS) * -1;

        date_offset (&tempdate, &tempdate, daycount);

        if (tempdate.day == dt->day)
            return 1;
        else
            return 0;
    }

    return 0;
}


/*
 * Description: calculates whether a particular date is in the last week
 * of the month.
 *
 * Note: not currently used / not currently coded
 * ASSUMES FIRST DAY OF WEEK IS SUNDAY!
 */

int islastweek(struct DateTime *dt)
{
    struct DateTime tempdate; 
    int daycount; 

    if (dt->day < (WEEKDAYS*(MINNUMTTLWKS-1))) { /* if the date of month is less
            then 22 it cannot be the last week. */
        return 0;
    }
    else if (dt->day == daysinmonths[isleapyear(dt)][dt->month]) {
        /* Determine whether the day is the last day of the month. If the date
         * is the last day of the month it is by definition in the last week.
         * The array subscripts work like this.  The first dimension is a one
         * or a zero depending on whether this is a leap year. The second
         * dimension is the month of the date argument.
         */
    
        return 1;
    }
    else {
        /* set tempdate to the last date of the month */
        tempdate.year = dt->year;
        tempdate.month = dt->month;
        tempdate.day = daysinmonths[isleapyear(dt)][dt->month];

        /* calculate day of week of dt and tempdate */
        set_weekday(dt);
        set_weekday(&tempdate);

        /* calculate number of days between the argument and the last day of
            the month. */
        daycount = date_difference(dt, &tempdate);
        if (daycount >= 7)
            return 0;
        else if (dt->day_of_week > tempdate.day_of_week)
            return 0; 
        else
            return 1;
    }

    return 0;
}

int isholiday(struct DateTime *dt)
{
    struct HolidayNode *nodecheckptr;

    /* First, calculate whether this date falls on a weekend */ 
    set_weekday(dt);
    /* if (isweekend(dt))
        return 1;
        commented out because I need to see if when there is an official "rule"
        version picks up the weekend as a holiday. */

    /* Second, calculate whether an ALLMONTHS rule applies and whether this date
    falls on a weekend */

    nodecheckptr = holidayhashtable[ALLMONTHS];
    while(nodecheckptr != NULL)
        {
            if (holiday_tbl_checkrule(dt,nodecheckptr) == 1)
                return 1;
            nodecheckptr = nodecheckptr->nextrule;
        }

    /* Third, calculate whether there are any holidays on the month of the
        argument's date */

    nodecheckptr = holidayhashtable[dt->month];
    while(nodecheckptr != NULL)
        {
            if (holiday_tbl_checkrule(dt,nodecheckptr) == 1)
                return 1;
            nodecheckptr = nodecheckptr->nextrule;
        }

    return 0;
}

void printholidayrules(void)
{
    struct HolidayNode *tempnode;
    int monthctr; /* counter to loop through months */

    for(monthctr = 0; monthctr < TTLMONTHS; monthctr++)
    {
        tempnode = holidayhashtable[monthctr];
        /* sets a temporary pointer to the first node so we traverse
            the list. */

        printf("-------------------------------------------------------\n");
        if (tempnode != NULL)
        {
            switch (monthctr)
            {
            case ALLMONTHS: 
                printf("The holiday rules that apply to all months");
                printf(" are as follows:\n");
                break;
            case JANUARY:
                printf("The holiday rules for January are as follows:\n");
                break;
            case FEBRUARY:
                printf("The holiday rules for February are as follows:\n");
                break;
            case MARCH:
                printf("The holiday rules for March are as follows:\n");
                break;
            case APRIL:
                printf("The holiday rules for April are as follows:\n");
                break;
            case MAY:
                printf("The holiday rules for May are as follows:\n");
                break;
            case JUNE:
                printf("The holiday rules for June are as follows:\n");
                break;
            case JULY:
                printf("The holiday rules for July are as follows:\n");
                break;
            case AUGUST:
                printf("The holiday rules for August are as follows:\n");
                break;
            case SEPTEMBER:
                printf("The holiday rules for September are as follows:\n");
                break;
            case OCTOBER:
                printf("The holiday rules for October are as follows:\n");
                break;
            case NOVEMBER:
                printf("The holiday rules for November are as follows:\n");
                break;
            case DECEMBER:
                printf("The holiday rules for December are as follows:\n");
                break;
            default:
                printf("\n\n\n ERROR BAD MONTH.\n");
                break;
            }
        }
        else
        {
            switch (monthctr)
            {
            case ALLMONTHS:
                printf("No holidays in apply to all months.");
                break;
            case JANUARY:
                printf("No holidays in January.\n");
                break;
            case FEBRUARY:
                printf("No holidays in February.\n");
                break;
            case MARCH:
                printf("No holidays in March.\n");
                break;
            case APRIL:
                printf("No holidays in April.\n");
                break;
            case MAY:
                printf("No holidays in May.\n");
                break;
            case JUNE:
                printf("No holidays in June.\n");
                break;
            case JULY:
                printf("No holidays in July.\n");
                break;
            case AUGUST:
                printf("No holidays in August.\n");
                break;
            case SEPTEMBER:
                printf("No holidays in September.\n");
                break;
            case OCTOBER:
                printf("No holidays in October.\n");
                break;
            case NOVEMBER:
                printf("No holidays in November.\n");
                break;
            case DECEMBER:
                printf("No holidays in December.\n");
                break;
            default:
                printf("\n\n\n ERROR BAD MONTH.\n");
                break;
            }
        }
        while (tempnode != NULL) { /* checks to see if list is empty */
            printf("The applicable holiday is %s.\n",
                   tempnode->rule.holidayname);
            printf("The applicable ruletype is %c.\n",
                   tempnode->rule.ruletype);
            if ((tempnode->rule.ruletype != 'a') &&
                    (tempnode->rule.ruletype != 'A'))
            {
                printf("The applicable weekday is %d.\n",
                       tempnode->rule.wkday);
                printf("The applicable weeknumber is %d.\n",
                       tempnode->rule.wknum);
            }
            else
            {
                printf("The Day is %d.\n", tempnode->rule.day);
            }
            printf("The governing authority is %s.\n",
                   tempnode->rule.authority);
            printf("\n");
            /* move to the next node */
            tempnode = tempnode->nextrule;
        }

    }
    return;
}

/*
 * Name: wkday_to_string
 *
 * Description: Gets the weekday string corresponding to a day of the enum days.
 *
 * Parameters: Takes an integer corresponding to a day of the week.
 *
 * Return: The string of the day name or a NULL pointer.
 * 
 */

const char* wkday_to_string(int day)
{
    switch (day) {
        case SUNDAY:
            return "Sunday";
            break;
        case MONDAY:
            return "Monday";
            break;
        case TUESDAY:
            return "Tuesday";
            break;
        case WEDNESDAY:
            return "Wednesday";
            break;
        case THURSDAY:
            return "Thursday";
            break;
        case FRIDAY:
            return "Friday";
            break;
        case SATURDAY:
            return "Saturday";
            break;
        default:
            /* do nothing */
            break;
    }

    return NULL;
}

/*
 * Name: month_to_string
 * 
 * Description: Gets the month corresponding to a month of the enum MONTHS. 
 *
 * Parameters: Takes an integer corresponding to a MONTH of the year.
 *
 * Return: A string corresponding to the month of the year, or a NULL pointer.
 *
 */

const char* month_to_string(int month)
{
    switch (month) {
        case JANUARY:
            return "January";
            break;
        case FEBRUARY:
            return "February";
            break;
        case MARCH:
            return "March";
            break;
        case APRIL:
            return "April";
            break;
        case MAY:
            return "May";
            break;
        case JUNE:
            return "June";
            break;
        case JULY:
            return "July";
            break;
        case AUGUST:
            return "August";
            break;
        case SEPTEMBER:
            return "September";
            break;
        case OCTOBER:
            return "October";
            break;
        case NOVEMBER:
            return "November";
            break;
        case DECEMBER:
            return "December";
            break;
        default:
            /* do nothing */
            break;
    }

    return NULL;
}

void date_to_string(char * rtnstring, const struct DateTime *dt, int date_order)
{
    switch(date_order){
        case MDY:
            sprintf(rtnstring, "%d/%d/%d", dt->month, dt->day, dt->year);
            break;
        case YMD:
            sprintf(rtnstring, "%d/%d/%d", dt->year, dt->month, dt->day);
            break;
        default:
            break;
        
    }
    return;
}

void errorprocessor(int error_code)
{
    /* not yet coded */
    fprintf (stderr, "Error:%d\n", error_code);
    exit(0);
    return;
}

#ifdef UNDEF /* presently the remainng code in this source file is
                removed from compilation for testing purposes. */

/* Add ideas for functions here! */


#endif /* UNDEF */
