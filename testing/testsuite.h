#ifndef _TESTSUITE_H_INCLUDED_
#define _TESTSUITE_H_INCLUDED_
#include <string.h>

/*-----------------------------------------------------------------------------
 * Symbolic Constants
 *----------------------------------------------------------------------------*/

enum TESTFILETYPES {
    DERIVEDATES,
    CALCS,
    LEAPDATES,
    RULECHECK
};


struct teststats{
    int ttl_tests;
    int successful_tests;
};

/*-----------------------------------------------------------------------------
 * Prototypes 
 *----------------------------------------------------------------------------*/

/* Test Batteries */
void testsuite_interactive(void);
void testsuite_run_check(enum TESTFILETYPES testtype, const char *testfile_name);
void testsuite_check_calcs(FILE *openedtestfile);
void testsuite_check_deriveday(FILE *openedtestfile);
void testsuite_check_holidays(FILE *openedtestfile);
void testsuite_check_leap(FILE *openedtestfile);

/* Display Manager */
void display_stats(struct teststats *printstats);
void display_results(const char *message, int testphase);
void display_frame(const char *section_name, int framepos);
void print_repeat_char(int count, const char s);
void message_right_justify(char *text, const char * addedtext, int linewidth);

#endif /*  _TESTSUITE_H_INCLUDED_ */
