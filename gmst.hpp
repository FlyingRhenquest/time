/**
 * Greenwich Mean Sidereal Time, as per 
 * http://aa.usno.navy.mil/faq/docs/GAST.php
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

/**
 * Takes a time_t or a double corresponding to time_t and returns the
 * GMST. If you want apparent, you'll still need to adjust for your
 * longitude.
 */

namespace fr {

  namespace time {

    class gmst {
      double start_time;
      double _gmst;
      double hours;
      double minutes;
      double seconds;

    public:

      gmst(time_t start) : start_time((double) start) {
	init();
      }

      gmst(double start) : start_time(start) {
	init();
      }

      ~gmst()
      {
      }

      void init()
      {
	julian_date now(start_time);
	/**
	 * Get the POSIX time for the previous midnight. Since time_t
	 * is number of seconds since the POSIX epoch, we can divide it
	 * by the number of seconds in the day (86400) and the remainder
	 * will be the number of seconds since the previous midnight.
	 * It really doesn't matter which epoch you use either, as
	 * long as it starts at midnight (If you use J2K you'll
	 * get the previous noon and will want to add 43200 to
	 * that to get midnight.)
	 */
	julian_date midnight(start_time - fmod(start_time, 86400.0));
	/**
	 * Adjust both Julian dates to the J2K Epoch by
	 * subtracting the Julian date of the epoch.
	 * TODO: Pull constants like this out and put them in
	 * a namespace.
	 */
	double now_jd = now.get_jd() - 2451545.0;
	double midnight_jd = midnight.get_jd() - 2451545.0;
	double t = now_jd / 36525.0; // Number of days in a century
	double h = fmod(start_time, 86400.0) / 3600; // Get hour of day
	/**
	 * I wish they'd explained what these constants are on that page.
	 */
	_gmst = 6.697374558 + 0.06570982441908 * midnight_jd + 1.00273790935 * h + 0.000026 * pow(t,2.0);
	/**
	 * Well that's entirely the answer I'm looking for, though,
	 * because this still has to be normalized to 24 hours...
	 */
	_gmst = fmod(_gmst, 24.0);
	hours = floor(_gmst);
	double raw_minutes = ((_gmst - floor(_gmst)) * 60.0);
	minutes = floor(raw_minutes);
	// You may want to round your seconds to something
	seconds = (raw_minutes - minutes) * 60.0;
      }

      double get_gmst()
      {
	return _gmst;
      }

      double get_hours()
      {
	return hours;
      }

      double get_minutes()
      {
	return minutes;
      }

      double get_seconds()
      {
	return seconds;
      }

    };

  }

}
