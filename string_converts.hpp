// Assorted conversion objects

#ifndef _HPP_STRING_CONVERTS
#define _HPP_STRING_CONVERTS

#include <boost/xpressive/xpressive.hpp>
#include <string>
#include <string.h>
#include <sstream>
#include <time.h>
#include <sys/time.h>

#include <iomanip>

namespace fr {

  namespace time {

    /**
     * Convert a string to anything supported by ostream operator >>
     */

    template <class T>
    class string_to {
    public:
      typedef T value_type;

      string_to()
      {}

      ~string_to()
      {}

      T operator()(const std::string &cvt)
      {
	std::stringstream stream(cvt);
	T retval;
	stream >> retval;
	return retval;
      }
    };

    /**
     * string_to specialization for timeval (from man 2 gettimeofday)
     *
     * To use: Create with a string with a regex in it. The regex
     * should have the following named capture groups:
     *   year
     *   month
     *   day
     *   hour
     *   minute
     *   second
     *   frac (optional)
     *
     * If frac is used, it should match decimal fractions of a second.
     * Note, this will work in whatever timezone your program is operating
     * in. Use timezone_manager if you want to change it.
     *
     * Once your object is created it will happily convert times in that
     * format until it's no longer needed.
     */

    template <>
    class string_to<timeval> {
      boost::xpressive::sregex re;

      // Checks a field and throws a string if it's missing
      void check_defined(const boost::xpressive::smatch &matcher, const std::string &field) throw(std::string)
      {
	bool field_found = false;
	std::string additional_info;
	try {
	  field_found = (matcher[field].length() > 0);
	} catch (boost::xpressive::regex_error &e) {
	  additional_info = e.what();
	}
	if (!field_found) {
	  std::stringstream error;
	  error << "Error at field \"";
	  error << field << "\" during time conversion";
	  if (additional_info.length() > 0) {
	    error << ": " << additional_info;
	  }
	  throw(error.str());
	}
      }
      
    public:
      // TODO: Fix Y10K problem sometime in the next 7987 years
      // (Probably by just matching any arbitrary number
      // before the - so I don't have to worry about a Y100K problem
      // 90K years later.
      string_to(const std::string &matcher = "(?P<year>[0-9]{4})-(?P<month>[0-9]{2})-(?P<day>[0-9]{2})T(?P<hour>[0-9]{2}):(?P<minute>[0-9]{2}):(?P<second>[0-9]{2})(?P<fract>\\.[0-9]{1,6})Z") : re(boost::xpressive::sregex::compile(matcher))
      {
      }

      string_to(const string_to<timeval> &copy) : re(copy.re)
      {
      }

      ~string_to()
      {
      }

      inline timeval operator()(const std::string &at_time)
      {
	timeval retval;
	string_to<double> string_to_double;
	string_to<int> string_to_int;
	boost::xpressive::smatch matcher;
	double fract = 0.0d;
	boost::xpressive::regex_search(at_time, matcher, re);
	tm temp;
	memset((void *)&temp, '\0', sizeof(tm));
	check_defined(matcher, "year");
	check_defined(matcher, "month");
	check_defined(matcher, "day");
	check_defined(matcher, "hour");
	check_defined(matcher, "minute");
	check_defined(matcher, "second");
	temp.tm_year = string_to_int(matcher["year"]) - 1900;
	temp.tm_mon = string_to_int(matcher["month"]) - 1;
	temp.tm_mday = string_to_int(matcher["day"]);
	temp.tm_hour = string_to_int(matcher["hour"]);
	temp.tm_min = string_to_int(matcher["minute"]);
	temp.tm_sec = string_to_int(matcher["second"]);
	if (matcher[fract].length() > 0) {
	  fract = string_to_double(matcher["fract"]);
	  fract *= 1000000; // Microseconds
	}

	time_t ptime = timelocal(&temp);
	retval.tv_sec = ptime;
	retval.tv_usec = fract;
	return retval;
      }

    };

  }
}

#endif
