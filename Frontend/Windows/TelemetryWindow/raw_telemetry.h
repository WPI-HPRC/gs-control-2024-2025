//
// Created by Rafael on 28.09.24.
//

#ifndef RAW_TELEMETRY_H
#define RAW_TELEMETRY_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class Raw_Telemetry; }
QT_END_NAMESPACE

class Raw_Telemetry : public QMainWindow {
Q_OBJECT

public:
    explicit Raw_Telemetry(QWidget *parent = nullptr);
    ~Raw_Telemetry() override;

private:
    Ui::Raw_Telemetry *ui;
};


#endif //RAW_TELEMETRY_H
