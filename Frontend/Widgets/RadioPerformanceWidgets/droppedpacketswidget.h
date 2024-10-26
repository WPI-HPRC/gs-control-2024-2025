//
// Created by Nicolas DeBruin on 10/26/2024.
//

#ifndef DROPPEDPACKETSWIDGET_H
#define DROPPEDPACKETSWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class DroppedPacketsWidget; }
QT_END_NAMESPACE

class DroppedPacketsWidget : public QWidget {
Q_OBJECT

public:
    explicit DroppedPacketsWidget(QWidget *parent = nullptr);
    ~DroppedPacketsWidget() override;

private:
    Ui::DroppedPacketsWidget *ui;
};


#endif //DROPPEDPACKETSWIDGET_H
