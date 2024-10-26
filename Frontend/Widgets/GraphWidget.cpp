//
// Created by clive on 10/24/2024.
//

#include "GraphWidget.h"
#include <QtCharts/QChart>

GraphWidget::GraphWidget(const QString &title, const QBrush &brush, int range, QGraphicsItem *parent)
    : QChart(parent)  {

    setBackgroundBrush(brush);
    setTitle(title);

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, range);
    axisX->setTickCount(10);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 1);
    this->setAxisX(axisX);
    this->setAxisY(axisY);
}

void GraphWidget::addSeriesCustom(QLineSeries *series, const QString &name, QColor color) {
    series->setColor(color);
    series->setName(name);
    dataSeries.push_back(series);
    series->append(0,0);
    addSeries(series);
}

QLineSeries *GraphWidget::getSeries(int x) {
    return dataSeries.at(x);
}

void GraphWidget::addToSeries(int x, qreal valx, qreal valy) {
    dataSeries.at(x)->append(valx, valy);
    createDefaultAxes();
    updateAxisRange(dataSeries.at(x));
}

void GraphWidget::yScream(qreal valx) {
    for(QLineSeries *l : dataSeries){
        if(l->count() > 0) {
            l->append(valx, l->at(l->count() - 1).y());
        }
    }
}

//TODO would be MUCH better if QLineSeries was using a list instead of a vector
void GraphWidget::removeTail() {
    for(QLineSeries *l : dataSeries){
        if(l->count() > 0){
            l->remove(0);
        }
    }
}

void GraphWidget::updateAxisRange(QLineSeries *series){
    // Get the min and max of the x and y values
    qreal minX = series->points().first().x();
    qreal maxX = series->points().last().x();
    qreal minY = series->points().first().y();
    qreal maxY = series->points().first().y();

    for (const QPointF &point : series->points()) {
        minX = std::min(minX, point.x());
        maxX = std::max(maxX, point.x());
        minY = std::min(minY, point.y());
        maxY = std::max(maxY, point.y());
    }

    this->axisX()->setRange(minX, maxX);
    this->axisY()->setRange(minY, maxY);
}
