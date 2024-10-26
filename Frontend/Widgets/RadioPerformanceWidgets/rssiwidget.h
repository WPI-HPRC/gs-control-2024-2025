//
// Created by Nicolas DeBruin on 10/26/2024.
//

#ifndef RSSIWIDGET_H
#define RSSIWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class RSSIWidget; }
QT_END_NAMESPACE

class RSSIWidget : public QWidget {
Q_OBJECT

public:
    explicit RSSIWidget(QWidget *parent = nullptr);
    ~RSSIWidget() override;

private:
    Ui::RSSIWidget *ui;

public slots:
    void receiveAtCommandResponse(uint16_t command, const uint8_t *response, size_t response_length_bytes);
};


#endif //RSSIWIDGET_H
