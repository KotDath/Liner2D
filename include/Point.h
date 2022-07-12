#pragma once
#include <QString>
#include <format>

namespace Liner {

struct Point2D {
  Point2D();
  explicit Point2D(double x, double y = 0, double ux = 0, double uy = 0);
  QString toString(int id) const;
  double x;
  double y;

  double ux;
  double uy;
};

}