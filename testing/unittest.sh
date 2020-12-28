#!/bin/bash

HFILE="./testrules/holidays_casuper.csv"
DERIVE="./testscripts/derive_date_test.csv"
CALC="/testscripts/datecalc_test.csv"

bin/test_datetimetools -h$HFILE -w$DERIVE -c.$CALC
