#include "monkey.h"

Monkey::Monkey()
{
    m_Monkey.load(MONKEY_PATH);

    m_X = 200;
    m_Y = 700;
    v_X = 0;
    v_Y = 0;

    m_free = true;
    m_limit = false;
    m_life = true;

    m_Rect.setWidth(m_Monkey.width());
    m_Rect.setHeight(m_Monkey.height());
    m_Rect.moveTo(m_X,m_Y);

    n_Monkey.load(MONKEY_NPATH);

    n_X = 225;
    n_Y = 700;
    v_n_X = 0;
    v_n_Y = 0;

    n_free = true;
    n_limit = false;
    n_life = true;

    n_Rect.setWidth(n_Monkey.width());
    n_Rect.setHeight(n_Monkey.height());
    n_Rect.moveTo(n_X,n_Y);

    m_land = true;
    n_land = true;


}

void Monkey::jumpSet(int l)
{
    m_free = false;
    m_land =false;
    v_X = MONKEY_SPEED;
    v_n_X = - v_X;
    v_n_Y = 0;
    if(!n_life)
    {
        v_n_X = 0;
        v_n_Y = -20;

        if(n_X > m_X)
        {
            v_n_X = -v_X;
        }
    }
    v_Y = l * 0.02;
}

void Monkey::jumpFollow(int l)
{
    n_free = false;
    n_limit = false;
    v_n_X = MONKEY_SPEED;
    v_X = -v_n_X;
    v_Y = 0;
    if(!m_life)
    {
        v_X = 0;
        v_Y = -20;

        if(m_X > n_X)
        {
            v_X = -v_n_X;
        }
    }
    v_n_Y = l * 0.02;
}

void Monkey::updatePosition()
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

    if(m_Y >= GAME_HEIGHT - m_Monkey.height())
    {
        m_limit = true;
        m_life = false;
        m_free = true;
        v_X = 0;
        v_n_X = 0;

        v_Y = 0;

        m_Y = GAME_HEIGHT - m_Monkey.height();


    }



    m_Rect.moveTo(m_X,m_Y);
}

void Monkey::updatePositionN()
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


    if(n_Y >= GAME_HEIGHT - n_Monkey.height())
    {
        n_free = true;
        n_limit = false;
        n_life = false;
        v_X = 0;
        v_n_X = 0;

        v_n_Y = 0;

        n_Y = GAME_HEIGHT - n_Monkey.height();

    }

    n_Rect.moveTo(n_X,n_Y);
}

void Monkey::setPosition(int x, int y)
{
    m_X = x;
    m_Y = y;
    m_Rect.moveTo(m_X,m_Y);
}

void Monkey::setPositionN(int x, int y)
{
    n_X = x;
    n_Y = y;
    n_Rect.moveTo(n_X,n_Y);
}

void Monkey::down()
{
    if(m_Y<=GAME_HEIGHT - m_Monkey.height())
    {
        m_Y += 8;
    }
}

void Monkey::downN()
{
    if(n_Y<=GAME_HEIGHT - n_Monkey.height())
    {
        n_Y += 8;
    }
}

void Monkey::slideBack()
{
    if(m_X >= 0.1 * GAME_WIDTH)
    {
        m_X -= MAP_SPEED+1;
        if(n_X >= 0)
        {
            n_X -= MAP_SPEED+1;
        }
    }
}

void Monkey::sildeBackN()
{
    if(n_X >= 0.1 * GAME_WIDTH)
    {
        n_X -= MAP_SPEED+1;
        if(m_X >= 0)
        {
            m_X -= MAP_SPEED;
        }
    }

}

void Monkey::disElastic()
{
    dis = abs(m_X - n_X);
    if(dis > 0.35 * GAME_WIDTH)
    {
        if(m_X > n_X && m_X - n_X > 0.2 * GAME_WIDTH)
        {
            m_X -= 5;
            n_X += 5;
        }
        if(m_X < n_X && n_X - m_X > 0.2 * GAME_WIDTH)
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
