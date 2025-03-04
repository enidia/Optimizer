#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<malloc.h>
#include<math.h>
#include<float.h>
#include<io.h>
#include<direct.h>
 
#define Dimension 12 //表示解集和的维度
 
 
#define M_PI 3.1415926
#define NP 5 //将要搜索的狼群的数量
#define maxCycle 200 //最大迭代次数
#include "CostFunction.h"
 
typedef struct Wolf
{
    double Position[Dimension];
    double Fitness;
} Wolf;
Wolf wolf[NP];
double Record[maxCycle];
//轮盘赌进行选择，输入数组指针*P,以及一个Count的数组合计元素，数组指针*P 将会是一个 0-1之间，总和为1的数组集合
int RouletteWheelSelection(double *P, int Count){
    double Probility = rand()/(RAND_MAX + 1.0);
    double Sum = 0;
    for(int i =0;i<Count;i++){
        Sum = Sum + P[i];
        if(Sum >= Probility){
            return i;
        }
    }
    return -1;
}
/*
 * 打印狼群
 * 1.输出所有狼群当前的存储状态
*/
void PrintWolf(int flag){
    if(flag != 0) {
        for (int i = 0; i < NP; i++) {
            printf("\n\n");
            printf("-------------------------------\n");
            printf("No.%d\n", i);
            for (int j = 0; j < Dimension; j++) {
                printf("Dimension %d : %.20f\t", j, wolf[i].Position[j]);
            }
            printf("\n");
            printf("%.20f\n", wolf[i].Fitness);
            printf("-------------------------------\n");
            printf("\n\n");
        }
    } else{
        printf("the best wolf is: \n\n");
        for (int j = 0; j < Dimension; j++) {
            printf("Dimension %d : %.20f\t", j, wolf[0].Position[j]);
        }
        printf("\n");
        printf("%.20f\n", wolf[0].Fitness);
    }
}
/*
本函数主要用于打印wolf数组中的情况
输出项如下:
1. 输出wolf内的所有数据项的Position
2. 输出wolf内的所有的Fitness
*/
void IntoFiles(double *Record){
    FILE *pointFitness;
    char *Source = "./Result";
    if(_access(Source ,0) == -1){
        _mkdir(Source);
    }
    char fitnessFile[200];
    strcpy(fitnessFile,Source);
    strcat(fitnessFile,"\\\\Fitness.data");
    if((pointFitness = fopen(fitnessFile,"w+")) == NULL){
        printf("Error to open the File Fitness.data\n");
    }
    if(pointFitness != NULL){
        for(int i = 0; i < maxCycle; i++){
            fprintf(pointFitness,"%.20f\n",Record[i]);
        }
        fclose(pointFitness);
    }
    else{
        printf("The Files can not open, please check the state of the Files");    }
}
/*
 * 初始化一个狼群结构体变量
 * 输入:index为狼的下标
 * 1.随机赋予index号狼它的position
 * 2.根据新的position计算它的fitness
*/
void Init(int index){
    for(int i=0;i<Dimension;i++){
        double r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
        (wolf+index)->Position[i] = r*(ub-lb)+lb;
    }
    (wolf+index)->Fitness = function((wolf+index)->Position);
}
/*
 * 初始化所有的狼群
 * 1.输出所有的狼群的初始化
*/
void InitAll(){
    for(int i=0;i<NP;i++){
        Init(i);
    }
}
void Sort(){
    for(int i =1;i<NP;i++){
        for(int j =0;j<NP-i;j++){
            if(wolf[j].Fitness > wolf[j+1].Fitness){
                Wolf Temp = wolf[j];
                wolf[j] = wolf[j+1];
                wolf[j+1] = Temp;
            }
        }
    }
}
/*
 * 更新狼群算法的Position
 * 输入iter迭代数目
 * 1.更新狼群的position
 * 2.更新狼群的Fitness
*/
void CalculateWolf(int iter){
    double a = (double)(2) - (double)(2)*(double)iter/(double)maxCycle;
    for(int i=3;i<NP;i++){
        double A1 = ((double)2*a*(((double)rand() / ((double)(RAND_MAX) + (double)(1)))))-a;
        double C1 = (double)(2)*(((double)rand() / ((double)(RAND_MAX) + (double)(1))));
        double A2 = ((double)2*a*(((double)rand() / ((double)(RAND_MAX) + (double)(1)))))-a;
        double C2 = (double)(2)*(((double)rand() / ((double)(RAND_MAX) + (double)(1))));
        double A3 = ((double)2*a*(((double)rand() / ((double)(RAND_MAX) + (double)(1)))))-a;
        double C3 = (double)(2)*(((double)rand() / ((double)(RAND_MAX) + (double)(1))));
        Wolf Temp1,Temp2,Temp3;
        for(int j=0;j<Dimension;j++){
            Temp1.Position[j] = fabs(C1*wolf[0].Position[j] - wolf[i].Position[j]);//alpha
            Temp2.Position[j] = fabs(C2*wolf[1].Position[j] - wolf[i].Position[j]);//beta
            Temp3.Position[j] = fabs(C3*wolf[2].Position[j] - wolf[i].Position[j]);//theta
            Temp1.Position[j] = wolf[0].Position[j] - Temp1.Position[j]*A1;
            Temp2.Position[j] = wolf[1].Position[j] - Temp2.Position[j]*A2;
            Temp3.Position[j] = wolf[2].Position[j] - Temp3.Position[j]*A3;
            wolf[i].Position[j] = (Temp1.Position[j] + Temp2.Position[j] + Temp3.Position[j])/(double)(3);
        }
        wolf[i].Fitness = function(wolf[i].Position);
    }
}
int main(){
    system("clear");
    int iter=0;
    printf("Please Tap your interested Formula:\n\n\n");
    char* func = SwitchFunction();//选中公式
    printf("the formula you have chosen is :%s\n",func);
    srand(time(NULL));
    InitAll();
//    PrintWolf(1);
    int flag = 0;
    Sort();
    for(iter = 0; iter<maxCycle; iter ++){
        CalculateWolf(iter);
        Sort();
        Record[iter] = wolf[0].Fitness;
        if(flag == 100){
            flag = 0;
            printf("finished the precent: %.2f\n",100*(double)iter/(double)maxCycle);
        }
        flag ++;
    }
    PrintWolf(1);
    IntoFiles(Record);
    printf("the formula you have chosen is :%s\n",func);
    printf("-----------------------over---------------------");
    return 0;
}