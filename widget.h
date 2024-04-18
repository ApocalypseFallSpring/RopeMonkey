#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QIcon>
#include <QPainter>
#include "map.h"
#include <QTimer>
#include "bird.h"
#include <QMouseEvent>
#include <QTime>
#include <QKeyEvent>
#include "phase.h"
#include <ctime>
#include "stage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


    Map m_map;

    Bird m_bird;

    Phase m_Phases[PHASE_NUM];
    int m_recorder;
    int cnt;

    stage m_Stage;

    QTimer m_timer;
    QTimer m_back;
    QTimer m_dis;
    QTimer m_slide;

    bool slide = false;

    bool flag1 = false;
    bool flag2 = false;



    void initScence();

    void playGame();

    void updatePosition();

    void landSuccess();
    void landSuccessN();

    void phaseToScene();

    void land();
    void landN();

    void recover();
    void recoverN();

    void fall();
    void fallN();



    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    //void mouseMoveEvent(QMouseEvent *);
    QTime time_len;
    int time_lens;

    //void keyPressEvent(QKeyEvent *);
    //void keyReleaseEvent(QKeyEvent *);
    QTime time_len_n;
    int time_lens_n;

    int bird_1_or_2 = -1;




private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
