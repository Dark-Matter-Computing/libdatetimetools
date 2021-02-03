#!/bin/bash

HFILE="./testrules/crlf_files/holidays_casuper_CRLF.csv"
DERIVE="./testscripts/crlf_files/derive_date_test_CRLF.csv"
CALC="./testscripts/crlf_files/datecalc_test_CRLF.csv"
LEAP="./testscripts/crlf_files/leap_test_CRLF.csv"
MATH="./testscripts/crlf_files/math_test_CRLF.csv"
RULE="./testscripts/crlf_files/check_rule_test_CRLF.csv"

bin/test_datetimetools -h$HFILE -w$DERIVE -c$CALC -l$LEAP -r$RULE -m$MATH
