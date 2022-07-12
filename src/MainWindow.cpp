#include <QMessageBox>
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Liner2D");
  resize(800, 600);

  auto fileMenu = new QMenu{tr("Файл")};

  auto quitAction = new QAction{tr("Выход")};
  quitAction->setShortcut(QKeySequence::Quit);
  fileMenu->addAction(quitAction);
  connect(quitAction, &QAction::triggered, this, &MainWindow::close);

  auto importAction = new QAction{tr("Импорт сетки")};
  importAction->setShortcut(QKeySequence::New);
  fileMenu->addAction(importAction);
  connect(importAction, &QAction::triggered, this, &MainWindow::gridPick);

  auto exportAction = new QAction{tr("Экспорт линий тока")};
  exportAction->setShortcut(QKeySequence::Save);
  fileMenu->addAction(exportAction);
  connect(exportAction, &QAction::triggered, this, &MainWindow::exportStreamLine);

  auto clearAction = new QAction{tr("Очистка")};
  clearAction->setShortcut(QKeySequence::Close);
  fileMenu->addAction(clearAction);
  connect(clearAction, &QAction::triggered, this, [this]() {
    GLWidget->clearMesh();
  });

  menuBar()->addMenu(fileMenu);

  GLWidget = new GridWindow{};

  setCentralWidget(GLWidget);

  auto pointMenu = new QMenu{tr("Линии тока")};

  auto deltaAction = new QAction{tr("Задание временного интервала")};
  deltaAction->setShortcut(QKeySequence::Quit);
  pointMenu->addAction(deltaAction);
  connect(deltaAction, &QAction::triggered, this, &MainWindow::setDelta);

  auto streamlineAction = new QAction{tr("Построение линии тока")};
  streamlineAction->setShortcut(QKeySequence::New);
  pointMenu->addAction(streamlineAction);
  connect(streamlineAction, &QAction::triggered, this, &MainWindow::buildStreamLine);

  menuBar()->addMenu(pointMenu);
}

MainWindow::~MainWindow() {}

void MainWindow::gridPick() {
  auto filename = QFileDialog::getOpenFileName(this,
                                               tr("Open msh file"),
                                               QStandardPaths::displayName(QStandardPaths::HomeLocation),
                                               tr("MSH files (*.msh)"));

  GLWidget->loadMesh(filename);

}
void MainWindow::buildStreamLine() {
  QDialog dialog(this);
// Use a layout allowing to have a label next to each field
  QFormLayout form(&dialog);

// Add some text above the fields
  form.addRow(new QLabel("Выбор начальной точки?"));

// Add the lineEdits with their respective labels
  QList<QLineEdit *> fields;
  for (int i = 0; i < 3; ++i) {
    QLineEdit *lineEdit = new QLineEdit(&dialog);

    QString label;
    switch (i) {
      case (0):label = QString("X");
        break;
      case (1):label = QString("Y");
        break;
      case (2):label = QString("Количество итераций");
        break;
    }
    form.addRow(label, lineEdit);


    fields << lineEdit;
  }

// Add some standard buttons (Cancel/Ok) at the bottom of the dialog
  QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                             Qt::Horizontal, &dialog);
  form.addRow(&buttonBox);
  QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
  QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

// Show the dialog as modal
  if (dialog.exec() == QDialog::Accepted) {
    bool ok;
    double x = fields[0]->text().toDouble(&ok);
    if (!ok) {
      QMessageBox::warning(nullptr, "Ошибка", "Неверный формат координаты x");
      return;
    }

    double y = fields[1]->text().toDouble(&ok);
    if (!ok) {
      QMessageBox::warning(nullptr, "Ошибка", "Неверный формат координаты y");
      return;
    }

    int count = fields[2]->text().toInt(&ok);
    if (!ok) {
      QMessageBox::warning(nullptr, "Ошибка", "Неверный формат количества итераций");
      return;
    }

    GLWidget->buildStreamLine(Liner::Point2D{x, y}, count);
  }
}
void MainWindow::exportStreamLine() {

}

void MainWindow::setDelta() {
  if (!deltaForm) {
    deltaForm = new QWidget{};
    auto main = new QVBoxLayout{deltaForm};
    deltaForm->setLayout(main);

    auto slider = new QSlider{Qt::Horizontal, deltaForm};
    slider->setRange(0, 100000);
    slider->setSingleStep(1);
    slider->setValue(GLWidget->getDelta() * 100000);

    main->addWidget(slider);

    auto buffer = new QWidget{deltaForm};
    main->addWidget(buffer);

    auto subLayout = new QHBoxLayout{buffer};
    auto txt = new QLabel{QString("△t = %1").arg(GLWidget->getDelta()), buffer};

    subLayout->addWidget(txt);

    connect(slider, &QSlider::valueChanged, this, [this, txt](int value) {
      txt->setText(QString("△t = %1").arg((double) value / 100000.0));
      GLWidget->setDelta((double) value / 100000.0);
    });

    deltaForm->setAttribute(Qt::WA_DeleteOnClose, true);

    connect(deltaForm, &QWidget::destroyed, this, [this]() {
      deltaForm = nullptr;
    });

    deltaForm->show();
  }
}
