#pragma once

#include <QWidget>
#include <QOpenGLWidget>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <QMouseEvent>
#include <QDebug>
#include <QList>

#include "Polygon.h"

class GridWindow : public QOpenGLWidget {

 Q_OBJECT
 public:
  GridWindow(QWidget *parent = 0);
  ~GridWindow();

  void loadMesh(const QString &filename);
  void buildStreamLine(const Liner::Point2D& start, int iterCount);
  void setDelta(double delta);
  double getDelta();

 protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

 protected:
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void wheelEvent(QWheelEvent *event);
 private:
  void DrawGrid();
  void DrawPoints();
  void DrawVelocity();
 public slots:
  void clearMesh();
 private:
  QPoint meshPos = {0, 0};
  QPoint previousPos;

  float distance = 1.0f;
  bool rightPressed = false;


  QList<Liner::Point2D> streamline;

  Liner::Point2D* meshPoints = nullptr;
  int meshSize;

  Liner::Polygon2D* polygons = nullptr;
  int polygonsSize;


  double deltaTime = 0.0001;
};

