#include <math.h>

#include "macro.h"

#define ETA 0.7

double input_layer[N*N];
double competition_layer[N*N];
double output_layer;

double y_i;
double theta_i;
double x_ji[N*N];
double w_ji[N*N];
double y_j[N*N];
double theta_j[N*N];
double x_kj[N*N][N*N];
double w_kj[N*N][N*N];
double d_i;

// 初始化神经元函数。将神经元初始值置为0。
// 输入：void
// 返回值：void
void init_neuron()
{
    for(int i=0;i<N*N;i++)
    {
        input_layer[i] = 0.0;
        competition_layer[i] = 0.0;
    }
    output_layer = 0.0;
}

// 初始化权值函数。将各个权值随机的初始化为接近于0的浮点数。
// 输入：void
// 返回值：void
void init_weight()
{
    srand((int)time(0));
    for(int i=0;i<N*N;i++)
    {
        w_ji[i] = (double)rand()/RAND_MAX;
    }
    for(int i=0;i<N*N;i++)
    {
        for(int j=0;j<N*N;j++)
        {
            w_kj[i][j] = (double)rand()/RAND_MAX;
        }
    }
    return ;
}

// 激活函数。采用双曲正切函数。
// 输入：整合函数值（整合函数为加权求和）。
// 返回值：激活函数值。
double logistic_S(double theta)
{
    return (1.0-exp(-theta))/(1.0+exp(-theta));
}

// 调整权值函数。进行不超过最大迭代次数的正向/反向传播过程，不断调整权值。如果误差小于允许值则结束。
// 输入：一组学习样本（代表棋盘的16维向量以及对应倒推值）。
// 返回值：void
void adapt_BP_net_weight(int * chess_board,int backed_up_value)
{
    int iteration_times = 1000000;
    int temp_iteration_times = 0;
    while(1)
    {
        init_neuron();
        // 开始正向传播
        for(int i=0;i<N*N;i++)
        {
            input_layer[i] = (double)chess_board[i];
        }
        for(int k=0;k<N*N;k++)
        {
            for(int j=0;j<N*N;j++)
            {
                x_kj[k][j] = input_layer[k];
                competition_layer[j] += w_kj[k][j]*x_kj[k][j];
                theta_j[j] = competition_layer[j];
            }
        }
        for(int j=0;j<N*N;j++)
        {
            y_j[j] = logistic_S(competition_layer[j]);
            x_ji[j] = y_j[j];
            output_layer += w_ji[j]*x_ji[j];
            theta_i = output_layer;
        }
        y_i = logistic_S(theta_i);

        // 如果达到允许的最小误差或最大迭代次数，退出并结束本次学习
        if(fabs(y_i-(double)backed_up_value/(double)MAX)<0.000001||temp_iteration_times>=iteration_times)
            break;

        // 开始反向传播
        for(int j=0;j<N*N;j++)
        {
            w_ji[j] = w_ji[j] - ETA*x_ji[j]*(1-y_i*y_i)*(y_i-(double)backed_up_value/(double)MAX);
        }
        for(int k=0;k<N*N;k++)
        {
            for(int j=0;j<N;j++)
            {
                w_kj[k][j] = w_kj[k][j] - ETA*x_kj[k][j]*(1-y_j[j]*y_j[j])*w_ji[j]*(1-y_i*y_i)*(y_i-(double)backed_up_value/(double)MAX);
            }
        }
        temp_iteration_times ++;
    }
    return ;
}

// BP神经网络应用函数。输入棋盘，立即得到其倒推值。
// 输入：当前棋盘状态。
// 返回值：神经网络输出值（需要乘以MAX才等于倒推值）。
double BP_net_value(int * chess_board)
{
    init_neuron();
    for(int i=0;i<N*N;i++)
    {
        input_layer[i] = (double)chess_board[i];
    }
    for(int k=0;k<N*N;k++)
    {
        for(int j=0;j<N*N;j++)
        {
            x_kj[k][j] = input_layer[k];
            competition_layer[j] += w_kj[k][j]*x_kj[k][j];
            theta_j[j] = competition_layer[j];
        }
    }
    for(int j=0;j<N*N;j++)
    {
        y_j[j] = logistic_S(competition_layer[j]);
        x_ji[j] = y_j[j];
        output_layer += w_ji[j]*x_ji[j];
        theta_i = output_layer;
    }
    y_i = logistic_S(theta_i);

    return y_i;
}
