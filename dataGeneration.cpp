#include<iostream>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

int main()
{
    printf("Enter no. of records in relations and the range of values: ");
    int n1,n2,range;
    scanf("%d %d %d",&n1,&n2,&range);

    FILE *f1 = fopen("relation1.txt","w");
    FILE *f2 = fopen("relation2.txt","w");


    while(n1--)
        fprintf(f1,"%d\n",1 + (rand()%range));
    while(n2--)
        fprintf(f2,"%d\n",1 + (rand()%range));

    fclose(f1);
    fclose(f2);

    return 0;
}
