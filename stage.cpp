#include "stage.h"

stage::stage()
{
    m_stage.load(STAGE_PATH);

    m_X = 300;
    m_Y = 725;

    m_Rect.setWidth(m_stage.width());
    m_Rect.setHeight(m_stage.height());
    m_Rect.moveTo(m_X,m_Y);
}

void stage::updatePosition()
{
    m_X -= 5;
    m_Rect.moveTo(m_X,m_Y);
}
