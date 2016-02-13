////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013 - 2016, Göteborg Bit Factory.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// http://www.opensource.org/licenses/mit-license.php
//
////////////////////////////////////////////////////////////////////////////////

#include <cmake.h>
#include <Datetime.h>
#include <test.h>
#include <iostream>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////
void testParse (
  UnitTest& t,
  const std::string& input,
  int in_start,
  int in_year,
  int in_month,
  int in_week,
  int in_weekday,
  int in_julian,
  int in_day,
  int in_seconds,
  int in_offset,
  bool in_utc,
  time_t in_date)
{
  std::string label = std::string ("parse (\"") + input + "\") --> ";

  Datetime iso;
  std::string::size_type start = 0;

  t.ok (iso.parse (input, start),             label + "true");
  t.is ((int) start,        in_start,         label + "[]");
  t.is (iso._year,          in_year,          label + "_year");
  t.is (iso._month,         in_month,         label + "_month");
  t.is (iso._week,          in_week,          label + "_week");
  t.is (iso._weekday,       in_weekday,       label + "_weekday");
  t.is (iso._julian,        in_julian,        label + "_julian");
  t.is (iso._day,           in_day,           label + "_day");
  t.is (iso._seconds,       in_seconds,       label + "_seconds");
  t.is (iso._offset,        in_offset,        label + "_offset");
  t.is (iso._utc,           in_utc,           label + "_utc");
  t.is ((size_t) iso._date, (size_t) in_date, label + "_date");
}

////////////////////////////////////////////////////////////////////////////////
int main (int, char**)
{
  UnitTest t (206);

  Datetime iso;
  std::string::size_type start = 0;
  t.notok (iso.parse ("foo", start), "foo --> false");
  t.is ((int)start, 0,               "foo[0]");

  // Determine local and UTC time.
  time_t now = time (nullptr);
  struct tm* local_now = localtime (&now);
  int local_s = (local_now->tm_hour * 3600) +
                (local_now->tm_min  * 60)   +
                local_now->tm_sec;
  local_now->tm_hour  = 0;
  local_now->tm_min   = 0;
  local_now->tm_sec   = 0;
  local_now->tm_isdst = -1;
  time_t local = mktime (local_now);
  std::cout << "# local midnight today " << local << "\n";

  local_now->tm_year  = 2013 - 1900;
  local_now->tm_mon   = 12 - 1;
  local_now->tm_mday  = 6;
  local_now->tm_isdst = 0;
  time_t local6 = mktime (local_now);
  std::cout << "# local midnight 2013-12-06 " << local6 << "\n";

  local_now->tm_year  = 2013 - 1900;
  local_now->tm_mon   = 12 - 1;
  local_now->tm_mday  = 1;
  local_now->tm_isdst = 0;
  time_t local1 = mktime (local_now);
  std::cout << "# local midnight 2013-12-01 " << local1 << "\n";

  struct tm* utc_now = gmtime (&now);
  int utc_s = (utc_now->tm_hour * 3600) +
              (utc_now->tm_min  * 60)   +
              utc_now->tm_sec;
  utc_now->tm_hour  = 0;
  utc_now->tm_min   = 0;
  utc_now->tm_sec   = 0;
  utc_now->tm_isdst = -1;
  time_t utc = timegm (utc_now);
  std::cout << "# utc midnight today " << utc << "\n";

  utc_now->tm_year  = 2013 - 1900;
  utc_now->tm_mon   = 12 - 1;
  utc_now->tm_mday  = 6;
  utc_now->tm_isdst = 0;
  time_t utc6 = timegm (utc_now);
  std::cout << "# utc midnight 2013-12-06 " << utc6 << "\n";

  utc_now->tm_year  = 2013 - 1900;
  utc_now->tm_mon   = 12 - 1;
  utc_now->tm_mday  = 1;
  utc_now->tm_isdst = 0;
  time_t utc1 = timegm (utc_now);
  std::cout << "# utc midnight 2013-12-01 " << utc1 << "\n";

  int hms = (12 * 3600) + (34 * 60) + 56; // The time 12:34:56 in seconds.
  int hm  = (12 * 3600) + (34 * 60);      // The time 12:34:00 in seconds.
  int z   = 3600;                         // TZ offset.

  int ld = local_s > hms ? 86400 : 0;     // Local extra day if now > hms.
  int ud = utc_s   > hms ? 86400 : 0;     // UTC extra day if now > hms.
  std::cout << "# ld " << ld << "\n";
  std::cout << "# ud " << ud << "\n";

  // datetime-ext
  //            input                         i  Year  Mo  Wk WD  Jul  Da   Secs     TZ    UTC      time_t
  testParse (t, "2013-12-06",                10, 2013, 12,  0, 0,   0,  6,     0,     0, false, local6    );
  testParse (t, "2013-340",                   8, 2013,  0,  0, 0, 340,  0,     0,     0, false, local6    );
  testParse (t, "2013-W49-5",                10, 2013,  0, 49, 5,   0,  0,     0,     0, false, local6    );
  testParse (t, "2013-W49",                   8, 2013,  0, 49, 0,   0,  0,     0,     0, false, local1    );

  // The only non-extended forms.
  testParse (t, "20131206T123456Z",          16, 2013, 12,  0, 0,   0,  6,   hms,     0,  true, utc6+hms  );
  testParse (t, "20131206T123456",           15, 2013, 12,  0, 0,   0,  6,   hms,     0, false, local6+hms);

  try
  {
    Datetime now;
    t.ok (now.toISO ().find ("1969") == std::string::npos, "'now' != 1969");

    Datetime yesterday;
    yesterday -= 86400;
    Datetime tomorrow;
    tomorrow += 86400;

    t.ok    (yesterday <= now,       "yesterday <= now");
    t.ok    (yesterday <  now,       "yesterday < now");
    t.notok (yesterday == now,       "!(yesterday == now)");
    t.ok    (yesterday != now,       "yesterday != now");
    t.ok    (now       >= yesterday, "now >= yesterday");
    t.ok    (now       >  yesterday, "now > yesterday");

    t.ok    (tomorrow >= now,        "tomorrow >= now");
    t.ok    (tomorrow >  now,        "tomorrow > now");
    t.notok (tomorrow == now,        "!(tomorrow == now)");
    t.ok    (tomorrow != now,        "tomorrow != now");
    t.ok    (now      <= tomorrow,   "now <= tomorrow");
    t.ok    (now      <  tomorrow,   "now < tomorrow");

    // Validity.
    t.ok    (Datetime::valid (2, 29, 2008), "valid: 2/29/2008");
    t.notok (Datetime::valid (2, 29, 2007), "invalid: 2/29/2007");

    t.ok    (Datetime::valid ("2/29/2008", "m/d/Y"), "valid: 2/29/2008");
    t.notok (Datetime::valid ("2/29/2007", "m/d/Y"), "invalid: 2/29/2007");

    t.ok    (Datetime::valid (366, 2008), "valid: 366 days in 2008");
    t.notok (Datetime::valid (366, 2007), "invalid: 366 days in 2007");

    // Time validity.
    t.ok    (Datetime::valid (2, 28, 2010,  0,  0,  0), "valid 2/28/2010 0:00:00");
    t.ok    (Datetime::valid (2, 28, 2010, 23, 59, 59), "valid 2/28/2010 23:59:59");
    t.notok (Datetime::valid (2, 28, 2010, 24, 59, 59), "valid 2/28/2010 24:59:59");
    t.notok (Datetime::valid (2, 28, 2010, -1,  0,  0), "valid 2/28/2010 -1:00:00");

    // Leap year.
    t.ok    (Datetime::leapYear (2008), "2008 is a leap year");
    t.notok (Datetime::leapYear (2007), "2007 is not a leap year");
    t.ok    (Datetime::leapYear (2000), "2000 is a leap year");
    t.notok (Datetime::leapYear (1900), "1900 is not a leap year");

    // Days in year.
    t.is (Datetime::daysInYear (2016), 366, "366 days in 2016");
    t.is (Datetime::daysInYear (2015), 365, "365 days in 2015");

    // Days in month.
    t.is (Datetime::daysInMonth (2, 2008), 29, "29 days in February 2008");
    t.is (Datetime::daysInMonth (2, 2007), 28, "28 days in February 2007");

    // Names.
    t.is (Datetime::monthName (1),  "January",   "1 = January");
    t.is (Datetime::monthName (2),  "February",  "2 = February");
    t.is (Datetime::monthName (3),  "March",     "3 = March");
    t.is (Datetime::monthName (4),  "April",     "4 = April");
    t.is (Datetime::monthName (5),  "May",       "5 = May");
    t.is (Datetime::monthName (6),  "June",      "6 = June");
    t.is (Datetime::monthName (7),  "July",      "7 = July");
    t.is (Datetime::monthName (8),  "August",    "8 = August");
    t.is (Datetime::monthName (9),  "September", "9 = September");
    t.is (Datetime::monthName (10), "October",   "10 = October");
    t.is (Datetime::monthName (11), "November",  "11 = November");
    t.is (Datetime::monthName (12), "December",  "12 = December");

    // Names.
    t.is (Datetime::monthNameShort (1),  "Jan", "1 = Jan");
    t.is (Datetime::monthNameShort (2),  "Feb", "2 = Feb");
    t.is (Datetime::monthNameShort (3),  "Mar", "3 = Mar");
    t.is (Datetime::monthNameShort (4),  "Apr", "4 = Apr");
    t.is (Datetime::monthNameShort (5),  "May", "5 = May");
    t.is (Datetime::monthNameShort (6),  "Jun", "6 = Jun");
    t.is (Datetime::monthNameShort (7),  "Jul", "7 = Jul");
    t.is (Datetime::monthNameShort (8),  "Aug", "8 = Aug");
    t.is (Datetime::monthNameShort (9),  "Sep", "9 = Sep");
    t.is (Datetime::monthNameShort (10), "Oct", "10 = Oct");
    t.is (Datetime::monthNameShort (11), "Nov", "11 = Nov");
    t.is (Datetime::monthNameShort (12), "Dec", "12 = Dec");

    // Names.
    t.is (Datetime::monthOfYear ("January"),   1,  "January   =  1");
    t.is (Datetime::monthOfYear ("February"),  2,  "February  =  2");
    t.is (Datetime::monthOfYear ("March"),     3,  "March     =  3");
    t.is (Datetime::monthOfYear ("April"),     4,  "April     =  4");
    t.is (Datetime::monthOfYear ("May"),       5,  "May       =  5");
    t.is (Datetime::monthOfYear ("June"),      6,  "June      =  6");
    t.is (Datetime::monthOfYear ("July"),      7,  "July      =  7");
    t.is (Datetime::monthOfYear ("August"),    8,  "August    =  8");
    t.is (Datetime::monthOfYear ("September"), 9,  "September =  9");
    t.is (Datetime::monthOfYear ("October"),   10, "October   = 10");
    t.is (Datetime::monthOfYear ("November"),  11, "November  = 11");
    t.is (Datetime::monthOfYear ("December"),  12, "December  = 12");

    t.is (Datetime::dayName (0), "Sunday",    "0 == Sunday");
    t.is (Datetime::dayName (1), "Monday",    "1 == Monday");
    t.is (Datetime::dayName (2), "Tuesday",   "2 == Tuesday");
    t.is (Datetime::dayName (3), "Wednesday", "3 == Wednesday");
    t.is (Datetime::dayName (4), "Thursday",  "4 == Thursday");
    t.is (Datetime::dayName (5), "Friday",    "5 == Friday");
    t.is (Datetime::dayName (6), "Saturday",  "6 == Saturday");

    t.is (Datetime::dayNameShort (0), "Sun",  "0 == Sun");
    t.is (Datetime::dayNameShort (1), "Mon",  "1 == Mon");
    t.is (Datetime::dayNameShort (2), "Tue",  "2 == Tue");
    t.is (Datetime::dayNameShort (3), "Wed",  "3 == Wed");
    t.is (Datetime::dayNameShort (4), "Thu",  "4 == Thu");
    t.is (Datetime::dayNameShort (5), "Fri",  "5 == Fri");
    t.is (Datetime::dayNameShort (6), "Sat",  "6 == Sat");

    t.is (Datetime::dayOfWeek ("SUNDAY"),    0, "SUNDAY == 0");
    t.is (Datetime::dayOfWeek ("sunday"),    0, "sunday == 0");
    t.is (Datetime::dayOfWeek ("Sunday"),    0, "Sunday == 0");
    t.is (Datetime::dayOfWeek ("Monday"),    1, "Monday == 1");
    t.is (Datetime::dayOfWeek ("Tuesday"),   2, "Tuesday == 2");
    t.is (Datetime::dayOfWeek ("Wednesday"), 3, "Wednesday == 3");
    t.is (Datetime::dayOfWeek ("Thursday"),  4, "Thursday == 4");
    t.is (Datetime::dayOfWeek ("Friday"),    5, "Friday == 5");
    t.is (Datetime::dayOfWeek ("Saturday"),  6, "Saturday == 6");

    Datetime happyNewYear (1, 1, 2008);
    t.is (happyNewYear.dayOfWeek (),         2, "1/1/2008 == Tuesday");
    t.is (happyNewYear.month (),             1, "1/1/2008 == January");
    t.is (happyNewYear.day (),               1, "1/1/2008 == 1");
    t.is (happyNewYear.year (),           2008, "1/1/2008 == 2008");
    t.is (happyNewYear.toString (), "1/1/2008", "toString 1/1/2008");

    int m, d, y;
    happyNewYear.toMDY (m, d, y);
    t.is (m, 1, "1/1/2008 == January");
    t.is (d, 1, "1/1/2008 == 1");
    t.is (y, 2008, "1/1/2008 == 2008");

    Datetime epoch (9, 8, 2001);
    t.ok ((int)epoch.toEpoch () < 1000000000, "9/8/2001 < 1,000,000,000");
    epoch += 172800;
    t.ok ((int)epoch.toEpoch () > 1000000000, "9/10/2001 > 1,000,000,000");

    Datetime fromEpoch (epoch.toEpoch ());
    t.is (fromEpoch.toString (), epoch.toString (), "ctor (time_t)");

    Datetime iso (1000000000);
    t.is (iso.toISO (), "20010909T014640Z", "1,000,000,000 -> 20010909T014640Z");

    // Quantization.
    Datetime quant (1234526400);
    t.is (quant.startOfDay ().toString ("YMDHNS"),   "20090213000000", "1234526400 -> 2/13/2009 12:00:00 UTC -> 2/13/2009 0:00:00");
    t.is (quant.startOfWeek ().toString ("YMDHNS"),  "20090208000000", "1234526400 -> 2/13/2009 12:00:00 UTC -> 2/8/2009 0:00:00");
    t.is (quant.startOfMonth ().toString ("YMDHNS"), "20090201000000", "1234526400 -> 2/13/2009 12:00:00 UTC -> 2/1/2009 0:00:00");
    t.is (quant.startOfYear ().toString ("YMDHNS"),  "20090101000000", "1234526400 -> 2/13/2009 12:00:00 UTC -> 1/1/2009 0:00:00");

    // Datetime::operator-
    Datetime r25 (1234567890);
    t.is ((r25 - 1).toEpoch (), 1234567889, "1234567890 - 1 = 1234567889");

    // Datetime::operator--
    Datetime r26 (11, 7, 2010, 23, 59, 59);
    r26--;
    t.is (r26.toString ("YMDHNS"), "20101106235959", "decrement across fall DST boundary");

    Datetime r27 (3, 14, 2010, 23, 59, 59);
    r27--;
    t.is (r27.toString ("YMDHNS"), "20100313235959", "decrement across spring DST boundary");

    // Datetime::operator++
    Datetime r28 (11, 6, 2010, 23, 59, 59);
    r28++;
    t.is (r28.toString ("YMDHNS"), "20101107235959", "increment across fall DST boundary");

    Datetime r29 (3, 13, 2010, 23, 59, 59);
    r29++;
    t.is (r29.toString ("YMDHNS"), "20100314235959", "increment across spring DST boundary");

    // int Datetime::length (const std::string&);
    t.is (Datetime::length ("m"), 2,  "length 'm' --> 2");
    t.is (Datetime::length ("M"), 2,  "length 'M' --> 2");
    t.is (Datetime::length ("d"), 2,  "length 'd' --> 2");
    t.is (Datetime::length ("D"), 2,  "length 'D' --> 2");
    t.is (Datetime::length ("y"), 2,  "length 'y' --> 2");
    t.is (Datetime::length ("Y"), 4,  "length 'Y' --> 4");
    t.is (Datetime::length ("a"), 3,  "length 'a' --> 3");
    t.is (Datetime::length ("A"), 10, "length 'A' --> 10");
    t.is (Datetime::length ("b"), 3,  "length 'b' --> 3");
    t.is (Datetime::length ("B"), 10, "length 'B' --> 10");
    t.is (Datetime::length ("v"), 2,  "length 'v' --> 2");
    t.is (Datetime::length ("V"), 2,  "length 'V' --> 2");
    t.is (Datetime::length ("h"), 2,  "length 'h' --> 2");
    t.is (Datetime::length ("H"), 2,  "length 'H' --> 2");
    t.is (Datetime::length ("n"), 2,  "length 'n' --> 2");
    t.is (Datetime::length ("N"), 2,  "length 'N' --> 2");
    t.is (Datetime::length ("s"), 2,  "length 's' --> 2");
    t.is (Datetime::length ("S"), 2,  "length 'S' --> 2");
    t.is (Datetime::length ("j"), 3,  "length 'j' --> 3");
    t.is (Datetime::length ("J"), 3,  "length 'J' --> 3");
    t.is (Datetime::length (" "), 1,  "length ' ' --> 1");
  }

  catch (const std::string& e)
  {
    t.fail ("Exception thrown.");
    t.diag (e);
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
