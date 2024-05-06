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
    QPixmap m_house;
    QRect m_Rect;

    int m_X;
    int m_Y;
    int m_h_X;
    int m_h_Y;

    void updatePosition();
    void slideBack();

};

#endif // STAGE_H
