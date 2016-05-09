#ifndef FILE_TIMER_H
#define FILE_TIMER_H

float init_time; //初始间隔时间
#define time_shrink_rate 0.75  //蛇越长，前进速度越快，通过这个比率修改间隔时间以实现之

void timer_handler(int sig); //定时器事件处理函数
void init_sigaction(void (* hdler)(int)); //初始化信号处理结构，参数为事件处理函数
void init_timer(float t); //初始化定时器，参数为间隔时间

#endif
