//
// Created by William Scheirey on 12/16/24.
//

// Code from https://forum.qt.io/topic/101648/how-to-create-simply-virtual-led-indicator/2

#ifndef GS_BACKEND_2024_2025_QLEDLABEL_H
#define GS_BACKEND_2024_2025_QLEDLABEL_H

#include <QLabel>

class QLedLabel : public QLabel
{
Q_OBJECT
public:
    explicit QLedLabel(QWidget *parent = 0);

    enum State{
        StateOk,
        StateWarning,
        StateError
    };


signals:

public slots:
    void setState(State state);
    void setState(bool state);
};

#endif //GS_BACKEND_2024_2025_QLEDLABEL_H
