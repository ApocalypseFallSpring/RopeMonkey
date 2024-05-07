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

    QPixmap m_gold;
    QRect g_Rect;
    int g_X;
    int g_Y;
    int g_speed;
    bool gold;

    QPixmap m_flower;
    QRect f_Rect;
    int f_X;
    int f_Y;
    int f_speed;
    bool flower;

    void updatePosition();

    void slideBack();

};

#endif // PHASE_H
