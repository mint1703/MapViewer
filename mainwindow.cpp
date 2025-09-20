#include "mainwindow.h"
#include "mapwidget.h"
#include "utmconverter.h"
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QCursor>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    setWindowTitle("Map Viewer");
    resize(800, 600);
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout *controlLayout = new QHBoxLayout();

    QLabel *latLabel = new QLabel("Latitude:", this);
    latEdit = new QLineEdit("55.7558", this);

    QLabel *lonLabel = new QLabel("Longitude:", this);
    lonEdit = new QLineEdit("37.6176", this);

    QLabel *zoomLabel = new QLabel("Zoom:", this);
    zoomEdit = new QLineEdit("12", this);

    loadButton = new QPushButton("Load Map", this);

    controlLayout->addWidget(latLabel);
    controlLayout->addWidget(latEdit);
    controlLayout->addWidget(lonLabel);
    controlLayout->addWidget(lonEdit);
    controlLayout->addWidget(zoomLabel);
    controlLayout->addWidget(zoomEdit);
    controlLayout->addWidget(loadButton);
    controlLayout->addStretch();

    mapWidget = new MapWidget(this);

    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(mapWidget);

    setCentralWidget(centralWidget);

    cursorTimer = new QTimer(this);
    cursorTimer->setInterval(50); // 50ms
    connect(cursorTimer, &QTimer::timeout, this, &MainWindow::updateCursorPosition);
    cursorTimer->start();

    connect(loadButton, &QPushButton::clicked, this, &MainWindow::onLoadMapClicked);
}

void MainWindow::onLoadMapClicked() {
    bool ok1, ok2, ok3;
    double lat = latEdit->text().toDouble(&ok1);
    double lon = lonEdit->text().toDouble(&ok2);
    int zoom = zoomEdit->text().toInt(&ok3);

    if (!ok1 || !ok2 || !ok3) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите корректные числовые значения");
        return;
    }

    if (lat < -90 || lat > 90) {
        QMessageBox::warning(this, "Ошибка", "Широта должна быть от -90 до 90");
        return;
    }

    if (lon < -180 || lon > 180) {
        QMessageBox::warning(this, "Ошибка", "Долгота должна быть от -180 до 180");
        return;
    }

    if (zoom < 1 || zoom > 20) {
        QMessageBox::warning(this, "Ошибка", "Масштаб может быть от 1 до 20");
        return;
    }

    mapWidget->loadMap(lat, lon, zoom);
}

void MainWindow::updateCursorPosition() {
    QPoint globalPos = QCursor::pos();

    QPoint localPos = mapWidget->mapFromGlobal(globalPos);

    if (mapWidget->rect().contains(localPos)) {
        QPointF wgs84Coord = mapWidget->pixelToWGS84(localPos);

        double lon = wgs84Coord.x();
        double lat = wgs84Coord.y();

        QString wgs84Text = QString("WGS84: %1, %2")
                                .arg(lat, 0, 'f', 6)
                                .arg(lon, 0, 'f', 6);

        UTMCooordinates utm = UTMConverter::wgs84ToUTM(lat, lon);
        QString utmText = UTMConverter::utmToString(utm);

        setWindowTitle(wgs84Text + " | " + utmText);

        update();
        QApplication::processEvents();
    }
}
