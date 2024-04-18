#include "phase.h"

Phase::Phase()
{
    m_phase.load(PHASE_PATH);


    m_Rect.setWidth(m_phase.width());
    m_Rect.setHeight(m_phase.height());
    m_Rect.moveTo(m_X,m_Y);

    m_free = true;

    m_speed =PHASE_SPEED;


}

void Phase::updatePosition()
{
    if(m_free)
    {
        return;
    }

    m_X -= m_speed;
    m_Rect.moveTo(m_X,m_Y);

    if(m_X < -m_phase.width())
    {
        m_free = true;
    }
}

void Phase::slideBack()
{
    if(m_free)
    {
        return;
    }

    m_X -= MAP_SPEED+5;
    m_Rect.moveTo(m_X,m_Y);

    if(m_X < -m_phase.width())
    {
        m_free = true;
    }
}


