#!/bin/bash

HFILE="./testrules/holidays_casuper.csv"
DERIVE="./testscripts/derive_date_test.csv"
CALC="./testscripts/datecalc_test.csv"
LEAP="./testscripts/leap_test.csv"
MATH="./testscripts/math_test.csv"
RULE="./testscripts/check_rule_test.csv"

bin/test_datetimetools -h$HFILE -w$DERIVE -c$CALC -l$LEAP -r$RULE -m$MATH
