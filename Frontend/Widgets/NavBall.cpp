#include "NavBall.h"

#include <QMatrix4x4>

#include <math.h>
#include <tuple>

NavBall::NavBall(QWidget *parent) : QOpenGLWidget(parent) {
  this->setAttribute(Qt::WA_AlwaysStackOnTop);
}


void NavBall::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0,0,0,0);

}


void NavBall::paintGL()
{
    QMatrix4x4 transform;
    static float angle = 0.0f;
    transform.rotate(angle, 1, 0, 0);
    angle += 10.0f;
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(transform.transposed().constData());

    glClear(GL_COLOR_BUFFER_BIT 
          | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    auto spherical = [](float theta, float rho) {
      if (rho < 0) {
        glColor3f(0.6,0.2,0);
      }
      else {
        glColor3f(0,1,1);
      }
      glVertex3f(
        cosf(theta) * cosf(rho),
        sinf(rho),
        sinf(theta) * cosf(rho)
      );
    };

    glBegin(GL_TRIANGLES);
    constexpr float THETA_STEP = M_PI * 2.0f / 50.0f;
    constexpr float RHO_STEP   = 2.0f * M_PI / 50.0f;
    for (float theta = 0; theta < M_PI * 2.0f; theta += THETA_STEP) {
      for (float rho = -M_PI / 2.0f; rho < M_PI / 2.0f; rho += RHO_STEP) {
        spherical(theta,              rho);
        spherical(theta + THETA_STEP, rho);
        spherical(theta + THETA_STEP, rho + RHO_STEP);

        spherical(theta,              rho);
        spherical(theta + THETA_STEP, rho + RHO_STEP);
        spherical(theta,              rho + RHO_STEP);
      }
    }
    glEnd();
}

void NavBall::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const float aspect = float(w) / float(h);
    glOrtho(-aspect, aspect, -1, 1, -1, 1);
}
