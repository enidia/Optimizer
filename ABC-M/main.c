#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<malloc.h>
#include<math.h>
#include<float.h>
#include<io.h>
#include<direct.h>
/*
1. NP 表示当前要搜索的所有的蜜源的总数（实际上也是蜂群的总数）
2. Rep 表示非支配解的额定的数量，超过这个数量应该予以取舍非支配解集和
3. limit 表示蜜源的搜索次数上限，超过这个限制，则侦察蜂运行，放弃蜜源
4. maxCycle 表示当前的最大迭代次数，若超过这个限制，则算法结束
5. nGrid 表示划分的网格的数量
6. Target 表示目标的数量，也就是多目标的目标解集和数量，也就是 Y 的数量
7. Dimension 表示当前的自变量的维度解，即 X 的数量
8. CostFunction 为当前的计算的代价函数
9. Alpha 用于扩大Fitness的最大，最小值，以获得最好的网格（扩张的网格）
*/
 
#define Dimension 12 //表示解集和的维度
#define Target 3
 
 
#define M_PI 3.1415926
#define NP 200 //将要搜索的蜜源的数量
#define Rep 100 //获取的非支配解集合
#define LowRep Rep-1//非支配解最少数量限制
#define limit 5 //表示100次后将放弃当前的蜜源
#define maxCycle 10000 //最大迭代次数
#define nGrid 10 //单个元素的网格数量
#define Alpha 0.1 //网格扩张指数
#include "CostFunction.h"
/*
结构体的逻辑结构如下：
1. 首先，根据结构体形成蜜源以及非支配解集和
2. 当结构体的变量为蜜源的时候，Position  PreF Fitness Trial Prob IsDominated 有效
3. 当结构体的变量为非支配解集的时候， Position PreF Fitness Grid IsDominated 有效
4. Position 为输入的目标变量 x1 x2 .... xn
5. PreF 为通过Function求解出来的目标解集和 Py1 Py2 Py3 ... Pyn, 为真实解
6. Fitness 则为通过蜂群的特殊算法，求得的 Algorithm(Py1, Py2, ..., Pyn) = y1 y2 y3 ... yn, 用于判断蜜源是否能够使用观察蜂进行操作
7. Trial 为当前的蜜源的访问的次数，过多需要通过侦察蜂更新蜜源
8. Prob 为当前蜜源可能的观察蜂采蜜的几率，实际上就是通过当前蜜源的位置，搜索临近的蜜源
9. Grid 表述了非支配解的网格位置，由于返回的目标结果有y1 y2 y3 ... yn 所以对应的也有 Grid1 Grid2 ... Gridn
10. Isdominated 表述了当前的支配关系。 1 为被支配了，0 则是未被支配
*/
typedef struct Nectar
{
    double Position[Dimension];
    double PreF[Target];
    double Fitness[Target];
    double Trial;
    double Prob;
    int Grid[Target];
    int IsDominated;
} Nectar;
/*
Repository 为当前的非支配解集和
BeesPoint 为蜜源的集合
Grid 为计算网格所用的数组
neighbour 为一个 Employ 和 Onlook 所用的分享值
sharing_factor 为一个计算 Fitness 所用的分享值
*/
Nectar Reposity[Rep], BeesPiont[NP];
double Grid[Target][nGrid+1];
int neighbour;
float sharing_factor;
//随机生成start-double内的double浮点类型数字
double RandomProb(double start, double end)
{
    return start + (end - start) * rand() / (RAND_MAX + 1.0);
}
//输入一个double类型的预处理的Fitness进行处理，输入PreF，返回Fitness，使用Fitness进行概率计算，以获得观察蜂采蜜的概率，考虑取大取小的问题
double CalculateFitness(double F){
    if(FLAG == 0){
        double result = 0;
        if (F >= 0){
            result = 1/(F+1);
        }
        else{
            result = 1+fabs(F);
        }
        return result;
    }else if(FLAG == 1){
        double result = 0;
        if (F >= 0){
            result = 1.1 - 1/(F+1);
        }
        else{
            result = 1+fabs(F);
        }
        return result;
    }
}
/*
输入蜜源的索引值(从0开始计数），即BeesPoint[]结构体数组，返回的初始化后的目标索引值的蜜源：
1. 随机产生Position: x1, x2 ... xn
2. Position给出PreF = function(x1, x2, ... ,xn)
3. Pref给出Fitness = Algorithm(Py1, Py2, ... , Pyn)
4. 重置 Prob 为 0
5. 重置 Trial 为 0
6. 重置 Isdominated 为 0，
7. 重置 Grid[] 为 {-1,-1, ..., -1}
*/
void Init(int index){
    Nectar *beetemp = BeesPiont;
    for(int j=0;j<Dimension;j++){
        double r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
        (beetemp+index)->Position[j] = r*(ub[j]-lb[j]) + lb[j];
    }
    for(int j=0; j<Target;j++){
        (beetemp+index)->PreF[j] = function[j]((beetemp+index)->Position);
    }
    for(int j=0;j<Target;j++){
        (beetemp+index)->Fitness[j] = CalculateFitness((beetemp+index)->PreF[j]);
    }
    (beetemp+index)->Prob = 0;
    (beetemp+index)->Trial = 0;
    (beetemp+index)->IsDominated = 0;
    for(int i =0 ;i<Target;i++){
        (beetemp+index)->Grid[i] = -1;
    }
}
/*
输入非支配解集的索引值(从0开始计数），即Repository[]结构体数组，返回的初始化后的目标索引值的非支配解：
1. 使得 x1 x2 ... xn = {0 0 ... 0}
2. 使得 PreF[] = {0 0 ... 0}
3. 使得 Fitness[] = {0 0 ... 0}
4. 重置 Prob 为 0
5. 重置 Trial 为 0
6. 重置 Isdominated 为 1 （在Repository可以表示为收支配的解，或者说是Repository内的无效解，是可以加入新的非支配解的区域，而真正的非支配解，Isdominated = 0）
7. 重置 Grid[] 为 {-1,-1, ..., -1}
*/
void InitRep(int index){
    Nectar *repotemp = Reposity;
    for(int j = 0;j<Dimension;j++){
        (repotemp+index)->Position[j] = 0;
    }
    for(int j =0 ;j<Target;j++){
        (repotemp+index)->PreF[j] = 0;
    }
    for(int j=0; j<Target;j++){
        (repotemp+index)->Fitness[j] = 0;
    }
    (repotemp+index)->Prob = 0;
    (repotemp+index)->Trial = 0;
    (repotemp+index)->IsDominated = 1;
    for(int i = 0;i<Target;i++){
        (repotemp+index)->Grid[i] = -1;
    }
}
//通过Init以及InitRep来初始化所有的解集合
void InitAllBees(){
    for(int i=0;i<NP;i++){
        Init(i);
    }
    for(int j=0;j<Rep;j++){
        InitRep(j);
    }
}
/*
输入Nectar的指针变量*Bee，将会刷新*Bee的元素:
1. Pref[] = new (x1 x2 x3 ... xn)
2. Fitness[] = new (Py1 Py2 ... Pyn)
*/
void RefreshBeeFitness(Nectar *Bee){
    for(int j=0;j<Target;j++){
        Bee->PreF[j] = function[j](Bee->Position);
    }
    for(int j=0;j<Target;j++){
        Bee->Fitness[j] = CalculateFitness(Bee->PreF[j]);
    }
}
/*
输出蜜源，从0开始计数index
输入位置 x1 x2 ... xn
1. 输出 PreF Py1 Py2 ... PyF
2. 输出 Fitenss y1 y2 ... yn
3. 输出 Trail
4. 输出 Prob
5. 输出 IsDominated
*/
void PrintIndexBee(int index){
    Nectar *beetemp = BeesPiont;
    printf("this is the No.%d Bees details:\n", index+1);
    printf("the values in Dimension are shows below:\n");
    for(int j=0;j<Dimension;j++){
        double position = (beetemp+index)->Position[j];
        printf("No.%d %.5f\t",j+1,position);
    }
    printf("\n the PreFitness is shown below: (which is only be needed in Bees Colony)\n");
    for(int j=0;j<Target;j++){
        double pref = (beetemp+index)->PreF[j];
        printf("No.%d %.20f\t",j+1,pref);
    }
    printf("\n the fitness is shown below: \n");
    for(int j=0;j<Target;j++){
        double fitness = (beetemp+index)->Fitness[j];
        printf("No.%d %.10f\t",j+1,fitness);
    }
    printf("\n the Trial is shown below (in order to delete the useless Nectar Point)\n");
    double trial = (beetemp+index)->Trial;
    printf("%.2f\n",trial);
    printf(" the Prob is a param which means possibility\n");
    double prob = (beetemp+index)->Prob;
    printf("%.2f\n", prob);
    printf(" the relationship of domination is shown below\n");
    int dominated = (beetemp+index)->IsDominated;
    printf("%d\n\n\n", dominated);
}
//输出所有的蜜源详细信息
void PrintBees(){
    printf("\n\n\n----------------------------------------------------------\
    here are the Bees value\
    ------------------------------------------------------------------\n\n");
    for(int i=0; i<NP;i++){
        PrintIndexBee(i);
    }
    printf("\n\n----------------------------------------------------------\
    Bees Value are Over!!!!!!!!\
    ------------------------------------------------------------------\n\n\n");
}
/*
输出非支配解，从0开始计数index
1. 输出位置 x1 x2 ... xn
2. 输出 PreF Py1 Py2 ... PyF
3. 输出 Fitenss y1 y2 ... yn
4. 输出 Trail
5. 输出 Prob
6. 输出 IsDominated
*/
void PrintIndexReposity(int index){
    Nectar *repotemp = Reposity;
    printf("the values in Dimension are shows below:\n");
    for(int j=0; j<Dimension;j++){
        double position = (repotemp+index)->Position[j];
        printf("No.%d %.5f\t",j+1,position);
    }
    printf("\n the PreFitness is shown below:(which is only be needed in Bees Colony)\n");
    for(int j=0;j<Target;j++){
        double pref = (repotemp+index)->PreF[j];
        printf("No.%d %.20f\t",j+1,pref);
    }
    printf("\n the fitness is shown below: \n");
    for(int j=0;j<Target;j++){
        double fitness = (repotemp+index)->Fitness[j];
        printf("No.%d %.20f\t",j+1,fitness);
    }
    printf("\n the Girds are below\n");
    for(int j=0;j<Target;j++){
        int grid = (repotemp+index)->Grid[j];
        printf("No.%d %d\t",j+1,grid);
    }
    printf("\n the Trial is shown below (in order to delete the useless Nectar Point, No of use in this time\n");
    double trial = (repotemp+index)->Trial;
    printf("%.2f\n",trial);
    printf(" the Prob is a param which means possibility\n");
    double prob = (repotemp+index)->Prob;
    printf("%.2f\n",prob);
    printf(" the relatoinship of domination is shown below:\n");
    int dominated = (repotemp+index)->IsDominated;
    printf("%d\n\n\n",dominated);
}
//输出所有的非支配解
void PrintReposities(){
    printf("\n\n\n----------------------------------------------------------\
    here are the Repositories value\
    ------------------------------------------------------------------\n\n");
    int j = 0;
    for(int i=0;i<Rep;i++){
        if (Reposity[i].IsDominated == 0)
        {
            printf("this is the No.%d Reposity details:\n", j+1);
            PrintIndexReposity(i);
            j++;
        }
    }
    printf("\n\n----------------------------------------------------------\
    Repositories are Over\
    ------------------------------------------------------------------\n\n\n");
}
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
/*输入Obj1 和 Obj2 两个Nectar结构体的指针，判断支配关系:
1. PreF的越小越好，PreF越小，随机数Fitness越大
2. 1表示Obj2被Obj1支配，
3. 0表示Obj2没有被Obj1支配
4. 不保证Obj1没有被Obj2支配，需要进行双向验证。
*/
int isDominated(Nectar* Obj1,Nectar* Obj2){
    int flag = 0;
    if(FLAG == 0){
        for(int i=0;i<Target;i++){
            if(Obj1->PreF[i] > Obj2->PreF[i]){
                return 0;
            }
            if(Obj1->PreF[i] < Obj2->PreF[i]){
                flag = 1;
            }
        }
        if(flag == 1){
            return 1;
        }
    }else if(FLAG == 1){
        for(int i=0;i<Target;i++){
            if(Obj1->PreF[i] < Obj2->PreF[i]){
                return 0;
            }
            if(Obj1->PreF[i] > Obj2->PreF[i]){
                flag = 1;
            }
        }
        if(flag == 1){
            return 1;
        }
    }
    //which means something wrong with the algorithm
    return -1;
}
/*
本函数主要用于打印Repository数组中的情况
输出项如下:
1. 输出Repository内的所有数据项的Position
2. 输出Repository内的所有的PreFitness，即真实需要进行优化产生值的fitness
3. 输出Repository内的所有的Fitness，即蜂群算法中需要使用过多参数
4. 输出Repository内的所有的项的网格值，即位于当前的网格的第几个格子里面
*/
void IntoFiles(){
    FILE *pointPosition, *pointPreFitness, *pointFitness, *pointGrid;
    char *Source = "./Result";
    if(_access(Source ,0) == -1){
        _mkdir(Source);
    }
    char positionFile[200];
    char prefFile[200];
    char fitnessFile[200];
    char gridFile[200];
    strcpy(positionFile,Source);
    strcpy(prefFile,Source);
    strcpy(fitnessFile,Source);
    strcpy(gridFile,Source);
    strcat(positionFile,"\\\\Position.data");
    strcat(prefFile,"\\\\PreFitness.data");
    strcat(fitnessFile,"\\\\Fitness.data");
    strcat(gridFile,"\\\\Grid.data");
    if((pointPosition = fopen(positionFile,"w+")) == NULL){
        printf("Error to open the File Position.data\n");
    }
    if((pointPreFitness = fopen(prefFile,"w+")) == NULL){
        printf("Error to open the File PreFitness.data\n");
    }
    if((pointFitness = fopen(fitnessFile,"w+")) == NULL){
        printf("Error to open the File Fitness.data\n");
    }
    if((pointGrid = fopen(gridFile,"w+")) == NULL){
        printf("Error to open the File Grid.data\n");
    }
    if(pointPosition != NULL && pointPreFitness != NULL && pointFitness != NULL && pointGrid != NULL){
        for(int i = 0; i < Rep; i++){
            if(Reposity[i].IsDominated == 0){
                for(int j = 0; j < Dimension-1 ; j++){
                    fprintf(pointPosition,"%.20f\t",Reposity[i].Position[j]);
                }
                fprintf(pointPosition,"%.20f\n",Reposity[i].Position[Dimension-1]);
                for(int j = 0; j <Target-1; j++){
                    fprintf(pointPreFitness,"%.20f\t",Reposity[i].PreF[j]);
                    fprintf(pointFitness,"%.20f\t",Reposity[i].Fitness[j]);
                    fprintf(pointGrid,"%d\t",Reposity[i].Grid[j]);
                }
                fprintf(pointPreFitness,"%.20f\n",Reposity[i].PreF[Target-1]);
                fprintf(pointFitness,"%.20f\n",Reposity[i].Fitness[Target-1]);
                fprintf(pointGrid,"%d\n",Reposity[i].Grid[Target-1]);
            }
        }
        fclose(pointPosition);
        fclose(pointPreFitness);
        fclose(pointFitness);
        fclose(pointGrid);
    }
    else{
        printf("The Files can not open, please check the state of the Files");    }
}
/*判断蜜源，BeesPoint内部的支配关系:
1. 被支配的蜜源将会使得其 IsDominated = 1
2. 非支配解将保持原来的 IsDominated = 0
*/
void BeesDominated(){
    Nectar* bees = BeesPiont;
    for(int i=0;i<NP;i++){
        for(int j =i+1;j<NP;j++){
            int idomj = isDominated((bees+i),(bees+j));
            int jdomi = isDominated((bees+j),(bees+i));
            if(idomj == 1){
                (bees+j)->IsDominated = 1;
            }
            if(jdomi == 1){
                (bees+i)->IsDominated = 1;
            }
        }
    }
}
/*判断非支配解集和，Repository内部的支配关系:
1. 被支配的蜜源将会使得其 IsDominated = 1（这里将表示这个解可以被置换，已经无效）
2. 非支配解将保持原来的 IsDominated = 0
*/
void RepositiesDominated(){
    Nectar* repotemp = Reposity;
    for(int i=0;i<Rep;i++){
        for(int j =i+1;j<Rep;j++){
            if((repotemp+i)->IsDominated == 0 && (repotemp+j)->IsDominated ==0) {
                int idomj = isDominated((repotemp + i), (repotemp + j));
                int jdomi = isDominated((repotemp + j), (repotemp + i));
                if (idomj == 1) {
                    (repotemp + j)->IsDominated = 1;
                }
                if (jdomi == 1) {
                    (repotemp + i)->IsDominated = 1;
                }
            }
        }
    }
    for(int i =0;i<Rep;i++){
        if(Reposity[i].IsDominated == 1){
            InitRep(i);
        }
    }
}
/*
计算Onlooker的可能性值：
1. 返回蜜源 BeesPoint 的 Prob 值，表示蜜源将会在 Onlooker中是否会被用到
2. 所有的蜜源可能性之间无关，而是跟Onlooker中的开始时，会计算一个概率，Prob 超过本概率，将会被观察蜂采蜜
*/
void CalculateProbabilities(){
    double MaxFit[Target];
    for(int j=0;j<Target;j++){
        MaxFit[j] = BeesPiont[0].Fitness[j];
    }
    for(int i=0;i<NP;i++){
        for(int j=0;j<Target;j++){
            if(BeesPiont[i].Fitness[j] > MaxFit[j]){
                MaxFit[j] = BeesPiont[i].Fitness[j];
            }
        }
    }
    double calcul = (double)(1.0/Target);
    for(int i = 0;i<NP;i++){
        double sum = 0;
        for(int j = 0; j < Target; j++){
            sum = sum + ((0.9 * (BeesPiont[i].Fitness[j]/MaxFit[j]))+0.1)*calcul;
        }
        BeesPiont[i].Prob = sum;
    }
}
/*
将蜜源 BeesPoint 中的数据输入非支配解 Repository 中：
1. 保证输入的非支配解不会超过非支配解集和的上限Rep
2. 输入的非支配解集和若过多，将会忽略之后的蜜源中的非支配解
3. 若遍历完成后Rep还有多余，即输入的非支配解集和不够多的时候，将会结束，非支配解集和中，剩下的空余区块可以进行下一次输入，它的 Isdominated 还是 1
*/
void IntitalReposity(){
    int i = 0, j =0;
    while (1)
    {
        if(i >= Rep || j >= NP){
            break;
        }
        if(BeesPiont[j].IsDominated == 0){
            Reposity[i] = BeesPiont[j];
            i++;
        }
        j++;
    }
}
//初始化Grid
void InitGrid(){
    double Low[Target] , Uper[Target];
    for(int i = 0;i<Target;i++){
        Low[i] = Reposity[0].PreF[i];
        Uper[i] = Reposity[0].PreF[i];
    }
    for(int i = 0; i < Rep;i++){
        for(int j = 0;j < Target;j++){
            if(Reposity[i].IsDominated == 0){
                if(Low[j] > Reposity[i].PreF[j]){
                    Low[j] = Reposity[i].PreF[j];
                }
                if(Uper[j] < Reposity[i].PreF[j]){
                    Uper[j] = Reposity[i].PreF[j];
                }
            }
        }
    }
    for(int i = 0; i <Target;i++){
        Low[i] = Low[i] - Alpha * (Uper[i] - Low[i]);
        Uper[i] = Uper[i] + Alpha * (Uper[i] - Low[i]);
    }
    double Temp[Target];
    for(int i = 0; i < Target; i++){
        if((Uper[i] - Low[i]) > 0){
            Temp[i] =  (Uper[i] - Low[i])/nGrid;
        }
        else{
            Temp[i] = Low[i];
        }
    }
    for(int i = 0 ;i < Target;i++){
        Grid[i][nGrid] = DBL_MAX;
        Grid[i][0] = -DBL_MAX;
        for(int j =0;j<(nGrid - 1);j++){
            Grid[i][j+1] = (double)j*Temp[i] + Low[i];
        }
    }
}
/*将Repository内部的非支配解放入Grid
输入: PreF[] = {Py1 Py2 ... Pyn}, 且IsDominated = 0 , 即表示当前的非支配解可用
输出: Grid[] = {1 2 ... n}
*/
void IntoGrid(){
    for(int i=0 ; i < Rep; i++){
        for(int j = 0; j < Target ; j ++){
            for(int k =0;k< nGrid ;k++){
                if(Reposity[i].IsDominated == 0){
                    if(Grid[j][k] < Reposity[i].PreF[j] && Reposity[i].PreF[j] < Grid[j][k+1]){
                        Reposity[i].Grid[j] = k;
                        break;
                    }
                }
            }
        }
    }
}
// 打印出网格的区间
void PrintGrid(){
    for(int i = 0;i<Target;i++){
        printf("the Colum is : %d\n",i+1);
        for(int j =0;j<(nGrid+1);j++){
            printf("%e\t",Grid[i][j]);
        }
        printf("\n\n");
    }
}
/*
利用网格，计算Repository被删除的可能性
这里做删除的可能性的决策，决定了哪个Repository更可能被删除
即:
1. Repository.Prob 输入值，利用这个值来进行删除
2. 考虑到同时使用网格和方差，即利用网格的相减法来代表其方差即可
3. 在计算概率的时候使用了CalculateFitness，即BeesPoint在Onlooker是相同的概率的算法
*/
void CalCulateRepProb(){
    for(int i =0;i<Rep;i++){
        Reposity[i].Prob = 0;
    }
    for(int i = 0 ;i < Rep-1;i++){
        for(int j =i+1 ; j< Rep;j++){
            if(Reposity[i].IsDominated == 0 && Reposity[j].IsDominated == 0){
                for(int k = 0; k<Target;k++){
                    Reposity[i].Prob = Reposity[i].Prob + fabs(Reposity[i].Grid[k] - Reposity[j].Grid[k]);
                    Reposity[j].Prob = Reposity[j].Prob + fabs(Reposity[i].Grid[k] - Reposity[j].Grid[k]);
                }
            }
        }
    }
    for(int i =0 ;i<Rep;i++){
        if(Reposity[i].IsDominated == 0) {
            Reposity[i].Prob = 1.0/(Reposity[i].Prob + 1);
        }
    }
}
/*
删除策略，现在使用的是类似网格方差的策略进行删除
 首先求出网格，再然后求出网格之间的方差（由于只需要进行大小比较，不做具体的方差计算)
*/
void DeleteByStrategy(){
    for(int i =0;i<Rep;i++) {
        if(Reposity[i].IsDominated == 1) {
            double r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
            if(r > 0.5) {
                return;
            } else{
                break;
            }
        }
    }
    int MAXIndex = 0;
    double MAXProb = Reposity[0].Prob;
    for(int i = 0; i<Rep;i++){
        if(Reposity[i].Prob > MAXProb){
            MAXIndex = i;
            MAXProb = Reposity[i].Prob;
        }
    }
    int Count = 0;
    for(int i=0;i<Rep;i++){
        if(Reposity[i].IsDominated == 0){
            Count++;
        }
    }
    if(Count>LowRep){
        InitRep(MAXIndex);
    }
}
//雇佣蜂运行
void SendEmployedBees(int iter){
    Nectar Temp;
    sharing_factor = sin(M_PI * iter/2 / maxCycle + M_PI) +2;
    for(int identity =0;identity<NP;identity++){
        double r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
        int param2change = (int)(r * Dimension);
        while (neighbour == identity)
        {
            r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
            neighbour = (int)(r * NP);
        }
 
        for(int j = 0; j < Dimension; j++){
            Temp.Position[j] = BeesPiont[identity].Position[j];
        }
        r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
        Temp.Position[param2change] = BeesPiont[identity].Position[param2change] + (BeesPiont[identity].Position[param2change] - BeesPiont[neighbour].Position[param2change]) * sharing_factor *(r-0.5)*2 ;
 
        if(Temp.Position[param2change] < lb[param2change]){
            Temp.Position[param2change] = lb[param2change];
        }
        if(Temp.Position[param2change] > ub[param2change]){
            Temp.Position[param2change] = ub[param2change];
        }
        Temp.IsDominated = 0;
        Temp.Prob = 0;
        Temp.Trial = 0;
        for(int j = 0;j<Target;j++){
            Temp.Grid[j] = -1;
        }
        Nectar *NewBee = &Temp;
        RefreshBeeFitness(NewBee);
        Nectar *OldBee = &BeesPiont[identity];
        int judge = isDominated(NewBee,OldBee);
        if(judge == 1){
            BeesPiont[identity] = Temp;
            DeleteByStrategy();
            for(int count =0; count<Rep;count++){
                if(Reposity[count].IsDominated == 1){
                    Reposity[count] = BeesPiont[identity];
                    break;
                }
            }
            RepositiesDominated();
        }
        else{
            BeesPiont[identity].Trial = BeesPiont[identity].Trial + 1;
        }
    }
}
//观察蜂运行
void OnlookBees(){
    int times = 0;
    int identity = 0;
    Nectar Temp;
    while (times < NP)
    {
        double r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
        if(r < BeesPiont[identity].Prob){
            times ++;
            r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
            int param2change = (int)(r * Dimension);
 
            r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
            neighbour = (int)(r * NP);
            while (neighbour == identity)
            {
                r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
                neighbour = (int)(r * NP);
            }
            for(int j = 0; j < Dimension; j++){
                Temp.Position[j] = BeesPiont[identity].Position[j];
            }
            r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
            Temp.Position[param2change] = BeesPiont[identity].Position[param2change] + (BeesPiont[identity].Position[param2change] - BeesPiont[neighbour].Position[param2change]) * sharing_factor *(r-0.5)*2 ;
 
            if(Temp.Position[param2change] < lb[param2change]){
                Temp.Position[param2change] = lb[param2change];
            }
            if(Temp.Position[param2change] > ub[param2change]){
                Temp.Position[param2change] = ub[param2change];
            }
            Temp.IsDominated = 0;
            Temp.Prob = 0;
            Temp.Trial = 0;
            for(int j=0;j<Target;j++){
                Temp.Grid[j] = -1;
            }
            Nectar *NewBee = &Temp;
            RefreshBeeFitness(NewBee);
            Nectar *OldBee = &BeesPiont[identity];
            int judge = isDominated(NewBee,OldBee);
            if(judge == 1){
                BeesPiont[identity] = Temp;
                DeleteByStrategy();
                for(int count =0; count<Rep;count++){
                    if(Reposity[count].IsDominated == 1){
                        Reposity[count] = BeesPiont[identity];
                        break;
                    }
                }
                RepositiesDominated();
            }
            else{
                BeesPiont[identity].Trial = BeesPiont[identity].Trial + 1;
            }
        }
        identity++;
        if(identity == NP){
            identity = 0;
        }
    }
 
}
//侦察蜂运行
void ScoutBees(){
    int MaxTrialIndex = 0;
    for(int i = 1; i < NP ; i++){
        if(BeesPiont[i].Trial > BeesPiont[MaxTrialIndex].Trial){
            MaxTrialIndex = i;
        }
    }
    if(BeesPiont[MaxTrialIndex].Trial >= limit){
        Init(MaxTrialIndex);
    }
}
int main(){
    system("clear");
    int iter=0;
    printf("Please Tap your interested Formula:\n\n\n");
    //选择输入的公式
    Multi();
    srand(time(NULL));
 
    //初始化了所有的蜜源以及非支配解的存储，即初始化了所有的Repository和BeesPoint
    InitAllBees();
    //判断BeesPoint区间内的蜜蜂的支配情况
    BeesDominated();
    //将最开始的一些BeesPoint内的非支配解，放入Repository中
    IntitalReposity();
    //单纯将BeesPoint中的解复制了过去
    InitGrid();
    //由于放入的点本身也具有的PreF的计算，因此，可以进行Grid的绘画
    IntoGrid();
    //将Repository中的点插入刚刚绘画的图中去
    CalCulateRepProb();
    //计算一下当前的Repository中，容易被删除的值
    int flag = 0;
    for(iter = 0; iter<maxCycle; iter ++){
        //放出雇佣蜂，新的结果和之前的结果比较，若为非支配解，则准备放入Repository
        //1. 首先，若Repository过多，则删除一个
        //2. 将当前的新的蜜源放入进去
        //3. 判断放入后的Repository之间的非支配解之间的关系
        SendEmployedBees(iter);
        //非支配解集合计算完毕后进行重新绘图
        InitGrid();
        //将解集和重新放入新的图中
        IntoGrid();
        //重新计算所有的非支配解的置换可能性大小
        CalCulateRepProb();
        //计算BeesPoint中的可能性值，用于Onlooker使用
        CalculateProbabilities();
        //放出观察蜂，新的结果和之前的结果比较，若为非支配解，则准备放入Repository
        //1. 需要按照之前计算的改了来进行观察蜂运行的可能性
        //2. 首先，若Repository过多，则删除一个
        //3. 将当前的新的蜜源放入进去
        //4. 判断放入后的Repository之间的非支配解之间的关系
        OnlookBees();
        //重新计算Grid
        InitGrid();
        //将Repository中的解集和插入Grid中
        IntoGrid();
        //重新计算新的Prob
        CalCulateRepProb();
        //食物源停留次数过大，则刷新
        ScoutBees();
        if(flag == 100){
            flag = 0;
            printf("finished the precent: %.2f\n",100*(double)iter/(double)maxCycle);
        }
        flag ++;
    }
    // PrintBees();
    RepositiesDominated();
    //PrintReposities();
    IntoFiles();
    printf("-----------------------over---------------------");
    return 0;
}