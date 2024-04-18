#ifndef STAGE_H
#define STAGE_H
#include "config.h"
#include <QPixmap>
#include <QRect>

class stage
{
public:
    stage();


    QPixmap m_stage;
    QRect m_Rect;

    int m_X;
    int m_Y;

    void updatePosition();




};

#endif // STAGE_H
