#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
int** a;
int** b;
int** c;
int r1,r2,c1,c2;
int m2row=0,m2col=0;
//Multiplication of method1(thread for each row)
void *method1Mult(void* v){
long int row=(long int)v;
for(int j=0;j<c2;j++){
    for(int k=0;k<c1;k++){
    c[row][j]+=a[row][k]*b[k][j];
    }
}
pthread_exit(NULL);
return 0;
}
//Multiplication of method2(thread for each element in resultant array c)
void *method2mult(void* v){
c[m2row][m2col]=0;
for(int i=0;i<c1;i++)
c[m2row][m2col]+=a[m2row][i]*b[i][m2col];
pthread_exit(NULL);
return 0;
}

int main()
{
struct timeval stop ,start;
FILE *file,*file1;
char* l1;
size_t l1size=32;
l1=(char*)malloc(l1size*(sizeof(char)));
getline(&l1,&l1size,stdin);
l1=strtok(l1,"\n");
char *stok=strtok(l1," ");
stok=strtok(NULL," ");
char* f1=stok;
stok=strtok(NULL," ");
char* f2=stok;
char* out;
stok=strtok(NULL," ");
if(strcmp(stok,"Matout.txt")==0){
out=stok;
}
else{
out="c.out";
}
if((file1=fopen(f2,"r"))==NULL||(file=fopen(f1,"r"))==NULL){
file1=fopen("B.txt","r");file=fopen("A.txt","r");
if(file==NULL||file1==NULL)
{printf("Error! opening file");exit(1);}
}
//read matrix1
fgets(l1,l1size*sizeof(char),file);
char* row;
char* col;
char *p=strtok(l1," ");
row=p;
p=strtok(NULL," ");
col=p;
p=strtok(row,"=");
p=strtok(NULL,"=");
r1=atoi(p);
p=strtok(col,"=");
p=strtok(NULL,"=");
c1=atoi(p);
a=(int**)malloc(c1*(sizeof(int)));
    for (int i = 0; i < r1; i++){
        a[i] = (int*)malloc(c1*(sizeof(int)));
    }
    int ind=0;
while( ind<r1){int j=0;
fgets(l1,l1size*sizeof(char),file);
p=strtok(l1,"\t");
while(p!=NULL){
a[ind][j++]=atoi(p);
p=strtok(NULL,"\t");
}
ind++;
}
//read matrix2
fgets(l1,l1size*sizeof(char),file1);
p=strtok(l1," ");
row=p;
p=strtok(NULL," ");
col=p;
p=strtok(row,"=");
p=strtok(NULL,"=");
r2=atoi(p);
p=strtok(col,"=");
p=strtok(NULL,"=");
c2=atoi(p);
b= (int**)malloc(r2*(sizeof(int *)));
    for (int i = 0; i < r2; i++){
       b[i] = (int*)malloc(c2*(sizeof(int)));
    }
ind=0;
while(ind<r2){
int j=0;
fgets(l1,l1size*sizeof(char),file1);
p=strtok(l1,"\t");
while(p!=NULL){
b[ind][j++]=atoi(p);
p=strtok(NULL,"\t");}
ind++;
}
if(c1!=r2)
{
printf("Error dimensions");
exit(1);}
//start to calculate resultant matrix from multiplication
  c = (int**)malloc(r1*(sizeof(int *)));
    for (int i = 0; i < r1; i++){
        c[i] = (int*)malloc(c2*(sizeof(int)));
    }
pthread_t threads[r1*c2];

gettimeofday(&start,NULL);
//method1
//thread by row
int status;
for(long i=0;i<r1;i++){
status=pthread_create(&threads[i],NULL,method1Mult,(void *)i);
pthread_join(threads[i],NULL);
if(status){
printf("ERROR!");
exit(-1);
}
}

FILE* f=fopen(out,"w");
fprintf(f,"first method:\n****************\nrow=%d col=%d\n",r1,c2);
for(int i=0;i<r1;i++){
    for(int j=0;j<c2;j++){
        if(j==c2-1)
        fprintf(f,"%d\n",c[i][j]);
        else fprintf(f,"%d\t",c[i][j]);
    }
}
gettimeofday(&stop,NULL);
printf("TimeTaken by method one=%d sec and =%d microsec\n",(int)(stop.tv_sec-start.tv_sec),(int)(stop.tv_usec-start.tv_usec));
printf("number of threads created by method 1: %d\n",r1);
//method 2
//thread by element
gettimeofday(&start,NULL);
int num_threads=0;
for(int i=0;i<r1;i++){
    m2col=0;
    for(int j=0;j<c2;j++){
        pthread_create(&threads[num_threads++],NULL,method2mult,(void*)j);
        pthread_join(threads[j],NULL);
    }m2row++;
}
fprintf(f,"second method:\n****************\nrow=%d col=%d\n",r1,c2);
for(int i=0;i<r1;i++){
    for(int j=0;j<c2;j++){
        if(j==c2-1)
        fprintf(f,"%d\n",c[i][j]);
        else fprintf(f,"%d\t",c[i][j]);
    }
}
gettimeofday(&stop,NULL);
printf("Time taken by second method =%d sec and = %d microsec\n",(int)(stop.tv_sec-start.tv_sec),(int)(stop.tv_usec-start.tv_usec));
printf("Number of threads created by method 2= %d\n",num_threads);
fclose(file);
fclose(f);
    return 0;
    }

