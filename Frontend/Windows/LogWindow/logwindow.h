//
// Created by William Scheirey on 12/17/24.
//

#ifndef GS_BACKEND_2024_2025_LOGWINDOW_H
#define GS_BACKEND_2024_2025_LOGWINDOW_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui
{
    class LogWindow;
}
QT_END_NAMESPACE

class LogWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LogWindow(QWidget *parent = nullptr);

    void displayCsv(const QString &filepath);

    QString currentCSV;

    ~LogWindow() override;

private:
    Ui::LogWindow *ui;
};


#endif //GS_BACKEND_2024_2025_LOGWINDOW_H
