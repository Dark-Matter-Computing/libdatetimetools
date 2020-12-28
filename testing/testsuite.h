/*-----------------------------------------------------------------------------
 * Symbolic Constants
 *----------------------------------------------------------------------------*/

enum TESTFILETYPES {
    DERIVEDATES = 4,
    CALCS = 5
};


struct teststats{
    int ttl_tests;
    int successful_tests;
};

/*-----------------------------------------------------------------------------
 * Prototypes 
 *----------------------------------------------------------------------------*/
void holidayprinttest(struct DateTime *dt);
void testsuite_check_calcs(const char *receivedtestfile);
void testsuite_check_leap(const char *receivedtestfile);
void testsuite_check_deriveday(const char *receivedtestfile);
void testsuite_checkholidays(void);
void test_loop(enum TESTFILETYPES testtype, FILE *testfile, struct teststats *stats);
void print_stats(struct teststats *printstats);
