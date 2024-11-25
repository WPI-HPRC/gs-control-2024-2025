//
// Created by William Scheirey on 11/24/24.
//

#ifndef GS_BACKEND_2024_2025_GRAPHICSWINDOW_H
#define GS_BACKEND_2024_2025_GRAPHICSWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui
{
    class GraphicsWindow;
}
QT_END_NAMESPACE

class GraphicsWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit GraphicsWindow(QWidget *parent = nullptr);

    ~GraphicsWindow() override;

private:
    Ui::GraphicsWindow *ui;
};


#endif //GS_BACKEND_2024_2025_GRAPHICSWINDOW_H
