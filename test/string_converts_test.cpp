// Test string converts

#include "cppunit"
#include <string>
#include <sys/time.h>
#include "string_converts.hpp"
#include "timezone_manager.hpp"

class string_converts_test : public test_class {

  CPPUNIT_TEST_SUITE(string_converts_test);
  CPPUNIT_TEST(test_time_convert);
  CPPUNIT_TEST(test_broken_re);
  CPPUNIT_TEST(test_string_to);
  CPPUNIT_TEST_SUITE_END();

public:

  void test_time_convert()
  {
    fr::time::timezone_manager tz("GMT");
    std::string epoch("1970-01-01T00:00:00.000Z");
    fr::time::string_to<timeval> string_to_time;
    timeval et = string_to_time(epoch);
    if (et.tv_sec != 0l && et.tv_usec != 0l) {
      std::cout << std::endl << "Epoch Fail" << std::endl;
      std::cout << "tv_sec == " << et.tv_sec << std::endl;
      std::cout << "tv_usec == " << et.tv_usec << std::endl;
    }
    CPPUNIT_ASSERT(et.tv_sec == 0l);
    CPPUNIT_ASSERT(et.tv_usec == 0l);

    std::string epoch_ish("1970-01-01T00:00:00.500Z");
    et = string_to_time(epoch_ish);
    CPPUNIT_ASSERT(et.tv_sec == 0l);
    CPPUNIT_ASSERT(et.tv_usec == 500000l);

    std::string some_other_date("2013-02-20T13:37:28.00000Z");
    et = string_to_time(some_other_date);
    CPPUNIT_ASSERT(et.tv_sec == 1361367448l);
    CPPUNIT_ASSERT(et.tv_usec == 0l);
  }

  // string_to<timeval> needs a specific regexp. Make sure a throw
  // occurs if it doesn't get it.
  void test_broken_re()
  {
    fr::time::timezone_manager tz("GMT");
    std::string broken_re("(?P<yy>[0-9]{4})-(?P<mmm>[0-9]{2})-(?P<dd>[0-9]{2})T(?P<hour>[0-9]{2}):(?P<minute>[0-9]{2}):(?P<second>[0-9]{2})(?P<fract>\\.[0-9]{1,6})Z"); // For the purposes of our test
    std::string epoch("1970-01-01T00:00:00.000Z");
    fr::time::string_to<timeval> string_to_time(broken_re);
    bool failsauce = true;
    timeval et;
    try {
      et = string_to_time(epoch);
    } catch (std::string e) {
      failsauce = false;
    }
    CPPUNIT_ASSERT(!failsauce);
  }

  void test_string_to()
  {
    long longcvt = 12345l;
    std::string as_string = fr::time::to_string(longcvt);
    CPPUNIT_ASSERT(as_string == "12345");
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(string_converts_test);
