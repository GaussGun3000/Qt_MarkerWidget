#pragma once
#include <qwidget.h>
#include <qpainter.h>
#include <vector>

class MarkerWidget : public QWidget
{
    Q_OBJECT
public:
    MarkerWidget(QWidget* parent);
    void render(std::vector<QPointF>& points);
    void render();
    std::string getScale();

signals:
    void scaleChanged();

protected:
    void paintEvent(QPaintEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    const int maxScaleSteps = 2;
    enum defaultMarkerParameters;
    void createMarker(QPointF p);
    QPoint getOffset();
    QVector<QLineF> lines;
    std::vector<QPointF> points;
    bool increaseScale();
    bool decreaseScale();
    int scale;
    int coordMultiplyer;
    int markerSize; 
};

