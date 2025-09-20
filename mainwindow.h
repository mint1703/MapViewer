#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPointF>

class QLineEdit;
class QPushButton;
class MapWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoadMapClicked();
    void updateCursorPosition();

private:
    QLineEdit *latEdit;
    QLineEdit *lonEdit;
    QLineEdit *zoomEdit;
    QPushButton *loadButton;
    MapWidget *mapWidget;
    QTimer *cursorTimer;

    void setupUI();
};

#endif // MAINWINDOW_H
