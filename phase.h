#ifndef PHASE_H
#define PHASE_H

#include <QPixmap>
#include "config.h"
#include <QRect>


class Phase
{
public:
    Phase();

    QPixmap m_phase;
    QRect m_Rect;

    int m_X;
    int m_Y;
    int m_speed;

    bool m_free;


    void updatePosition();

    void slideBack();

};

#endif // PHASE_H
