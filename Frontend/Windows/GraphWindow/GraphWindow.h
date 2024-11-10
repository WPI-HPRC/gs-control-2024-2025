#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>

#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QList>
#include <QValueAxis>
#include <QTimer>

#include "Frontend/Widgets/GraphWidget.h"
#include "Backend/Backend.h"
namespace Ui {
    class GraphWindow;
}



class GraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    static GraphWindow &getInstance()
    {
        static GraphWindow instance;
        return instance;
    }
    explicit GraphWindow(QWidget *parent = nullptr);
    //TODO enable_if?
    template <class T>
    GraphWidget* makeGraphSimple(std::initializer_list<T> list) {
        GraphWidget* graph = new GraphWidget("Velocity", QBrush(QColor(30, 30, 19)), 8);
        for( auto elem : list) {
            graph->addSeriesCustom(elem);
        }
        allGraphs.push_back();
        return graph;
    }
    ~GraphWindow();

private:
    Ui::GraphWindow *ui;
    qreal seconds;
    int resolution;
    bool telemflag;
    bool etmflag;
    bool disconnect;
    qreal samplerate;
    QTimer* timer;
    GraphWidget* acceleration;
    GraphWidget* position;
    GraphWidget* gyro;
    GraphWidget* velocity;
    std::vector<GraphWidget*> allGraphs;

public slots:
    void scroll();
    void telemetryAvailable(Backend::Telemetry telemetry);

};


#endif // GRAPHWINDOW_H
