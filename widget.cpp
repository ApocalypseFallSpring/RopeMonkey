
#include "widget.h"
#include "ui_widget.h"
#include "config.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    initScence();

    playGame();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initScence()
{
    setFixedSize(GAME_WIDTH,GAME_HEIGHT);

    setWindowTitle(GAME_TITLE);

    setWindowIcon(QIcon(GAME_ICON));

    m_timer.setInterval(GAME_RATE);
    m_back.setInterval(GAME_RATE);
    m_dis.setInterval(GAME_RATE);
    m_slide.setInterval(GAME_RATE);

    m_recorder = 0;
    cnt = 300;

    srand((unsigned int)time(NULL));


    m_Phases[0].m_free = false;
    m_Phases[0].m_X = 600 + rand()%300;
    m_Phases[0].m_Y = rand()%350 + 500;
    cnt = m_Phases[0].m_X;
    m_Phases[0].m_Rect.moveTo(m_Phases[0].m_X,m_Phases[0].m_Y);


}

void Widget::playGame()
{
    m_timer.start();

    m_back.start();

    m_dis.start();

    m_slide.start();

    connect(&m_timer, &QTimer::timeout,[=](){

        if(m_bird.m_free == false || m_bird.n_free == false)
        {
            if(m_bird.m_life||m_bird.n_life)
            {
                phaseToScene();

            }
        }


        updatePosition();




        update();

    });

    connect(&m_back, &QTimer::timeout, [=](){
        if(m_bird.m_free)
        {
            fall();
            if(m_bird.m_land == false)
            {
                m_bird.down();
                m_bird.m_life = false;
            }
        }

        if(m_bird.n_free)
        {
            fallN();
            if(m_bird.n_land == false)
            {
                m_bird.downN();
                m_bird.n_life = false;
            }
        }


        update();
    });


    connect(&m_dis, &QTimer::timeout,[=](){

        m_bird.disElastic();
        if(m_bird.m_life == false)
        {
            recover();
            if(m_bird.m_land == false)
            {
                m_bird.down();
            }
        }
        if(m_bird.n_life == false)
        {
            recoverN();
            if(m_bird.n_land == false)
            {
                m_bird.downN();
            }
        }

        if(m_bird.m_life == false && m_bird.n_life == false)
        {
            m_bird.down();
            m_bird.downN();
        }

        update();

    });

    connect(&m_slide, &QTimer::timeout,[=](){

        if(slide)
        {
            if((m_bird.m_free && m_bird.m_land && m_bird.n_free && m_bird.n_land && m_bird.m_life && m_bird.n_life) || (m_bird.m_free && m_bird.m_land && m_bird.m_life && !m_bird.n_life) || (m_bird.n_free && m_bird.n_land && m_bird.n_life && !m_bird.m_life))
            {

                    m_bird.slideBack();
                    m_bird.sildeBackN();

                    phaseToScene();
                    m_bird.updatePosition();
                    m_map.slideBack();
                    m_Stage.updatePosition();


                    for(int i = 0;i<PHASE_NUM;i++)
                    {
                        if(m_Phases[i].m_free == false)
                        {
                            m_Phases[i].slideBack();
                        }
                    }

            }
            if(m_bird.m_X < 0.3 * GAME_WIDTH && m_bird.n_X < 0.3 * GAME_WIDTH)
            {
                slide = false;
            }
        }


        update();
    });







}

void Widget::updatePosition()
{
    m_bird.updatePosition();
    if(m_bird.m_life)
    {
        land();
    }


    m_bird.updatePositionN();
    if(m_bird.n_life)
    {
        landN();
    }




    if(m_bird.m_free == false || m_bird.n_free == false)
    {
        if(m_bird.m_life || m_bird.n_life)
        {
            m_map.mapPosition();

            m_Stage.updatePosition();

            for(int i = 0;i<PHASE_NUM;i++)
            {
                if(m_Phases[i].m_free == false)
                {
                    m_Phases[i].updatePosition();
                }
            }


        }
    }

    if(m_bird.m_X > 0.6 * GAME_WIDTH || m_bird.n_X > 0.6 * GAME_WIDTH)
    {
        slide = true;
    }



}



void Widget::phaseToScene()
{
    m_recorder++;
    if(m_recorder < PHASE_INTERVAL)
    {
        return;
    }

    m_recorder = 0;

    for(int i = 0;i < PHASE_NUM;i++)
    {
        if(m_Phases[i].m_free)
        {
            m_Phases[i].m_free = false;

            m_Phases[i].m_X = cnt + 200 + rand()%300;
            m_Phases[i].m_Y = rand()%350 + 500;

            cnt = m_Phases[i].m_X;

            m_Phases[i].m_Rect.moveTo(m_Phases[i].m_X,m_Phases[i].m_Y);

            break;
        }
    }
}

void Widget::land()
{
    if(m_bird.m_Rect.intersects(m_Stage.m_Rect))
    {
        m_bird.setPosition(m_bird.m_X,m_Stage.m_Y-m_bird.m_Rect.height()-1);
        m_bird.v_Y = 0;
        m_bird.m_free = true;
        m_bird.m_limit = false;
        m_bird.m_land = true;
    }


    for(int i = 0;i<PHASE_NUM;i++)
    {
        if(m_Phases[i].m_free)
        {
            continue;
        }

        if(m_bird.m_Rect.intersects(m_Phases[i].m_Rect))
        {
            if(m_bird.m_Y > m_Phases[i].m_Y + m_bird.m_Bird.height()*0.3)
            {
                break;
            }


            if(m_bird.m_X + m_bird.m_Bird.width() * 0.7 >= m_Phases[i].m_X && m_bird.m_X+m_bird.m_Bird.width() * 0.3 <= m_Phases[i].m_X +m_Phases[i].m_phase.width())
            {
                m_bird.m_Y = m_Phases[i].m_Y - m_bird.m_Rect.height() - 1 ;
                m_bird.v_Y = 0;
                m_bird.m_free = true;
                m_bird.m_limit = false;
                m_bird.setPosition(m_bird.m_X, m_bird.m_Y);
                m_bird.m_land = true;
                break;
            }
        }

    }


}

void Widget::landN()
{

    if(m_bird.n_Rect.intersects(m_Stage.m_Rect))
    {
        m_bird.setPositionN(m_bird.n_X,m_Stage.m_Y-m_bird.n_Rect.height()-1);
        m_bird.v_n_Y = 0;
        m_bird.n_free = true;
        m_bird.n_limit = false;
        m_bird.n_land = true;
    }


    for(int i = 0;i<PHASE_NUM;i++)
    {
        if(m_Phases[i].m_free)
        {
            continue;
        }

        if(m_bird.n_Rect.intersects(m_Phases[i].m_Rect))
        {
            if(m_bird.n_Y > m_Phases[i].m_Y + m_bird.n_Bird.height()*0.3)
            {
                break;
            }

            if(m_bird.n_X + m_bird.n_Bird.width() * 0.7 >= m_Phases[i].m_X && m_bird.n_X+m_bird.n_Bird.width() * 0.3 <= m_Phases[i].m_X +m_Phases[i].m_phase.width())
            {
                m_bird.n_Y = m_Phases[i].m_Y - m_bird.n_Rect.height() - 1 ;
                m_bird.v_n_Y = 0;
                m_bird.n_free = true;
                m_bird.n_limit = false;
                m_bird.setPositionN(m_bird.n_X, m_bird.n_Y);
                m_bird.n_land = true;
                break;
            }
        }

    }


}

void Widget::recover()
{

    for(int i = 0;i<PHASE_NUM;i++)
    {
        if(m_Phases[i].m_free)
        {
            continue;
        }



            if(m_bird.m_Y + m_bird.m_Rect.height() * 0.8 < m_Phases[i].m_Y && m_bird.m_X + m_bird.m_Bird.width() * 0.7 > m_Phases[i].m_X && m_bird.m_X+m_bird.m_Bird.width() * 0.3 <= m_Phases[i].m_X +m_Phases[i].m_phase.width())
            {
                m_bird.m_Y = m_Phases[i].m_Y - m_bird.m_Rect.height() - 1;
                m_bird.v_Y = 0;
                m_bird.m_free = true;
                m_bird.m_life = true;
                m_bird.m_land = true;
                m_bird.setPosition(m_bird.m_X, m_bird.m_Y);
                break;
            }
    }

}

void Widget::recoverN()
{
    for(int i = 0;i<PHASE_NUM;i++)
    {
        if(m_Phases[i].m_free)
        {
            continue;
        }



            if(m_bird.n_Y + m_bird.n_Rect.height() * 0.8 < m_Phases[i].m_Y && m_bird.n_X + m_bird.n_Bird.width() * 0.7 > m_Phases[i].m_X && m_bird.n_X+m_bird.n_Bird.width() * 0.3 <= m_Phases[i].m_X +m_Phases[i].m_phase.width())
            {
                m_bird.n_Y = m_Phases[i].m_Y - m_bird.n_Rect.height() - 1;
                m_bird.v_n_Y = 0;
                m_bird.n_free = true;
                m_bird.n_life = true;
                m_bird.n_land = true;
                m_bird.setPositionN(m_bird.n_X, m_bird.n_Y);
                break;
            }
    }
}

void Widget::fall()
{
    m_bird.m_land = false;

    if(m_bird.m_X + m_bird.m_Bird.width() * 0.3 >= m_Stage.m_X && m_bird.m_X+m_bird.m_Bird.width() * 0.3 <= m_Stage.m_X +m_Stage.m_stage.width())
    {

        m_bird.m_land = true;
    }

    for(int i = 0;i<PHASE_NUM;i++)
    {
        if(m_Phases[i].m_free)
        {
            continue;
        }

        if(m_bird.m_X + m_bird.m_Bird.width() * 0.3 >= m_Phases[i].m_X && m_bird.m_X+m_bird.m_Bird.width() * 0.3 <= m_Phases[i].m_X +m_Phases[i].m_phase.width())
        {
            m_bird.m_land = true;
            break;
        }


    }

}

void Widget::fallN()
{
    m_bird.n_land = false;

    if(m_bird.n_X + m_bird.n_Bird.width() * 0.3 >= m_Stage.m_X && m_bird.n_X+m_bird.n_Bird.width() * 0.3 <= m_Stage.m_X +m_Stage.m_stage.width())
    {

        m_bird.n_land = true;
    }

    for(int i = 0;i<PHASE_NUM;i++)
    {
        if(m_Phases[i].m_free)
        {
            continue;
        }

        if(m_bird.n_X + m_bird.n_Bird.width() * 0.3 >= m_Phases[i].m_X && m_bird.n_X+m_bird.n_Bird.width() * 0.3 <= m_Phases[i].m_X +m_Phases[i].m_phase.width())
        {
            m_bird.n_land = true;
            break;
        }


    }
}


void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.drawPixmap(m_map.m_map1_posX,m_map.m_map1_posY,m_map.m_map1);
    painter.drawPixmap(m_map.m_map2_posX,m_map.m_map2_posY,m_map.m_map2);
    painter.drawPixmap(m_Stage.m_X,m_Stage.m_Y,m_Stage.m_stage);

    for(int i = 0;i<PHASE_NUM;i++)
    {
        if(m_Phases[i].m_free == false)
        {
            painter.drawPixmap(m_Phases[i].m_X,m_Phases[i].m_Y,m_Phases[i].m_phase);
        }
    }


    painter.drawPixmap(m_bird.m_X,m_bird.m_Y,m_bird.m_Bird);
    painter.drawPixmap(m_bird.n_X,m_bird.n_Y,m_bird.n_Bird);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        time_len = QTime::currentTime();

    }
    if(event->button()==Qt::RightButton)
    {
        time_len_n = QTime::currentTime();
    }


}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{

    if(event->button()==Qt::LeftButton)
    {
        time_lens = time_len.msecsTo(QTime::currentTime());
        if(m_bird.m_life)
        {
            m_bird.jumpSet(time_lens);
        }


    }

    if(event->button()==Qt::RightButton)
    {
        time_lens_n = time_len_n.msecsTo(QTime::currentTime());
        if(m_bird.n_life)
        {
            m_bird.jumpFollow(time_lens_n);
        }


    }
    //bird_1_or_2 *= -1;

}




