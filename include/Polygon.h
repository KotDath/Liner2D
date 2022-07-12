#pragma once
#include "Point.h"

namespace Liner {

struct Polygon2D {

  Polygon2D() {
    indexes[0] = 0;
    indexes[1] = 0;
    indexes[2] = 0;
    indexes[3] = 0;
    vertices = nullptr;
  }
  bool contains(const Point2D &point);
  bool onEdge(const Point2D &first, const Point2D &second, const Point2D &point);

  unsigned int indexes[4];
  static Point2D *vertices;

 private:
  double minX();
  double minY();
  double maxX();
  double maxY();
  bool isInBoundingBox(const Point2D &point);
  bool isInElement(const Point2D &point);
};
}
