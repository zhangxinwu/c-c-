#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define N (3)
#define M 10000 //八数码数量

short Pre_value = 0;
int st[1000];
short aa[][2] = {0, 0, 0, 1, 0, 2, 1, 0, 1, 1, 1, 2, 2, 0, 2, 1, 2, 2};

enum
{
    LEFT,
    UP,
    RIGHT,
    DOWN
};

typedef struct //图形，其中ij表示0（即空位）的位置
{
    short a[N * N];
    short ij;
    short nextCouldStep;
} G;

void swap(short *a, short *b);
void shuffle(short *a, int len);
void init(G *g);
void print(const G *g);
bool isok(const G *g);

//需要关心的
bool move(G *g, short n);
short update(short m, short n, G *g);
int idaStar(G *g);
bool dfs(short stp, short k, G *g);

int main()
{
    srand(time(NULL));
    G g[M];
    int sum_step = 0;
    int sum_time = 0;
    int i;
    short j;
    for (i = 0; i < M; ++i)
        init(g + i);
    for (i = 0; i < M; ++i)
    {
        int t = clock();
        short n = idaStar(g + i);
        t = clock() - t;
        for (j = 0; j < n; j++)
            move(g + i, st[j]);
        if (!isok(g + i))
            break;
        sum_time += t;
        sum_step += n;
    }
    if (i < M)
        printf("第%d组测试未完成\n", i);
    else
        printf("总用时%d毫秒，总步数%d,成绩%f\n", sum_time, sum_step, sum_time / 10.0 + sum_step);
    return 0;
}

void swap(short *a, short *b)
{
    short t = *a;
    *a = *b;
    *b = t;
}

//随机交换 init() 的辅助函数
void shuffle(short *a, int len)
{
    int i;
    for (i = 0; i < len; i++)
        swap(a + rand() % len, a + rand() % len);
}

//伪随机初始图形，仅生成有解的
void init(G *g) 
{
    int i, j;
    
    //按次序初始
    for (i = 0; i < N * N; ++i) 
        g->a[i] = i;
    int count;

    //随机打乱且保证有解
    do
    {
        count = 0;
        shuffle(g->a, N * N);
        for (i = 0; i < N * N - 1; ++i)
            for (j = i; j < N * N; ++j)
                if (g->a[j] && g->a[i] > g->a[j])
                    ++count;
    } while (count & 1);

    for (i = 0; i < N * N; ++i)
        if (!g->a[i])
        {
            g->ij = i;
            break;
        }
    //nextCouldStep初始化
    g->nextCouldStep = 0;
    for (int i = 0; i < N * N; i++)
    {
        short m = g->a[i];
        g->nextCouldStep += abs(aa[i][0] - aa[m][0]) + abs(aa[i][1] - aa[m][1]);
    }
}

//打印图形，调试用
void print(const G *g) 
{
    int i, j;
    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < N; ++j)
            if (g->a[i * N + j])
                printf("%3d", g->a[i * N + j]);
            else
                printf("   ");
        printf("\n");
    }
}

//更新nextCouldStep的值
short update(short m, short n, G *g)
{
    short p = g->a[m];
    short t0 = abs(aa[n][0] - aa[p][0]) + abs(aa[n][1] - aa[p][1]);
    short t1 = abs(aa[m][0] - aa[p][0]) + abs(aa[m][1] - aa[p][1]);
    return t1 - t0;
}

//移动，n表示方向 
//return 0 : can't move
//return 1 : can move and moved
bool move(G *g, short n) 
{
    short i = g->ij;
    if (n == LEFT) //右侧向空位填充为“左”操作
    {
        if (aa[i][1] == N - 1)
            return 0;
        g->a[i] = g->a[i + 1];
        g->a[i + 1] = 0;
        ++g->ij;
    }
    if (n == UP) //下侧向空位填充为“上”操作
    {
        if (aa[i][0] == N - 1)
            return 0;
        g->a[i] = g->a[i + N];
        g->a[i + N] = 0;
        g->ij += N;
    }
    if (n == RIGHT) //左侧向空位填充为“右”操作
    {
        if (aa[i][1] == 0)
            return 0;
        g->a[i] = g->a[i - 1];
        g->a[i - 1] = 0;
        --g->ij;
    }
    if (n == DOWN) //上侧向空位填充为“下”操作
    {
        if (aa[i][0] == 0)
            return 0;
        g->a[i] = g->a[i - N];
        g->a[i - N] = 0;
        g->ij -= N;
    }
    g->nextCouldStep += update(i, g->ij, g);
    return 1;
}

//判断是否已经恢复序列
bool isok(const G *g)
{
    int i;
    for (i = 0; i < N * N; ++i)
        if (g->a[i] != i)
            return 0;
    return 1;
}

//dfs过程
bool dfs(short stp, short k, G *g)
{
    if (stp + g->nextCouldStep >= Pre_value)
        return false;
    if (isok(g))
    {
        Pre_value = stp;
        return true;
    }
    for (short i = 0; i < 4; i++)
        if (i != k)
        {
            G p = *g;
            if (move(&p, i))
                if (dfs(stp + 1, (i + 2) % 4, &p))
                {
                    st[stp] = i;
                    return true;
                }
        }
    return false;
}

//主要算法 IDA* 
int idaStar(G *g)
{
    //更改Pre_value可以控制步数， 最佳平均步数21.6 
    //将Pre_value更改到21以上可将时间优化
    //但步数也会增加，且稳定在接近Pre_value的范围
    Pre_value = 0;

    while (!dfs(0, -1, g))
        ++Pre_value;

    return Pre_value;
}
