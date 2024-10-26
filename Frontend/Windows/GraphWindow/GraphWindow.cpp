//Clive

#include "GraphWindow.h"
#include "ui_GraphWindow.h"

void GraphWindow::scroll()
{
    if(seconds > resolution) {
        acceleration->removeTail();
    }
    if(!lastRecieved){
        acceleration->yScream(seconds);
    } else {
        printf("%f\n", lastRecieved->data.rocketData->accelX);
        acceleration->addToSeries(0,seconds, qreal(lastRecieved->data.rocketData->accelX));
        acceleration->addToSeries(1, seconds, qreal(lastRecieved->data.rocketData->accelY));
        acceleration->addToSeries(2, seconds, qreal(lastRecieved->data.rocketData->accelZ));
        lastRecieved = nullptr;
    }
    seconds++;
}

void GraphWindow::telemetryAvailable(Backend::Telemetry telemetry)
{
    lastRecieved = std::make_unique<Backend::Telemetry>(telemetry);
}

GraphWindow::GraphWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::GraphWindow)
{
    seconds = 1;
    int samplerate = 40;
    resolution = samplerate * 10;
    ui->setupUi(this);
    timer = new QTimer(this);
    //TODO sample at 40HZ display at 4HZ or sample+display at 4HZ
    timer->start(1000 / samplerate);
    acceleration = new GraphWidget("Acceleration", QBrush(QColor(40, 40, 45)), resolution);
    acceleration->addSeriesCustom(new QLineSeries(), "x", QColor(245, 60, 25));
    acceleration->addSeriesCustom(new QLineSeries(), "y", QColor(24, 245, 47));
    acceleration->addSeriesCustom(new QLineSeries(), "z",QColor(25, 60, 255));
    ui->GraphA->setChart(acceleration);
    connect(timer, &QTimer::timeout, this, &GraphWindow::scroll);
    connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, &GraphWindow::telemetryAvailable);
    scroll();
}


GraphWindow::~GraphWindow()
{
    delete ui;
}
