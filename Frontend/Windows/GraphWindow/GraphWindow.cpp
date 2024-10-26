//Clive

#include "GraphWindow.h"
#include "ui_GraphWindow.h"

void GraphWindow::scroll()
{

    if(!telemflag){
        if(!disconnect) {
            acceleration->yScream(seconds);
            position->yScream(seconds);
            gyro->yScream(seconds);
            velocity->yScream(seconds);

            acceleration->removeTail(seconds);
            position->removeTail(seconds);
            gyro->removeTail(seconds);
            velocity->removeTail(seconds);
            acceleration->rescale();
            position->rescale();
            gyro->rescale();
            velocity->rescale();
        } else {
            acceleration->yZeroFill(seconds);
            position->yZeroFill(seconds);
            gyro->yZeroFill(seconds);
            velocity->yZeroFill(seconds);

            acceleration->removeTail(seconds);
            position->removeTail(seconds);
            gyro->removeTail(seconds);
            velocity->removeTail(seconds);
            acceleration->rescale();
            position->rescale();
            gyro->rescale();
            velocity->rescale();
        }
    }
    //TODO fpp
    if(std::fmod(seconds, (2.0 / samplerate)) >= 0.00001){

    }
    //TODO fpp
    if(std::fmod(seconds, 0.5) >= 0.00001){
        if(etmflag){
            disconnect = true;
        } else {
            etmflag = true;
        }
    }
    seconds += (1.0 / samplerate);
    telemflag = false;

}

void GraphWindow::telemetryAvailable(Backend::Telemetry telemetry)
{
    acceleration->removeTail(seconds);
    position->removeTail(seconds);
    gyro->removeTail(seconds);
    velocity->removeTail(seconds);

    acceleration->rescale();
    position->rescale();
    gyro->rescale();
    velocity->rescale();

    acceleration->addToSeries(0,seconds, qreal(telemetry.data.rocketData->accelX));
    acceleration->addToSeries(1, seconds, qreal(telemetry.data.rocketData->accelY));
    acceleration->addToSeries(2, seconds, qreal(telemetry.data.rocketData->accelZ));

    position->addToSeries(0,seconds, qreal(telemetry.data.rocketData->posX));
    position->addToSeries(1, seconds, qreal(telemetry.data.rocketData->posY));
    position->addToSeries(2, seconds, qreal(telemetry.data.rocketData->posZ));

    gyro->addToSeries(0,seconds, qreal(telemetry.data.rocketData->gyroX));
    gyro->addToSeries(1, seconds, qreal(telemetry.data.rocketData->gyroY));
    gyro->addToSeries(2, seconds, qreal(telemetry.data.rocketData->gyroZ));

    velocity->addToSeries(0,seconds, qreal(telemetry.data.rocketData->velX));
    velocity->addToSeries(1, seconds, qreal(telemetry.data.rocketData->velY));
    velocity->addToSeries(2, seconds, qreal(telemetry.data.rocketData->velZ));

    telemflag = true;
    etmflag = false;
    disconnect = false;
}
//TODO just pass through the pointer to the seconds variable
GraphWindow::GraphWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::GraphWindow)
{
    telemflag = false;
    etmflag = false;
    disconnect = false;
    seconds = 0;
    samplerate = 19;
    resolution = 13;
    ui->setupUi(this);
    timer = new QTimer(this);
    timer->start(1000 / samplerate);

    acceleration = new GraphWidget("Acceleration", QBrush(QColor(30, 30, 19)), resolution);
    acceleration->addSeriesCustom(new QLineSeries(), "x", QColor(245, 60, 25));
    acceleration->addSeriesCustom(new QLineSeries(), "y", QColor(24, 245, 47));
    acceleration->addSeriesCustom(new QLineSeries(), "z",QColor(25, 60, 255));
    ui->GraphA->setChart(acceleration);

    position = new GraphWidget("Position", QBrush(QColor(30, 30, 19)), resolution);
    position->addSeriesCustom(new QLineSeries(), "x", QColor(245, 60, 25));
    position->addSeriesCustom(new QLineSeries(), "y", QColor(24, 245, 47));
    position->addSeriesCustom(new QLineSeries(), "z",QColor(25, 60, 255));
    ui->GraphA_2->setChart(position);

    gyro = new GraphWidget("Gyro", QBrush(QColor(30, 30, 19)), 8);
    gyro->addSeriesCustom(new QLineSeries(), "x", QColor(245, 60, 25));
    gyro->addSeriesCustom(new QLineSeries(), "y", QColor(24, 245, 47));
    gyro->addSeriesCustom(new QLineSeries(), "z",QColor(25, 60, 255));
    ui->GraphA_3->setChart(gyro);

    gyro = new GraphWidget("Gyro", QBrush(QColor(30, 30, 19)), 8);
    gyro->addSeriesCustom(new QLineSeries(), "x", QColor(245, 60, 25));
    gyro->addSeriesCustom(new QLineSeries(), "y", QColor(24, 245, 47));
    gyro->addSeriesCustom(new QLineSeries(), "z",QColor(25, 60, 255));
    ui->GraphA_3->setChart(gyro);


    velocity = new GraphWidget("Velocity", QBrush(QColor(30, 30, 19)), 8);
    velocity->addSeriesCustom(new QLineSeries(), "x", QColor(245, 160, 125));
    velocity->addSeriesCustom(new QLineSeries(), "y", QColor(124, 245, 147));
    velocity->addSeriesCustom(new QLineSeries(), "z", QColor(125, 160, 255));
    ui->GraphA_4->setChart(velocity);

    connect(timer, &QTimer::timeout, this, &GraphWindow::scroll);
    connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, &GraphWindow::telemetryAvailable);
    scroll();
}


GraphWindow::~GraphWindow()
{
    delete ui;
}
