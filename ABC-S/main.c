#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
 
#define M_PI 3.1415926
#define NP 20
#define FoodNumber NP/2
#define limit 100
#define maxCycle 1000
#define runtime 1
 
#define D 30
#include "CostFunction.h"
// 获得全局变量function指向测试函数，lb，ub全局变量，确定函数范围
// 返回值为名称
// 需要定义维度#define D
 
double Foods[FoodNumber][D];//食物源位置
double f[FoodNumber];//食物源损失值
double fitness[FoodNumber];//食物源的实际适应度函数
double trial[FoodNumber];//试验使用值，可以提升算法的性能
double prob[FoodNumber];//食物源选择几率
double solution[D];//解决方案，即测试值
double ObjValSol;//新解决方案的维度值
double FitnessSol;//新解决方案的适应度函数
int neighbour,param2change;//算式中担任参数值
double GlobalMin;//最优解
double GlobalParams[D];//最优解维度取值
double r;//随机数
double Global_onetime[maxCycle];//记录值
 
float sharing_factor;
 
double CalculateFitness(double fun){
    double result = 0;
    if(fun >= 0){
        result = 1 / (fun + 1);
    }
    else{
        result = 1 + fabs(fun);
    }
    return result;
}
//计算损失值
void MemorizeBestSource(){
    int i,j;
    for(i = 0; i < FoodNumber; i++){
        if(f[i] < GlobalMin){
            GlobalMin = f[i];
            for(j = 0; j < D; j++){
                GlobalParams[j] = Foods[i][j];
            }
        }
    }
}
//记录值
void init(int index){
    int j;
    for(j=0 ; j < D; j++){
        r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
        Foods[index][j] = r * (ub - lb) + lb;
        solution[j] = Foods[index][j];
    }
    f[index] = function(solution);
    fitness[index] = CalculateFitness(f[index]);
    trial[index] = 0;
}
//依次初始化食物源
void initial(){
    int i;
    for(i = 0; i < FoodNumber; i++){
        init(i);
    }
    GlobalMin = f[0];
    for(i = 0; i < D; i++){
        GlobalParams[i] = Foods[0][i];
    }
}
//开始初始化食物源
void CalculateProbabilities(){
    int i;
    double maxfit;
    maxfit = fitness[0];
    for(i = 1; i < FoodNumber; i++){
        if(fitness[i] > maxfit){
            maxfit = fitness[i];
        }
    }
    for(i = 0; i < FoodNumber; i++){
        prob[i] = (0.9 * (fitness[i] / maxfit)) + 0.1;
    }
}
//计算食物源的选择的可能性
void SendEmployedBees(int t){
    int i,j;
    //修改自适应权重因子
    sharing_factor = sin(M_PI * t / 2 / maxCycle + M_PI) + 2;
    for(i = 0; i< FoodNumber; i++){
        //随机选择维度
        r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
        param2change = (int)(r * D);
        //随机选择变异（不能是当前i进行变异）
        while (neighbour == i)
        {
            r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
            neighbour = (int)(r * FoodNumber);
        }
 
        //选择修改一个食物源的一个维度，计算适应度，确定适应度不越界
        for(j=0; j<D; j++){
            solution[j] = Foods[i][j];
        }
        r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
        solution[param2change] = Foods[i][param2change] + (Foods[i][param2change] - Foods[neighbour][param2change]) * sharing_factor * (r - 0.5) * 2;
        if(solution[param2change] < lb){
            solution[param2change] = lb;
        }
        if(solution[param2change] > ub){
            solution[param2change] = ub;
        }
        ObjValSol = function(solution);
        FitnessSol = CalculateFitness(ObjValSol);
 
        //贪婪选择最优，更新食物源
        if(FitnessSol > fitness[i]){
            trial[i] = 0;
            for(j = 0; j < D; j++){
                Foods[i][j] = solution[j];
            }
            f[i] = ObjValSol;
            fitness[i] = FitnessSol;
        }
        //用于去除长期且无意义的食物源
        else{
            trial[i] = trial[i] + 1;
        }
    }
}
//雇佣蜂运行
void SendOnlookBees(){
    int i=0, j, t=0;
    while (t < FoodNumber)
    {
        r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
        if(r < prob[i]){
            t++;
            //随机选出维度以及其修改的参数变量
            r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
            param2change = (int)(r * D);
            r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
            neighbour = (int)(r * FoodNumber);
            while (neighbour == i)
            {
                r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
                neighbour = (int)(r * FoodNumber);
            }
            for(j = 0; j < D; j++){
                solution[j] = Foods[i][j];
            }
 
            //选择更新单一食物源的单一维度
            r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
            solution[param2change] = Foods[i][param2change] + (Foods[i][param2change] - Foods[neighbour][param2change]) * sharing_factor * (r - 0.5) * 2;
            if(solution[param2change] < lb){
                solution[param2change] = lb;
            }
            if(solution[param2change] > ub){
                solution[param2change] = ub;
            }
            ObjValSol = function(solution);
            FitnessSol = CalculateFitness(ObjValSol);
 
            //更新食物源
            if(FitnessSol > fitness[i]){
                trial[i] = 0;
                for(j = 0; j < D; j++){
                    Foods[i][j] = solution[j];
                }
                f[i] = ObjValSol;
                fitness[i] = FitnessSol;
            }
            else{
                trial[i] = trial[i] + 1;
            }
        }
        i ++;
        if(i == FoodNumber -1){
            i = 0;
        }
    }
}
//观察蜂运行
void SendScoutBees(){
    int maxtrialindex, i;
    maxtrialindex = 0;
    //更新掉当前最差的超过食物停留上限的食物源
    for(i = 1; i < FoodNumber; i++){
        if(trial[i] > trial[maxtrialindex]){
            maxtrialindex = i;
        }
        if(trial[maxtrialindex] >= limit){
            init(maxtrialindex);
        }
    }
}
//侦察蜂运行
int main()
{
    srand(time(NULL));
    //优化函数之后的存储预备
    FILE *rf, *wf;
    int iter, run, j;
    double mean;
    mean = 0;
    // char outputfilename[100] = "";
    int i = 1;
 
    //选择测试的函数
    printf("eg:sphere 1(default: sphere)\r\n");
    printf("Please input function ID:");
 
    //写文件操作
    char* name = SwitchFunction();
    // memcpy(outputfilename,name,sizeof(name));
    char filename[100] = "";
    // sprintf(filename, "Fitness.data",outputfilename);
    sprintf(filename, "PreFitness.data");
    wf = fopen(filename, "w+");
 
    //进入主循环
    for(run = 0; run<runtime; run++){
        initial();
        MemorizeBestSource();
        //迭代操作
        for(iter = 0; iter < maxCycle; iter++){
            SendEmployedBees(iter);
            CalculateProbabilities();
            SendOnlookBees();
            MemorizeBestSource();
            Global_onetime[iter] = GlobalMin;
            SendScoutBees();
            fprintf(wf, "%.20f\n", Global_onetime[iter]);
            printf("Global_onetime[%d]: %e\n", iter, Global_onetime[iter]);
        }
        printf("%d, run: %e \n", run+1, GlobalMin);
    }
    //关闭指针
    fclose(wf);
    free(rf);
    free(wf);
    return 0;
}