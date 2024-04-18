#include "bird.h"
#include "config.h"

Bird::Bird()
{
    m_Bird.load(BIRD_PATH);

    m_X = 300;
    m_Y = 630;
    v_X = 0;
    v_Y = 0;

    m_free = true;
    m_limit = false;
    m_life = true;

    m_Rect.setWidth(m_Bird.width());
    m_Rect.setHeight(m_Bird.height());
    m_Rect.moveTo(m_X,m_Y);

    n_Bird.load(BIRD_NPATH);

    n_X = 450;
    n_Y = 635;
    v_n_X = 0;
    v_n_Y = 0;

    n_free = true;
    n_limit = false;
    n_life = true;

    n_Rect.setWidth(n_Bird.width());
    n_Rect.setHeight(n_Bird.height());
    n_Rect.moveTo(n_X,n_Y);

    m_land = true;
    n_land = true;



}

void Bird::jumpSet(int l)
{
    m_free = false;
    m_land =false;
    v_X = 5;
    v_n_X = - v_X;
    v_n_Y = 0;
    if(!n_life)
    {
        v_n_X = 0;
        v_n_Y = -10;

        if(n_X > m_X)
        {
            v_n_X = -v_X;
        }
    }
    v_Y = l * 0.02;
}

void Bird::jumpFollow(int l)
{
    n_free = false;
    n_limit = false;
    v_n_X = 5;
    v_X = -v_n_X;
    v_Y = 0;
    if(!m_life)
    {
        v_X = 0;
        v_Y = -10;

        if(m_X > n_X)
        {
            v_X = -v_n_X;
        }
    }
    v_n_Y = l * 0.02;
}

void Bird::updatePosition()
{
    if(m_free)
    {
        return;
    }

    if(n_X > 0)
    {
        m_Y -= v_Y;
        m_X += v_X;
        m_limit = false;

        n_X += v_n_X;
        n_Y += v_n_Y;

        v_Y -= 1;


    }

    if(n_X <= 0)
    {
        m_limit = true;
        v_X = 0;
        v_n_X = 0;

        m_Y -= v_Y;
        n_Y += v_n_Y;

        v_Y -= 3;

    }

    if(m_Y >= GAME_HEIGHT - m_Bird.height())
    {
        m_limit = true;
        m_life = false;
        m_free = true;
        v_X = 0;
        v_n_X = 0;

        v_Y = 0;

        m_Y = GAME_HEIGHT - m_Bird.height();


    }



    m_Rect.moveTo(m_X,m_Y);



}

void Bird::updatePositionN()
{
    if(n_free)
    {
        return;
    }


    if(m_X > 0)
    {
        n_Y -= v_n_Y;
        n_X += v_n_X;
        n_limit = false;

        m_X += v_X;
        m_Y += v_Y;

        v_n_Y -= 1;

    }


    if(m_X <= 0)
    {
        n_limit = true;
        v_X = 0;
        v_n_X = 0;

        n_Y -= v_n_Y;
        m_Y += v_Y;

        v_n_Y -= 3;
    }


    if(n_Y >= GAME_HEIGHT - n_Bird.height())
    {
        n_free = true;
        n_limit = false;
        n_life = false;
        v_X = 0;
        v_n_X = 0;

        v_n_Y = 0;

        n_Y = GAME_HEIGHT - n_Bird.height();

    }

    n_Rect.moveTo(n_X,n_Y);
}

void Bird::setPosition(int x, int y)
{
    m_X = x;
    m_Y = y;
    m_Rect.moveTo(m_X,m_Y);
}

void Bird::setPositionN(int x, int y)
{
    n_X = x;
    n_Y = y;
    n_Rect.moveTo(n_X,n_Y);
}

void Bird::down()
{
    if(m_Y<=GAME_HEIGHT - m_Bird.height())
    {
        m_Y += 8;
    }
}

void Bird::downN()
{
    if(n_Y<=GAME_HEIGHT - n_Bird.height())
    {
        n_Y += 8;
    }
}

void Bird::slideBack()
{
    if(m_X >= 0.1 * GAME_WIDTH)
    {
        m_X -= MAP_SPEED;
        if(n_X >= 0)
        {
            n_X -= MAP_SPEED;
        }
    }
}

void Bird::sildeBackN()
{
    if(n_X >= 0.1 * GAME_WIDTH)
    {
        n_X -= MAP_SPEED;
        if(m_X >= 0)
        {
            m_X -= MAP_SPEED;
        }
    }
}

void Bird::disElastic()
{
    dis = abs(m_X - n_X);
    if(dis > 0.2 * GAME_WIDTH)
    {
        if(m_X > n_X && m_X - n_X > 0.15 * GAME_WIDTH)
        {
            m_X -= 5;
            n_X += 5;
        }
        if(m_X < n_X && n_X - m_X > 0.15 * GAME_WIDTH)
        {
            m_X += 5;
            n_X -= 5;
        }
    }

    else if(m_X < 0)
    {
        m_X += 5;
        n_X -= 5;
    }

    else if(n_X < 0)
    {
        n_X += 5;
        m_X -= 5;
    }
}

