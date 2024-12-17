//
// Created by William Scheirey on 12/16/24.
//

#ifndef GS_BACKEND_2024_2025_LEDWIDGET_H
#define GS_BACKEND_2024_2025_LEDWIDGET_H

#include <QWidget>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui
{
    class LedWidget;
}
QT_END_NAMESPACE

class LedWidget : public QWidget
{
Q_OBJECT

public:
    explicit LedWidget(QWidget *parent = nullptr);

    void resetTimer();
    void setLabel(const QString &label);
    void updateLabel();

    ~LedWidget() override;

    int timeoutDuration_sec = 3;

private:
    Ui::LedWidget *ui;
    QTimer timer;

public slots:
    void timedOut();
};


#endif //GS_BACKEND_2024_2025_LEDWIDGET_H
