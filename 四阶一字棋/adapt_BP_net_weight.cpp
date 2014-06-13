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

// ��ʼ����Ԫ����������Ԫ��ʼֵ��Ϊ0��
// ���룺void
// ����ֵ��void
void init_neuron()
{
    for(int i=0;i<N*N;i++)
    {
        input_layer[i] = 0.0;
        competition_layer[i] = 0.0;
    }
    output_layer = 0.0;
}

// ��ʼ��Ȩֵ������������Ȩֵ����ĳ�ʼ��Ϊ�ӽ���0�ĸ�������
// ���룺void
// ����ֵ��void
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

// �����������˫�����к�����
// ���룺���Ϻ���ֵ�����Ϻ���Ϊ��Ȩ��ͣ���
// ����ֵ�������ֵ��
double logistic_S(double theta)
{
    return (1.0-exp(-theta))/(1.0+exp(-theta));
}

// ����Ȩֵ���������в���������������������/���򴫲����̣����ϵ���Ȩֵ��������С������ֵ�������
// ���룺һ��ѧϰ�������������̵�16ά�����Լ���Ӧ����ֵ����
// ����ֵ��void
void adapt_BP_net_weight(int * chess_board,int backed_up_value)
{
    int iteration_times = 1000000;
    int temp_iteration_times = 0;
    while(1)
    {
        init_neuron();
        // ��ʼ���򴫲�
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

        // ����ﵽ�������С�����������������˳�����������ѧϰ
        if(fabs(y_i-(double)backed_up_value/(double)MAX)<0.000001||temp_iteration_times>=iteration_times)
            break;

        // ��ʼ���򴫲�
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

// BP������Ӧ�ú������������̣������õ��䵹��ֵ��
// ���룺��ǰ����״̬��
// ����ֵ�����������ֵ����Ҫ����MAX�ŵ��ڵ���ֵ����
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
