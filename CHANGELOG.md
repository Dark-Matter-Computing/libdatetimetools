## 1.0.0 (February 05, 2021)
  - Merge branch 'errorhandling' to bring fixes into master
  - Change date_difference parameters to structs; Add calendar-date unit test
  - Fixed file handling to deal with CRLF line endings
  - Added test for computing date using court days
  - Debugged courtday_difference; added testsuite
  - Initiate Error Handling Module
  
  ## 0.0.11 (January 11, 2021)
  - Added testsuite_interactive
  - Removed test_loop in favor of testsuite_run_check
  - Minor cleanups re MAXMESSAGELEN and commenting date_to_string
  - Added display manager to test suite; Added date_to_string
  - Improved data hiding; Improved function naming
  - Added the Xcode project
  - Add .gitattributes; modified .gitignore
  
  ## 0.0.10 (December 28, 2020)
  - Validating day_of_week precondition
  - Add new set_weekday function
  - Built initial unit test and test scripts
  - Update Makefile with install; start preparing timetools
  - Cleanups after the ALLMONTHS merge.
  
  ## 0.0.9 (December 26, 2020)
  - Merge pull request #1 from Dark-Matter-Computing/ALLMONTHS
  - Merge branch 'master' into ALLMONTHS
  - Refactored with ALLMONTHS at 0 and tested. Results were clear.
  - Refactored the code to define ALLMONTHS as 0 instead of 13.
  - neglected to add the file.
  - created version.h
  
  ## 0.0.8 (December 23, 2020)
  - Cleaned up the internal header file. Renamed the weekday fuction to something that made more sense for users. Added range checking for the weekday fuction. Began cleaning up comments.
  - Removed the diagnostic printfs after debugging the checkrule function.
  - Debugged the checkrule function.
  - removed some remaining diagnostic printif's remaining from debugging the holiday hash table.
  - Removed diagnostic printf commands after debugging the holiday hash table.
  - Finished debugging the holiday hash table, before unit testing.
  - Debugging the add_rule function that builds that Holiday hash table.
  - Debugged the processtoken function
  - Debugged the get_fields function
  - Removed diagnostic printf commands.
  - Debugged holiday_rules_get_tokens function.
  
  ## 0.0.7 (December 22, 2020)
  - Addressed all compilation errors after adding holiday rule processing. Debugged the tokenizer.
  - Refactored the code to bring the holiday rule lexical analysis into the library. Added a private header and moved some data types and functions into the private header.
  
  ## 0.0.6 (December 18, 2020)
  - Refactored by bringing certain functions from DocketMaster's former ruleprocessor module into the library (where they belong). Updated the README.
  
  ## 0.0.5 (December 15, 2020)
  - renamed the struct DATETIME to DateTime to comply with naming conventions for structs.
  
  ## 0.0.4 (December 10, 2020)
  - Began refactoring code, beginning with the comments.
  
  ## 0.0.3 (November 28, 2020)
  - Rename COPYING.LESSER to LICENSE for clarity.
  - Automated makefile. Separated timetools prototypes into timetools.h.
  - Makefile + Cleanup + Fixme
  
  ## 0.0.2 (November 25, 2020)
  - Pre-release vers - Changed organization name to Dark Matter Computing
  - Pre-release vers - Added LGPL License
  - Pre-release vers - 2020-Jul-03
  
  ## 0.0.1 (October 22, 2011)
  - Pre-release vers - added timetools - 2011-Oct-22
  - Pre-release vers - added timetools - 2011-Oct-20
  - Pre-release vers - added timetools - 2011-Oct-09
  - Pre-release vers - see ChangesLog - 2011-Oct-09
  - Pre-release vers - see ChangesLog - 2011-Oct-09
  - Pre-release vers - see ChangesLog - 2011-Oct-09
  - Pre-release vers - see ChangesLog - 2011-Oct-09
  - Pre-release vers - see ChangesLog - 2011-Oct-09
  - Pre-release vers - see ChangesLog - 2011-Oct-09
  - Pre-release vers - see ChangesLog - 2011-Oct-09
  - Pre-release vers -  2011-Oct-09
  - Pre-release vers - islastweek
  
  ## 0.0.0 (August 18, 2011)
  - initial project version - created on 2011-Aug-18


