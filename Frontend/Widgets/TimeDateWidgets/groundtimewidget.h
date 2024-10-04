//
// Created by Nicolas DeBruin on 10/3/2024.
//

#ifndef GROUNDTIMEWIDGET_H
#define GROUNDTIMEWIDGET_H

#include <QWidget>
#include <QLabel>

#include <Utility/Utility.h>
#include "Backend/Backend.h"


QT_BEGIN_NAMESPACE
namespace Ui { class GroundTimeWidget; }
QT_END_NAMESPACE

class GroundTimeWidget : public QWidget {
Q_OBJECT

public:
    explicit GroundTimeWidget(QWidget *parent = nullptr);
    ~GroundTimeWidget() override;

private:
    Ui::GroundTimeWidget *ui;

    QLabel *groundTimeText;

public slots:
    void newTime(std::tm* currentTime);
};


#endif //GROUNDTIMEWIDGET_H
