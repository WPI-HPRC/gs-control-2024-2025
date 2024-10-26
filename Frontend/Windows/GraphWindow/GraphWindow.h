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
    ~GraphWindow();

private:
    Ui::GraphWindow *ui;
    double seconds;
    int resolution;
    bool telemflag;
    bool etmflag;
    bool disconnect;
    qreal samplerate;
    QTimer *timer;
    GraphWidget *acceleration;
    GraphWidget *position;
    GraphWidget *gyro;
    GraphWidget *velocity;

public slots:
    void scroll();
    void telemetryAvailable(Backend::Telemetry telemetry);

};
#endif // GRAPHWINDOW_H
