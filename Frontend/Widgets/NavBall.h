// it's 2024, compilers will support this...
#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#define GL_SILENCE_DEPRECATION
struct NavBall : public QOpenGLWidget, protected QOpenGLFunctions {

    NavBall(QWidget *parent = nullptr);

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int, int) override;

    QSize sizeHint() const override {
        return QSize(400, 400);
    }
    
    Q_OBJECT
};
