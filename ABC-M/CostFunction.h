#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <malloc.h>
 
#define M_PI 3.1415926
int FLAG = 0;
typedef double (*FunctionCallback)(double *sol);
FunctionCallback function[Target];
double lb[Dimension], ub[Dimension];
 
double random(double start, double end)
{
    return start + (end - start) * rand() / (RAND_MAX + 1.0);
}
double round(double r)
{
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}
 
 
 
/*------------------------------ZDT series for the test function--------------------------------*/
double F_X1(double *sol){
    return sol[0];
}
double F_ZDT6(double *sol){
    double result = exp((-4)*sol[0]);
    result = result * pow(sin(6*M_PI*sol[0]),6);
    result = 1.0 - result;
    return result;
}
double Sub_g(double *sol){
    double result = 0;
    for(int i=1;i<Dimension;i++){
        result = result + sol[i]/(double)(Dimension-1);
    }
    result = 1 + result * 9;
    return result;
}
double Sub_g2(double *sol){
    double result = 0;
    for(int i =1;i<Dimension;i++){
        result = result + sol[i]*sol[i] - 10*cos(4*M_PI*sol[i]);
    }
    result = result + 10*(Dimension-1) + 1;
    return result;
}
double Sub_g3(double *sol){
    double result = 0;
    for(int i =1;i<Dimension;i++){
        result = result + sol[i]/(double)(Dimension-1);
    }
    result = pow(result, 0.25);
    result = 1.0 + result*9;
    return result;
}
double F_gsqrt(double *sol){
    double g = Sub_g(sol);
    double h = 1.0 - sqrt(sol[0]/g);
    double result = g * h;
    return result;
}
double F_gsquare(double *sol){
    double g = Sub_g(sol);
    double h = 1 - pow((sol[0]/g),2);
    double result = g * h;
    return result;
}
double F_sin(double *sol){
    double g = Sub_g(sol);
    double h = 1 - sqrt(sol[0]/g) - (sol[0]/g)*sin(10*M_PI*sol[0]);
    double result = g * h;
    return result;
}
double F_mu(double *sol){
    double g = Sub_g2(sol);
    double h = 1 - sqrt(sol[0]/g);
    double result = g*h;
    return result;
}
double F_Ssqrt(double *sol){
    double g = Sub_g3(sol);
    double f1 = exp((-4)*sol[0]);
    f1 = f1 * pow(sin(6*M_PI*sol[0]),6);
    f1 = 1.0 - f1;
    double h = 1 - pow((f1/g),2);
    double result = h * g;
    return result;
}
/*----------------------------this is the over for ZDT series--------------------*/
 
/*----------------------------start the SCH and FON------------------------------*/
double SCHF1(double *sol){
    double result;
    result = sol[0]*sol[0];
    return result;
}
double SCHF2(double *sol){
    double result;
    result = (sol[0]-2)*(sol[0]-2);
    return result;
}
 
double FON1(double *sol){
    double result;
    for(int i=0;i<Dimension;i++){
        result = result + (sol[i] - ((double)1/(double)sqrt((double)Dimension)))*(sol[i] - ((double)1/(double)sqrt((double)Dimension)));
    }
    result = -result;
    result = exp(result);
    result = 1.0-result;
    return result;
}
 
double FON2(double *sol){
    double result;
    for(int i=0;i<Dimension;i++){
        result = result + (sol[i] + ((double)1/(double)sqrt((double)Dimension)))*(sol[i] + ((double)1/(double)sqrt((double)Dimension)));
    }
    result = -result;
    result = exp(result);
    result = 1.0-result;
    return result;
}
/*----------------------------end the SCH and FON--------------------------------*/
 
/*----------------------------start the DTLZ series------------------------------*/
double Sub_gD(double *sol){
    double result = 0;
    for(int i=Target-1;i<Dimension;i++){
        result = result + ((sol[i] - 0.5)*(sol[i] - 0.5) - cos(20*M_PI*(sol[i] - 0.5)));
    }
    result = result + 10.0;
    result = result * 100.0;
    return result;
}
double DTLZ1F1(double *sol){
    double g = Sub_gD(sol);
    double result = (1+g)*sol[0]*sol[1]/(double)2;
    return result;
}
double DTLZ1F2(double *sol){
    double g = Sub_gD(sol);
    double result = sol[0]*(1-sol[1])*(1+g)/(double)2;
    return result;
}
double DTLZ1F3(double *sol){
    double g = Sub_gD(sol);
    double result = ((double)1-sol[0])*(1+g)/(double)2;
    return result;
}
double Sub_gD2(double *sol){
    double result = 0;
    for(int i = Target - 1;i<Dimension;i++){
        result = result + (sol[i]-0.5)*(sol[i]-0.5);
    }
    return result;
}
double DTLZ2F1(double *sol){
    double g = Sub_gD2(sol);
    double result = cos(M_PI*sol[0]/(double)2)*cos(M_PI*sol[1]/(double)2)*((double)1+g);
    return result;
}
double DTLZ2F2(double *sol){
    double g = Sub_gD2(sol);
    double result = cos(M_PI*sol[0]/(double)2)*sin(M_PI*sol[1]/(double)2)*((double)1+g);
    return result;
}
double DTLZ2F3(double *sol){
    double g = Sub_gD2(sol);
    double result = sin(M_PI*sol[0]/(double)2)*(1+g);
    return result;
}
double DTLZ3F1(double *sol){
    double g = Sub_gD(sol);
    double result = (1+g)*cos(M_PI*sol[0]/(double)2)*cos(M_PI*sol[1]/(double)2);
    return result;
}
double DTLZ3F2(double *sol){
    double g = Sub_gD(sol);
    double result = (1+g)*cos(M_PI*sol[0]/(double)2)*sin(M_PI*sol[1]/(double)2);
    return result;
}
double DTLZ3F3(double *sol){
    double g = Sub_gD(sol);
    double result = (1+g)*sin(M_PI*sol[0]/(double)2);
    return result;
}
double DTLZ4F1(double *sol){
    double g = Sub_gD2(sol);
    double alpha = 100.0;//from the paper
    double result = (1+g)*cos(pow(sol[0],alpha)*M_PI/(double)2)*cos(pow(sol[1],alpha)*M_PI/(double)2);
    return result;
}
double DTLZ4F2(double *sol){
    double g = Sub_gD2(sol);
    double alpha = 100.0;//from the paper
    double result = (1+g)*cos(pow(sol[0],alpha)*M_PI/(double)2)*sin(pow(sol[1],alpha)*M_PI/(double)2);
    return result;
}
double DTLZ4F3(double *sol){
    double g = Sub_gD2(sol);
    double alpha = 100.0;//from the paper
    double result = (1+g)*sin(pow(sol[0],alpha)*M_PI/(double)2);
    return result;
}
double DTLZ7F1(double *sol){
    return sol[0];
}
double DTLZ7F2(double *sol){
    return sol[1];
}
double DTLZ7F3(double *sol){
    double result = 0;
    for(int i= 2;i<Dimension;i++){
        result = result + sol[i];
    }
    result = 1.0 + result * 0.9;
    double temp = (1.0+sin(3.0*M_PI*sol[0]))*sol[0]/(1.0+result) + (1.0+sin(3.0*M_PI*sol[1]))*sol[1]/(1.0+result);
    temp = 3.0 - temp;
    return (1.0+result)*temp;
}
double Theta(double *sol,int iter){
    double g = Sub_gD2(sol);
    double result = (double)1+2*g*sol[iter];
    result = result*M_PI/(4*(1+g));
    return result;
}
double DTLZ5F1(double *sol){
    double g = Sub_gD2(sol);
    double x1 = Theta(sol,0);
    double x2 = Theta(sol,1);
    double result = ((double)1+g)*cos(x1*M_PI/(double)2)*cos(x2*M_PI/(double)2);
    return result;
}
double DTLZ5F2(double *sol){
    double g = Sub_gD2(sol);
    double x1 = Theta(sol,0);
    double x2 = Theta(sol,1);
    double result = ((double)1+g)*cos(x1*M_PI/(double)2)*sin(x2*M_PI/(double)2);
    return result;
}
double DTLZ5F3(double *sol){
    double g = Sub_gD2(sol);
    double x1 = Theta(sol,0);
    double result = ((double)1+g)*sin(x1*M_PI/(double)2);
    return result;
}
/*-----------------------------end the DTLZ series-------------------------------*/
 
/*-----------------------------start the DEB and KUR-----------------------------*/
double DEBF1(double *sol){
    return sol[0];
}
double DEBF2(double *sol){
    double result = sin(8*M_PI*sol[0])*sol[0]/(1.0+10*sol[1]);
    double temp = 1.0 - (sol[0]/(1.0+10*sol[1]))*(sol[0]/(1.0+10*sol[1]));
    result = temp - result;
    result = result*(1.0+10*sol[1]);
    return result;
}
double KURF1(double *sol){
    double result = 0;
    for(int i=0;i<Dimension-1;i++){
        result = result +(-10.0*exp((-0.2)*sqrt(sol[i]*sol[i] + sol[i+1]*sol[i+1])));
    }
    return result;
}
double KURF2(double *sol){
    double result = 0;
    for(int i=0;i<Dimension;i++){
        result = result + pow(fabs(sol[i]),0.8)+5.0*sin(pow(sol[i],3.0));
    }
    return result;
}
/*-------------------------------end the DEB and KUR-----------------------------*/
/*-------------------------------start Belegundu and Binh------------------------*/
double BelegunduF1(double *sol){
    if(sol[0]+sol[1] <= 7.0&&sol[1]-sol[0]<=1.0){
        return -2.0*sol[0]+sol[1];
    }
    else{
        return DBL_MAX;
    }
}
double BelegunduF2(double *sol){
    if(sol[0]+sol[1] <= 7.0&&sol[1]-sol[0]<=1.0){
        return 2.0*sol[0]+sol[1];
    }
    else{
        return DBL_MAX;
    }
}
double BinhF1(double *sol){
    double Judge1 = 0;
    double Judge2 = 0;
    Judge1 = (sol[0] - 5.0)*(sol[0] -5.0) + sol[1]*sol[1] - 25;
    Judge2 = -(sol[0] - 8.0)*(sol[0] -8.0)-(sol[1] - 3.0)*(sol[1] -3.0) + 7.7;
    if(Judge1 <=0 && Judge2 <=0){
        return 4.0*sol[0]*sol[0] + 4.0*sol[1]*sol[1];
    }
    else{
        return DBL_MAX;
    }
}
double BinhF2(double *sol){
    double Judge1 = 0;
    double Judge2 = 0;
    Judge1 = (sol[0] - 5.0)*(sol[0] -5.0) + sol[1]*sol[1] - 25;
    Judge2 = -(sol[0] - 8.0)*(sol[0] -8.0)-(sol[1] - 3.0)*(sol[1] -3.0) + 7.7;
    if(Judge1 <=0 && Judge2 <=0){
        return (sol[0] - 5.0)*(sol[0] - 5.0) + (sol[1] - 5.0)*(sol[1] -5.0);
    }
    else{
        return DBL_MAX;
    }
}
/*---------------------------------end the Belegundu and Binh--------------------*/
/*---------------------------------start the Srinivas and Tanaka-----------------*/
double SrinivasF1(double *sol){
    double Judge1 = sol[0]*sol[0] + sol[1]*sol[1] -225.0;
    double Judge2 = sol[0] - 3.0*sol[1] + 10.0;
    if(Judge1 <=0 && Judge2 <= 0){
        return (sol[0] - 2.0)*(sol[0] - 2.0) + (sol[1] - 1.0)*(sol[1] - 1.0) + 2.0;
    }
    else
    {
        return DBL_MAX;
    }  
}
double SrinivasF2(double *sol){
    double Judge1 = sol[0]*sol[0] + sol[1]*sol[1] -225.0;
    double Judge2 = sol[0] - 3.0*sol[1] + 10.0;
    if(Judge1 <=0 && Judge2 <= 0){
        return 9.0*sol[0] - (sol[1]-1.0)*(sol[1]*1.0);
    }
    else
    {
        return DBL_MAX;
    }  
}
double TanakaF1(double *sol){
    double Judge1 = -sol[0]*sol[0]-sol[1]*sol[1] + 1.0-0.1*cos(16.0*atan(sol[0]/sol[1]));
    double Judge2 = (sol[0] - 0.5)*(sol[0] - 0.5) + (sol[1] -0.5)*(sol[1] -0.5) - 0.5;
    if(Judge1 <=0 && Judge2 <= 0){
        return sol[0];
    }
    else
    {
        return DBL_MAX;
    }  
}
double TanakaF2(double *sol){
    double Judge1 = -sol[0]*sol[0]-sol[1]*sol[1] + 1.0-0.1*cos(16.0*atan(sol[0]/sol[1]));
    double Judge2 = (sol[0] - 0.5)*(sol[0] - 0.5) + (sol[1] -0.5)*(sol[1] -0.5) - 0.5;
    if(Judge1 <=0 && Judge2 <= 0){
        return sol[1];
    }
    else
    {
        return DBL_MAX;
    }  
}
/*---------------------------------end the Srinivas and Tanaka-------------------*/
/*---------------------------------start the Jimenez and Obayashi----------------*/
double JimenezF1(double *sol){
    double Judge1 = sol[0] + 4.0*sol[1] - 100.0;
    double Judge2 = 3.0*sol[0] + 2.0*sol[1] - 150.0;
    double Judge3 = 200.0 - 5.0*sol[0] - 3.0*sol[1];
    double Judge4 = 75.0 - 2.0*sol[0] -8.0*sol[1];
    if(Judge1 <=0 && Judge2 <=0&& Judge3 <=0 && Judge4 <=0){
        return 5.0*sol[0] + 3.0*sol[1];
    }
    else
    {
        return 0;
    }  
}
double JimenezF2(double *sol){
    double Judge1 = sol[0] + 4.0*sol[1] - 100.0;
    double Judge2 = 3.0*sol[0] + 2.0*sol[1] - 150.0;
    double Judge3 = 200.0 - 5.0*sol[0] - 3.0*sol[1];
    double Judge4 = 75.0 - 2.0*sol[0] -8.0*sol[1];
    if(Judge1 <=0 && Judge2 <=0 && Judge3 <=0 && Judge4 <=0){
        return 2.0*sol[0] + 8.0*sol[1];
    }
    else
    {
        return 0;
    }  
}
double ObayashiF1(double *sol){
    double Judge = sol[0]*sol[0] + sol[1]*sol[1];
    if(Judge <= 0){
        return sol[0];
    }
    else
    {
        return 0;
    }
}
double ObayashiF2(double *sol){
    double Judge = sol[0]*sol[0] + sol[1]*sol[1];
    if(Judge <= 0){
        return sol[1];
    }
    else
    {
        return 0;
    }
}
/*----------------------------------end the Jimenez and Obayashi-----------------*/
/*---------------------------------start the SRN TNK OSY and CONSTR--------------*/
double SRNF1(double *sol){
    double Judge1 = sol[0]*sol[0] + sol[1]*sol[1];
    double Judge2 = sol[0] - 3.0*sol[1] + 10.0;
    if(Judge1 <= 225 && Judge2 <= 0){
        return 2.0+(sol[0] - 2.0)*(sol[0] - 2.0) + (sol[1] - 1.0)*(sol[1] - 1.0);
    }
    else
    {
        return DBL_MAX;
    }
}
double SRNF2(double *sol){
    double Judge1 = sol[0]*sol[0] + sol[1]*sol[1];
    double Judge2 = sol[0] - 3.0*sol[1] + 10.0;
    if(Judge1 <= 225 && Judge2 <= 0){
        return 9.0*sol[0] - (sol[1] - 1.0)*(sol[1] - 1.0);
    }
    else
    {
        return DBL_MAX;
    }
}
double TNKF1(double *sol){
    double Judge1 = sol[0]*sol[0] + sol[1]*sol[1] -1.0 -0.1*cos(16.0*atan(sol[0]/sol[1]));
    double Judge2 = (sol[0] - 0.5)*(sol[0] - 0.5) + (sol[1] -0.5)*(sol[1] -0.5);
    if(Judge1 >= 0 && Judge2 <= 0.5){
        return sol[0];
    }
    else
    {
        return DBL_MAX;
    }
}
double TNKF2(double *sol){
    double Judge1 = sol[0]*sol[0] + sol[1]*sol[1] -1.0 -0.1*cos(16.0*atan(sol[0]/sol[1]));
    double Judge2 = (sol[0] - 0.5)*(sol[0] - 0.5) + (sol[1] -0.5)*(sol[1] -0.5);
    if(Judge1 >= 0 && Judge2 <= 0.5){
        return sol[1];
    }
    else
    {
        return DBL_MAX;
    }
}
double OSYF1(double *sol){
    double Judge1 = sol[0] + sol[1] - 2.0;
    double Judge2 = 6.0 - sol[0] - sol[1];
    double Judge3 = 2.0 - sol[1] + sol[0];
    double Judge4 = 2.0 - sol[0] + 3.0*sol[1];
    double Judge5 = 4.0 - (sol[2] - 3.0)*(sol[2] - 3.0) - sol[3];
    double Judge6 = (sol[4] - 3.0)*(sol[4] - 3.0) + sol[5] - 4.0;
    if(Judge1 >= 0 && Judge2 >= 0 && Judge3 >= 0 && Judge4 >= 0&&Judge5>=0&&Judge6>=0){
        return -25.0*(sol[0] - 2.0)*(sol[0] - 2.0) - (sol[1] - 2.0)*(sol[1] - 2.0) - (sol[2] - 1.0)*(sol[2] - 1.0) - (sol[3] - 4.0)*(sol[3] - 4.0) - (sol[4] - 1.0)*(sol[4] - 1.0);
    }
    else
    {
        return DBL_MAX;
    }  
}
double OSYF2(double *sol){
    double Judge1 = sol[0] + sol[1] - 2.0;
    double Judge2 = 6.0 - sol[0] - sol[1];
    double Judge3 = 2.0 - sol[1] + sol[0];
    double Judge4 = 2.0 - sol[0] + 3.0*sol[1];
    double Judge5 = 4.0 - (sol[2] - 3.0)*(sol[2] - 3.0) - sol[3];
    double Judge6 = (sol[4] - 3.0)*(sol[4] - 3.0) + sol[5] - 4.0;
    if(Judge1 >= 0 && Judge2 >= 0 && Judge3 >= 0 && Judge4 >= 0&&Judge5>=0&&Judge6>=0){
        return sol[0]*sol[0] + sol[1]*sol[1] + sol[2]*sol[2] +sol[3]*sol[3] +sol[4]*sol[4] +sol[5]*sol[5];
    }
    else
    {
        return DBL_MAX;
    }  
}
double CONSTRF1(double *sol){
    double Judge1 = sol[1] + 9.0*sol[0];
    double Judge2 = -sol[1] + 9.0*sol[0];
    if(Judge1>=6.0 && Judge2>=1.0){
        return sol[0];
    }
    else
    {
        return DBL_MAX;
    }  
}
double CONSTRF2(double *sol){
    double Judge1 = sol[1] + 9.0*sol[0];
    double Judge2 = -sol[1] + 9.0*sol[0];
    if(Judge1>=6.0 && Judge2>=1.0){
        return (1.0+sol[1])/sol[0];
    }
    else
    {
        return DBL_MAX;
    }  
}
/*---------------------------------end the SRN TNK OSY and CONSTR--------------*/
/*-------------------------------------CTP series------------------------------*/
double CTPF1(double *sol, double theta, double a,double b, double c,double d,double e, double gx){
}
double CTPF2(double *sol, double theta, double a,double b, double c,double d,double e, double gx){
}
double CTP1F1(double *sol){
    double Judge1 = ((1.0+sol[1])*exp(-sol[0]/(1.0+sol[1])))/(0.858*exp(-0.541*sol[0]));
    double Judge2 = ((1.0+sol[1])*exp(-sol[0]/(1.0+sol[1])))/(0.728*exp(-0.295*sol[0]));
    if(Judge1 >= 1&& Judge2 >=1){
        return sol[0];
    }
    else{
        return DBL_MAX;
    }
}
double CTP1F2(double *sol){
    double Judge1 = ((1.0+sol[1])*exp(-sol[0]/(1.0+sol[1])))/(0.858*exp(-0.541*sol[0]));
    double Judge2 = ((1.0+sol[1])*exp(-sol[0]/(1.0+sol[1])))/(0.728*exp(-0.295*sol[0]));
    if(Judge1 >= 1&& Judge2 >=1){
        return (1.0+sol[1])*exp(-sol[0]/(1.0+sol[1]));
    }
    else{
        return DBL_MAX;
    }
}
/*--------------------------------------end CTP---------------------------------*/
 
char *Multi()
{
    printf("Here is the list of the formula below:\n\
    firstly, the unrestraint formula:\n\
    1.ZDT1\t2.ZDT2\t3.ZDT3\t4.ZDT4\t\n\
    5.ZDT6\t6.SCH\t7.FON\t8.DTLZ1\t\n\
    9.DTLZ2\t10.DTLZ3\t11.DTLZ4\t12.DTLZ7\t\n\
    13.DEB\t14.KUR\t\n\
    finally, the restraint formula:\n\
    15.Belegundu\t16.Binh(2)\t17.Srinivas\t\n\
    18.Tanaka\t19.Jimenez\t20.Obayashi\t21.SRN\t\n\
    22.TNK\t23.OSY\t24.CONSTR\t25.CTP1\t26.DTLZ5(Remain)\n");
    printf("Please insert the number of the tap formula: ");
    int choose = 0;
    scanf("%d",&choose);
    switch (choose)
    {
    case 1: function[0] = &F_X1;
            function[1] = &F_gsqrt;
            for(int i=0;i<Dimension;i++){
                lb[i] = 0;
                ub[i] = 1;
            }
            FLAG = 0;
            return "ZDT1";
        break;
    case 2: function[0] = &F_X1;
            function[1] = &F_gsquare;
            for(int i =0;i<Dimension;i++){
                lb[i] = 0;
                ub[i] = 1;
            }
            FLAG = 0;
            return "ZDT2";
        break;
    case 3: function[0] = &F_X1;
            function[1] = &F_sin;
            for(int i =0;i<Dimension;i++){
                lb[i] = 0;
                ub[i] = 1;
            }
            FLAG = 0;
            return "ZDT3";
        break;
    case 4: function[0] = &F_X1;
            function[1] = &F_mu;
            lb[0] = 0;
            ub[0] = 1;
            for(int i =1;i<Dimension;i++){
                lb[i] = -5;
                ub[i] = 5;
            }
            FLAG = 0;
            return "ZDT4";
        break;
    case 5: function[0] = &F_ZDT6;
            function[1] = &F_Ssqrt;
            for(int i =0;i<Dimension;i++){
                lb[i] = 0;
                ub[i] = 1;
            }
            FLAG = 0;
            return "ZDT6";
        break;
    case 6: function[0] = &SCHF1;
            function[1] = &SCHF2;
            lb[0] = -100000;
            ub[0] = 100000;
            FLAG = 0;
            return "SCH";
        break;
    case 7: function[0] = &FON1;
            function[1] = &FON2;
            for(int i =0;i<Dimension;i++){
                lb[i] = -4;
                ub[i] = 4;
            }
            FLAG = 0;
            return "FON";
        break;
    case 8: function[0] = &DTLZ1F1;
            function[1] = &DTLZ1F2;
            function[2] = &DTLZ1F3;
            for(int i =0;i<Dimension;i++){
                lb[i] = 0;
                ub[i] = 1;
            }
            FLAG = 0;
            return "DTLZ1";
        break;
    case 9: function[0] = &DTLZ2F1;
            function[1] = &DTLZ2F2;
            function[2] = &DTLZ2F3;
            for(int i =0;i<Dimension;i++){
                lb[i] = 0;
                ub[i] = 1;
            }
            FLAG = 0;
            return "DTLZ2";
        break;
    case 10:function[0] = &DTLZ3F1;
            function[1] = &DTLZ3F2;
            function[2] = &DTLZ3F3;
            for(int i =0;i<Dimension;i++){
                lb[i] = 0;
                ub[i] = 1;
            }
            FLAG = 0;
            return "DTLZ3";
        break;
    case 11:function[0] = &DTLZ4F1;
            function[1] = &DTLZ4F2;
            function[2] = &DTLZ4F3;
            for(int i =0;i<Dimension;i++){
                lb[i] = 0;
                ub[i] = 1;
            }
            FLAG = 0;
            return "DTLZ4";
        break;
    case 12:function[0] = &DTLZ7F1;
            function[1] = &DTLZ7F2;
            function[2] = &DTLZ7F3;
            for(int i =0;i<Dimension;i++){
                lb[i] = 0;
                ub[i] = 1;
            }
            FLAG = 0;
            return "DTLZ7";
        break;
    case 13:function[0] = &DEBF1;
            function[1] = &DEBF2;
            for(int i =0;i<Dimension;i++){
                lb[i] = 0;
                ub[i] = 1;
            }
            FLAG = 0;
            return "DEB";
        break;
    case 14:function[0] = &KURF1;
            function[1] = &KURF2;
            for(int i =0;i<Dimension;i++){
                lb[i] = -5;
                ub[i] = 5;
            }
            FLAG = 0;
            return "KUR";
        break;
    case 15:function[0] = &BelegunduF1;
            function[1] = &BelegunduF2;
            lb[0] = 0;
            ub[0] = 5;
            lb[1] = 0;
            ub[1] = 3;
            FLAG = 0;
            return "Belegundu";
        break;
    case 16:function[0] = &BinhF1;
            function[1] = &BinhF2;
            lb[0] = -15;
            ub[0] = 30;
            lb[1] = -15;
            ub[1] = 30;
            FLAG = 0;
            return "Binh(2)";
        break;
    case 17:function[0] = &SrinivasF1;
            function[1] = &SrinivasF2;
            lb[0] = -20;
            ub[0] = 20;
            lb[1] = -20;
            ub[1] = 20;
            FLAG = 0;
            return "Srinivas";
        break;
    case 18:function[0] = &TanakaF1;
            function[1] = &TanakaF2;
            lb[0] = 0;
            ub[0] = M_PI;
            lb[1] = 0;
            ub[1] = M_PI;
            FLAG = 0;
            return "Tanaka";
        break;
    case 19:function[0] = &JimenezF1;//gonna some problem
            function[1] = &JimenezF2;
            lb[0] = 25;
            ub[0] = 55;
            lb[1] = -10;
            ub[1] = 20;
            FLAG = 1;
            return "Jimenez";
        break;
    case 20:function[0] = &ObayashiF1;//to gain the Max shall turn to gain the negative of it 
            function[1] = &ObayashiF2;
            lb[0] = 0;
            ub[0] = 1;
            lb[1] = 0;
            ub[1] = 1;
            FLAG = 1;
            return "Obayashi";
        break;
    case 21:function[0] = &SRNF1;
            function[1] = &SRNF2;
            lb[0] = -20;
            ub[0] = 20;
            lb[1] = -20;
            ub[1] = 20;
            FLAG = 0;
            return "SRN";
        break;
    case 22:function[0] = &TNKF1;
            function[1] = &TNKF2;
            lb[0] = 0;
            ub[0] = M_PI;
            lb[1] = 0;
            ub[1] = M_PI;
            FLAG = 0;
            return "TNK";
        break;
    case 23:function[0] = &OSYF1;
            function[1] = &OSYF2;
            lb[0] = 0;
            ub[0] = 10;
            lb[1] = 0;
            ub[1] = 10;
            lb[2] = 1;
            ub[2] = 5;
            lb[3] = 0;
            ub[3] = 6;
            lb[4] = 1;
            ub[4] = 5;
            lb[5] = 0;
            ub[5] = 10;
            FLAG = 0;
            return "OSY";
        break;
    case 24:function[0] = &CONSTRF1;
            function[1] = &CONSTRF2;
            lb[0] = 0.1;
            ub[0] = 1;
            lb[1] = 0;
            ub[1] = 5;
            FLAG = 0;
            return "CONSTR";
        break;
    case 25:function[0] = &CTP1F1;
            function[1] = &CTP1F2;
            lb[0] = 0;
            ub[0] = 1;
            lb[1] = 0;
            ub[1] = 1;
            FLAG = 0;
            return "CTP1";
        break;
    case 26:function[0] = &DTLZ5F1;
            function[1] = &DTLZ5F2;
            function[2] = &DTLZ5F3;
            for(int i =0;i<Dimension;i++){
                lb[i] = 0;
                ub[i] = 1;
            }
            FLAG = 0;
            return "DTLZ5";//remain
        break;
    default: function[0] = &F_X1;
             function[1] = &F_gsqrt;
             for(int i=0;i<Dimension;i++){
                 lb[i] = 0;
                 ub[i] = 1;
             }
             FLAG = 0;
             return "ZDT1";
        break;
    }
}