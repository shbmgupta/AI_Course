#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define H 6 	//hidden network neurons
#define F 10	//output
#define L 0.01	//learning rate
#define epoch 100	//epoch
#define epsilon 0.03	//stopping criteria

double train[2216][17],hidden[2216][H],label[2216];
double ith[17][H], hto[H+1][10],labelT[1000],test[998][17];
//double wgt_01[17][H], wgt_02[H+1][F];
double final[2216][F];


double get_rand(void){
    	double a = 1.0,po;
        po=((double)rand()/(float)(RAND_MAX)) * a;
    	return po;
	}

double sigmoid(double x){
	return 1.0/(1.0 + exp(-x));
	}

double sigmiodDeriv(double x){
	return sigmoid(x)*(1.0-sigmoid(x));
	}

int max_index(double output[]){
    double n = -9999999999.0;
    int index = 0;
    for(int i=0; i<10; i++){
        if(output[i] >= n){
            n = output[i];
            index = i;
        }
    }
    //printf(" Index:%d \n",index+1);
    return index+1;
}

void random_wgt(double ith[][H], double hto[][10]){
	
	for(int i=0; i<17; i++){
		for(int j=0; j<H; j++){
			ith[i][j] = get_rand();
		}
	}	
		
	for(int p=0; p<H+1; p++){
		for(int q=0; q<10; q++){
			hto[p][q] = get_rand();
		}
	}	
}

void enterdataset(double train[][17], double label[]){
	
	FILE * file;
	char ch ;
    file = fopen( "train02.txt" , "r");
   	int a , x=0 , y=0;

   	int i,j;
   	for(i = 0; i < 2216; i++){
       	for(j = 0; j < 17; j++){
           if (!fscanf(file, " %lf", &train[i][j]))
               break;
           //printf("%lf ",train[i][j]);
       	}
       	//printf("\n");
   	}
   	for (int i = 0; i < 2216; ++i)
   	{
   		label[i] = train[i][0];
		train[i][0] = 1;
   	}
   	fclose(file);
}


void multilayer(double train[], double label, double ith[][H], double hto[][10], int choice)
{
	double input[17], hidden[H+1], output[10], netHid[10], netInput[17], deltaOut[10];
	double delInput[17][H+1], delHidden[H+1][10],sum,sigma,deltaH,delta;
	//int i=1,j;

	for (int i = 1; i < 17; ++i)
	{
		sum = 0;
		for (int j = 0; j < H+1; ++j)
		{
			sum += train[j] * ith[i-1][j];
		}
		netInput[i] = sum;
		hidden[i] = sigmoid(sum);
	}
	hidden[0] = 1;

	for (int i = 0; i < H+1; ++i)
	{
		sum = 0;
		for (int j = 0; j < 10; ++j)
		{
			sum += hidden[i] * hto[i][j];
		}
		netHid[i] = sum;
		output[i] = sigmoid(sum);
	}
//--------------------------------------------------------------------------------------------------
	//for (int i = 0; i < 10; ++i)
	//	printf("%lf\n\n",output[i]);
	double error1[10], error2[10],err[10];

	for (int i = 0; i < 10; ++i)
	{
		if(i==label-1)
			err[i] = 1;
		else
			err[i] = 0;
	}


	for (int i = 0; i < 10; ++i){
		error1[i] = err[i] - output[i]; 
		//printf("%lf\n", error1[i]);
	}
	//error1 = -(label - (double)max_index(output));
	//printf("%lf %lf %lf\n", label, (double)max_index(output), error1);


	if(choice==1){
//-----------------------------------------BACK PROPAGATING HIDDEN LAYER------------------------------
	for (int i = 0; i < H+1; ++i)
	{
		deltaH = error1[i] * sigmiodDeriv(netHid[i]);
		deltaOut[i] = deltaH; 
		for (int j = 0; j < 10; ++j)
		{
			delHidden[i][j] = L * deltaH * hidden[j];
		}
	}

//---------------------------------------- BACK PROPAGATING INPUT LAYER---------------------------------
	for (int i = 0; i < 17; ++i)
	{
		for (int j = 1; j < H+1; ++j)				//maybe H+1
		{
			sigma = 0;
			for (int r = 0; r < 10; ++r)
			{
				sigma += deltaOut[r] * hto[j-1][r] * sigmiodDeriv(netInput[j]);
			}
		delInput[i][j-1] = L * sigma * train[i];
		}
	}
}
else if(choice==2){
	for (int i = 0; i < H+1; ++i)
	{
		deltaH = error1[i];
		deltaOut[i] = deltaH; 
		for (int j = 0; j < 10; ++j)
		{
			delHidden[i][j] = L * deltaH * hidden[j];
		}
	}

//---------------------------------------- BACK PROPAGATING INPUT LAYER---------------------------------
	for (int i = 0; i < 17; ++i)
	{
		for (int j = 1; j < H+1; ++j)				//maybe H+1
		{
			sigma = 0;
			for (int r = 0; r < 10; ++r)
			{
				sigma += deltaOut[r] * hto[j-1][r] * sigmiodDeriv(netInput[j]);
			}
		delInput[i][j-1] = L * sigma * train[i];
		}
	}
	
}


	int flag1=0, flag2=0;

	for (int i = 0; i < 17; ++i)
	{
		for (int j = 0; j < H; ++j)
		{
			if(delHidden[i][j]> epsilon)
				flag1 = 1;
		}
	}

	for (int i = 0; i < H+1; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			if(delInput[i][j]> epsilon)
				flag2 = 1;
		}
	}


	if(flag1==1 && flag2==1)
	{
//-------------------------- W + DEL(W) for hidden layer-----------------------------------
	for (int i = 0; i < 17; ++i)
	{
		for (int j = 0; j < H; ++j)
		{
			ith[i][j] += delHidden[i][j];
		}
	}
//-------------------------- W + DEL(W) for input layer--------------------------------------
	for (int i = 0; i < H+1; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			hto[i][j] += delInput[i][j];
		}
	}
	}

	/*for (int i = 0; i < 17; ++i)
	{
		for (int j = 0; j < H; ++j)
		{
			printf("%lf ",ith[i][j]);
		}
		printf("\n");
	}*/


}

void load_test(double test[][17], double labelT[]){
	
	FILE * file;
	char ch ;
    	file = fopen( "test01.txt" , "r");
   	int a , x=0 , y=0;

   	int i,j;
   	for(i = 0; i < 998; i++){
       	for(j = 0; j < 17; j++){
           if (!fscanf(file, " %lf", &test[i][j]))
               break;
           //printf("%lf ",test[i][j]);
       	}
       	//printf("\n");
   	}
   	for (int i = 0; i < 998; ++i)
   	{
   		labelT[i] = test[i][0];
		//test[i][0] = 1;
   	}
   	fclose(file);
}

double prediction(double test[], double ith[][H], double hto[][10])
{
	double class;
	double hidden[H+1], output[10], netHid[10], netInput[17],sum;

	for (int i = 1; i < 17; ++i)
	{
		sum = 0;
		for (int j = 0; j < H+1; ++j)
		{
			sum += test[j] * ith[i-1][j];
		}
		netInput[i] = sum;
		hidden[i] = sigmoid(sum);
	}
	hidden[0] = 1;

	for (int i = 0; i < H+1; ++i)
	{
		sum = 0;
		for (int j = 0; j < 10; ++j)
		{
			sum += hidden[i] * hto[i][j];
		}
		netHid[i] = sum;
		output[i] = sigmoid(sum);
	}

	class = max_index(output);
	return class;
}

//-----------------------------------------------------------------------------------------------------

int main()
{
	enterdataset(train,label);
	srand((unsigned)(time(0)));
	random_wgt(ith,hto);
	int choice,count = 0;
	printf("Enter your choice: \n");
	scanf("%d",&choice);

	/*for (int i = 0; i < 17; ++i)
	{
		for (int j = 0; j < H; ++j)
		{
			printf("%lf ",ith[i][j]);
		}
		printf("\n");
	}*/
	printf("Training Dataset\n");
	for (int i = 0; i < epoch; ++i)
	{
		for (int j = 0; j < 2216; ++j)
		{
			multilayer(train[j], label[j], ith, hto, choice);
		}
		printf("%d\n", i);
	}
	printf("Done!\n");
	load_test(test, labelT);

	for (int i = 0; i < 998; ++i)
	{
		if(labelT[i] == prediction(test[i], ith, hto))
			count++;
	}
	printf("Correct predictions: %d\n",count);

	return 0;
}