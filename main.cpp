#include <QApplication>
#include <QPushButton>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  MainWindow w{};
  w.show();

  return a.exec(); // .exec starts QApplication and related GUI, this line starts 'event loop'
}