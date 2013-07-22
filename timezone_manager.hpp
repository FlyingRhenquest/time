/**
 * Manage your timezone in whatever scope. There are a couple ways you
 * can do this:
 *
 * 1) Example: Oh, I want GMT globally! Make a global instance of this
 *    object before main, with "GMT" as the time zone. It'll instantiate
 *    before anything else and destruct after everything else. This is
 *    probably the most-best thing to do when using threads, as I've had
 *    really bad luck with tzset not being thread safe.
 *
 * 2) Example: I want users to be able to set the time zone and am not
 *    using threads (If you are, set the timezone before starting any
 *    threads.) Take a timezone string from the user and create an
 *    instance of this object in a scope that won't go away until
 *    they're exiting the program.
 *
 * Just make these objects on the stack for great justice. The right
 * thing should happen when they go out of scope.
 *
 * Copyright 2013 Bruce Ide
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <stdlib.h>
#include <string>
#include <time.h>

#ifndef _HPP_TIMEZONE_MANAGER
#define _HPP_TIMEZONE_MANAGER

namespace fr {

  namespace time {
    
    class timezone_manager {
      char *old_time_zone;
      std::string time_zone;
    public:
      timezone_manager(const std::string &time_zone) : time_zone(time_zone)
      {
	old_time_zone = getenv("TZ");
	setenv("TZ", time_zone.c_str(), 1);
	tzset();
      }

      // Will be GMT, or at least something your computer thinks is GMT.
      timezone_manager()
      {
	old_time_zone = getenv("TZ");
	setenv("TZ", "", 1);
	tzset();
      }

      ~timezone_manager()
      {
	if (NULL != old_time_zone) {
	  setenv("TZ", old_time_zone, 1);
	} else {
	  unsetenv("TZ");
	}
	tzset();
      }

    };

  }

}

#endif
