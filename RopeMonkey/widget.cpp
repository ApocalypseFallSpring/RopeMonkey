#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    initScence();

    ui->replayButton->setEnabled(false);
    ui->replayButton->setVisible(false);
    connect(ui->startButton,SIGNAL(clicked()),ui->startButton,SLOT(hide()));
    connect(ui->startButton, &QPushButton::clicked, [&]() {
            playGame();
            start = true;
            ui->startButton->setEnabled(false);
            ui->label_3->setVisible(false);
            ui->label_4->setVisible(false);
        });

    goldIcon.load(GOLD_ICON_PATH);
    ui->label->setPixmap(goldIcon);
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber->setStyleSheet("background:transparent;color:#000000;");
    ui->marklcd->setSegmentStyle(QLCDNumber::Flat);
    ui->marklcd->setStyleSheet("background:transparent;color:#000000;");


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

    monkey_turn = -1;

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

        ui->lcdNumber->display(goldcnt);
        ui->marklcd->display(distance);

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
                m_monkey.m_life = false;
            }
        }
        if(m_monkey.n_life == false)
        {
            recoverN();
            if(m_monkey.n_land == false)
            {
                m_monkey.downN();
                m_monkey.n_life = false;
            }
        }

        if(m_monkey.m_life == false && m_monkey.n_life == false)
        {
            m_monkey.down();
            m_monkey.downN();
            ui->replayButton->setEnabled(true);
            ui->replayButton->setVisible(true);
            connect(ui->replayButton, &QPushButton::clicked, [&]() {
                ui->replayButton->setEnabled(false);
                ui->replayButton->setVisible(false);
                replay();
                });
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
                    if(m_monkey.m_life)
                    {
                        m_monkey.m_land = true;
                    }
                    if(m_monkey.n_life)
                    {
                        m_monkey.n_land = true;
                    }
                    phaseToScene();
                    m_monkey.updatePosition();
                    m_map.slideBack();
                    m_Stage.slideBack();




                    for(int i = 0;i<PHASE_NUM;i++)
                    {
                        if(m_Phases[i].m_free == false)
                        {
                            m_Phases[i].slideBack();
                        }
                    }

            }
            if(m_monkey.m_X < 0.4 * GAME_WIDTH && m_monkey.n_X < 0.4 * GAME_WIDTH)
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
        getGold();
        instantOver();
    }


    m_monkey.updatePositionN();
    if(m_monkey.n_life)
    {
        landN();
        land();
        getGold();
        instantOver();
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

            m_Phases[i].gold = (rand()%2)*(rand()%2);
            if(m_Phases[i].gold)
            {
                m_Phases[i].g_X = m_Phases[i].m_X + (m_Phases[i].m_phase.width()-m_Phases[i].m_gold.width())*0.5;
                m_Phases[i].g_Y = m_Phases[i].m_Y - m_Phases[i].m_gold.height()*1.5;
                m_Phases[i].g_Rect.moveTo(m_Phases[i].g_X,m_Phases[i].g_Y);
            }

            m_Phases[i].flower = rand()%2;
            m_Phases[i].flower *= rand()%2;
            m_Phases[i].flower *= rand()%2;
            if(!m_Phases[i].gold && m_Phases[i].flower)
            {
                m_Phases[i].f_X = m_Phases[i].m_X + (m_Phases[i].m_phase.width()-m_Phases[i].m_flower.width());
                m_Phases[i].f_Y = m_Phases[i].m_Y - m_Phases[i].m_flower.height();
                m_Phases[i].f_Rect.moveTo(m_Phases[i].f_X,m_Phases[i].f_Y);
            }

            break;
        }
    }
}

void Widget::getGold()
{
    for(int i = 0;i<PHASE_NUM;i++)
    {
        if(m_Phases[i].m_free || m_Phases[i].gold == false)
        {
            continue;
        }

        if(m_monkey.m_Rect.intersects(m_Phases[i].g_Rect) || m_monkey.n_Rect.intersects(m_Phases[i].g_Rect))
        {
            m_Phases[i].gold = false;
            goldcnt++;
        }


    }
}

void Widget::instantOver()
{
    for(int i = 0;i<PHASE_NUM;i++)
    {
        if(m_Phases[i].m_free || m_Phases[i].flower == false)
        {
            continue;
        }

        if(m_monkey.m_Rect.intersects(m_Phases[i].f_Rect) || m_monkey.n_Rect.intersects(m_Phases[i].f_Rect))
        {
            if((m_monkey.m_X + m_monkey.m_Monkey.width() > m_Phases[i].f_X + 20 && m_monkey.m_X + 20 < m_Phases[i].f_X + 40 && m_monkey.m_Y + m_monkey.m_Monkey.height() > m_Phases[i].f_Y + 20)|| (m_monkey.n_X + m_monkey.n_Monkey.width() > m_Phases[i].f_X + 20 && m_monkey.n_X + 20 < m_Phases[i].f_X + 40 &&m_monkey.n_Y + m_monkey.n_Monkey.height() > m_Phases[i].f_Y + 20))
            {
                m_monkey.m_life = false;
                m_monkey.n_life = false;
                ui->replayButton->setEnabled(true);
                ui->replayButton->setVisible(true);
                connect(ui->replayButton, &QPushButton::clicked, [&]() {
                    ui->replayButton->setEnabled(false);
                    ui->replayButton->setVisible(false);
                    replay();
                    });
            }

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


            if(m_monkey.m_X + m_monkey.m_Monkey.width() >= m_Phases[i].m_X && m_monkey.m_X+m_monkey.m_Monkey.width() * 0.9 <= m_Phases[i].m_X +m_Phases[i].m_phase.width())
            {
                m_monkey.m_Y = m_Phases[i].m_Y - m_monkey.m_Rect.height() - 1 ;
                m_monkey.v_Y = 0;
                m_monkey.m_free = true;
                m_monkey.m_limit = false;
                m_monkey.setPosition(m_monkey.m_X, m_monkey.m_Y);
                m_monkey.m_land = true;
                distance += m_monkey.m_X%100;
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

            if(m_monkey.n_X + m_monkey.n_Monkey.width() >= m_Phases[i].m_X && m_monkey.n_X+m_monkey.n_Monkey.width() * 0.9 <= m_Phases[i].m_X +m_Phases[i].m_phase.width())
            {
                m_monkey.n_Y = m_Phases[i].m_Y - m_monkey.n_Rect.height() - 1 ;
                m_monkey.v_n_Y = 0;
                m_monkey.n_free = true;
                m_monkey.n_limit = false;
                m_monkey.setPositionN(m_monkey.n_X, m_monkey.n_Y);
                m_monkey.n_land = true;
                distance += m_monkey.n_X%100;
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
                monkey_turn *= -1;
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
                monkey_turn *= -1;
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
    painter.drawPixmap(m_Stage.m_h_X,m_Stage.m_h_Y,m_Stage.m_house);

    for(int i = 0;i<PHASE_NUM;i++)
    {
        if(m_Phases[i].m_free == false)
        {
            painter.drawPixmap(m_Phases[i].m_X,m_Phases[i].m_Y,m_Phases[i].m_phase);
            if(m_Phases[i].gold)
            {
                painter.drawPixmap(m_Phases[i].g_X,m_Phases[i].g_Y,m_Phases[i].m_gold);
            }
            if(m_Phases[i].flower)
            {
                painter.drawPixmap(m_Phases[i].f_X,m_Phases[i].f_Y,m_Phases[i].m_flower);
            }
        }
    }

    if(m_monkey.m_free)
    {
        m_monkey.m_Monkey.load(MONKEY_PATH);
        painter.drawPixmap(m_monkey.m_X,m_monkey.m_Y,m_monkey.m_Monkey);
    }
    if(!m_monkey.m_free)
    {
        m_monkey.m_Monkey.load(MONKEY_PATH_JUMP);
        painter.drawPixmap(m_monkey.m_X,m_monkey.m_Y,m_monkey.m_Monkey);
    }

    if(m_monkey.n_free)
    {
        m_monkey.n_Monkey.load(MONKEY_NPATH);
        painter.drawPixmap(m_monkey.n_X,m_monkey.n_Y,m_monkey.n_Monkey);
    }
    if(!m_monkey.n_free)
    {
        m_monkey.n_Monkey.load(MONKEY_NPATH_JUMP);
        painter.drawPixmap(m_monkey.n_X,m_monkey.n_Y,m_monkey.n_Monkey);
    }


    // 曲线上的点
    QList<QPointF> points = QList<QPointF>() << QPointF(m_monkey.m_X+m_monkey.m_Monkey.width()*0.1, m_monkey.m_Y-m_monkey.m_Monkey.height()*0.8)
                                                << QPointF(m_monkey.m_X*0.5+m_monkey.n_X*0.5, (m_monkey.m_Y+m_monkey.n_Y)*0.5+20)
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


void Widget::mousePressEvent(QMouseEvent *)
{
    time_len = QTime::currentTime();

}

void Widget::mouseReleaseEvent(QMouseEvent *)
{

    if(monkey_turn == 1 && m_monkey.m_life)
    {
        time_lens = time_len.msecsTo(QTime::currentTime());
        if(start)
        {
            m_monkey.jumpSet(time_lens);
            monkey_turn *= -1;

        }
        if(m_monkey.n_life == false)
        {
            monkey_turn *= -1;
        }
    }
    else if(monkey_turn == -1 && m_monkey.n_life)
    {
        time_lens = time_len.msecsTo(QTime::currentTime());
        if(start)
        {
            m_monkey.jumpFollow(time_lens);
            monkey_turn *= -1;

        }
        if(m_monkey.m_life == false)
        {
            monkey_turn *= -1;
        }
    }
    else if(monkey_turn == 1 && !m_monkey.m_life)
    {
        monkey_turn = -1;
        time_lens = time_len.msecsTo(QTime::currentTime());
        if(start)
        {
            m_monkey.jumpFollow(time_lens);
            monkey_turn *= -1;

        }
        if(m_monkey.m_life == false)
        {
            monkey_turn *= -1;
        }
    }
    else if(monkey_turn == -1 && !m_monkey.n_life)
    {
        monkey_turn = 1;
        time_lens = time_len.msecsTo(QTime::currentTime());
        if(start)
        {
            m_monkey.jumpSet(time_lens);
            monkey_turn *= -1;

        }
        if(m_monkey.n_life == false)
        {
            monkey_turn *= -1;
        }
    }

}

void Widget::replay()
{
    m_monkey.m_X = 200;
    m_monkey.m_Y = 700;
    m_monkey.v_X = 0;
    m_monkey.v_Y = 0;

    m_monkey.m_free = true;
    m_monkey.m_limit = false;
    m_monkey.m_life = true;

    m_monkey.m_Rect.moveTo(m_monkey.m_X,m_monkey.m_Y);


    m_monkey.n_X = 225;
    m_monkey.n_Y = 700;
    m_monkey.v_n_X = 0;
    m_monkey.v_n_Y = 0;

    m_monkey.n_free = true;
    m_monkey.n_limit = false;
    m_monkey.n_life = true;

    m_monkey.n_Rect.moveTo(m_monkey.n_X,m_monkey.n_Y);

    m_monkey.m_land = true;
    m_monkey.n_land = true;


    m_Stage.m_X = 150;
    m_Stage.m_Y = 784;
    m_Stage.m_h_X = 140;
    m_Stage.m_h_Y = 604;

    m_Stage.m_Rect.moveTo(m_Stage.m_X,m_Stage.m_Y);

    for(int i = 0;i < PHASE_NUM;i++)
    {
        m_Phases[i].m_free = true;
        m_Phases[i].gold = false;
        m_Phases[i].flower = false;
    }
    m_Phases[0].m_free = false;
    m_Phases[0].m_X = 400 + rand()%100;
    m_Phases[0].m_Y = rand()%350 + 700;
    cnt = m_Phases[0].m_X;
    m_Phases[0].m_Rect.moveTo(m_Phases[0].m_X,m_Phases[0].m_Y);

    goldcnt = 0;
    distance = 0;

    monkey_turn = -1;

}

void Widget::on_pushButton_clicked(){};
void Widget::on_startButton_clicked(){};
