/**
 * Computes Julian Day Number and date, given a Posix time_t
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

#include <time.h>
#include <math.h>

/**
 * JD uses localtime, so if you want GMT you'll have to unset your
 * timezone and call tzset.
 */

namespace fr {

  namespace time {

    class julian_date {
      
      time_t start_time;
      time_t jdn;
      double jd;
      
    public:
      julian_date(time_t at_time) : start_time(at_time)
      {
	struct tm time_buf;
	localtime_r(&start_time, &time_buf);
	int year = time_buf.tm_year + 1900;
	int month = time_buf.tm_mon + 1;
	int day = time_buf.tm_mday;
	int a = (14 - month) / 12;
	int y = year + 4800 - a;
	int m = month + 12 * a - 3;
	jdn = day + floor((153 * m + 2) / 5) + 365 * y + floor(y / 4) - floor(y/100) + floor(y/400) - 32045;
	jd = (double) jdn + ((double) time_buf.tm_hour - 12.0) / 24.0 + ((double) time_buf.tm_min / 1440.0) + (double) time_buf.tm_sec / 86400.0;
      }
      
      ~julian_date()
      {
      }

      /**
       * Returns Julian Day number for the day
       */
      
      time_t get_jdn() {
	return jdn;
      }
      
      /**
       * Returns full julian date
       */
      
      double get_jd() {
	return jd;
      }
      
      /**
       * Returns time you entered as the time to convert
       */
      
      time_t get_posix_time() {
	return start_time;
      }
      
    };

  }

}
