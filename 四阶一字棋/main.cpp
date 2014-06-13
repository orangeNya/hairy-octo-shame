#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "adapt_BP_net_weight.cpp"

int chess_board[N*N];

typedef enum{A=1, B=-1}piece;

// ��ʼ�����̺�������������Ϊ�ա�
// ���룺void
// ����ֵ��void
void init_chess_board()
{
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            chess_board[i*N+j] = 0;
        }
    }
    return ;
}

// ��ӡ���̺�����
// ����:void
// ����ֵ��void
void print_chess_board(int * chess_board)
{
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(j<N-1)
                printf("%d ",chess_board[i*N+j]);
            else
                printf("%d\n",chess_board[i*N+j]);
        }
    }
    return ;
}

// ������(x,y)���������ӡ���������Ϊö��piece��{A,B}��
// ���룺���̡�x��y���������͡�
// ����ֵ��void
void place_piece_on_chess_board(int * chess_board,int x,int y,piece type)
{
    chess_board[x*N+y] = type;
    return ;
}

// ���Ե�ǰ���˭Ӯ��
// ���룺���̡�
// ����ֵ��Ϊ0��ʾA��B��ûӮ��Ϊ1��ʾAʤ��Ϊ-1��ʾBʤ��
int test_who_win(int * chess_board)
{
    // �ֱ����һ�С�һ�С������Խ�������һ����������µ���Ӯ
    int i, j;
    for(i=0;i<N;i++)
    {
        bool check_row_flag = true;
        for(j=0;j<N-1;j++)
        {
            if(chess_board[i*N+j]!=chess_board[i*N+j+1])
            {
                check_row_flag = false;
                break;
            }
        }
        if(check_row_flag&&chess_board[i*N+j]!=0)
            return chess_board[i*N+j];
    }
    for(j=0;j<N;j++)
    {
        bool check_line_flag = true;
        for(i=0;i<N-1;i++)
        {
            if(chess_board[i*N+j]!=chess_board[(i+1)*N+j])
            {
                check_line_flag = false;
                break;
            }
        }
        if(check_line_flag&&chess_board[i*N+j]!=0)
            return chess_board[i*N+j];
    }
    bool check_diagonal_flag = true;
    for(i=0;i<N-1;i++)
    {
        if(chess_board[i*N+i]!=chess_board[(i+1)*N+i+1])
        {
            check_diagonal_flag = false;
            break;
        }
    }
    if(check_diagonal_flag&&chess_board[i*N+i]!=0)
        return chess_board[i*N+i];
    check_diagonal_flag = true;
    for(i=0;i<N-1;i++)
    {
        if(chess_board[i*N+(N-1-i)]!=chess_board[(i+1)*N+(N-1-(i+1))])
        {
            check_diagonal_flag = false;
            break;
        }
    }
    if(check_diagonal_flag&&chess_board[i*N+(N-1-i)]!=0)
        return chess_board[i*N+(N-1-i)];
    return 0;
}

// ���Ե�ǰ���̣���������㣩���ڼ������С��㡣
// ���룺���̡�
// ����ֵ��Ϊ1��ʾ��ǰΪ�����㣬Ϊ-1��ʾ��ǰΪ��С��㡣
int test_node_type(int * chess_board)
{
    // �����õ�ǰ˫�������������жϼ���/��С���
    int count_of_A = 0, count_of_B = 0;
    for(int i=0;i<N*N;i++)
    {
        if(chess_board[i]==A)
            count_of_A ++;
        else if(chess_board[i]==B)
            count_of_B ++;
        else
            ;
    }
    if(count_of_A>count_of_B)
        return -1;
    else
        return 1;
}

// �������������Ƿ�ȼۣ�������ת��ԳƱ任ȫ�ȣ��������ĺ�����
// ���룺��������������
// ����ֵ��true��ʾ��ȣ�false��ʾ���ȡ�
bool test_if_chess_boards_equal(int * chess_board1,int * chess_board2)
{
    int temp_chess_board[N*N];
    bool same_flag = true;
    for(int i=0;i<4;i++)
    {
        int rotation_operator[2][2] = {{(int)cos(M_PI_2*i), (int)-sin(M_PI_2*i)},
                                       {(int)sin(M_PI_2*i), (int)cos(M_PI_2*i)}};
        for(int k=0;k<N;k++)
        {
            for(int h=0;h<N;h++)
            {
                int temp_k = (k*2-(N-1)) * rotation_operator[0][0] + (h*2-(N-1)) * rotation_operator[1][0];
                int temp_h = (k*2-(N-1)) * rotation_operator[0][1] + (h*2-(N-1)) * rotation_operator[1][1];
                temp_k = (temp_k + (N-1)) / 2;
                temp_h = (temp_h + (N-1)) / 2;
                temp_chess_board[temp_k*N+temp_h] = chess_board2[k*N+h];
            }
        }
        same_flag = true;
        for(int k=0;k<N*N;k++)
        {
            if(chess_board1[k]!=temp_chess_board[k])
            {
                same_flag = false;
                break;
            }
        }
        if(same_flag)
            return true;
    }

    same_flag = true;
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(chess_board1[i*N+j]!=chess_board2[(N-1-i)*N+j])
            {
                same_flag = false;
                break;
            }
        }
    }
    if(same_flag)
        return true;

    same_flag = true;
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(chess_board1[i*N+j]!=chess_board2[i*N+(N-1-j)])
            {
                same_flag = false;
                break;
            }
        }
    }
    if(same_flag)
        return true;

    same_flag = true;
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(chess_board1[i*N+j]!=chess_board2[j*N+i])
            {
                same_flag = false;
                break;
            }
        }
    }
    if(same_flag)
        return true;

    same_flag = true;
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(chess_board1[i*N+j]!=chess_board2[(N-1-j)*N+(N-1-i)])
            {
                same_flag = false;
                break;
            }
        }
    }
    if(same_flag)
        return true;

    return false;
}

// ���������Ƿ�������
// ���룺���̡�
// ����ֵ����������true�����򷵻�false��
bool test_if_chess_board_full(int * chess_board)
{
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(chess_board[i*N+j]==0)
                return false;
        }
    }
    return true;
}

// �������̡���chess_board2����chess_board1��
// ���룺��������������
// ����ֵ��void
void copy_chess_board(int * chess_board1, int * chess_board2)
{
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            chess_board1[i*N+j] = chess_board2[i*N+j];
        }
    }
    return ;
}

int evaluation_function(int * );

// ��ʼ���������顣�����������ڦ�-�¼�֦��
// ���룺�������顢��ǰ�ڵ����ͣ������С��㣩��
// ����ֵ��void
void init_ancestor(int * ancestor,int node_type)
{
    // ���뽫������ĳ�ֵ��ΪMIN����СΪMAX���Ա��-�¼�֦ʱ�ܹ��ı����ǵ�ֵ
    if(node_type==1)
    {
        for(int i=0;i<N*N;i++)
        {
            if(i%2==0)
                ancestor[i] = MIN;
            else
                ancestor[i] = MAX;
        }
    }
    else
    {
        for(int i=0;i<N*N;i++)
        {
            if(i%2==0)
                ancestor[i] = MAX;
            else
                ancestor[i] = MIN;
        }
    }
}

// ������ǰ��㵹��ֵ��
// ���룺��ǰ��֡�Ĭ����ȣ�Ĭ��Ϊ0���������ȡ��������顢��ǰ������͡�
// ����ֵ����ֵ���ֵ��
int get_backed_up_value_of_node(int * chess_board,int h,int depth,int * ancestor,int root_type)
{
    if(h==depth||test_if_chess_board_full(chess_board)||test_who_win(chess_board)!=0)
    {
        // �����Ҷ�ӽ�㣬��������ֵ��������������ֵ�У�
        return evaluation_function(chess_board);
    }
    else
    {
        int temp_backed_up_value;
        int max_node_value = MIN, min_node_value = MAX;
        for(int i=0;i<N*N;i++)
        {
            if(chess_board[i]==0)
            {
                int temp_chess_board[N*N];
                copy_chess_board(temp_chess_board,chess_board);
                place_piece_on_chess_board(temp_chess_board,i/N,i%N,(piece)test_node_type(chess_board));

                temp_backed_up_value = get_backed_up_value_of_node(temp_chess_board,h+1,depth,ancestor,root_type);

                if(test_node_type(chess_board)==1)
                {
                    if(temp_backed_up_value>max_node_value)
                        max_node_value = temp_backed_up_value;
                }
                else
                {
                    if(temp_backed_up_value<min_node_value)
                        min_node_value = temp_backed_up_value;
                }

                // ���ݸ����Ƚ��Ħ����ֵ����֦
                bool continue_search = true;
                if(test_node_type(chess_board)==1)
                {
                    for(int j=0;j<h;j++)
                    {
                        if(max_node_value>ancestor[j]&&((j%2!=0&&root_type==1)||(j%2==0&&root_type==-1)))
                        {
                            continue_search = false;
                            break;
                        }
                    }
                }
                else
                {
                    for(int j=0;j<h;j++)
                    {
                        if(min_node_value<ancestor[j]&&((j%2==0&&root_type==1)||(j%2!=0&&root_type==-1)))
                        {
                            continue_search = false;
                            break;
                        }
                    }
                }
                if(continue_search==false)
                    break;
            }
            else
                ;
        }
        // ���Ҷ�ӽ��������󣬸��¸����ȵĦ�-��ֵ
        if(test_node_type(chess_board)==1)
        {
            if(h-1>=0)
            {
                if(root_type==1)
                {
                    if((h-1)%2==0)
                    {
                        if(max_node_value>ancestor[h-1])
                            ancestor[h-1] = max_node_value;
                    }
                    else
                    {
                        if(max_node_value<ancestor[h-1])
                            ancestor[h-1] = max_node_value;
                    }
                }
                else
                {
                    if((h-1)%2==0)
                    {
                        if(max_node_value<ancestor[h-1])
                            ancestor[h-1] = max_node_value;
                    }
                    else
                    {
                        if(max_node_value>ancestor[h-1])
                            ancestor[h-1] = max_node_value;
                    }
                }
            }
            return max_node_value;
        }
        else
        {
            if(h-1>=0)
            {
                if(root_type==-1)
                {
                    if((h-1)%2==0)
                    {
                        if(min_node_value<ancestor[h-1])
                            ancestor[h-1] = min_node_value;
                    }
                    else
                    {
                        if(min_node_value>ancestor[h-1])
                            ancestor[h-1] = min_node_value;
                    }
                }
                else
                {
                    if((h-1)%2==0)
                    {
                        if(min_node_value>ancestor[h-1])
                            ancestor[h-1] = min_node_value;
                    }
                    else
                    {
                        if(min_node_value<ancestor[h-1])
                            ancestor[h-1] = min_node_value;
                    }
                }
            }
            return min_node_value;
        }
    }
}

// ����������
// ���룺��ǰ��֡�
// ����ֵ����ֶ�Ӧ��������ֵ��
int evaluation_function(int * chess_board)
{
    if(test_who_win(chess_board)==1)
        return MAX;
    else if(test_who_win(chess_board)==-1)
        return MIN;
    else
    {
        int temp = BP_net_value(chess_board)*MAX;
        return temp;
        // �����������������֮ǰ����������
        /*int A_win = 0, B_win = 0;
        int temp_chess_board[N*N];
        for(int i=0;i<N;i++)
        {
            for(int j=0;j<N;j++)
            {
                temp_chess_board[i*N+j] = chess_board[i*N+j];
                if(temp_chess_board[i*N+j]!=B)
                    place_piece_on_chess_board(temp_chess_board,i,j,A);
            }
        }
        int i, j;
        for(i=0;i<N;i++)
        {
            bool check_row_flag = true;
            for(j=0;j<N-1;j++)
            {
                if(temp_chess_board[i*N+j]!=temp_chess_board[i*N+j+1])
                {
                    check_row_flag = false;
                    break;
                }
            }
            if(check_row_flag&&temp_chess_board[i*N+j]==A)
                A_win ++;
        }
        for(j=0;j<N;j++)
        {
            bool check_line_flag = true;
            for(i=0;i<N-1;i++)
            {
                if(temp_chess_board[i*N+j]!=temp_chess_board[(i+1)*N+j])
                {
                    check_line_flag = false;
                    break;
                }
            }
            if(check_line_flag&&temp_chess_board[i*N+j]==A)
                A_win ++;
        }
        bool check_diagonal_flag = true;
        for(i=0;i<N-1;i++)
        {
            if(temp_chess_board[i*N+i]!=temp_chess_board[(i+1)*N+i+1])
            {
                check_diagonal_flag = false;
                break;
            }
        }
        if(check_diagonal_flag&&temp_chess_board[i*N+i]==A)
            A_win ++;
        check_diagonal_flag = true;
        for(i=0;i<N-1;i++)
        {
            if(temp_chess_board[i*N+(N-1-i)]!=temp_chess_board[(i+1)*N+(N-1-(i+1))])
            {
                check_diagonal_flag = false;
                break;
            }
        }
        if(check_diagonal_flag&&temp_chess_board[i*N+(N-1-i)]==A)
            A_win ++;

        for(int i=0;i<N;i++)
        {
            for(int j=0;j<N;j++)
            {
                temp_chess_board[i*N+j] = chess_board[i*N+j];
                if(temp_chess_board[i*N+j]!=A)
                    place_piece_on_chess_board(temp_chess_board,i,j,B);
            }
        }
        for(i=0;i<N;i++)
        {
            bool check_row_flag = true;
            for(j=0;j<N-1;j++)
            {
                if(temp_chess_board[i*N+j]!=temp_chess_board[i*N+j+1])
                {
                    check_row_flag = false;
                    break;
                }
            }
            if(check_row_flag&&temp_chess_board[i*N+j]==B)
                B_win ++;
        }
        for(j=0;j<N;j++)
        {
            bool check_line_flag = true;
            for(i=0;i<N-1;i++)
            {
                if(temp_chess_board[i*N+j]!=temp_chess_board[(i+1)*N+j])
                {
                    check_line_flag = false;
                    break;
                }
            }
            if(check_line_flag&&temp_chess_board[i*N+j]==B)
                B_win ++;
        }
        check_diagonal_flag = true;
        for(i=0;i<N-1;i++)
        {
            if(temp_chess_board[i*N+i]!=temp_chess_board[(i+1)*N+i+1])
            {
                check_diagonal_flag = false;
                break;
            }
        }
        if(check_diagonal_flag&&temp_chess_board[i*N+i]==B)
            B_win ++;
        check_diagonal_flag = true;
        for(i=0;i<N-1;i++)
        {
            if(temp_chess_board[i*N+(N-1-i)]!=temp_chess_board[(i+1)*N+(N-1-(i+1))])
            {
                check_diagonal_flag = false;
                break;
            }
        }
        if(check_diagonal_flag&&temp_chess_board[i*N+(N-1-i)]==B)
            B_win ++;
        return A_win - B_win;*/
    }
}

GtkWidget * main_window;
GdkPixbuf * background;
GdkPixbuf * A_pic;
GdkPixbuf * B_pic;
GtkWidget * draw_window;
GtkWidget * about_window;
GtkWidget * about_image;

bool start_flag = false;
int who_first = 0;

int draw_position[N*N][2] = {{50,25},{180,25},{310,25},{440,25},
                             {50,160},{180,160},{310,160},{440,160},
                             {55,300},{190,290},{320,290},{445,290},
                             {65,435},{195,425},{325,420},{445,415}};
int tell_position[(N+1)*2] = {0,178,302,434,627,
                              0,146,284,409,570};

// �������ַ�ת��Ϊ������gtk��ʾ�ı��롣
// ���룺�ַ���
// ����ֵ����������ַ����롣
char *_(const char * str){ return g_locale_to_utf8(str, -1, 0, 0, 0); }

// ��ʾ�Ի�����Ӯ�ˡ�
// ���룺void
// ����ֵ��void
void message_box_you_win()
{
    GtkWidget * dialog = gtk_dialog_new_with_buttons ("YOU WIN",GTK_WINDOW(main_window),GTK_DIALOG_MODAL,"OK",GTK_RESPONSE_OK,NULL);
    gtk_dialog_set_has_separator(GTK_DIALOG (dialog),FALSE);
    GtkWidget * label = gtk_label_new ("YOU WIN!");
    GtkWidget * hbox = gtk_hbox_new (FALSE,5);
    gtk_box_pack_start_defaults(GTK_BOX(hbox),label);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox),hbox);
    gtk_widget_set_size_request(dialog,150,80);
    gtk_window_set_resizable(GTK_WINDOW(dialog),FALSE);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy (dialog);
}

// ��ʾ�Ի��������ˡ�
// ���룺void
// ����ֵ��void
void message_box_you_lose()
{
    GtkWidget * dialog = gtk_dialog_new_with_buttons ("YOU LOSE",GTK_WINDOW(main_window),GTK_DIALOG_MODAL,"OK",GTK_RESPONSE_OK,NULL);
    gtk_dialog_set_has_separator(GTK_DIALOG (dialog),FALSE);
    GtkWidget * label = gtk_label_new ("YOU LOSE!");
    GtkWidget * hbox = gtk_hbox_new (FALSE,5);
    gtk_box_pack_start_defaults(GTK_BOX(hbox),label);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox),hbox);
    gtk_widget_set_size_request(dialog,150,80);
    gtk_window_set_resizable(GTK_WINDOW(dialog),FALSE);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy (dialog);
}

// ��ʾ�Ի���ƽ�֡�
// ���룺void
// ����ֵ��void
void message_box_draw()
{
    GtkWidget * dialog = gtk_dialog_new_with_buttons ("DRAW",GTK_WINDOW(main_window),GTK_DIALOG_MODAL,"OK",GTK_RESPONSE_OK,NULL);
    gtk_dialog_set_has_separator(GTK_DIALOG (dialog),FALSE);
    GtkWidget * label = gtk_label_new ("DRAW GAME.");
    GtkWidget * hbox = gtk_hbox_new (FALSE,5);
    gtk_box_pack_start_defaults(GTK_BOX(hbox),label);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox),hbox);
    gtk_widget_set_size_request(dialog,150,80);
    gtk_window_set_resizable(GTK_WINDOW(dialog),FALSE);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy (dialog);
}

// ����������з�ӳ�ļ���������
// ���룺�������ؼ���draw_window�����¼�Դ�����������data��
// ����ֵ��FALSE
static gboolean when_key_pressed(GtkWidget * widget,GdkEventButton * button,gpointer data)
{
    printf("click at (%d,%d)\n",(int)button->x,(int)button->y);
    int i;
    int temp_x = button->x, temp_y = button->y;
    for(i=0;i<N;i++)
    {
        if(temp_x>tell_position[i]&&temp_x<tell_position[i+1])
            break;
    }
    temp_x = i;
    for(i=0;i<N;i++)
    {
        if(temp_y>tell_position[i+(N+1)]&&temp_y<tell_position[i+(N+1)+1])
            break;
    }
    temp_y = i;
    // �������-���Է������ӵĸ��·�ʽ��ÿ����������������������岢���½���
    // ����������֣�������ѡ��computer first���������ɵ������壬֮��Ĳ�����Ȼ�����-��������
    if(start_flag&&chess_board[temp_y*N+temp_x]==0)
    {
        if(who_first==1)
        {
            place_piece_on_chess_board(chess_board,temp_y,temp_x,A);
            gtk_widget_queue_draw(widget);
            if(test_who_win(chess_board)==1)
            {
                start_flag = false;
                message_box_you_win();
            }
            int min = MAX+1;
            int step;
            for(int i=0;i<N*N;i++)
            {
                if(chess_board[i]!=0)
                    continue;
                int temp_chess_board[N*N];
                copy_chess_board(temp_chess_board,chess_board);
                int ancestor[N*N];
                init_ancestor(ancestor,test_node_type(temp_chess_board));
                place_piece_on_chess_board(temp_chess_board,i/N,i%N,B);
                int temp = get_backed_up_value_of_node(temp_chess_board,0,6,ancestor,test_node_type(temp_chess_board));
                printf("temp is %d at (%d,%d)\n",temp,i/N,i%N);
                if(temp<min)
                {
                    min = temp;
                    step = i;
                }
            }
            if(test_who_win(chess_board)==0&&test_if_chess_board_full(chess_board)!=true)
                place_piece_on_chess_board(chess_board,step/N,step%N,B);
            gtk_widget_queue_draw(widget);
            if(test_who_win(chess_board)==-1)
            {
                start_flag = false;
                message_box_you_lose();
            }
            if(test_if_chess_board_full(chess_board)&&test_who_win(chess_board)==0)
            {
                start_flag = false;
                message_box_draw();
            }
        }
        else
        {
            place_piece_on_chess_board(chess_board,temp_y,temp_x,B);
            gtk_widget_queue_draw(widget);
            if(test_who_win(chess_board)==-1)
            {
                start_flag = false;
                message_box_you_win();
            }
            if(test_if_chess_board_full(chess_board)&&test_who_win(chess_board)==0)
            {
                start_flag = false;
                message_box_draw();
            }
            int max = MIN-1;
            int step;
            for(int i=0;i<N*N;i++)
            {
                if(chess_board[i]!=0)
                    continue;
                int temp_chess_board[N*N];
                copy_chess_board(temp_chess_board,chess_board);
                int ancestor[N*N];
                init_ancestor(ancestor,test_node_type(temp_chess_board));
                place_piece_on_chess_board(temp_chess_board,i/N,i%N,A);
                int temp = get_backed_up_value_of_node(temp_chess_board,0,6,ancestor,test_node_type(temp_chess_board));
                printf("temp is %d at (%d,%d)\n",temp,i/N,i%N);
                if(temp>max)
                {
                    max = temp;
                    step = i;
                }
            }
            if(test_who_win(chess_board)==0&&test_if_chess_board_full(chess_board)!=true)
                place_piece_on_chess_board(chess_board,step/N,step%N,A);
            gtk_widget_queue_draw(widget);
            if(test_who_win(chess_board)==1)
            {
                start_flag = false;
                message_box_you_lose();
            }
        }
    }
    return FALSE;
}

// ����chess_board���ϸ��½���ĺ���������ʾ����ʱ�ͱ����ϵ��á�
// ���룺�ؼ�draw_window��
// ����ֵ��void
void draw_main(GtkWidget * widget)
{
    cairo_t * cr = gdk_cairo_create(widget->window);
    gdk_cairo_set_source_pixbuf(cr,background,0,0);
    cairo_paint(cr);
    cairo_fill (cr);

    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(chess_board[i*N+j]==1)
            {
                gdk_cairo_set_source_pixbuf(cr,A_pic,draw_position[i*N+j][0],draw_position[i*N+j][1]);
                cairo_paint(cr);
                cairo_fill (cr);
            }
            if(chess_board[i*N+j]==-1)
            {
                gdk_cairo_set_source_pixbuf(cr,B_pic,draw_position[i*N+j][0],draw_position[i*N+j][1]);
                cairo_paint(cr);
                cairo_fill (cr);
            }
        }
    }

    cairo_destroy(cr);
}

// ���º���������Ƕ��draw_main����Ϊ�����ڱ������в������뻭��cario��
// ���룺�ؼ�����ʾ�¼���data��
// ����ֵ��FALSE
static gboolean window_drawing(GtkWidget * widget,GdkEventExpose * expose,gpointer data)
{
    draw_main(widget);
    return FALSE;
}

// ���player first����õĺ�����
// ���룺void
// ����ֵ��void
void player_first()
{
    init_chess_board();
    start_flag = true;
    who_first = 1;
    gtk_widget_queue_draw(draw_window);
}

// ���computer first����õĺ�����
// ���룺void
// ����ֵ��void
void computer_first()
{
    init_chess_board();
    start_flag = true;
    who_first = -1;

    int max = MIN-1;
    int step;
    for(int i=0;i<N*N;i++)
    {
        int temp_chess_board[N*N];
        copy_chess_board(temp_chess_board,chess_board);
        int ancestor[N*N];
        init_ancestor(ancestor,test_node_type(temp_chess_board));
        place_piece_on_chess_board(temp_chess_board,i/N,i%N,A);
        int temp = get_backed_up_value_of_node(temp_chess_board,0,6,ancestor,test_node_type(temp_chess_board));
        printf("temp is %d at (%d,%d)\n",temp,i/N,i%N);
        if(temp>max)
        {
            max = temp;
            step = i;
        }
    }
    place_piece_on_chess_board(chess_board,step/N,step%N,A);
    gtk_widget_queue_draw(draw_window);
}

// ���about��ť�󵯳��İ�����Ϣ��
// ���룺void
// ����ֵ��void
void about_pressed()
{
    about_image = gtk_image_new_from_file("res/sample.jpg");

    about_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(about_window,627,570);
    gtk_window_set_resizable(GTK_WINDOW(about_window),FALSE);
    gtk_window_set_title(GTK_WINDOW(about_window),("about"));
    gtk_widget_show(about_window);

    gtk_container_add(GTK_CONTAINER(about_window),about_image);
    gtk_widget_show(about_image);
}

// ����GUI������
// ���룺void
// ����ֵ��void
void construct_GUI()
{
    background = gdk_pixbuf_new_from_file("res/background.jpg",NULL);
    A_pic = gdk_pixbuf_new_from_file("res/A.png",NULL);
    B_pic = gdk_pixbuf_new_from_file("res/B.png",NULL);

    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(main_window,627,570);
    gtk_window_set_resizable(GTK_WINDOW(main_window),FALSE);
    gtk_window_set_title(GTK_WINDOW(main_window),("Tic Tac toe"));
    g_signal_connect(G_OBJECT(main_window),"destroy",G_CALLBACK(gtk_main_quit),NULL);

    GtkWidget * layout = gtk_layout_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(main_window),layout);

    GtkWidget * menubar = gtk_menu_bar_new();
    GtkWidget * menu1 = gtk_menu_new();

    GtkWidget * option = gtk_menu_item_new_with_mnemonic("option");
    GtkWidget * player = gtk_image_menu_item_new_with_label("player first");
    GtkWidget * computer = gtk_image_menu_item_new_with_label("computer first");
    GtkWidget * sep = gtk_separator_menu_item_new();
    g_signal_connect(G_OBJECT(player),"activate",G_CALLBACK(player_first),NULL);
    g_signal_connect(G_OBJECT(computer),"activate",G_CALLBACK(computer_first),NULL);

    GtkWidget * quit = gtk_image_menu_item_new_with_label("quit");
    g_signal_connect(G_OBJECT(quit),"activate",G_CALLBACK(gtk_main_quit),NULL);

    GtkWidget * menu2 = gtk_menu_new();
    GtkWidget * help = gtk_menu_item_new_with_mnemonic("help");
    GtkWidget * about = gtk_image_menu_item_new_with_label("about..");
    g_signal_connect(G_OBJECT(about),"activate",G_CALLBACK(about_pressed),NULL);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(option),menu1);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu1),player);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu1),computer);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu1),sep);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu1),quit);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(help),menu2);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu2),about);

    gtk_menu_shell_append(GTK_MENU_SHELL(menubar),option);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar),help);

    gtk_layout_put(GTK_LAYOUT(layout),menubar,0,0);
    gtk_widget_set_size_request(menubar,627,20);

    draw_window = gtk_drawing_area_new();
    gtk_layout_put(GTK_LAYOUT(layout),draw_window,0,20);
    gtk_widget_set_size_request(draw_window,627,570);
    g_signal_connect(draw_window,"expose-event",G_CALLBACK(window_drawing),NULL);

    gtk_widget_set_events(draw_window,gtk_widget_get_events(draw_window)|GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(draw_window),"button-press-event",G_CALLBACK(when_key_pressed),NULL);

    gtk_widget_show_all(main_window);
}

int main(int argc,char * argv[])
{
    FILE * fp;
    fp = fopen("BP_net_weight.txt","r");

    for(int i=0;i<N*N;i++)
    {
        fscanf(fp,"%lf",&w_ji[i]);
    }
    for(int i=0;i<N*N;i++)
    {
        for(int j=0;j<N*N;j++)
        {
            fscanf(fp,"%lf ",&w_kj[i][j]);
        }
    }

    gtk_init(&argc,&argv);

    construct_GUI();
    gtk_widget_show(main_window);

    gtk_main();

    // �������������л���ѧϰ/����������Ĵ���
    // ���������ָ����������ּ��䵹��ֵ���ü����ѧϰ����ֵ�����ֵ�������磨�����Լ����Լ���ս��
    /*FILE * fp;
    fp = fopen("BP_net_weight.txt","w");

    int learn_times = 10;
    int temp_learn_times = 0;

    init_weight();
    srand((int)time(0));
    while(1)
    {
        int count = 0;
        init_chess_board();
        while(!test_if_chess_board_full(chess_board)&&test_who_win(chess_board)==0)
        {
            if(count%2==0)
            {
                while(1)
                {
                    int temp = rand()%(N*N);
                    if(chess_board[temp]==0)
                    {
                        place_piece_on_chess_board(chess_board,temp/N,temp%N,A);
                        break;
                    }
                }
            }
            else
            {
                while(1)
                {
                    int temp =rand()%(N*N);
                    if(chess_board[temp]==0)
                    {
                        place_piece_on_chess_board(chess_board,temp/N,temp%N,B);
                        break;
                    }
                }
            }
            count ++;
            if(count>=8)
            {
                int ancestor[N*N];
                init_ancestor(ancestor,test_node_type(chess_board));
                int backed_up_value = get_backed_up_value_of_node(chess_board,0,6,ancestor,test_node_type(chess_board));
                adapt_BP_net_weight(chess_board,backed_up_value);
                temp_learn_times ++;
                printf("learn %d times.\n",temp_learn_times);
            }
        }
        if(temp_learn_times>=learn_times)
            break;
    }

    for(int i=0;i<N*N;i++)
    {
        fprintf(fp,"%lf ",w_ji[i]);
    }
    fprintf(fp,"\n");
    for(int i=0;i<N*N;i++)
    {
        for(int j=0;j<N*N;j++)
        {
            fprintf(fp,"%lf ",w_kj[i][j]);
        }
    }
    fprintf(fp,"\n");*/

    return 0;
}
