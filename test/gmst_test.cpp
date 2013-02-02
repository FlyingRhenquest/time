/**
 * Basic validation that GMST is returning expected values
 *
 * Copyright 2011 Bruce Ide
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "gmst.hpp"
#include "timezone_manager.hpp"
// You'd think memset would be defined somewhere else...
#include <string.h>
// getenv, setenv
#include <stdlib.h>
#include <iostream>

// See my cppunit_contrib project
#include "cppunit"

class GmstTest : public test_class {
  CPPUNIT_TEST_SUITE(GmstTest);

  CPPUNIT_TEST(testGmst);

  CPPUNIT_TEST_SUITE_END();

  time_t getTestTime(int year, int month, int day, int hours, int minutes, int seconds)
  {
    struct tm testTime;
    time_t retval;
    memset(&testTime, '\0', sizeof(struct tm));
    testTime.tm_year = year; // Number of years since 1900
    testTime.tm_mon = month; // Month of year - 1
    testTime.tm_mday = day;
    testTime.tm_hour = hours;
    testTime.tm_min = minutes;
    testTime.tm_sec = seconds;
    retval = timegm(&testTime);
    return retval;
  }

  // That's equal enough for me!
  bool roughlyEqual(double a, double b)
  {
    return (a - b) < .01;
  }

  bool expected(fr::time::gmst &check, double hours, double minutes, double seconds)
  {
    bool retval = roughlyEqual(check.get_hours(), hours) && roughlyEqual(check.get_minutes(), minutes) && roughlyEqual(check.get_seconds(), seconds);
    if (!retval) {
      std::cout << std::endl << "Expected: " << hours << "H " << minutes << "M " << seconds << "S, got: " << check.get_hours() << "H " << check.get_minutes() << "M " << check.get_seconds() << "S" << std::endl;
    }
    return retval;
  }

  fr::time::timezone_manager *tz;
public:

  void setUp()
  {
    tz = new fr::time::timezone_manager();
  }

  void tearDown()
  {
    delete tz;
  }
  
  void testGmst() 
  {
    /**
     * Taking test values from http://zeitladen.de/time.html
     */
    // Midnight Nov 1 2011
    time_t testtime = getTestTime(111, 10, 1, 0, 0, 0);
    fr::time::gmst *gmst = new fr::time::gmst(testtime);
    CPPUNIT_ASSERT(expected(*gmst, 2.0, 39.0, 44.6));
    delete gmst;
    // Nov 4 18:00
    testtime = getTestTime(111, 10, 4, 18, 0, 0);
    gmst = new fr::time::gmst(testtime);
    CPPUNIT_ASSERT(expected(*gmst, 20, 54, 31.7));
    delete gmst;
    // Nov 16, midnight
    testtime = getTestTime(111, 10, 16, 0, 0, 0);
    gmst = new fr::time::gmst(testtime);
    CPPUNIT_ASSERT(expected(*gmst, 3, 38, 52.9));
    delete gmst;
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(GmstTest);
