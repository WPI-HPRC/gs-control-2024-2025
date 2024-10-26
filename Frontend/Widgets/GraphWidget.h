//
// Created by clive on 10/24/2024.
//

#ifndef GS_BACKEND_2024_2025_GRAPHWIDGET_H
#define GS_BACKEND_2024_2025_GRAPHWIDGET_H

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QString>

class GraphWidget : public QChart {

Q_OBJECT

public:
    explicit GraphWidget(const QString &title, const QBrush &brush, int range, QGraphicsItem *parent = nullptr);
    void addSeriesCustom(QLineSeries *series, const QString& name, QColor color);
    QLineSeries* getSeries(int x);
    void addToSeries(int x, qreal valx, qreal valy);
    void yScream(qreal valx);
    void removeTail();
    void updateAxisRange(QLineSeries* series);
    //TODO DESTROY
    //~CustomChart();

private:
    std::vector<QLineSeries*> dataSeries;

};
#endif //GS_BACKEND_2024_2025_GRAPHWIDGET_H
