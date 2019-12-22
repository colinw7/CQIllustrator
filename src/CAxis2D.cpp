#include <CAxis2D.h>
#include <CMathGen.h>
#include <CMathRound.h>
#include <CStrUtil.h>

#include <cstring>
#include <algorithm>

#define MIN_GOOD_TICKS 4
#define MAX_GOOD_TICKS 12
#define OPTIMUM_TICKS  10

#define MAX_GAP_TESTS 14

struct CAxis2DIncrementTest {
  double factor;
  uint   num_ticks;
  double inc_factor;
};

static CAxis2DIncrementTest
axes_increment_tests[MAX_GAP_TESTS] = {
  {  1.0, 5, 0 },
  {  1.2, 3, 0 },
  {  2.0, 4, 0 },
  {  2.5, 5, 0 },
  {  4.0, 4, 0 },
  {  5.0, 5, 0 },
  {  6.0, 3, 0 },
  {  8.0, 4, 0 },
  { 10.0, 5, 0 },
  { 12.0, 3, 0 },
  { 20.0, 4, 0 },
  { 25.0, 5, 0 },
  { 40.0, 4, 0 },
  { 50.0, 5, 0 }
};

CAxis2D::
CAxis2D(double start, double end) :
 start_         (start),
 end_           (end),
 start1_        (start),
 end1_          (end),
 num_ticks1_    (1),
 num_ticks2_    (0),
 tick_increment_(0),
 tick_spaces_   (),
 label_         (""),
 displayed_     (true)
{
  calc();
}

CAxis2D::
~CAxis2D()
{
}

void
CAxis2D::
setRange(double start, double end)
{
  start_ = start;
  end_   = end;

  calc();
}

void
CAxis2D::
setTickIncrement(uint tick_increment)
{
  tick_increment_ = tick_increment;

  calc();
}

void
CAxis2D::
setTickSpaces(double *tick_spaces, uint num_tick_spaces)
{
  tick_spaces_.resize(num_tick_spaces);

  memcpy(&tick_spaces_[0], tick_spaces, num_tick_spaces*sizeof(double));
}

void
CAxis2D::
setLabel(const std::string &str)
{
  label_ = str;
}

bool
CAxis2D::
calc()
{
  num_ticks1_ = 1;
  num_ticks2_ = 0;

  //------

  /* Ensure Axis Start and End are in the Correct Order */

  double min_axis = std::min(start_, end_);
  double max_axis = std::max(start_, end_);

  //------

  /* Calculate Length */

  double length = fabs(max_axis - min_axis);

  if (length == 0.0)
    return false;

  //------

  /* Calculate nearest Power of Ten to Length */

  int power = CMathRound::RoundDown(log10(length));

  //------

  /* Set Default Increment to 0.1 * Power of Ten */

  double increment = 0.1;

  if      (power < 0)
    for (int i = 0; i < -power; i++)
      increment /= 10.0;
  else if (power > 0)
    for (int i = 0; i <  power; i++)
      increment *= 10.0;

  //------

  /* Calculate other test Increments */

  for (int i = 0; i < MAX_GAP_TESTS; i++)
    axes_increment_tests[i].inc_factor = increment*axes_increment_tests[i].factor;

  //------

  /* Set Default Start/End to Force Update */

  start1_ = 0.0;
  end1_   = 0.0;

  //------

  /* Test each Increment in turn */

  uint num_gaps, num_gap_ticks;

  for (int i = 0; i < MAX_GAP_TESTS; i++) {
    if (tick_increment_ > 0) {
      int inc_factor1 = (int) axes_increment_tests[i].inc_factor;

      if (((double) inc_factor1) != axes_increment_tests[i].inc_factor)
        continue;

      if (inc_factor1 % tick_increment_ != 0)
         continue;
    }

    testAxisGaps(min_axis, max_axis,
                 axes_increment_tests[i].inc_factor,
                 axes_increment_tests[i].num_ticks,
                 &start1_, &end1_, &increment,
                 &num_gaps, &num_gap_ticks);
  }

  //------

  /* Set the Gap Positions */

  num_ticks1_ = CMathRound::RoundDown((end1_ - start1_)/increment + 0.5);
  num_ticks2_ = num_gap_ticks;

  return true;
}

bool
CAxis2D::
testAxisGaps(double start, double end, double test_increment, uint test_num_gap_ticks,
             double *start1, double *end1, double *increment, uint *num_gaps, uint *num_gap_ticks)
{
  /* Calculate New Start and End implied by the Test Increment */

  double new_start = CMathRound::RoundDown(start/test_increment)*test_increment;
  double new_end   = CMathRound::RoundUp  (end  /test_increment)*test_increment;

  while (new_start > start)
    new_start -= test_increment;

  while (new_end < end)
    new_end += test_increment;

  uint test_num_gaps = CMathRound::RoundUp((new_end - new_start)/test_increment);

  //------

  /* If nothing set yet just update values and return */

  if (*start1 == 0.0 && *end1 == 0.0) {
    *start1 = new_start;
    *end1   = new_end;

    *increment     = test_increment;
    *num_gaps      = test_num_gaps;
    *num_gap_ticks = test_num_gap_ticks;

    return true;
  }

  //------

  /* If the current number of gaps is not within the acceptable range
     and the new number of gaps is within the acceptable range then
     update current */

  if ((    *num_gaps <  MIN_GOOD_TICKS ||     *num_gaps >  MAX_GOOD_TICKS) &&
      (test_num_gaps >= MIN_GOOD_TICKS && test_num_gaps <= MAX_GOOD_TICKS)) {
    *start1 = new_start;
    *end1   = new_end;

    *increment     = test_increment;
    *num_gaps      = test_num_gaps;
    *num_gap_ticks = test_num_gap_ticks;

    return true;
  }

  //------

  /* If the current number of gaps is not within the acceptable range
     and the new number of gaps is not within the acceptable range then
     consider it for update of current if better fit */

  if ((    *num_gaps < MIN_GOOD_TICKS ||     *num_gaps > MAX_GOOD_TICKS) &&
      (test_num_gaps < MIN_GOOD_TICKS || test_num_gaps > MAX_GOOD_TICKS)) {
    /* Calculate how close fit is to required range */

    double delta1 = fabs(new_start - start) + fabs(new_end - end);

    //------

    /* If better fit than current fit or equally good fit and
       number of gaps is nearer to optimum (OPTIMUM_TICKS) then
       update current */

    double delta2 = fabs(*start1 - start) + fabs(*end1 - end);

    if (((fabs(delta1 - delta2) < 1E-6) &&
         (abs(test_num_gaps - OPTIMUM_TICKS) < abs(*num_gaps - OPTIMUM_TICKS))) ||
        delta1 < delta2) {
      *start1 = new_start;
      *end1   = new_end;

      *increment     = test_increment;
      *num_gaps      = test_num_gaps;
      *num_gap_ticks = test_num_gap_ticks;

      return true;
    }
  }

  //------

  /* If the current number of gaps is within the acceptable range
     and the new number of gaps is within the acceptable range then
     consider it for update of current if better fit */

  if ((    *num_gaps >= MIN_GOOD_TICKS &&     *num_gaps <= MAX_GOOD_TICKS) &&
      (test_num_gaps >= MIN_GOOD_TICKS && test_num_gaps <= MAX_GOOD_TICKS)) {
    /* Calculate how close fit is to required range */

    double delta1 = fabs(new_start - start) + fabs(new_end - end);

    //------

    /* If better fit than current fit or equally good fit and
       number of gaps is nearer to optimum (OPTIMUM_TICKS) then
       update current */

    double delta2 = fabs(*start1 - start) + fabs(*end1 - end);

    if (((fabs(delta1 - delta2) < 1E-6) &&
         (abs(test_num_gaps - OPTIMUM_TICKS) < abs(*num_gaps - OPTIMUM_TICKS))) ||
        delta1 < delta2) {
      *start1 = new_start;
      *end1   = new_end;

      *increment     = test_increment;
      *num_gaps      = test_num_gaps;
      *num_gap_ticks = test_num_gap_ticks;

      return true;
    }
  }

  return false;
}

double
CAxis2D::
getMajorIncrement() const
{
  return (end1_ - start1_)/num_ticks1_;
}

double
CAxis2D::
getMinorIncrement() const
{
  return (end1_ - start1_)/(num_ticks1_*num_ticks2_);
}

std::string
CAxis2D::
getValueStr(double pos) const
{
  return CStrUtil::toString(pos);
}
