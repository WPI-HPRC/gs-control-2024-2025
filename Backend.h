//
// Created by William Scheirey on 8/20/24.
//

#ifndef GS_BACKEND_2024_2025_BACKEND_H
#define GS_BACKEND_2024_2025_BACKEND_H

#include <QObject>

class Backend: public QObject
{
public:
    explicit Backend(QObject *parent = nullptr);
    int loopCount;

private:

};


#endif //GS_BACKEND_2024_2025_BACKEND_H
