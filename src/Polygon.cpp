//
// Created by epida on 09.07.2022.
//

#include "Polygon.h"

namespace Liner {
bool Polygon2D::contains(const Point2D &point) {
  int count = 0;
  if (isInBoundingBox(point)) {
    for (int i = 0; i < 4; ++i) {
      auto first = vertices[indexes[i]];
      auto second = vertices[indexes[(i + 1) % 4]];

      if (onEdge(first, second, point)) {
        return true;
      }


      auto max = std::max(first.x, second.x);
      auto min = std::min(first.x, second.x);

      if (first.y > point.y && second.y > point.y) {
        continue;
      }

      if (first.y < point.y && second.y < point.y) {
        continue;
      }

      if (point.x <= min) {
        ++count;
        continue;
      }

      if (point.x > max) {
        continue;
      }

      if (first.x < second.x && first.y > second.y) {
        std::swap(first, second);
      }

      if ((second.x - first.x) * (point.y - first.y) - (second.y - first.y) * (point.x - first.x) >= 0) {
        ++count;
      }
    }
  }

  return count % 2 == 1;
}

double Polygon2D::minX() {
  double minX = vertices[indexes[0]].x;
  for (unsigned int i: indexes) {
    if (vertices[i].x < minX) {
      minX = vertices[i].x;
    }
  }

  return minX;
}
double Polygon2D::minY() {
  double minY = vertices[indexes[0]].y;
  for (unsigned int i: indexes) {
    if (vertices[i].y < minY) {
      minY = vertices[i].y;
    }
  }

  return minY;
}
double Polygon2D::maxX() {
  double maxX = vertices[indexes[0]].x;
  for (unsigned int i: indexes) {
    if (vertices[i].x > maxX) {
      maxX = vertices[i].x;
    }
  }

  return maxX;
}
double Polygon2D::maxY() {
  double maxY = vertices[indexes[0]].y;
  for (unsigned int i: indexes) {
    if (vertices[i].y > maxY) {
      maxY = vertices[i].y;
    }
  }

  return maxY;
}
bool Polygon2D::isInBoundingBox(const Point2D &point) {
  return (point.x >= minX()) && (point.y >= minY()) && (point.x <= maxX()) && (point.y <= maxY());
}
bool Polygon2D::isInElement(const Point2D &point) {
  return true;
}
bool Polygon2D::onEdge(const Point2D &first, const Point2D &second, const Point2D &point) {
  if (first.x == second.x && first.x == point.x) {
    return true;
  }

  if (first.y == second.y && first.y == point.y) {
    return true;
  }

  if ((point.x - first.x) / (second.x - first.x) == (point.y - first.y) / (second.y - first.y)) {
    return true;
  }

  return false;
}
}