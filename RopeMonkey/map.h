#ifndef MAP_H
#define MAP_H

#include "config.h"
#include <QPixmap>


class Map
{
public:
    Map();

    QPixmap m_map1;
    QPixmap m_map2;

    int m_map1_posX;
    int m_map1_posY;
    int m_map2_posX;
    int m_map2_posY;

    int m_map_scroll;

    void mapPosition();

    void slideBack();



};

#endif // MAP_H
