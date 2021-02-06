#!/bin/bash

HFILE="./testrules/holidays_casuper.csv"
DERIVE="./testscripts/derive_date_test.csv"
CALC="./testscripts/datecalc_test.csv"
LEAP="./testscripts/leap_test.csv"
COURTMATH="./testscripts/math_test.csv"
CALMATH="./testscripts/caldays_test.csv"
RULE="./testscripts/check_rule_test.csv"

bin/test_datetimetools -h$HFILE -w$DERIVE -c$CALC -l$LEAP -r$RULE -m$COURTMATH -k$CALMATH
