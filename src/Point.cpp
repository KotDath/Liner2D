#include "Point.h"

namespace Liner {

Point2D::Point2D() : x(0), y(0), ux(0), uy(0) {

}
Point2D::Point2D(double x, double y, double ux, double uy) : x(x), y(y), ux(ux), uy(uy) {

}
QString Point2D::toString(int id) const {
  return QString("Point(%1) = {%2, %3, %4, %5};").arg(QString::number(id),
                                                      QString::number(x),
                                                      QString::number(y),
                                                      QString::number(0),
                                                      QString::number(1));
}

}

