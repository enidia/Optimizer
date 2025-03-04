#include<stdio.h>
#include<stdlib.h>
 
#define a  9
int main(){
    srand(time(NULL));
    for (int i = 0; i < 100; i++)
    {
        double r = (((double)rand() / ((double)(RAND_MAX) + (double)(1))));
        double at = (((double)rand() / ((double)(RAND_MAX) + (double)(1))));
        printf("%.20f\n%.20f\n",r,at);
    }
     
    // FILE *fp;
    // fp = fopen("./Hyp.txt","w+");
    // if(fp != NULL)
    // {
    //     fprintf(fp,"this is not only a test");
    // }
    // fclose(fp);
    // int A[a];
    // for(int i =0;i<9;i++){
    // A[i] = i;
    // printf("%d\n",A[i]);
    // }
}