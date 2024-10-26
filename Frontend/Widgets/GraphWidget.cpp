//
// Created by clive on 10/24/2024.
//

#include "GraphWidget.h"
#include <QtCharts/QChart>

GraphWidget::GraphWidget(const QString &title, const QBrush &brush, int range, QGraphicsItem *parent)
    : QChart(parent)  {

    setBackgroundBrush(brush);
    setTitleBrush(QBrush(QColor(232, 218, 197)));
    setTitle(title);
    windowRange = range;

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, range);
    axisX->setTickCount(10);
    axisX->setLabelsBrush(QBrush(QColor(232, 218, 197)));
    axisX->setLabelsColor(QColor(232, 218, 197));
    axisX->colorChanged(QColor(232, 218, 197));

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 1);
    axisY->setLabelsBrush(QBrush(QColor(232, 218, 197)));
    axisY->setLabelsColor(QColor(232, 218, 197));
    axisY->colorChanged(QColor(232, 218, 197));


    this->setPlotAreaBackgroundBrush(QBrush(QColor(232, 218, 197)));
    this->setAxisX(axisX);
    this->setAxisY(axisY);
}

void GraphWidget::addSeriesCustom(QLineSeries *series, const QString &name, QColor color) {
    series->setColor(color);
    series->setName(name);
    dataSeries.push_back(series);
    series->append(0,0);
    addSeries(series);
    legend()->markers(series).first()->setLabelBrush(QBrush(QColor(232, 218, 197)));
}

QLineSeries *GraphWidget::getSeries(int x) {
    return dataSeries.at(x);
}

void GraphWidget::addToSeries(int x, qreal valx, qreal valy) {
    dataSeries.at(x)->append(valx, valy);
}

void GraphWidget::yScream(qreal valx) {
    for(QLineSeries *l : dataSeries){
        if(l->count() > 0) {
            l->append(valx, l->at(l->count() - 1).y());
        }
    }
}

void GraphWidget::yZeroFill(qreal valx) {
    for(QLineSeries *l : dataSeries){
        if(l->count() > 0) {
            l->append(valx, 0);
        }
    }
}

//TODO would be MUCH better if QLineSeries was using a list instead of a vector
void GraphWidget::removeTail(qreal now) {
    if(now > windowRange) {
        for (QLineSeries *l: dataSeries) {
            if (l->count() > 0) {
                l->remove(0);
            }
        }
    }
}
//TODO do directly on addSeriesCustom or is every rescale okay
void GraphWidget::rescale() {
    qreal minX = dataSeries.at(0)->at(0).x();
    qreal maxX = dataSeries.at(0)->points().last().x();
    qreal minY = dataSeries.at(0)->at(0).y();
    qreal maxY = dataSeries.at(0)->at(0).y();

    for(QLineSeries *l : dataSeries){
        for (const QPointF &point : l->points()) {
            minY = std::min(minY, point.y());
            maxY = std::max(maxY, point.y());
        }
    }

    this->axisX()->setRange(minX, maxX);
    this->axisY()->setRange(minY, maxY);


    createDefaultAxes();
}
