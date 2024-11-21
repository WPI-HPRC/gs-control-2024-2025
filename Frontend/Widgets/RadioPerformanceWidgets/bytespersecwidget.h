//
// Created by Nicolas DeBruin on 10/26/2024.
//

#ifndef BYTESPERSECWIDGET_H
#define BYTESPERSECWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class BytesPerSecWidget; }
QT_END_NAMESPACE

class BytesPerSecWidget : public QWidget {
Q_OBJECT

public:
    explicit BytesPerSecWidget(QWidget *parent = nullptr);
    ~BytesPerSecWidget() override;

public slots:
    void bytesPerSecondUpdate(uint64_t);

private:
    Ui::BytesPerSecWidget *ui;
};


#endif //BYTESPERSECWIDGET_H
