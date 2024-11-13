#include "NavBall.h"

#include <QTimer>

#include <QQuaternion>
#include <cassert>
#include <math.h>
#include <tuple>

// struct Timer : public QTimer {
//   NavBall *nv;
//   Timer(NavBall *nv) : nv(nv) { }
//   void timerEvent(QTimerEvent *e) override {
//     nv->update();
//   }
// };

static const QImage *getNavballTexture() {
  static QImage q = []() {
    QImage q;
    if (!q.load("../assets/navball.jpg")) {
      q.allGray();
    }
    q.mirror();
    return q;
  }();

  return &q;
}

// static Timer* timer;

NavBall::NavBall(QWidget *parent) : QOpenGLWidget(parent) {
  this->setAttribute(Qt::WA_AlwaysStackOnTop);
  // timer = new Timer(this);
  // timer->setInterval(33);
  // timer->start();

  connect(&Backend::getInstance(), &Backend::telemetryAvailable, this,
          &NavBall::onTelemetryData);
}

static GLuint navball_texture;

void NavBall::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0, 0, 0, 0);

  const QImage *q = getNavballTexture();
  glGenTextures(1, &navball_texture);
  glBindTexture(GL_TEXTURE_2D, navball_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, q->width(), q->height(), 0, GL_BGRA,
               GL_UNSIGNED_BYTE, q->bits());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void NavBall::paintGL() {
  glMatrixMode(GL_MODELVIEW);
  auto V = QMatrix4x4(
     0, 1, 0, 0,
     0, 0, 1, 0,
     1, 0, 0, 0,
     0, 0, 0, 1
   );
  glLoadMatrixf((V * navball_transform).transposed().constData());

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, navball_texture);

  auto spherical = [](float theta, float rho) {
    glTexCoord2f(-theta / (2 * M_PI), (rho - M_PI / 2.0) / M_PI);
    glVertex3f(cosf(theta) * cosf(rho), sinf(rho), sinf(theta) * cosf(rho));
  };

  glBegin(GL_TRIANGLES);
  constexpr float THETA_STEP = M_PI * 2.0f / 50.0f;
  constexpr float RHO_STEP = 2.0f * M_PI / 50.0f;
  for (float theta = 0; theta < M_PI * 2.0f; theta += THETA_STEP) {
    for (float rho = -M_PI / 2.0f; rho < M_PI / 2.0f; rho += RHO_STEP) {
      spherical(theta, rho);
      spherical(theta + THETA_STEP, rho);
      spherical(theta + THETA_STEP, rho + RHO_STEP);

      spherical(theta, rho);
      spherical(theta + THETA_STEP, rho + RHO_STEP);
      spherical(theta, rho + RHO_STEP);
    }
  }
  glEnd();
}

void NavBall::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  const float aspect = float(w) / float(h);
  glOrtho(-aspect, aspect, -1, 1, -1, 1);
}

void NavBall::onTelemetryData(Backend::Telemetry telem) {
  if (telem.packetType == GroundStation::PacketType::Rocket) {
    const auto *data = telem.data.rocketData;
    QQuaternion q(data->w, data->i, data->j, data->k);
    navball_transform.setToIdentity();
    navball_transform.rotate(q);
    update();
  }
}
