#include "mapwidget.h"
#include <QPainter>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMouseEvent>
#include <cmath>

MapWidget::MapWidget(QWidget *parent) : QWidget(parent), tileSize(256) {
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &MapWidget::onTileLoaded);
}

void MapWidget::loadMap(double lat, double lon, int zoom) {
    centerLat = lat;
    centerLon = lon;
    currentZoom = zoom;

    currentMap = QPixmap(tileSize, tileSize);
    currentMap.fill(Qt::lightGray);

    QPoint tileCoords = wgs84ToTile(lat, lon, zoom);
    downloadTile(tileCoords.x(), tileCoords.y(), zoom);

    update();
}

QPoint MapWidget::wgs84ToTile(double lat, double lon, int zoom) const {
    double lat_rad = lat * M_PI / 180.0;
    double n = std::pow(2.0, zoom);
    int x = static_cast<int>((lon + 180.0) / 360.0 * n);
    int y = static_cast<int>((1.0 - std::asinh(std::tan(lat_rad)) / M_PI) / 2.0 * n);
    return QPoint(x, y);
}

QPointF MapWidget::pixelToWGS84(const QPoint &pixel) const {
    QPoint tileCoords = wgs84ToTile(centerLat, centerLon, currentZoom);

    double n = std::pow(2.0, currentZoom);
    double lon_deg = (tileCoords.x() + (pixel.x() - width()/2.0) / tileSize) / n * 360.0 - 180.0;

    double lat_rad = std::atan(std::sinh(M_PI * (1 - 2 * (tileCoords.y() + (pixel.y() - height()/2.0) / tileSize) / n)));
    double lat_deg = lat_rad * 180.0 / M_PI;

    return QPointF(lon_deg, lat_deg);
}

void MapWidget::downloadTile(int x, int y, int z) {
    QString url = QString("https://mt1.google.com/vt/lyrs=s&x=%1&y=%2&z=%3")
    .arg(x).arg(y).arg(z);

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("User-Agent", "MapViewer/1.0");

    networkManager->get(request);
}

void MapWidget::onTileLoaded(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray imageData = reply->readAll();
        currentMap.loadFromData(imageData);
        update();
    }
    reply->deleteLater();
}


void MapWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    if (!currentMap.isNull()) {
        QRect targetRect(0, 0, width(), height());
        painter.drawPixmap(targetRect, currentMap);
    } else {
        painter.fillRect(rect(), Qt::lightGray);
        painter.drawText(rect(), Qt::AlignCenter, "Загрузка карты...");
    }
}
