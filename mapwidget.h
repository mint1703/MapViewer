#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QPointF>

class MapWidget : public QWidget {
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = nullptr);
    void loadMap(double lat, double lon, int zoom);
    QPointF pixelToWGS84(const QPoint &pixel) const;

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onTileLoaded(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    QPixmap currentMap;
    double centerLat;
    double centerLon;
    int currentZoom;
    int tileSize;

    void downloadTile(int x, int y, int z);
    QPoint wgs84ToTile(double lat, double lon, int zoom) const;
};

#endif // MAPWIDGET_H
