#pragma once

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>

#include "GridWindow.h"

class MainWindow : public QMainWindow {
 Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void gridPick();
  void buildStreamLine();
  void exportStreamLine();
  void setDelta();

 private:
  GridWindow *GLWidget;

  QWidget *deltaForm = nullptr;
};


