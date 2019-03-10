#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define DIM 192
#define TRNSIZ 6670
#define TSTSIZ 3333 
int train[TRNSIZ][DIM],trainClass[TRNSIZ];
int test[TSTSIZ][DIM],testClass[TSTSIZ];
float** dt;
void initTrain()
{
	FILE *f = fopen("shuffled.dat","r");
	char buf[400];
	char *str;
	int i = 0; 
	while(fgets(buf,sizeof(buf),f))
	{
		str = strtok(buf," ");
		int j = 0;
		while(str!=NULL)
		{
			if(j == DIM)
			{
				trainClass[i]=atoi(str);
				break;
			}
			train[i][j++] = atoi(str);
			str = strtok(NULL," ");
		}
		i++;
	}
	fclose(f);
}
void initTest()
{
	FILE *f = fopen("pp_tes.dat","r");
	char buf[400];
	char *str;
	int i = 0; 
	while(fgets(buf,sizeof(buf),f))
	{
		str = strtok(buf," ");
		int j = 0;
		while(str!=NULL)
		{
			if(j == DIM)
			{
				testClass[i]=atoi(str);
				break;
			}
			test[i][j++] = atoi(str);
			str = strtok(NULL," ");
		}
		i++;
	}
	
	fclose(f);
}
typedef struct node
{
	float dist;
	int cl;
	struct node *next;
}PQ;
PQ *q;
void print()
{
	PQ *h=q;
	while(h!=NULL){

		printf("%f %d|",h->dist,h->cl);
		h=h->next;
	}
	printf("\n");
}
void check()
{
	PQ *h=q;

	while(h!=NULL && h->next!=NULL)
	{
		if(h->dist>h->next->dist){
			print();
			exit(0);
		}
		h=h->next;
	}
}

int length()
{
	PQ *h=q;
	int l=0;
	while(h!=NULL)
	{
		h=h->next;
		l++;
	}
	return l;
}
void insert(float d,int cl)
{
	if(q==NULL || q->dist>d){

		PQ *nq=(PQ*)malloc(sizeof(PQ));
		nq->dist=d;
		nq->cl=cl;
		nq->next=q;
		q=nq;
	}
	else{
		PQ *nq=(PQ*)malloc(sizeof(PQ));
		nq->dist=d;
		nq->cl=cl;
		nq->next=NULL;
		PQ *cur=q;
		while(cur->next!=NULL && cur->next->dist<d){
			cur = cur->next;
		}
		nq->next = cur->next;
		cur->next = nq;
	}
	int k=24;
	PQ *head = q;
	while(head->next!=NULL && k--)
		head = head->next;
	head->next=NULL;
	check();
}
float calDist(int ar1[],int ar2[])
{
	int i;
	float dtemp=0;
	for (i = 0; i < DIM; ++i)
	{
		dtemp+= pow(ar1[i]-ar2[i],2); 
	}
	return sqrt(dtemp);
}
float acCV1(int K)
{
	int i,j,k,acc=0;
	
	for(i=0;i<TRNSIZ/3;i++)
	{
		float oc[10]={0,0,0,0,0,0,0,0,0,0};
		q=NULL;
		for(j=TRNSIZ/3;j<2*TRNSIZ/3;j++)
		{
			insert(dt[i][j],trainClass[j]);
		}
		for(j=2*TRNSIZ/3;j<TRNSIZ;j++)
		{
			insert(dt[i][j],trainClass[j]);
		}
		PQ *head=q;
		for (int i = 0; i < K; ++i)
		{
			oc[head->cl]+=1.0/head->dist;
			head=head->next;
		}
		int max=0;
		for(k=1;k<10;k++){
			if(oc[k]>oc[max])
				max=k;
		}
		if(max==trainClass[i])
			acc++;
	}
	return 1.0*acc/(TRNSIZ/3);

}
float acCV2(int K)
{
	int i,j,k,acc=0;
	
	for(i=TRNSIZ/3;i<2*TRNSIZ/3;i++)
	{
		float oc[10]={0,0,0,0,0,0,0,0,0,0};
		q=NULL;
		for(j=0;j<TRNSIZ/3;j++)
		{
			insert(dt[i][j],trainClass[j]);
		}
		for(j=2*TRNSIZ/3;j<TRNSIZ;j++)
		{
			insert(dt[i][j],trainClass[j]);
		}
		PQ *head=q;
		for (int i = 0; i < K; ++i)
		{
			oc[head->cl]+=1.0/head->dist;
			head=head->next;
		}
		int max=0;
		for(k=1;k<10;k++){
			if(oc[k]>oc[max])
				max=k;
		}
		if(max==trainClass[i])
			acc++;
	}
	return 1.0*acc/(TRNSIZ/3);

}
float acCV3(int K)
{
	int i,j,k,acc=0;
	
	for(i=2*TRNSIZ/3;i<TRNSIZ;i++)
	{
		float oc[10]={0,0,0,0,0,0,0,0,0,0};
		q=NULL;
		for(j=0;j<TRNSIZ/3;j++)
		{
			insert(dt[i][j],trainClass[j]);
		}
		for(j=TRNSIZ/3;j<2*TRNSIZ/3;j++)
		{
			insert(dt[i][j],trainClass[j]);
		}
		PQ *head=q;
		for (int i = 0; i < K; ++i)
		{
			oc[head->cl]+=1.0/head->dist;
			head=head->next;
		}
		int max=0;
		for(k=1;k<10;k++){
			if(oc[k]>oc[max])
				max=k;
		}
		if(max==trainClass[i])
			acc++;
	}
	return 1.0*acc/(TRNSIZ/3);

}
float acTest(int K)
{
	int i,j,k,acc=0;
	
	for(i=0;i<TSTSIZ;i++)
	{
		float oc[10]={0,0,0,0,0,0,0,0,0,0};
		q=NULL;
		for(j=0;j<TRNSIZ;j++)
		{
			insert(calDist(test[i],train[j]),trainClass[j]);
		}
		PQ *head=q;
		for (int i = 0; i < K; ++i)
		{
			oc[head->cl]+=1.0/head->dist;
			head=head->next;
		}
		int max=0;
		for(k=1;k<10;k++){
			if(oc[k]>oc[max])
				max=k;
		}
		if(max==testClass[i])
			acc++;
	}
	return 1.0*acc/(TSTSIZ);

}
void distances()
{
	dt=(float**)malloc(TRNSIZ*sizeof(float*));
	for (int i = 0; i < TRNSIZ; ++i)
	{
		dt[i]=(float*)malloc(TRNSIZ*sizeof(float));
	}
	for (int i = 0; i < TRNSIZ; ++i)
	{
		for (int j = i+1; j < TRNSIZ; ++j)
		{
			dt[i][j]=dt[j][i]=calDist(train[i],train[j]);
			
		}
		printf("\rIn progress %2.f", 100.0*i/TRNSIZ);
		
	}
	printf("\n");
}
int accCV()
{
	FILE *f=fopen("val.txt","w");
	int i,j,k,K=1;
	float acc=0;
	int AcK=-1;
	while(K<=25)
	{
		printf("for k=%d ||", K);
		float acc1=acCV1(K);
		float acc2=acCV2(K);
		float acc3=acCV3(K);
		printf("CV1-%f CV2-%f CV3-%f||", acc1,acc2,acc3);
		float meanAc=(acc1+acc2+acc3)/3.0;
		printf("Accuracy = %f\n",100.0*meanAc );
		fprintf(f, "%f\n", 100*meanAc );
		if(meanAc>acc)
		{
			acc=meanAc;
			AcK=K;
		}
		K++;

	}
	fclose(f);
	printf("The Optimum value of K = %d and Accuracy = %f\n",AcK,acc*100);
	return AcK;
}
int main()
{
	initTrain();
	printf("initTrain success\n");
	initTest();
	printf("initTest success\n");
	printf("calculating distances...\n");
	distances();
	int AcK=accCV();
	printf("Testing for test data...\n");
	float accFinal=acTest(AcK);
	printf("Acuracy on test Set = %f\n",accFinal*100);
}
