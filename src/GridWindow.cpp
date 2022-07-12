#include <QTextStream>
#include <QMessageBox>
#include "GridWindow.h"

Liner::Point2D *Liner::Polygon2D::vertices = nullptr;

GridWindow::GridWindow(QWidget *parent)
    : QOpenGLWidget(parent) {
}

GridWindow::~GridWindow() {

}

void GridWindow::initializeGL() {
  glClearColor(0, 0, 0, 1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  glClearColor(0, 0, 0, 1.0f);
}

void GridWindow::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  float stepX = (-(float) meshPos.x() / 1000) * distance;
  float stepY = ((float) meshPos.y() / 1000) * distance;
  gluLookAt(stepX, stepY,
            distance,
            stepX, stepY,
            0,
            0,
            1,
            0);

  DrawGrid();
  DrawVelocity();
  DrawPoints();
}

void GridWindow::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, (float) w / h, 0.01, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  float stepX = (-(float) meshPos.x() / 1000) * distance;
  float stepY = ((float) meshPos.y() / 1000) * distance;
  gluLookAt(stepX, stepY,
            distance,
            stepX, stepY,
            0,
            0,
            1,
            0);
}
void GridWindow::mouseMoveEvent(QMouseEvent *event) {

  if (rightPressed) {
    meshPos += event->pos() - previousPos;
    update();
  }
  previousPos = event->pos();
  QOpenGLWidget::mouseMoveEvent(event);
}

void GridWindow::DrawGrid() {
  glLineWidth(2);

  for (int i = 0; i < polygonsSize; ++i) {
    glBegin(GL_LINE_LOOP);
    for (int j = 0; j < 4; ++j) {
      glColor3f(1, 1, 1);
      auto vertex = meshPoints[polygons[i].indexes[j]];
      glVertex2d(vertex.x, vertex.y);
    }
    glEnd();
  }

  glPointSize(7);
  glBegin(GL_POINTS);
  for (int i = 0; i < meshSize; ++i) {
    glColor3f(1, 0, 1);
    glVertex2d(meshPoints[i].x, meshPoints[i].y);
  }
  glEnd();
}
void GridWindow::DrawPoints() {
  glPointSize(4);
  glBegin(GL_POINTS);
  for (int i = 0; i < streamline.count(); ++i) {
    glColor3f(0, 0, 1);
    glVertex2d(streamline[i].x, streamline[i].y);
  }
  glEnd();
}
void GridWindow::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::RightButton) {
    previousPos = event->pos();
    rightPressed = true;
  }
  QWidget::mousePressEvent(event);
}
void GridWindow::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::RightButton) {
    rightPressed = false;
  }
  QWidget::mouseReleaseEvent(event);
}

void GridWindow::wheelEvent(QWheelEvent *event) {
  auto angle = event->angleDelta();
  if (angle.y() > 0) {
    distance *= 1.1f;

    update();
  }

  if (angle.y() < 0) {
    distance /= 1.1f;
    update();
  }
}
void GridWindow::loadMesh(const QString &filename) {
  if (meshPoints != nullptr) {
    QMessageBox::warning(nullptr, "Меш", "Ошибка. Меш уже загружен. Очистите поле перед загрузкой.");
    return;
  }

  QFile file(filename);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
    QString buffer = in.readLine();
    while (buffer != "$Nodes") {
      buffer = in.readLine();

    }

    buffer = in.readLine();
    meshSize = buffer.toInt();
    meshPoints = new Liner::Point2D[meshSize];

    while (buffer != "$EndNodes") {
      buffer = in.readLine();
      qDebug() << buffer;
      if (buffer != "" && buffer != "$EndNodes") {
        int id = buffer.split(" ")[0].toInt();
        double x = buffer.split(" ")[1].toDouble();
        double y = buffer.split(" ")[2].toDouble();
        meshPoints[id - 1].x = x;
        meshPoints[id - 1].y = y;
      }
    }

    while (buffer != "$Elements") {
      buffer = in.readLine();
    }

    buffer = in.readLine();
    QMessageBox::warning(nullptr, "Количество элементов", buffer);
    polygonsSize = buffer.toInt();
    polygons = new Liner::Polygon2D[polygonsSize];
    Liner::Polygon2D::vertices = meshPoints;
    while (buffer != "$EndElements") {
      buffer = in.readLine();
      if (buffer != "" && buffer != "$EndElements") {
        int id = buffer.split(" ")[0].toInt();
        polygons[id - 1].indexes[0] = buffer.split(" ")[3].toInt() - 1;
        polygons[id - 1].indexes[1] = buffer.split(" ")[4].toInt() - 1;
        polygons[id - 1].indexes[2] = buffer.split(" ")[5].toInt() - 1;
        polygons[id - 1].indexes[3] = buffer.split(" ")[6].toInt() - 1;
      }
    }

    while (buffer != "$NodeData") {
      buffer = in.readLine();
    }

    while (buffer.split(" ").size() <= 1) {
      buffer = in.readLine();
    }

    while (buffer != "$EndNodeData") {
      if (buffer != "" && buffer != "$EndNodeData") {
        int id = buffer.split(" ")[0].toInt();
        meshPoints[id - 1].ux = buffer.split(" ")[1].toDouble();
        meshPoints[id - 1].uy = buffer.split(" ")[2].toDouble();
        buffer = in.readLine();
      }
    }

    file.close();
  }
}
void GridWindow::setDelta(double delta) {
  deltaTime = delta;
}
double GridWindow::getDelta() {
  return deltaTime;
}
void GridWindow::clearMesh() {
  meshPos = {0, 0};
  distance = 1.0f;
  rightPressed = false;
  streamline.clear();

  if (polygons != nullptr) {
    delete[] polygons;
    polygons = nullptr;
  }

  if (meshPoints != nullptr) {
    delete[] meshPoints;
    meshPoints = nullptr;
  }

  meshSize = 0;
  polygonsSize = 0;

  update();
}
void GridWindow::DrawVelocity() {
  glLineWidth(4);


  double maxLen = 0;
  for (int i = 0; i < meshSize; ++i) {
    auto len = sqrt(meshPoints[i].ux * meshPoints[i].ux + meshPoints[i].uy * meshPoints[i].uy);
    if (len > maxLen) {
      maxLen = len;
    }
  }

  glBegin(GL_LINES);
  for (int i = 0; i < meshSize; ++i) {
    auto len = sqrt(meshPoints[i].ux * meshPoints[i].ux + meshPoints[i].uy * meshPoints[i].uy);
    glColor3d(abs(meshPoints[i].ux / len), abs(meshPoints[i].uy / len), 0);
    glVertex2d(meshPoints[i].x, meshPoints[i].y);
    glVertex2d(meshPoints[i].x + 0.1 * meshPoints[i].ux / maxLen, meshPoints[i].y + 0.1 * meshPoints[i].uy / maxLen);
  }
  glEnd();
}
void GridWindow::buildStreamLine(const Liner::Point2D &start, int iterCount) {
  streamline.clear();
  int index = 1;
  Liner::Point2D newPoint = start;
  streamline.append(newPoint);
  for (int i = 0; i < polygonsSize; ++i) {
    int newCount = 0;
    while (polygons[i].contains(newPoint) && index < iterCount) {
      auto successPolygon = polygons[i];
      int first = -1, second = -1;
      for (int j = 0; j < 4; ++j) {
        int firstIndex = successPolygon.indexes[j];
        int secondIndex = successPolygon.indexes[(j + 1) % 4];

        if (successPolygon.onEdge(meshPoints[firstIndex], meshPoints[secondIndex], newPoint)) {
          first = j;
          break;
        }

        if ((meshPoints[firstIndex].y <= newPoint.y && meshPoints[secondIndex].y >= newPoint.y) ||
            (meshPoints[firstIndex].y >= newPoint.y && meshPoints[secondIndex].y <= newPoint.y)) {
          if (first == -1) {
            first = j;
          } else {
            second = j;
          }
        }
      }

      Liner::Point2D B, C;
      if (second == -1) {
        B = meshPoints[successPolygon.indexes[first]];
        C = meshPoints[successPolygon.indexes[(first + 1) % 4]];
      } else {
        auto firstPoint = meshPoints[successPolygon.indexes[first]];
        auto secondPoint = meshPoints[successPolygon.indexes[(first + 1) % 4]];
        double t = (newPoint.y - firstPoint.y) / (secondPoint.y - firstPoint.y);
        B.x = (1 - t) * firstPoint.x + t * secondPoint.x;
        B.y = newPoint.y;
        B.ux = (1 - t) * firstPoint.ux + t * secondPoint.ux;
        B.uy = (1 - t) * firstPoint.uy + t * secondPoint.uy;

        firstPoint = meshPoints[successPolygon.indexes[second]];
        secondPoint = meshPoints[successPolygon.indexes[(second + 1) % 4]];

        t = (newPoint.y - firstPoint.y) / (secondPoint.y - firstPoint.y);
        C.x = (1 - t) * firstPoint.x + t * secondPoint.x;
        C.y = newPoint.y;
        C.ux = (1 - t) * firstPoint.ux + t * secondPoint.ux;
        C.uy = (1 - t) * firstPoint.uy + t * secondPoint.uy;
      }

      if (C.x != B.x) {
        double t = (newPoint.x - B.x) / (C.x - B.x);
        newPoint.ux = (1 - t) * B.ux + C.ux * t;
        newPoint.uy = (1 - t) * B.uy + C.uy * t;
      } else {
        if (C.y != B.y) {
          double t = (newPoint.y - B.y) / (C.y - B.y);
          newPoint.ux = (1 - t) * B.ux + C.ux * t;
          newPoint.uy = (1 - t) * B.uy + C.uy * t;
        } else {
          newPoint.ux = B.ux;
          newPoint.uy = B.uy;
        }
      }

      newPoint.x += newPoint.ux * deltaTime;
      newPoint.y += newPoint.uy * deltaTime;

      streamline.append(newPoint);
      ++newCount;

      ++index;
    }

    if (newCount != 0) {
      i = 0;
    }
  }


  update();
}

