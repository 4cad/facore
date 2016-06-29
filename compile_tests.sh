#!/bin/bash
g++ -std=c++11 test/TestMain.cpp test/TestDeterministicAutomaton.cpp test/TestNonDeterministicAutomaton.cpp test/TestDRegularLanguage.cpp -I src/
