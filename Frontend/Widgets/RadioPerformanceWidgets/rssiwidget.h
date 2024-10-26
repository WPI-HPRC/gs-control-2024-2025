//
// Created by Nicolas DeBruin on 10/26/2024.
//

#ifndef LASTPACKETDBM_H
#define LASTPACKETDBM_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class LastPacketdBm; }
QT_END_NAMESPACE

class LastPacketdBm : public QWidget {
Q_OBJECT

public:
    explicit LastPacketdBm(QWidget *parent = nullptr);
    ~LastPacketdBm() override;

private:
    Ui::LastPacketdBm *ui;
};


#endif //LASTPACKETDBM_H
