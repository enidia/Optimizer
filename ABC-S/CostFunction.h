#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <malloc.h>
 
#define M_PI 3.1415926
#ifdef D
#ifndef T
typedef double (*FunctionCallback)(double *sol);
FunctionCallback function;
double lb, ub;
//函数预定义
double Sphere(double *sol);
double Rosenbrock(double *sol); //我的f5-再没用
double Griewank(double *sol);   //我的f3
double Rastrigin(double *sol);  //我的f2
double Elliptic(double *sol);
double SumSquare(double *sol);
double SumPower(double *sol);
double Schwefel2_22(double *sol);
double Schwefel2_21(double *sol);
double Step(double *sol);
double Exponential(double *sol);
double Quartic(double *sol);
double Schwefel2_26(double *sol);
double Alpine(double *sol);
double Levy(double *sol);
double Weierstrass(double *sol);
double Himmelblau(double *sol);
double Michalewicz(double *sol);
double NCRastrigin(double *sol);
double Ackley(double *sol); //我的f4，自己加上去的这个函数！
 
double random(double start, double end)
{
    return start + (end - start) * rand() / (RAND_MAX + 1.0);
}
double round(double r)
{
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}
char *SwitchFunction()
{
    printf("Here is the list of function name:\n");
    printf("\n1.Sphere\t 2.Elliptic\t 3.SumSquare\t 4.SumPower\t 5.Schwefel2_22\n\
    6.Schwefel2_21\t 7.Step\t 8.Exponential\t 9.Quartic\t 10.Rosenbrock\n\
    11.Rastrigin\t 12.NCRastrigin\t 13.Griewank\t 14.Schwefel2_26\t 15.Alpine\n\
    16.Levy\t 17.Weierstrass\t 18.Himmelblau\t 19.Michalewicz\t 20.Ackley\n");
    printf("\nplease insert the right ID (Default Sphere): \0");
    int choose = 1; //选择对应的数字
    scanf("%d", &choose);
    char *name;
    switch (choose)
    {
    case 1:
        function = &Sphere;
        lb = -100;
        ub = 100;
        name = (char *)malloc(sizeof("Sphere") * sizeof(char));
        strcpy(name, "Sphere");
        break;
    case 2:
        function = &Elliptic;
        lb = -100;
        ub = 100;
        name = (char *)malloc(sizeof("Elliptic") * sizeof(char));
        strcpy(name, "Elliptic");
        break;
    case 3:
        function = &SumSquare;
        lb = -10;
        ub = 10;
        name = (char *)malloc(sizeof("SumSquare") * sizeof(char));
        strcpy(name, "SumSquare");
        break;
    case 4:
        function = &SumPower;
        lb = -10;
        ub = 10;
        name = (char *)malloc(sizeof("SumPower") * sizeof(char));
        strcpy(name, "SumPower");
        break;
    case 5:
        function = &Schwefel2_22;
        lb = -10;
        ub = 10;
        name = (char *)malloc(sizeof("Schwefel2_22") * sizeof(char));
        strcpy(name, "Schwefel2_22");
        break;
    case 6:
        function = &Schwefel2_21;
        lb = -100;
        ub = 100;
        name = (char *)malloc(sizeof("Schwefel2_21") * sizeof(char));
        strcpy(name, "Schwefel2_21");
        break;
    case 7:
        function = &Step;
        lb = -100;
        ub = 100;
        name = (char *)malloc(sizeof("Step") * sizeof(char));
        strcpy(name, "Step");
        break;
    case 8:
        function = &Exponential;
        lb = -10;
        ub = 10;
        name = (char *)malloc(sizeof("Exponential") * sizeof(char));
        strcpy(name, "Exponential");
        break;
    case 9:
        function = &Quartic;
        lb = -1.28;
        ub = 1.28;
        name = (char *)malloc(sizeof("Quartic") * sizeof(char));
        strcpy(name, "Quartic");
        break;
    case 10:
        function = &Rosenbrock;
        lb = -5;
        ub = 10;
        name = (char *)malloc(sizeof("Rosenbrock") * sizeof(char));
        strcpy(name, "Rosenbrock");
        break;
    case 11:
        function = &Rastrigin;
        lb = -5.12;
        ub = 5.12;
        name = (char *)malloc(sizeof("Rastrigin") * sizeof(char));
        strcpy(name, "Rastrigin");
        break;
    case 12:
        function = &NCRastrigin;
        lb = -5.12;
        ub = 5.12;
        name = (char *)malloc(sizeof("NCRastrigin") * sizeof(char));
        strcpy(name, "NCRastrigin");
        break;
    case 13:
        function = &Griewank;
        lb = -600;
        ub = 600;
        name = (char *)malloc(sizeof("Griewank") * sizeof(char));
        strcpy(name, "Griewank");
        break;
    case 14:
        function = &Schwefel2_26;
        lb = -500;
        ub = 500;
        name = (char *)malloc(sizeof("Schwefel2_26") * sizeof(char));
        strcpy(name, "Schwefel2_26");
        break;
    case 15:
        function = &Alpine;
        lb = -10;
        ub = 10;
        name = (char *)malloc(sizeof("Alpine") * sizeof(char));
        strcpy(name, "Alpine");
        break;
    case 16:
        function = &Levy;
        lb = -10;
        ub = 10;
        name = (char *)malloc(sizeof("Levy") * sizeof(char));
        strcpy(name, "Levy");
        break;
    case 17:
        function = &Weierstrass;
        lb = -1;
        ub = 1;
        name = (char *)malloc(sizeof("Weierstrass") * sizeof(char));
        strcpy(name, "Weierstrass");
        break;
    case 18:
        function = &Himmelblau;
        lb = -5;
        ub = 5;
        name = (char *)malloc(sizeof("Himmelblau") * sizeof(char));
        strcpy(name, "Himmelblau");
        break;
    case 19:
        function = &Michalewicz;
        lb = 0;
        ub = M_PI;
        name = (char *)malloc(sizeof("Michalewicz") * sizeof(char));
        strcpy(name, "Michalewicz");
        break;
    case 20:
        function = &Ackley;
        lb = -10;
        ub = 10;
        name = (char *)malloc(sizeof("Ackley") * sizeof(char));
        strcpy(name, "Ackley");
        break;
    default:
        function = &Sphere;
        lb = -100;
        ub = 100;
        name = (char *)malloc(sizeof("Sphere") * sizeof(char));
        strcpy(name, "Sphere");
        break;
        break;
    }
    return name;
}
 
double Sphere(double *sol)
{
    int j;
    double top = 0;
    for (j = 0; j < D; j++)
    {
        top = top + sol[j] * sol[j];
    }
 
    return top;
}
double Rosenbrock(double *sol)
{
    int j;
    double top = 0;
    for (j = 0; j < D - 1; j++)
    {
        top = top + 100 * pow((sol[j + 1] - pow((sol[j]), (double)2)), (double)2) + pow((sol[j] - 1), (double)2);
    }
    return top;
}
double Griewank(double *sol)
{
    int j;
    double top1, top2, top;
    top = 0;
    top1 = 0;
    top2 = 1;
    for (j = 0; j < D; j++)
    {
        top1 = top1 + pow((sol[j]), (double)2);
        top2 = top2 * cos((((sol[j]) / sqrt((double)(j + 1))) * M_PI) / 180);
    }
    top = (1 / (double)4000) * top1 - top2 + 1;
    return top;
}
double Rastrigin(double *sol)
{
    int j;
    double top = 0;
 
    for (j = 0; j < D; j++)
    {
        top = top + (pow(sol[j], (double)2) - 10 * cos(2 * M_PI * sol[j]) + 10);
    }
    return top;
}
double Ackley(double *sol) //自己编写的这个函数！可以正确使用！
{
    int j;
    double top = 0, top1 = 0, top2 = 0;
 
    for (j = 0; j < D; j++)
    {
        top1 += (pow(sol[j], (double)2));
        top2 += cos(2 * M_PI * sol[j]);
    }
    top = (-20) * exp((-0.2) * sqrt(top1 / 30)) - exp(top2 / 30) + 20 + exp(1.0);
 
    return top;
}
double Elliptic(double *sol)
{
    int j;
    double top = 0;
 
    for (j = 0; j < D; j++)
    {
        top = top + (pow(10000000.0, (j - 1) / (D - 1)) * sol[j] * sol[j]);
    }
 
    return top;
}
double SumSquare(double *sol)
{
    int j;
    double top = 0;
 
    for (j = 0; j < D; j++)
    {
        top = top + (j * sol[j] * sol[j]);
    }
 
    return top;
}
double SumPower(double *sol)
{
    int j;
    double top = 0;
 
    for (j = 0; j < D; j++)
    {
        top = top + (pow(abs(sol[j]), j + 1));
    }
 
    return top;
}
double Schwefel2_22(double *sol)
{
    int j;
    double top1 = 0;
    double top2 = 0;
 
    for (j = 0; j < D; j++)
    {
        top1 = top1 + (abs(sol[j]));
        top2 = top2 * (abs(sol[j])); //连乘
    }
 
    return top1 + top2;
}
double Schwefel2_21(double *sol)
{
    int j;
    double top = 0;
    top = sol[0];
 
    for (j = 1; j < D; j++)
    {
        top = top > sol[j] ? top : sol[j];
    }
 
    return top;
}
double Step(double *sol)
{
    int j;
    double top = 0;
 
    for (j = 0; j < D; j++)
    {
        top = top + pow(floor(sol[j] + 0.5), 2); //像下取整
    }
 
    return top;
}
double Exponential(double *sol)
{
    int j;
    double top = 0;
 
    for (j = 0; j < D; j++)
    {
        top = top + sol[j];
    }
 
    return exp(0.5 * top);
}
double Quartic(double *sol)
{
    int j;
    double top = 0;
 
    for (j = 0; j < D; j++)
    {
        top = top + (j * pow(sol[j], 4) + random(0, 1));
    }
 
    return top;
}
double Schwefel2_26(double *sol)
{
    int j;
    double top = 0;
 
    for (j = 0; j < D; j++)
    {
        top = top + (sol[j] * sin(pow(abs(sol[j]), 0.5)));
    }
 
    top = 418.98288727243380 * D - top;
 
    return top;
}
double Alpine(double *sol)
{
    int j;
    double top = 0;
 
    for (j = 0; j < D; j++)
    {
        top = top + abs(sol[j] * sin(sol[j]) + 0.1 * sol[j]);
    }
 
    return top;
}
double Levy(double *sol)
{
    int j;
    double top = 0;
 
    for (j = 0; j < D - 1; j++)
    {
        top = top + (pow(sol[j] - 1, 2) * (1 + pow(sin(3 * M_PI * sol[j + 1]), 2)) + sin(3 * M_PI * sol[j]) + abs(sol[D - 1] - 1) * (1 + pow(sin(3 * M_PI * *sol), 2)));
    }
 
    return top;
}
double Weierstrass(double *sol)
{
    double truefit = 0, a = 0.5, b = 3, k = 20;
    double truefit1 = 0;
    double truefit2 = 0;
 
    for (int i = 0; i < D; i++)
    {
        for (int j = 0; j < k; j++)
        {
            truefit1 += pow(a, k) * cos(2 * M_PI * pow(b, k) * (sol[i] + 0.5));
            truefit -= pow(a, k) * cos(2 * M_PI * pow(b, k) * 0.5);
        }
    }
    truefit = truefit1 + D * truefit2;
    return truefit;
}
double Himmelblau(double *sol)
{
    double truefit = 0;
 
    for (int i = 0; i < D; i++)
        truefit += pow(sol[i], 4) - 16 * pow(sol[i], 2) + 5 * sol[i];
 
    return truefit / D;
}
double Michalewicz(double *sol)
{
    double truefit = 0;
 
    for (int i = 0; i < D; i++)
        truefit += sin(sol[i]) * pow(sin(i * sol[i] * sol[i] / M_PI), 20);
 
    return -truefit;
}
double NCRastrigin(double *sol)
{
    double truefit = 0;
    double y = 0;
 
    for (int i = 0; i < D; i++)
    {
        if (0.5 > abs(sol[i]))
        {
            y = sol[i];
        }
        else
        {
            y = round(2 * sol[i]) / 2;
        }
 
        truefit += y * y - 10 * cos(2 * M_PI * y) + 10;
    }
 
    return truefit;
}
#endif
#endif
 
#ifdef D
#ifdef T
typedef double (*FunctionCallback)(double *sol);
FunctionCallback function1, function2;
double lb[T], ub[T];
double Sphere(double *sol)
{
    int j;
    double top = 0;
    for (j = 0; j < D; j++)
    {
        top = top + sol[j] * sol[j];
    }
 
    return top;
}
double Rosenbrock(double *sol)
{
    int j;
    double top = 0;
    for (j = 0; j < D - 1; j++)
    {
        top = top + 100 * pow((sol[j + 1] - pow((sol[j]), (double)2)), (double)2) + pow((sol[j] - 1), (double)2);
    }
    return top;
}
char *Multi()
{
    function1 = &Sphere;
    lb[0] = -100;
    ub[0] = 100;
    char* name1 = (char *)malloc(sizeof("Sphere+") * sizeof(char));
    strcpy(name1, "Sphere+");
    function2 = &Rosenbrock;
    lb[1] = -5;
    ub[1] = 10;
    char* name2 = (char *)malloc(sizeof("Rosenbrock") * sizeof(char));
    strcpy(name2, "Rosenbrock");
    strcat(name1,name2);
    return name1;
}
#endif
#endif
 
/*
本算式将会返回function作为对应的单目标的计算的函数，lb，ub为此函数的上下界以及重新定义M_PI
M_PI 3.1415926
lb 函数下界
ub 函数上界
function() 选择的函数功能
*/