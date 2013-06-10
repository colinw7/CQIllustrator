#ifndef KUHN_MUNKRES_HPP
#define KUHN_MUNKRES_HPP

/*
    Copyright (c) 2008-10 Qtrac Ltd. All rights reserved.
    This program or module is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 2 of
    the License, or version 3 of the License, or (at your option) any
    later version. This program is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the GNU General Public License for more details.

    This is based on Brian M. Clapper's Python 2 implementation
    at http://www.clapper.org/bmc/ itself based on the implementation at
    http://www.public.iastate.edu/~ddoty/HungarianAlgorithm.html
*/

#include <cfloat>
#include <cmath>
#include <vector>

class CKuhnMunkres {
 public:
  typedef std::pair<int, int> Index;
  typedef std::vector<Index>  Indexes;
  typedef std::vector<double> Row;
  typedef std::vector<Row>    Grid;

  explicit CKuhnMunkres() {}

  Indexes calculate(const Grid &grid);

  static bool is_zero(double x) { return std::fabs(x) <= DBL_EPSILON; }

 private:
  typedef std::pair<int, int> Dimensions;
  typedef std::vector<int>    IntRow;
  typedef std::vector<IntRow> IntGrid;
  typedef std::vector<bool>   Covered;

  enum {ZERO, STAR, PRIME};

  Dimensions ensure_grid_is_square();

  template<typename T>
  std::vector<std::vector<T> > make_grid(int size, T value);

  int step1();
  int step2();
  int step3();
  int step4();
  int step5();
  int step6();

  void clear_covers();

  Index find_a_zero();

  int find_star_in_row(int row);
  int find_star_in_column(int column);
  int find_prime_in_row(int row);

  void convert_path(int count);

  void erase_primes();

  double find_smallest();

  Grid grid_;

  Covered row_covered;
  Covered column_covered;

  int     z0_row;
  int     z0_column;
  IntGrid path;
  IntGrid marked;
  int     size;
};

#endif // KUHN_MUNKRES_HPP
