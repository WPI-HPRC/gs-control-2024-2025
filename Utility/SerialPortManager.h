//
// Created by William Scheirey on 8/27/24.
//

#ifndef GS_BACKEND_2024_2025_SERIALPORTMANAGER_H
#define GS_BACKEND_2024_2025_SERIALPORTMANAGER_H
#include <QObject>
#include <Backend/Backend.h>
#include <QThread>

class SerialPortManager: public QObject
{
    Q_OBJECT
public:
    static SerialPortManager &getInstance()
    {
        static SerialPortManager instance;
        return instance;
    }
    SerialPortManager(const SerialPortManager&) = delete;
    SerialPortManager &operator=(const SerialPortManager&) = delete;

public slots:
    void openPort(const QString&, Backend::RadioModuleType, int);
    void closePort(const QString&);

private:
    explicit SerialPortManager(QObject *parent = nullptr);
    QThread *thread;

};


#endif //GS_BACKEND_2024_2025_SERIALPORTMANAGER_H
