#!/bin/bash

HFILE="./testrules/holidays_casuper.csv"

bin/test_datetimetools -h$HFILE -i
