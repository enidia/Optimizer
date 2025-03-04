#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<malloc.h>
#include<math.h>
#include<float.h>
#include<io.h>
#include<direct.h>
 
#define Dimension 20 //表示X集和的维度
#define Target 2//表示解的维度
 
 
#define M_PI 3.1415926
#define NP 200 //将要搜索的狼群的数量
#define Rep 100 //要获取的支配解的集合
#define LowRep Rep-1 //最小的Rep数量，低于此不进行删除
#define maxCycle 10000 //最大迭代次数
#define nGrid 10 //单个元素的网格数量
#define Alpha 0.1 //网格的膨胀指数
#include "CostFunction.h"
 
typedef struct Wolf
{
    double Position[Dimension];
    double Fitness[Target];
    double Prob;
    int Grid[Target];
    int IsDominated;
} Wolf;
Wolf wolf[NP],Repository[Rep];
Wolf AlphaWolf,BetaWolf,ThetaWolf;
double Grid[Target][nGrid+1];
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
/* 打印狼群
 * 输入确定的狼来打印，或是展示出所有的狼的情况
 * 1.输出所有狼群当前的存储状态
*/
void PrintWolf(int flag){
    if(flag == -1) {
        for (int i = 0; i < NP; i++) {
            printf("\n\n");
            printf("-------------------------------\n");
            printf("No.%d\n", i);
            for (int j = 0; j < Dimension; j++) {
                printf("Dimension %d : %.20f\t", j, wolf[i].Position[j]);
            }
            printf("\n");
            for (int j = 0;j<Target;j++){
                printf("Target %d : %.20f\t",j,wolf[i].Fitness[j]);
            }
            printf("\n");
            for(int j=0;j<Target;j++){
                printf("Grid %d : %d\t",j,wolf[i].Grid[j]);
            }
            printf("\n The prob is %.20f\n",wolf[i].Prob);
            printf("\n The Dominaton Relationship is : %d\n",wolf[i].IsDominated);
            printf("-------------------------------\n");
            printf("\n\n");
        }
    }else
    {
        printf("\n\n");
        printf("-------------------------------\n");
        printf("No.%d\n", flag);
        for (int j = 0; j < Dimension; j++) {
            printf("Dimension %d : %.20f\t", j, wolf[flag].Position[j]);
        }
        printf("\n");
        for (int j = 0;j<Target;j++){
            printf("Target %d : %.20f\t",j,wolf[flag].Fitness[j]);
        }
        printf("\n");
        for(int j=0;j<Target;j++){
            printf("Grid %d : %d\t",j,wolf[flag].Grid[j]);
        }
        printf("\n The prob is %.20f\n",wolf[flag].Prob);
        printf("\n The Dominaton Relationship is : %d\n",wolf[flag].IsDominated);
        printf("-------------------------------\n");
        printf("\n\n");
    } 
}
/* 打印非支配解
 * 1.输出所有非支配解当前的存储状态
*/
void PrintRepository(){
    for (int i = 0; i < Rep; i++) {
        if(wolf[i].IsDominated == 0){
            printf("\n\n");
            printf("-------------------------------\n");
            printf("No.%d\n", i);
            for (int j = 0; j < Dimension; j++) {
                printf("Dimension %d : %.20f\t", j, wolf[i].Position[j]);
            }
            printf("\n");
            for (int j = 0;j<Target;j++){
                printf("Target %d : %.20f\t",j,wolf[i].Fitness[j]);
            }
            printf("\n");
            for(int j=0;j<Target;j++){
                printf("Grid %d : %d\t",j,wolf[i].Grid[j]);
            }
            printf("\n The prob is %.20f\n",wolf[i].Prob);
            printf("-------------------------------\n");
            printf("\n\n");
        }
    }
}
/*本函数主要用于打印Repository数组中的情况
输出项如下:
1. 输出Repository内的所有数据项的Position
2. 输出Repository内的所有的Fitness
3. 输出Repository内所有的点的网格位置
*/
void IntoFiles(){
    FILE *pointPosition, *pointFitness, *pointGrid;
    char *Source = "./Result";
    if(_access(Source ,0) == -1){
        _mkdir(Source);
    }
    char positionFile[200];
    char fitnessFile[200];
    char gridFile[200];
    strcpy(positionFile,Source);
    strcpy(fitnessFile,Source);
    strcpy(gridFile,Source);
    strcat(positionFile,"\\\\Position.data");
    strcat(fitnessFile,"\\\\Fitness.data");
    strcat(gridFile,"\\\\Grid.data");
    if((pointPosition = fopen(positionFile,"w+")) == NULL){
        printf("Error to open the File Position.data\n");
    }
    if((pointFitness = fopen(fitnessFile,"w+")) == NULL){
        printf("Error to open the File Fitness.data\n");
    }
    if((pointGrid = fopen(gridFile,"w+")) == NULL){
        printf("Error to open the File Grid.data\n");
    }
    if(pointPosition != NULL && pointFitness != NULL && pointGrid != NULL){
        for(int i = 0; i < Rep; i++){
            if(Repository[i].IsDominated == 0){
                for(int j = 0; j < Dimension-1 ; j++){
                    fprintf(pointPosition,"%.20f\t",Repository[i].Position[j]);
                }
                fprintf(pointPosition,"%.20f\n",Repository[i].Position[Dimension-1]);
                for(int j = 0; j <Target-1; j++){
                    fprintf(pointFitness,"%.20f\t",Repository[i].Fitness[j]);
                    fprintf(pointGrid,"%d\t",Repository[i].Grid[j]);
                }
                fprintf(pointFitness,"%.20f\n",Repository[i].Fitness[Target-1]);
                fprintf(pointGrid,"%d\n",Repository[i].Grid[Target-1]);
            }
        }
        fclose(pointPosition);
        fclose(pointFitness);
        fclose(pointGrid);
    }
    else{
        printf("The Files can not open, please check the state of the Files");    }
}
 
/* 初始化一个狼群结构体变量
 * 输入:index为狼的下标
 * 1.随机赋予index号狼它的position
 * 2.根据新的position计算它的fitness
*/
void Init(int index){
    for(int i=0;i<Dimension;i++){
        double r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
        (wolf+index)->Position[i] = r*(ub[i]-lb[i])+lb[i];
    }
    for(int j=0;j<Target;j++){
        (wolf+index)->Fitness[j] = function[j]((wolf+index)->Position);
    }
    (wolf+index)->Prob = 0;
    (wolf+index)->IsDominated =0;
    for(int i=0;i<Target;i++){
        (wolf+index)->Grid[i] = -1;
    }
}
/* 初始化非支配解集合
输入index为狼的下标
1.将Position重置为0
2.将Fitness重置为0
3.将Prob重置为0
4.将IsDominated重置为1
5.将Grid重置为-1
*/
void InitRep(int index){
    Wolf *repotemp = Repository;
    for(int j = 0;j<Dimension;j++){
        (repotemp+index)->Position[j] = 0;
    }
    for(int j=0; j<Target;j++){
        (repotemp+index)->Fitness[j] = 0;
    }
    (repotemp+index)->Prob = 0;
    (repotemp+index)->IsDominated = 1;
    for(int i = 0;i<Target;i++){
        (repotemp+index)->Grid[i] = -1;
    }
}
/* 初始化所有的狼群
 * 1.输出所有的狼群的初始化
 * 2.输出所有的非支配解的初始化
*/
void InitAll(){
    for(int i=0;i<NP;i++){
        Init(i);
    }
    for(int j=0;j<Rep;j++){
        InitRep(j);
    }
}
/* 更新狼群算法的Position
 * 输入iter迭代数目
 * 需要在AlphaWolf，BetaWolf以及ThetaWolf中填入数据
 * 1.更新狼群的position
 * 2.更新狼群的Fitness
*/
void CalculateWolf(int i, double a){
    double A1 = ((double)2*a*(((double)rand() / ((double)(RAND_MAX) + (double)(1)))))-a;
    double C1 = (double)(2)*(((double)rand() / ((double)(RAND_MAX) + (double)(1))));
    double A2 = ((double)2*a*(((double)rand() / ((double)(RAND_MAX) + (double)(1)))))-a;
    double C2 = (double)(2)*(((double)rand() / ((double)(RAND_MAX) + (double)(1))));
    double A3 = ((double)2*a*(((double)rand() / ((double)(RAND_MAX) + (double)(1)))))-a;
    double C3 = (double)(2)*(((double)rand() / ((double)(RAND_MAX) + (double)(1))));
    Wolf Temp1,Temp2,Temp3;
    for(int j=0;j<Dimension;j++){
        Temp1.Position[j] = fabs(C1*AlphaWolf.Position[j] - wolf[i].Position[j]);
        Temp2.Position[j] = fabs(C2*BetaWolf.Position[j] - wolf[i].Position[j]);
        Temp3.Position[j] = fabs(C3*ThetaWolf.Position[j] - wolf[i].Position[j]);
        Temp1.Position[j] = AlphaWolf.Position[j] - Temp1.Position[j]*A1;
        Temp2.Position[j] = BetaWolf.Position[j] - Temp2.Position[j]*A2;
        Temp3.Position[j] = ThetaWolf.Position[j] - Temp3.Position[j]*A3;
        wolf[i].Position[j] = (Temp1.Position[j] + Temp2.Position[j] + Temp3.Position[j])/(double)(3);
    }
    for(int j=0;j<Dimension;j++){
        if(wolf[i].Position[j] < lb[j]){
            wolf[i].Position[j] = lb[j];
        }
        if(wolf[i].Position[j] > ub[j]){
            wolf[i].Position[j] = ub[j];
        }
    }
    for(int k=0;k<Target;k++){
        wolf[i].Fitness[k] = function[k](wolf[i].Position);
    }
}
/* 输入Obj1 Obj2 两个 Wolf结构的指针，以判断支配关系
1. 若FLAG = 0，则表示，Fitness越小越好
2. 若FLAG = 1，则表示，Fitness越大越好
3. 返回1，表示Obj2被Obj1支配
4. 返回0，则表示Obj2和Obj1没有支配关系
*/
int isDominated(Wolf* Obj1, Wolf*Obj2){
    int flag = 0;
    if(FLAG == 0){
        for(int i=0;i<Target;i++){
            if(Obj1->Fitness[i] > Obj2->Fitness[i]){
                return 0;
            }
            if(Obj1->Fitness[i] < Obj2->Fitness[i]){
                flag = 1;
            }
        }
        if(flag == 1){
            return 1;
        }
    }else if(FLAG == 1){
        for(int i=0;i<Target;i++){
            if(Obj1->Fitness[i] < Obj2->Fitness[i]){
                return 0;
            }
            if(Obj1->Fitness[i] > Obj2->Fitness[i]){
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
/* 判断狼群之间的支配关系，用于处理接下来的输入非支配解的初始操作
1. 被支配的狼将会使得其Dominated = 1
2. 非支配的狼将会保持原来的 Dominated = 0
*/
void WolfDominated(){
    Wolf* temp = wolf;
    for(int i=0;i<NP;i++){
        for(int j =i+1;j<NP;j++){
            int idomj = isDominated((temp+i),(temp+j));
            int jdomi = isDominated((temp+j),(temp+i));
            if(idomj == 1){
                (temp+j)->IsDominated = 1;
            }
            if(jdomi == 1){
                (temp+i)->IsDominated = 1;
            }
        }
    }
}
/* 将狼群中的非支配解放入Repository中
1. 保证输入的非支配解不会超过非支配解集合的上限
2，输入过多的非支配解，则只取得前面的非支配解
3. 若遍历完所有的Repository还有多余，则直接结束
*/
void InitialRepository(){
    int i = 0, j =0;
    while (1)
    {
        if(i >= Rep || j >= NP){
            break;
        }
        if(wolf[j].IsDominated == 0){
            Repository[i] = wolf[j];
            i++;
        }
        j++;
    }
}
/* 初始化Grid
1. 通过当前的非支配解集合来获得当前的网格
2. 获取网格上下限后使用Alpha参数进行膨胀
3. 网格将会是划分的网格多出1
*/
void InitGrid(){
    double Low[Target] , Uper[Target];
    for(int i = 0;i<Target;i++){
        Low[i] = Repository[0].Fitness[i];
        Uper[i] = Repository[0].Fitness[i];
    }
    for(int i = 0; i < Rep;i++){
        for(int j = 0;j < Target;j++){
            if(Repository[i].IsDominated == 0){
                if(Low[j] > Repository[i].Fitness[j]){
                    Low[j] = Repository[i].Fitness[j];
                }
                if(Uper[j] < Repository[i].Fitness[j]){
                    Uper[j] = Repository[i].Fitness[j];
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
/* 将Repository内部的非支配解放入Grid
输入: PreF[] = {Py1 Py2 ... Pyn}, 且IsDominated = 0 , 即表示当前的非支配解可用
输出: Grid[] = {1 2 ... n}
*/
void IntoGrid(){
    for(int i=0 ; i < Rep; i++){
        for(int j = 0; j < Target ; j ++){
            for(int k =0;k< nGrid ;k++){
                if(Repository[i].IsDominated == 0){
                    if(Grid[j][k] < Repository[i].Fitness[j] && Repository[i].Fitness[j] < Grid[j][k+1]){
                        Repository[i].Grid[j] = k;
                        break;
                    }
                }
            }
        }
    }
}
/* 打印出网格的区间
1. 对于每一个Target都将有一个打印的网格区间
2. 横向为同一个Target 的网格
*/
void PrintGrid(){
    for(int i = 0;i<Target;i++){
        printf("the Colum is : %d\n",i+1);
        for(int j =0;j<(nGrid+1);j++){
            printf("%e\t",Grid[i][j]);
        }
        printf("\n\n");
    }
}
/* 判断非支配解集合内部的支配关系
1. 被支配的解将会使得其IsDominated = 1
2. 若没有被支配，则为 0
*/
void RepositoryDominated(){
    Wolf* repotemp = Repository;
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
        if(Repository[i].IsDominated == 1){
            InitRep(i);
        }
    }
}
/* 利用网格，计算Repository被删除的可能性
这里做删除的可能性的决策，决定了哪个Repository更可能被删除
即:
1. Repository.Prob 输入值，利用这个值来进行删除
2. 考虑到同时使用网格和方差，即利用网格的相减法来代表其方差即可
3. 在计算概率的时候使用了CalculateFitness，即BeesPoint在Onlooker是相同的概率的算法
*/
void CalCulateRepProb(){
    for(int i =0;i<Rep;i++){
        Repository[i].Prob = 0;
    }
    for(int i = 0 ;i < Rep-1;i++){
        for(int j =i+1 ; j< Rep;j++){
            if(Repository[i].IsDominated == 0 && Repository[j].IsDominated == 0){
                for(int k = 0; k<Target;k++){
                    Repository[i].Prob = Repository[i].Prob + fabs(Repository[i].Grid[k] - Repository[j].Grid[k]);
                    Repository[j].Prob = Repository[j].Prob + fabs(Repository[i].Grid[k] - Repository[j].Grid[k]);
                }
            }
        }
    }
    for(int i =0 ;i<Rep;i++){
        if(Repository[i].IsDominated == 0) {
            Repository[i].Prob = 1.0/(Repository[i].Prob + 1);
        }
    }
}
/* 删除策略，现在使用的是类似网格方差的策略进行删除
 首先求出网格，再然后求出网格之间的方差（由于只需要进行大小比较，不做具体的方差计算)
*/
void DeleteByStrategy(){
    for(int i =0;i<Rep;i++) {
        if(Repository[i].IsDominated == 1) {
            double r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)));
            if(r > 0.5) {
                return;
            } else{
                break;
            }
        }
    }
    int MAXIndex = 0;
    double MAXProb = Repository[0].Prob;
    for(int i = 0; i<Rep;i++){
        if(Repository[i].Prob > MAXProb){
            MAXIndex = i;
            MAXProb = Repository[i].Prob;
        }
    }
    int Count = 0;
    for(int i=0;i<Rep;i++){
        if(Repository[i].IsDominated == 0){
            Count++;
        }
    }
    if(Count>LowRep){
        InitRep(MAXIndex);
    }
}
/* 寻找在多目标解下的Leader，同样使用Prob最小的来解决，由于最大的为非支配解容易删除的解，则使用相对小的Prob值，即不易删除的点
1. 寻找Repository里面的Prob相对小的解
2. 最小的为 Alpha，次小的为Beta，第三小的为Theta
*/
void FindLeader(){
    for(int i=1; i<Rep;i++){
        for(int j=0;j<Rep-i;j++){
            if(Repository[j].Prob > Repository[j+1].Prob){
                Wolf Temp = Repository[j];
                Repository[j] = Repository[j+1];
                Repository[j+1] = Temp;
            }
        }
    }//排序，根据网格方差，Prob 方差
    //方差最大的作为Alpha
    int count = 0;
    for(int i=0;i<Rep;i++){
        if(Repository[i].IsDominated == 0){
            AlphaWolf = Repository[i];
            count = i;
            break;
        }
    }
    //方差次大作为Beta
    BetaWolf = AlphaWolf;//----1=Alpha----2=Beta,Thata-----////-------1=Alpha,Beta,Theta-----////----1=Alpha--2=Beta-----3=Theta-----//
    ThetaWolf = AlphaWolf;
    //200 1个非支配解如何取得3匹狼？
    for(int i=count;i<Rep;i++){
        if(Repository[i].IsDominated == 0 && count != i){
            BetaWolf = Repository[i];
            count = i;
            break;
        }
    }
    for(int i=count;i<Rep;i++){
        if(Repository[i].IsDominated == 0 && count != i){
            ThetaWolf = Repository[i];
            break;
        }
    }
}
 
int main(){
    system("clear");
    int iter=0;
    printf("Please Tap your interested Formula:\n\n\n");
 
    char *func = Multi();
 
    srand(time(NULL));
    InitAll();
    WolfDominated();
    InitialRepository();
    InitGrid();
    IntoGrid();
    CalCulateRepProb();
 
    int flag = 0;
    for(iter = 0; iter<maxCycle; iter ++){
        FindLeader();
        double a = (double)(2) - (double)(2)*(double)iter/(double)maxCycle;//a
        for(int i=0;i<NP;i++){
            Wolf Oldone = wolf[i];
            Wolf *Old =  &Oldone;
            CalculateWolf(i,a);
            int judge = isDominated(&wolf[i],Old);
            if(judge == 1){
                DeleteByStrategy();
                for(int count = 0; count<Rep; count++){
                    if(Repository[count].IsDominated == 1){
                        Repository[count] = wolf[i];
                        Repository[count].Prob = 0;
                        Repository[count].IsDominated = 0;
                        break;
                    }
                }
            }else{
                // wolf[i] = Oldone;这里写量子改进
            }
        }
        // printf("the rep %d\n",iter);
        // PrintGrid();
        RepositoryDominated();
        InitGrid();
        IntoGrid();
        CalCulateRepProb();
        // PrintRepository();
        if(flag == 100){
            flag = 0;
            printf("finished the precent: %.2f\n",100*(double)iter/(double)maxCycle);
        }
        flag ++;
    }
    RepositoryDominated();
    // PrintWolf(-1);
    IntoFiles();
    printf("the formula you have chosen is :%s\n",func);
    printf("-----------------------over---------------------");
    return 0;
}