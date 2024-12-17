//
// Created by William Scheirey on 11/24/24.
//

#include "Viewer3D.h"
#include <QVBoxLayout>

Viewer3D::Viewer3D(QWidget *parent): QWidget(parent)
{
    setMinimumSize(1, 1);

    auto *view = new Qt3DExtras::Qt3DWindow;

    auto *rootEntity = new Qt3DCore::QEntity;

    // Create rocket mesh and entity objects
    auto *rocketEntity = new Qt3DCore::QEntity(rootEntity);
    auto *rocketMesh = new Qt3DRender::QMesh();

    // Load the rocket mesh
    rocketMesh->setSource(QUrl(QStringLiteral("qrc:/Models/HPRC_rocket.stl")));

    // Set the material of the rocket
    auto *rocketMaterial = new Qt3DExtras::QDiffuseSpecularMaterial(rootEntity);
    rocketMaterial->setDiffuse(QColor(255, 0, 0)); // Set the diffuse color (red in this case)
    rocketMaterial->setAmbient(QColor(155, 0, 0));
    rocketMaterial->setSpecular(QColor(255, 255, 255)); // Set the specular color (white)
    rocketMaterial->setShininess(1); // Set the shininess (adjust to control specular highlights)
//    rocketMaterial->setNormal()

    auto *rocketTransform = new Qt3DCore::QTransform;
    rocketTransform->setTranslation(QVector3D(0.0, m_rocketVertOffset, 0.0));

    // Scale the rocket to fit correctly on the screen
    rocketTransform->setScale3D(QVector3D(m_rocketScale, m_rocketScale, m_rocketScale));

    // Set the rotation of the rocket based on the input quaternion
    rocketTransform->setRotation(
            QQuaternion::fromEulerAngles(0.0f, 0.0f, 0.0f) * m_rocketOrientVertically);

    rocketEntity->addComponent(rocketMesh);
    rocketEntity->addComponent(rocketTransform);
    rocketEntity->addComponent(rocketMaterial);

    Qt3DCore::QEntity *scene = rootEntity;

    // Camera
    Qt3DRender::QCamera *camera = view->camera();
    camera->setPosition(QVector3D(0, 0, m_cameraDistToRocket));
    camera->setViewCenter(QVector3D(0, 0, 0));

    view->setRootEntity(scene);
    view->defaultFrameGraph()->setClearColor(QWidget::palette().window().color());

    // Store the transform of the rocket
    m_rocketTransform = rocketTransform;

    // Store the 3d view
    m_view = view;

    // Store the rocket material
    m_rocketMaterial = rocketMaterial;

    QWidget *container = QWidget::createWindowContainer(view, this);
    container->setContentsMargins(0, 0, 0, 0);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(container);
    setLayout(layout);

    // Resize the 3D viewer to match the container widget
    m_view->resize(this->size());

    connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, &Viewer3D::telemetryAvailable);
}

void Viewer3D::orientRocket(QQuaternion orientation) const
{
    // Set the rotation of the rocket based on the input quaternion
    m_rocketTransform->setRotation(orientation * m_rocketOrientVertically);
    m_view->defaultFrameGraph()->setClearColor(QWidget::palette().window().color());
}

void Viewer3D::updateColors(QColor panel, QColor highlight) const
{
    m_rocketMaterial->setDiffuse(highlight);

    m_view->defaultFrameGraph()->setClearColor(panel);
}

void Viewer3D::telemetryAvailable(Backend::Telemetry telemetry) const
{
    if (telemetry.packetType == GroundStation::Rocket)
    {
        orientRocket({telemetry.data.rocketData->w(), telemetry.data.rocketData->i(), telemetry.data.rocketData->j(), telemetry.data.rocketData->k()});
    }
}