/*******************************************************************************
* Date Tools.c - This is a program to test various date function algorithms    *
*                which will be incorporated into the CaseMaker program for     *
*                attorneys.
*                                                                              *
* Author: Thomas Vidal                                                         *
*                                                                              *
* Purpose: Test various date computations, such as day of the week.            *
*                                                                              *
* Usage: The program sets up a test suite to test the date functions           *
*                                                                              *
* References: The program tests several different algorithms, which are        *
*             described at the introduction to the particular functions.       *
*             I obtained the algorithms from Wikipedia at:                     *
*             http://en.wikipedia.org/wiki/Calculating_the_day_of_the_week     *
*                                                                              *
* File/Data Formats: None incorporated as of August 18, 2011.                  *
*                                                                              *
* Restrictions:  Some of the algorithms are perpetual, some are valid          *
*                through a certain century                                     *
*                                                                              *
* Revision History: The first version was started on 8/18/2011                 *
*                                                                              *
* Error Handling: [describe]                                                   *
*                                                                              *
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*******************************************************************************
* Data Type Declarations                                                       *
*                                                                              *
*******************************************************************************/
enum days {
    Sunday = 0,
    Monday = 1,
    Tuesday = 2,
    Wednesday = 3,
    Thursday = 4,
    Friday = 5,
    Saturday = 6
};

struct DATETIME
{
    int year;
    int month;
    int day;
    enum days day_of_week;
};

/*******************************************************************************
* Function prototypes                                                          *
*                                                                              *
*******************************************************************************/

void wkday_lookup (struct DATETIME *dt);
void wkday_zeller (struct DATETIME *dt);
int wkday_sakamoto (struct DATETIME *dt);
int isweekend (struct DATETIME *dt);
int jdncnvrt (struct DATETIME *dt);
void jdn2greg (int jdn, struct DATETIME *calc_date);
int date_difference (struct DATETIME *date1, struct DATETIME *date2);
void date_offset (struct DATETIME *orig_date, struct DATETIME *calc_date,
                  int numdays);


/*******************************************************************************
********************************************************************************
********** MAIN PROGRAM BEGINS BELOW.                                 **********
**********                                                            **********
**********                                                            **********
********************************************************************************
*******************************************************************************/

int main()
{
    struct DATETIME date1; /* variable to test date calculations */
    struct DATETIME date2; /* second variable to test date calculations */
    struct DATETIME date3; /* third test variable to use for date  */
    /* struct DATETIME *tst_ptr; / pointer to datetime */
    int numdays = 366; /* this is a variable to test the date offset function.
                         25 is because the difference between 8/18/11 and
                         9/12/11 = 25 days. */

    /* Initialization of Variables date1 and date2 */

    date1.year = 1957; /* first date is 8/18/2011 */
    date1.month = 10;
    date1.day = 4;   /* A Thursday */

    date2.year = 2012; /* second date is 9/12/2011 */
    date2.month = 8;   /* this second date should be 25 days after date1 */
    date2.day = 18;    /* A Monday */

    /* tst_ptr = &date1;  / initialize pointer */


    /* test Sakamoto's formula */

    date1.day_of_week = wkday_sakamoto (&date1);
    date2.day_of_week = wkday_sakamoto (&date2);
    printf("The day of the week is %d\n", date1.day_of_week);
    if (isweekend(&date2))
        printf("That day falls on a weekend.\n");
        else printf ("That day falls on a weekday.\n");
    printf("The day of the week is %d\n", date2.day_of_week);
    if (isweekend(&date2))
        printf("That day falls on a weekend.\n");
        else printf ("That day falls on a weekday.\n");

    /* test date calculations */
    printf("\n\n=============================================\n");
    /* printf("The absolute date number for date1 =%d\n", absdatenum(&date1)); */
    printf("The Julian date for date1 = %d\n\n", jdncnvrt (&date1));
    /* printf("The absolute date number for date2 =%d\n\n", absdatenum(&date2)); */
    printf("The Julian date for date2 = %d\n\n", jdncnvrt (&date2));
    /* absdatenum2date (absdatenum(&date1), &date3); */

    printf("There are %d days between date1 and date2\n",
           date_difference(&date1, &date2));
    printf("There are %d days between date2 and date1\n\n",
           date_difference(&date2, &date1));

    date_offset(&date1, &date3, numdays);
    date3.day_of_week = wkday_sakamoto (&date3);

    printf("Using the Num for Date 1, Date 3 = %d, %d/%d/%d\n",
           date3.day_of_week, date3.month, date3.day, date3.year);

    printf("The due date is %d, %d/%d/%d\n",
           date3.day_of_week, date3.month, date3.day, date3.year);
    printf("That date is %d days AFTER date1\n", numdays);
    printf("The Julian date for date2 = %d\n\n", jdncnvrt (&date3));

    printf("\n\n=============================================\n");

    return 0;
}

/*******************************************************************************
********************************************************************************
********** DAY OF WEEK CALCULATIONS                                   **********
**********   The following are the functions implementing various     **********
**********   different algorithms to derive the day of the week for   **********
**********   a given date                                             **********
********************************************************************************
*******************************************************************************/

/*******************************************************************************
* Algorithm #1 Using Lookup Tables                                             *
*                                                                              *
* Usage/Limitations: The algorithm is valid for the Gregorian calendar.        *
*                                                                              *
* Text copied from Wikipedia.                                                  *
*                                                                              *
* Britain and its colonies started using the Gregorian calendar on Thursday,   *
* September 14, 1752 (the previous day was Wednesday, September 2).            *
* The areas now forming the United States adopted the calendar at different    *
* times depending on the colonial power; Spain, France, Italy, and others had  *
* started using it in 1582, while Russia still had not done so by 1867 when    *
* U.S. purchased Alaska.                                                       *
*                                                                              *
* This algorithm shows how to come up with four numbers. Find the sum of       *
* these four and using modulus to restrict the result to 0 through 6,          *
* the day of the week can be determined. Since this algorithm uses the         *
* "zeroth" day, we can add the day of the month directly (without              *
* subtracting 1).... The four numbers are:                                     *
*                                                                              *
*   1. Centuries: First, we can either refer to the centuries table below or   *
*       use the rule: Where century is the first two digits of the year,       *
*       define c = 2(3 - (century\mod {4})). With Sunday being day 0, these    *
*       numbers are the day of the week that January 0 (December 31 of the     *
*       previous year), fell on year 0 of the century, with one added to the   *
*       value when century mod 4 is 0. This corrects for the fact that the     *
*       next step undercounts the number of leap days by 1 on centuries        *
*       whose first two digits are evenly divisible by 4, such as the 21st     *
*       century, where year 0 (2000) was a leap year. (See Leap year).         *
*                                                                              *
*   2.  Years: Because there are 365 days in a common year, which is 52 weeks  *
*       plus 1 day, each year will start on the day of the week after that     *
*       starting the preceding year. Each leap year has of course one more day *
*       than a common year. Assuming we know on which day a century starts     *
*       (from above), if we add the number of years elapsed since the start of *
*       the century, plus the number of leap years that have elapsed since     *
*       the start of the century, we get the day of the week on which the year *
*       starts. Where year is the last two digits of the year,                 *
*       define y = year + {year/4}.                                            *
*                                                                              *
*   3.  Months: We refer to the months table below to work out on which day of *
*       the week a month starts. Notice that January starts on day 0, which is *
*       simply another way of saying that the year and January of that year    *
*       start on the same day. The months table shown allows for leap years;   *
*       other algorithms leave the correction to the end and then deduct 1     *
*       from the final figure if the month is a January or February of a leap  *
*       year.                                                                  *
*                                                                              *
*   4.  Day of the Month: Once we know on which day of the week the month      *
*       starts, we simply add the day of the month to find the final result    *
*       (noting that as mentioned above, we've been working with the "zeroth"  *
*       day of the month as the start).                                        *
*******************************************************************************/

void wkday_lookup (struct DATETIME *dt)
{
    return;
}

/*******************************************************************************
* Algorithm #2 Zeller algorithm                                                *
*                                                                              *
* Usage/Limitations: The algorithm is valid for the Gregorian calendar.        *
*                                                                              *
* Text copied from A Craps Tutorial                                            *
*       http://crapsmath.com/perpet_box/algorithm.htm                          *
*                                                                              *
* Formula wkday = (d + [2.6 * m - 0.2 ] + [y/4] + [c/4] - 2c) mod 7            *
*                                                                              *
*   d: day (1 to 31)                                                           *
*   m: shifted month (March=1,...February=12), eg. [((month + 9) % 12) + 1]    *
*       January and February are the 11 and 12 months of the PREVIOUS year.    *
*   y: last 2 digits of Y  [year/100] y is an integer (subtract by 1 if m =    *
*      January or February.                                                    *
*   c: first 2 digits of Y [d = year - (100*c)                                 *
*   w: day of week (0=Sunday,..6=Saturday)                                     *
*   NOTE: use integer division for [y/4] and [c/4]. We need the greatest       *
*         integer less than or equal to x                                      *
*                                                                              *
* References:   A derivation of the formula for the week day can be found in   *
*               the book Elementary Number Theory by J.V. Uspensky and M.A.    *
*               Heaslet. © 1939, McGraw-Hill Book Company. Pages 206-211.      *
*                                                                              *
* NOTE: I am having difficulty with this formula for January and February      *
*       dates.  I found another formula for Zeller's method with some slight   *
*       variations.  That is saved under "Date Calculation Algorithms.02.txt"  *
*******************************************************************************/

void wkday_zeller (struct DATETIME *dt)
{
    return;
}

/*******************************************************************************
* Algorithm #3 Sakamoto's Algorithm                                            *
*                                                                              *
* Usage/Limitations: The formula is accurate for any date in the range         *
*                    September 14, 1752–December 31, 9999                      *
*                                                                              *
* Source. Text copied from Wikipedia.                                          *
*                                                                              *
* Explanation.                                                                 *
*                                                                              *
* Sakamoto's code in C is as follows:                                          *
*   int dow(int y, int m, int d)                                               *
*      {                                                                       *
*       static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};                 *
*       y -= m < 3;                                                            *
*       return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;                     *
*      }                                                                       *
*                                                                              *
*   In the above, y, m and d are, respectively the year (e.g., 1988), month    *
*   (1-12) and day of the month (1-31).  The function returns 0 = Sunday, 1 =  *
*   Monday, etc.  As the algorithm performs no range checks on the function    *
*   arguments, unreasonable dates will produce erroneous results or run-time   *
*   errors.                                                                    *
*                                                                              *
*   I have modified the function to work with my data type                     *
*                                                                              *
* References. Devised by Tomohiko Sakamoto[1] in 1993.                         *
*******************************************************************************/

int wkday_sakamoto (struct DATETIME *dt)
{
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4}; /* I'm not sure what
                                                               this does. */
    dt->year -= dt->month < 3;

    return (dt->year + dt->year/4 - dt->year/100 + dt->year/400 +
            t[dt->month-1] + dt->day) % 7;
}

/*******************************************************************************
********************************************************************************
********** FUNCTION DECLARATIONS FOR DATE CALCULATIONS                **********
**********                                                            **********
**********                                                            **********
********************************************************************************
*******************************************************************************/

/*******************************************************************************
***************************   FUNCTION DEFINITION   ****************************
* Name: isweekend                                                              *
*                                                                              *
* Description: calculates whether a certain date falls                         *
*                                                                              *
* Arguments: Takes a pointer to a DATETIME structure.                          *
*                                                                              *
* Return: Returns an integer equal to zero if the date is NOT a weekend, or a  *
*         1 if the date IS a weekend.                                          *
*                                                                              *
********************************************************************************
*******************************************************************************/

int isweekend (struct DATETIME *dt)
{
    if (dt->day_of_week == Saturday || Sunday)
        return 1;
        else return 0;
}

/*******************************************************************************
***************************   FUNCTION DEFINITION   ****************************
* Name: jdncvrt                                                                *
*                                                                              *
* Description: converts a Gregorian calendar date to a Julian Day Number (JDN).*
*              The purpose of using the JDN is to create a unique number for   *
*              a day that we can use to make date calculations, such as the    *
*              number of days between two dates or the number of days before   *
*              or after a specified date. This function does the opposite of   *
*              the jdn2greg function.                                          *
*                                                                              *
* Arguments: Takes a pointer to a DATETIME structure.                          *
*                                                                              *
* Return: Returns an integer with the Julian Date.  Usually this number is     *
*         not stored for "permanent use" but is used as an argument for        *
*         anothe function, such as the date_offset function.                   *
*                                                                              *
* References: This algorithm was published onlne by Aesir Research.  It was    *
*             found at http://mysite.verizon.net/aesir_research/               *
*             date/jdalg2.htm. Aesir research of Onset Massachusetts           *
*             conducts research into AI, consumer electronics, scientific      *
*             instrumentations, optics, and mathematical algorithms.           *
*                                                                              *
*                                                                              *
* Notes:   The algorithm referenced several interim variables, but it was not  *
*          clear to me what the variables referenced.  Thus, I have retained   *
*          the variable names used by Aesir without change.  These variables   *
*          are not used in any place other than the function. So I felt it     *
*          acceptable to use the names "as is." The algorithm is saved in the  *
*          research subfolder titled "Julian Day Number - Algorithm Forms.txt" *
*                                                                              *
*          The algorithm results in a Julian Day Number, when tested with      *
*          Jan. 1, 2000, of 2,451,544.  Wikipedia lists the JDN for that date  *
*          as 2,454,545, and the Julian Date Calculator at www.convertalot.com *
*          lists it as 2451544.5.                                              *
*                                                                              *
*          I believe these differences are the result of the fact that pure    *
*          Julian Day Numbers go from noon to noon, and account for the time   *
*          of day.  The 0.5 difference does not appear to create any           *
*          cumulative error that spills over to other dates, so this does not  *
*          present an issue for this function or other functions that use      *
*          the JDN.                                                            *
*                                                                              *
********************************************************************************
*******************************************************************************/

int jdncnvrt (struct DATETIME *dt)
{
    int m;
    int z;
    float f;

    z = dt->year + (dt->month-14)/12;
    m = dt->month;
    if (m < 3)
        m += 12;
    f = floor(30.6*m-91.4);

   return dt->day + (153 * m - 457) / 5 + 365 * dt->year + (dt->year / 4) -
        (dt->year / 100) + (dt->year / 400) + 1721118.5;
}

/*******************************************************************************
***************************   FUNCTION DEFINITION   ****************************
* Name: jdn2greg                                                               *
*                                                                              *
* Description: Converts a Julian Day Number to the Gregorian calendar date.    *
*              The purpose of using the JDN is to create a unique number for   *
*              a day that we can use to make date calculations, such as the    *
*              number of days between two dates or the number of days before   *
*              or after a specified date. This function does the opposite      *
*              of the jdncvrt function.                                        *
*                                                                              *
* Arguments:  Takes an integer representing a JDN and a pointer to a DATETIME  *
*             structure.                                                       *
*                                                                              *
* Return: Does not return a value, but stores the value of the Gregorian       *
*         calendar date into the DATETIME structure that is passed to the      *
*         function through the pointer.                                        *
*                                                                              *
* References: This algorithm was published onlne by "Ask Dr. Math" at          *
*             http://mathforum.org/library/drmath/view/51907.html. Dr. Math    *
*             referenced two algorithms: Jean Meeus's algorithm in             *
*             "Astronomical Formulae for Calculators"; or the Fliegel-Van      *
*             Flandern algorithm given in the Astronomical Almanac.            *
*             I used the Meeus version, which should work for any positive     *
*             Julian date.                                                     *
*                                                                              *
* Notes:   The algorithm referenced several interim variables, but it was not  *
*          clear to me what the variables referenced.  Thus, I have retained   *
*          the variable names used by Dr. Math without change. These variables *
*          are not used in any place other than the function. So I felt it     *
*          acceptable to use the names "as is." The algorithms are stored in   *
*          the research subfolder titled "Dr Math re Julian to Calendar Date   *
*          Conversin.txt"                                                      *
*                                                                              *
*          For some inexplicable reason, the formula when used exactly as      *
*          described by Dr. Math results in a calculation that is one day      *
*          short of the actual JDN resulting from the jdncvrt function.        *
*          Consequently, I added a "+1" constant to the formula.               *
*                                                                              *
*          I believe the difference may be a result of the use of the greatest *
*          integer (the floor function in C) in his algorithm, but my use of   *
*          simple integer math.  I am not sure.  Needless to say, the          *
*          algorithm appears to produce accurate results using my correction   *
*          factor.                                                             *

********************************************************************************
*******************************************************************************/
void jdn2greg (int jdn, struct DATETIME *calc_date)
{
    int a, b, c, d;
    int alpha;
    int z;
    float e, f, m;

    /* run the intermediate calculations */
    z = jdn+.5; /* supposed to be +.5 */
    f = (jdn+.5)-z; /* supposed to be +.5 */
    if (z < 2299161)
        a=z;
        else
        {
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
                included. I use it b/c my result is always one day shy of
                the correct date.  Perhaps it is because of the use of integer
                math and because we drop the resulting "0.5" from the jdncnvrt
                algorithm (again b/c of the integers)? Needless to say, it
                works. */
      if (e < 13.5)
        calc_date->month = e-1;
      else calc_date->month = e-13;

      if (m > 2.5)
        calc_date->year = c-4716;
        else calc_date->year = c - 4715;

   return;
}

/*******************************************************************************
* date_difference - calculates the number of days between two dates.           *
*                                                                              *
*******************************************************************************/

int date_difference (struct DATETIME *date1, struct DATETIME *date2)
{

    int d1; /* the date number for the first date */
    int d2; /* the date number for the second date */

    d1 = jdncnvrt(date1);
    d2 = jdncnvrt(date2);
    return d1-d2;
}

/*******************************************************************************
* date_offset - calculates the day after a number days.                        *
*                                                                              *
*******************************************************************************/

void date_offset (struct DATETIME *orig_date, struct DATETIME *calc_date,
                  int numdays)
{
    jdn2greg ((jdncnvrt(orig_date) + numdays), calc_date);
    return;
}

/*
        Add functions to test date validity.

To check if a date is on the calendar:

  if [y,m,d] = d(g(y,m,d))


*/
/*******************************************************************************
* Functions that I tested or considered by that don't work or weren't          *
* implemented.                                                                 *
*******************************************************************************/

#ifdef UNDEF

/* Function Prototypes */
int absdatenum (struct DATETIME *dt);
void absdatenum2date (int datenum, struct DATETIME *calc_date);

/*******************************************************************************
* abstatenum is a function to calculate the absolute day number of a           *
* particular day of the year.  This funcion is used in several date            *
* such as figuring out the amount of days between a certain two dates.         *                                                                              *
*******************************************************************************/

int absdatenum (struct DATETIME *dt)
{
    int mo; /* month */
    int yr; /* day */

    mo = (dt->month + 9) % 12; /*calculate the month no. using the Gaussian
                                 month numbering sequence (March = 1) */
    yr = dt->year - mo/10;
    return 365*yr + yr/4 - yr/100 + yr/400 + (mo*306 + 5)/10 + ( dt->day - 1 );
}

/*******************************************************************************
* absdatenum2date takes an absdatenum and converts it to a calendar date.      *
* As of 8/18/2011 - it seems like this algorithm is only valid for certain yrs *
* it is not clear from the source of the code, G. Katch, what the valid date   *
* ranges are.
*                                                                              *
*******************************************************************************/

void absdatenum2date (int datenum, struct DATETIME *calc_date)
{
    int ddd; /* I have no idea what this variable is for */
    int mi; /* I have no idea with this variable is for */
    int mm; /* temporary month? */
    int yr; /* temporary year? */

    yr = (10000 * datenum + 14780) / 3652425;
    ddd = datenum - (365*yr + yr/4 - yr/100 + yr/400);
    if (ddd < 0)
    {
        yr--;
        ddd = datenum - (365*yr + yr/4 - yr/100 + yr/400);
    }
    mi = (52 + 100 * ddd)/3060;
    calc_date->month = (mi + 2)%12 + 1;
    calc_date->year = yr + (mi + 2)/12;
    calc_date->day = ddd - (mi*306 + 5)/10 + 1;
    return;

}

#endif /* UNDEF */
