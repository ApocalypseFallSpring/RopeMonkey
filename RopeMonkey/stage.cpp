#include "stage.h"

stage::stage()
{
    m_stage.load(STAGE_PATH);
    m_house.load(HOUSE_PATH);

    m_X = 150;
    m_Y = 784;

    m_h_X = 140;
    m_h_Y = 604;

    m_Rect.setWidth(m_stage.width());
    m_Rect.setHeight(m_stage.height());
    m_Rect.moveTo(m_X,m_Y);
}

void stage::updatePosition()
{
    m_X -= MONKEY_SPEED;
    m_h_X -= MONKEY_SPEED;
    m_Rect.moveTo(m_X,m_Y);
}

void stage::slideBack()
{
    m_X -= MONKEY_SPEED*2;
    m_h_X -= MONKEY_SPEED*2;
    m_Rect.moveTo(m_X,m_Y);
}
