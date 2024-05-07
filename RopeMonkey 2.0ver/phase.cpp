#include "phase.h"

Phase::Phase()
{
    m_phase.load(PHASE_PATH);

    m_gold.load(GOLD_PATH);

    m_flower.load(FLOWER_PATH);

    g_Rect.setWidth(m_gold.width());
    g_Rect.setHeight(m_gold.height());
    g_Rect.moveTo(g_X,g_Y);
    g_speed = PHASE_SPEED;


    f_Rect.setWidth(m_flower.width());
    f_Rect.setHeight(m_flower.height());
    f_Rect.moveTo(-m_flower.width(),-m_flower.height());
    f_speed = PHASE_SPEED;

    m_Rect.setWidth(m_phase.width());
    m_Rect.setHeight(m_phase.height());
    m_Rect.moveTo(m_X,m_Y);

    m_free = true;

    m_speed = PHASE_SPEED;


}

void Phase::updatePosition()
{
    if(m_free)
    {
        return;
    }

    m_X -= m_speed;
    m_Rect.moveTo(m_X,m_Y);

    g_X -= g_speed;
    g_Rect.moveTo(g_X,g_Y);

    f_X -= f_speed;
    f_Rect.moveTo(f_X,f_Y);

    if(m_X < -m_phase.width())
    {
        m_free = true;
    }

    if(m_X < -m_phase.width())
    {
        gold = false;
    }

    if(m_X < -m_phase.width())
    {
        flower = false;
    }
}

void Phase::slideBack()
{
    if(m_free)
    {
        return;
    }

    m_X -= MAP_SPEED*2;
    m_Rect.moveTo(m_X,m_Y);

    g_X -= MAP_SPEED*2;
    g_Rect.moveTo(g_X,g_Y);

    f_X -= MAP_SPEED*2;
    f_Rect.moveTo(f_X,f_Y);

    if(m_X < -m_phase.width())
    {
        m_free = true;
    }

    if(m_X < -m_phase.width())
    {
        gold = false;
    }

    if(m_X < -m_phase.width())
    {
        flower = false;
    }
}
