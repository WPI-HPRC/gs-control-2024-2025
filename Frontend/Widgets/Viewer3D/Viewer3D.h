//
// Created by William Scheirey on 11/24/24.
//

#ifndef GS_BACKEND_2024_2025_VIEWER3D_H
#define GS_BACKEND_2024_2025_VIEWER3D_H

#include <QWidget>

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QCamera>
#include <Qt3DCore/QTransform>

#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/Qt3DWindow>

#include "Backend/Backend.h"

class Viewer3D: public QWidget
{
    Q_OBJECT

public:
    explicit Viewer3D(QWidget *parent = nullptr);

    void orientRocket(QQuaternion orientation) const;
    void updateColors(QColor panel, QColor highlight) const;

    float m_rocketVertOffset = -1.8f;
    float m_cameraDistToRocket = 40.0f;
    float m_rocketScale = 0.18f;

    // 3D Props
    Qt3DCore::QTransform *m_rocketTransform;
    Qt3DExtras::Qt3DWindow *m_view;
    Qt3DExtras::QDiffuseSpecularMaterial *m_rocketMaterial;

    // Note: the -90 is there to orient the rocket correctly
    // since the STL will face out of the screen by default
    // Note: euler angles are pitch, yaw, roll
    QQuaternion m_rocketOrientVertically = QQuaternion::fromEulerAngles(-90.0f, 0.0f, 0.0f);

public slots:
    void telemetryAvailable(Backend::Telemetry telemetry) const;
};


#endif //GS_BACKEND_2024_2025_VIEWER3D_H
