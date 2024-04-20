#include "widget.h"
#include "ui_widget.h"

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
    cnt = 200;

    srand((unsigned int)time(NULL));


    m_Phases[0].m_free = false;
    m_Phases[0].m_X = 400 + rand()%100;
    m_Phases[0].m_Y = rand()%350 + 700;
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

        if(m_monkey.m_free == false || m_monkey.n_free == false)
        {
            if(m_monkey.m_life||m_monkey.n_life)
            {
                phaseToScene();

            }
        }


        updatePosition();




        update();

    });

    connect(&m_back, &QTimer::timeout, [=](){
        if(m_monkey.m_free)
        {
            fall();
            if(m_monkey.m_land == false)
            {
                m_monkey.down();
                m_monkey.m_life = false;
            }
        }

        if(m_monkey.n_free)
        {
            fallN();
            if(m_monkey.n_land == false)
            {
                m_monkey.downN();
                m_monkey.n_life = false;
            }
        }


        update();
    });


    connect(&m_dis, &QTimer::timeout,[=](){

        m_monkey.disElastic();
        if(m_monkey.m_life == false)
        {
            recover();
            if(m_monkey.m_land == false)
            {
                m_monkey.down();
            }
        }
        if(m_monkey.n_life == false)
        {
            recoverN();
            if(m_monkey.n_land == false)
            {
                m_monkey.downN();
            }
        }

        if(m_monkey.m_life == false && m_monkey.n_life == false)
        {
            m_monkey.down();
            m_monkey.downN();
        }

        update();

    });

    connect(&m_slide, &QTimer::timeout,[=](){

        if(slide)
        {
            if((m_monkey.m_free && m_monkey.m_land && m_monkey.n_free && m_monkey.n_land && m_monkey.m_life && m_monkey.n_life) || (m_monkey.m_free && m_monkey.m_land && m_monkey.m_life && !m_monkey.n_life) || (m_monkey.n_free && m_monkey.n_land && m_monkey.n_life && !m_monkey.m_life))
            {

                    m_monkey.slideBack();
                    m_monkey.sildeBackN();

                    phaseToScene();
                    m_monkey.updatePosition();
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
            if(m_monkey.m_X < 0.35 * GAME_WIDTH && m_monkey.n_X < 0.35 * GAME_WIDTH)
            {
                slide = false;
            }
        }


        update();
    });



}

void Widget::updatePosition()
{
    m_monkey.updatePosition();
    if(m_monkey.m_life)
    {
        land();
        landN();
    }


    m_monkey.updatePositionN();
    if(m_monkey.n_life)
    {
        landN();
        land();
    }




    if(m_monkey.m_free == false || m_monkey.n_free == false)
    {
        if(m_monkey.m_life || m_monkey.n_life)
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

    if(m_monkey.m_X > 0.6 * GAME_WIDTH || m_monkey.n_X > 0.6 * GAME_WIDTH)
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

            m_Phases[i].m_X = cnt + 75 + rand()%100;
            m_Phases[i].m_Y = rand()%400 + 500;

            cnt = m_Phases[i].m_X;

            m_Phases[i].m_Rect.moveTo(m_Phases[i].m_X,m_Phases[i].m_Y);

            break;
        }
    }
}

void Widget::land()
{
    if(m_monkey.m_Rect.intersects(m_Stage.m_Rect))
    {
        m_monkey.setPosition(m_monkey.m_X,m_Stage.m_Y-m_monkey.m_Rect.height()-1);
        m_monkey.v_Y = 0;
        m_monkey.m_free = true;
        m_monkey.m_limit = false;
        m_monkey.m_land = true;
    }


    for(int i = 0;i<PHASE_NUM;i++)
    {
        if(m_Phases[i].m_free)
        {
            continue;
        }

        if(m_monkey.m_Rect.intersects(m_Phases[i].m_Rect))
        {
            if(m_monkey.m_Y > m_Phases[i].m_Y - m_monkey.m_Monkey.height()*0.3)
            {
                break;
            }


            if(m_monkey.m_X + m_monkey.m_Monkey.width() * 0.7 >= m_Phases[i].m_X && m_monkey.m_X+m_monkey.m_Monkey.width() * 0.9 <= m_Phases[i].m_X +m_Phases[i].m_phase.width())
            {
                m_monkey.m_Y = m_Phases[i].m_Y - m_monkey.m_Rect.height() - 1 ;
                m_monkey.v_Y = 0;
                m_monkey.m_free = true;
                m_monkey.m_limit = false;
                m_monkey.setPosition(m_monkey.m_X, m_monkey.m_Y);
                m_monkey.m_land = true;
                break;
            }
        }

    }


}

void Widget::landN()
{

    if(m_monkey.n_Rect.intersects(m_Stage.m_Rect))
    {
        m_monkey.setPositionN(m_monkey.n_X,m_Stage.m_Y-m_monkey.n_Rect.height()-1);
        m_monkey.v_n_Y = 0;
        m_monkey.n_free = true;
        m_monkey.n_limit = false;
        m_monkey.n_land = true;
    }


    for(int i = 0;i<PHASE_NUM;i++)
    {
        if(m_Phases[i].m_free)
        {
            continue;
        }

        if(m_monkey.n_Rect.intersects(m_Phases[i].m_Rect))
        {
            if(m_monkey.n_Y > m_Phases[i].m_Y - m_monkey.n_Monkey.height()*0.3)
            {
                break;
            }

            if(m_monkey.n_X + m_monkey.n_Monkey.width() * 0.7 >= m_Phases[i].m_X && m_monkey.n_X+m_monkey.n_Monkey.width() * 0.9 <= m_Phases[i].m_X +m_Phases[i].m_phase.width())
            {
                m_monkey.n_Y = m_Phases[i].m_Y - m_monkey.n_Rect.height() - 1 ;
                m_monkey.v_n_Y = 0;
                m_monkey.n_free = true;
                m_monkey.n_limit = false;
                m_monkey.setPositionN(m_monkey.n_X, m_monkey.n_Y);
                m_monkey.n_land = true;
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



            if(m_monkey.m_Y + m_monkey.m_Rect.height() * 0.8 < m_Phases[i].m_Y && m_monkey.m_X + m_monkey.m_Monkey.width() * 0.7 > m_Phases[i].m_X && m_monkey.m_X+m_monkey.m_Monkey.width() * 0.3 <= m_Phases[i].m_X +m_Phases[i].m_phase.width())
            {
                m_monkey.m_Y = m_Phases[i].m_Y - m_monkey.m_Rect.height() - 1;
                m_monkey.v_Y = 0;
                m_monkey.m_free = true;
                m_monkey.m_life = true;
                m_monkey.m_land = true;
                m_monkey.setPosition(m_monkey.m_X, m_monkey.m_Y);
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



            if(m_monkey.n_Y + m_monkey.n_Rect.height() * 0.8 < m_Phases[i].m_Y && m_monkey.n_X + m_monkey.n_Monkey.width() * 0.7 > m_Phases[i].m_X && m_monkey.n_X+m_monkey.n_Monkey.width() * 0.3 <= m_Phases[i].m_X +m_Phases[i].m_phase.width())
            {
                m_monkey.n_Y = m_Phases[i].m_Y - m_monkey.n_Rect.height() - 1;
                m_monkey.v_n_Y = 0;
                m_monkey.n_free = true;
                m_monkey.n_life = true;
                m_monkey.n_land = true;
                m_monkey.setPositionN(m_monkey.n_X, m_monkey.n_Y);
                break;
            }
    }
}

void Widget::fall()
{
    m_monkey.m_land = false;

    if(m_monkey.m_X + m_monkey.m_Monkey.width() * 0.3 >= m_Stage.m_X && m_monkey.m_X+m_monkey.m_Monkey.width() * 0.3 <= m_Stage.m_X +m_Stage.m_stage.width())
    {

        m_monkey.m_land = true;
    }

    for(int i = 0;i<PHASE_NUM;i++)
    {
        if(m_Phases[i].m_free)
        {
            continue;
        }

        if(m_monkey.m_X + m_monkey.m_Monkey.width() * 0.3 >= m_Phases[i].m_X && m_monkey.m_X+m_monkey.m_Monkey.width() * 0.3 <= m_Phases[i].m_X +m_Phases[i].m_phase.width())
        {
            m_monkey.m_land = true;
            break;
        }


    }

}

void Widget::fallN()
{
    m_monkey.n_land = false;

    if(m_monkey.n_X + m_monkey.n_Monkey.width() * 0.3 >= m_Stage.m_X && m_monkey.n_X+m_monkey.n_Monkey.width() * 0.3 <= m_Stage.m_X +m_Stage.m_stage.width())
    {

        m_monkey.n_land = true;
    }

    for(int i = 0;i<PHASE_NUM;i++)
    {
        if(m_Phases[i].m_free)
        {
            continue;
        }

        if(m_monkey.n_X + m_monkey.n_Monkey.width() * 0.3 >= m_Phases[i].m_X && m_monkey.n_X+m_monkey.n_Monkey.width() * 0.3 <= m_Phases[i].m_X +m_Phases[i].m_phase.width())
        {
            m_monkey.n_land = true;
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


    painter.drawPixmap(m_monkey.m_X,m_monkey.m_Y,m_monkey.m_Monkey);
    painter.drawPixmap(m_monkey.n_X,m_monkey.n_Y,m_monkey.n_Monkey);


    // 曲线上的点
    QList<QPointF> points = QList<QPointF>() << QPointF(m_monkey.m_X+m_monkey.m_Monkey.width()*0.1, m_monkey.m_Y-m_monkey.m_Monkey.height()*0.8)
                                                << QPointF(m_monkey.m_X*0.5+m_monkey.n_X*0.5, (m_monkey.m_Y+m_monkey.n_Y)*0.5+30)
                                                << QPointF(m_monkey.n_X+m_monkey.n_Monkey.width()*0.1, m_monkey.n_Y-m_monkey.n_Monkey.height()*0.8);
    QPainterPath path(points[0]);
    for (int i = 0; i < points.size() - 1; ++i) {
        QPointF sp = points[i];
        QPointF ep = points[i+1];
        QPointF c1 = QPointF((sp.x() + ep.x()) / 2, sp.y());
        QPointF c2 = QPointF((sp.x() + ep.x()) / 2, ep.y());
        path.cubicTo(c1, c2, ep);
    }
    //设置渲染提示为消除锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);
    //设置画笔颜色和宽度
    painter.setPen(QPen(Qt::green, 5));
    //将坐标系转换为矢量
    painter.translate(40, 130);
    //绘制path
    painter.drawPath(path);



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
        if(m_monkey.m_life)
        {
            m_monkey.jumpSet(time_lens);
        }


    }

    if(event->button()==Qt::RightButton)
    {
        time_lens_n = time_len_n.msecsTo(QTime::currentTime());
        if(m_monkey.n_life)
        {
            m_monkey.jumpFollow(time_lens_n);
        }


    }

}
