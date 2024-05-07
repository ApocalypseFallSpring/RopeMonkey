#include "map.h"

Map::Map()
{
    m_map1.load(MAP_PATH);
    m_map2.load(MAP_PATH);

    m_map1_posX = GAME_WIDTH;
    m_map1_posY = 0;
    m_map2_posX = 0;
    m_map2_posY = 0;

    m_map_scroll = MAP_SPEED;



}


void Map::mapPosition()
{
    m_map1_posX -= MAP_SPEED;
    if(m_map1_posX <= 0)
    {
        m_map1_posX = GAME_WIDTH;
    }

    m_map2_posX -= MAP_SPEED;
    if(m_map2_posX <= -GAME_WIDTH)
    {
        m_map2_posX = 0;
    }
}

void Map::slideBack()
{


        m_map1_posX -= MAP_SPEED;
        m_map2_posX -= MAP_SPEED;
        if(m_map1_posX <= 0)
        {
            m_map1_posX = GAME_WIDTH;
        }
        if(m_map2_posX <= -GAME_WIDTH)
        {
            m_map2_posX = 0;
        }


}
