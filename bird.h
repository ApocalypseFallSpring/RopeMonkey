#ifndef BIRD_H
#define BIRD_H

#include "config.h"
#include <QPixmap>
#include <QRect>

class Bird
{
public:
    Bird();

    QPixmap m_Bird;
    QRect m_Rect;

    QPixmap n_Bird;
    QRect n_Rect;

    int m_X;
    int m_Y;

    int n_X;
    int n_Y;

    double v_X;
    double v_Y;

    double v_n_X;
    double v_n_Y;

    bool m_free;
    bool n_free;

    bool m_limit;
    bool n_limit;

    bool m_life;
    bool n_life;

    bool m_land;
    bool n_land;

    int dis;


    void jumpSet(int l);
    void jumpFollow(int l);

    void updatePosition();
    void updatePositionN();

    void setPosition(int x, int y);
    void setPositionN(int x, int y);

    void down();
    void downN();

    void slideBack();
    void sildeBackN();

    void disElastic();

};

#endif // BIRD_H
