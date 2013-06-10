#ifndef CAXIS_2D_H
#define CAXIS_2D_H

#include <sys/types.h>

#include <string>
#include <vector>

class CAxis2D {
 public:
  CAxis2D(double start=0.0, double end=1.0);

  virtual ~CAxis2D();

  double getStart() const { return start1_; }
  double getEnd  () const { return end1_  ; }

  uint getNumMajorTicks() const { return num_ticks1_; }
  uint getNumMinorTicks() const { return num_ticks2_; }

  uint getTickIncrement() const { return tick_increment_; }

  const double *getTickSpaces   () const { return &tick_spaces_[0]; }
  uint          getNumTickSpaces() const { return tick_spaces_.size(); }

  double getTickSpace(int i) const { return tick_spaces_[i]; }

  const std::string &getLabel() const { return label_; }

  bool getDisplayed() const { return displayed_; }

  void setRange(double start, double end);

  void setTickIncrement(uint tick_increment);

  void setTickSpaces(double *tick_spaces, uint num_tick_spaces);

  void setLabel(const std::string &str);

  double getMajorIncrement() const;
  double getMinorIncrement() const;

  virtual std::string getValueStr(double pos) const;

 private:
  bool calc();

  bool testAxisGaps(double start, double end, double test_increment,
                    uint test_num_gap_ticks, double *start1, double *end1,
                    double *increment, uint *num_gaps, uint *num_gap_ticks);

 private:
  double              start_, end_;
  double              start1_, end1_;
  uint                num_ticks1_, num_ticks2_;
  uint                tick_increment_;
  std::vector<double> tick_spaces_;
  std::string         label_;
  bool                displayed_;
};

#endif
