//
// Created by Nicolas DeBruin on 10/26/2024.
//

#ifndef PACKETSPERSECWIDGET_H
#define PACKETSPERSECWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class PacketsPerSecWidget; }
QT_END_NAMESPACE

class PacketsPerSecWidget : public QWidget {
Q_OBJECT

public:
    explicit PacketsPerSecWidget(QWidget *parent = nullptr);
    ~PacketsPerSecWidget() override;

private:
    Ui::PacketsPerSecWidget *ui;
};


#endif //PACKETSPERSECWIDGET_H
