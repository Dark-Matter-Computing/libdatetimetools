#ifndef _TESTSUITE_H_INCLUDED_
#define _TESTSUITE_H_INCLUDED_
#include <string.h>

/*-----------------------------------------------------------------------------
 * Symbolic Constants
 *----------------------------------------------------------------------------*/

enum TESTFILETYPES {
    DERIVEDATES = 4,
    CALCS = 5,
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
void testsuite_check_calcs(const char *receivedtestfile);
void testsuite_check_deriveday(const char *receivedtestfile);
void testsuite_check_holidays(const char *receivedtestfile);
void testsuite_check_leap(const char *receivedtestfile);
void test_loop(enum TESTFILETYPES testtype, FILE *testfile, struct teststats *stats);

/* Display Manager */
void display_stats(struct teststats *printstats);
void display_results(const char *message, int testphase);
void display_frame(const char *section_name, int framepos);
void print_repeat_char(int count, const char s);
void message_right_justify(char *text, const char * addedtext, int linewidth);

#endif /*  _TESTSUITE_H_INCLUDED_ */
