/**
 * Tests Julian Date class
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

#include "jd.hpp"
#include "timezone_manager.hpp"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include "cppunit"

class JdTest : public test_class {

  CPPUNIT_TEST_SUITE(JdTest);
  CPPUNIT_TEST(testJd);
  CPPUNIT_TEST_SUITE_END();
  fr::time::timezone_manager *tz;

public:

  void setUp() {
    tz = new fr::time::timezone_manager();
  }

  void tearDown() {
    delete tz;
  }

  void testJd()
  {
    struct tm j2kish; // Noon Jan 1, 2000, GMT
    memset(&j2kish, '\0', sizeof(struct tm));
    j2kish.tm_year = 100; // y2k
    j2kish.tm_hour = 12;
    j2kish.tm_mday = 1;
    time_t j2ktt = timegm(&j2kish);
    fr::time::julian_date j2kjd(j2ktt);

    CPPUNIT_ASSERT(j2kjd.get_jdn() == 2451545);
    CPPUNIT_ASSERT(j2kjd.get_jd() == 2451545.0);

    struct tm todayish;
    memset(&todayish, '\0', sizeof(struct tm));
    todayish.tm_year = 111; // 2011
    todayish.tm_mday = 5;
    todayish.tm_mon = 10; // November (UNIX months start at 0)
    todayish.tm_hour = 10;
    todayish.tm_min = 27;
    todayish.tm_sec = 23;
    time_t todaytt = timegm(&todayish);
    fr::time::julian_date todayjd(todaytt);

    double jdDiff = fabs(todayjd.get_jd() - 2455870.935683);

    struct tm later_that_day; // Actually the next day
    
    memset(&later_that_day, '\0', sizeof(struct tm));
    later_that_day.tm_year = 100; // y2k again
    later_that_day.tm_mday = 02;
    later_that_day.tm_mon - 0;
    later_that_day.tm_hour = 1;
    later_that_day.tm_min = 59;
    later_that_day.tm_sec = 31;
    time_t ldaytt = timegm(&later_that_day);
    fr::time::julian_date laterjd(ldaytt);

    CPPUNIT_ASSERT(fabs(laterjd.get_jd() - 2451545.583) < .0001);

    CPPUNIT_ASSERT(todayjd.get_jdn() == 2455871);
    CPPUNIT_ASSERT(jdDiff < .000001);
    
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(JdTest);
