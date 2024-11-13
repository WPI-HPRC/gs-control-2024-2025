// it's 2024, compilers will support this...
#pragma once

#include <Backend/Backend.h>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>

#define GL_SILENCE_DEPRECATION
class NavBall : public QOpenGLWidget, protected QOpenGLFunctions {    
    Q_OBJECT

private:
    QMatrix4x4 navball_transform;
    
public:
    NavBall(QWidget *parent = nullptr);

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int, int) override;

    QSize sizeHint() const override {
        return QSize(400, 400);
    }

public slots:
    void onTelemetryData(Backend::Telemetry);
};
