//
// Created by clive on 10/24/2024.
//

#ifndef GS_BACKEND_2024_2025_GRAPHWIDGET_H
#define GS_BACKEND_2024_2025_GRAPHWIDGET_H

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegendMarker>
#include <QString>
#include <QMutex>

class GraphWidget : public QChart {

Q_OBJECT

public:
    explicit GraphWidget(const QString &title, const QBrush &brush, int range, QGraphicsItem *parent = nullptr);
    void addSeriesCustom(const QString& name, QColor color);
    void addSeriesCustom(const QString& name);
    void rescale();
    QLineSeries* getSeries(int x);
    void addToSeries(int x, qreal valx, qreal valy);
    void yScream(qreal valx);
    void yZeroFill(qreal valx);
    void removeTail(qreal now);
    //TODO DESTROY
    //~CustomChart();

private:
    std::vector<QLineSeries*> dataSeries;
    int windowRange;
    qreal lastXUpdate;
    qreal lastYUpdate;
};
#endif //GS_BACKEND_2024_2025_GRAPHWIDGET_H
