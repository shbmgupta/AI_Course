#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

void load_traindata(int train[][200], int labelTr[]){
  
  FILE * file;
  char ch ;
      file = fopen( "pp_tra.dat" , "r");
    int a , x=0 , y=0;

    int i,j;
    for(i = 0; i < 6670; i++){
        for(j = 0; j < 193; j++){
           if (!fscanf(file, " %d", &train[i][j]))
               break;
           //printf("%d ",train[i][j]);
        }
        //break;
        //printf("\n\n");
    }
    for (int i = 0; i < 6670; ++i)
    {
      labelTr[i] = train[i][192];

    }
    fclose(file);
}

    
void matrixcode(int train[][200], int store[][5][10], double store2[][5][10],int labelTr[]){
	int count[192][10] = {0};
  int adder[192][10] = {0};
	// store[192][5][10] = {0};
	for(int p = 0 ; p < 6670; p++){
	    
  	     for(int i = 0; i < 192 ;i++){
			     
             for(int k = 0; k < 10 ;k++){

          if(train[p][i] == 0 && labelTr[p]==k){
						store[i][0][k] = store[i][0][k] + 1;
            count[i][k]++;
            } 	
					else if(train[p][i] == 1 && labelTr[p]==k){
            store[i][1][k] = store[i][1][k] + 1;
            count[i][k]++;
            }  
          else if(train[p][i] == 2 && labelTr[p]==k){
            store[i][2][k] = store[i][2][k] + 1;
            count[i][k]++;
            }  
          else if(train[p][i] == 3 && labelTr[p]==k){
            store[i][3][k] = store[i][3][k] + 1; 
            count[i][k]++;
            } 
          else if(train[p][i] == 4 && labelTr[p]==k){
            store[i][4][k] = store[i][4][k] + 1; 
            count[i][k]++;
            } 
					}
			}
		}

    for(int p = 0 ; p < 6670; p++){
       
         for(int i = 0; i < 192 ;i++){

            for(int k = 0; k < 10 ;k++){  

          if(train[p][i] == 0){
            store2[i][0][k] = (double)store[i][0][k] / count[i][k];
            }   
          else if(train[p][i] == 1){
            store2[i][1][k] = (double)store[i][1][k] / count[i][k];
            }  
          else if(train[p][i] == 2){
            store2[i][2][k] = (double)store[i][2][k] / count[i][k];
            }  
          else if(train[p][i] == 3){
            store2[i][3][k] = (double)store[i][3][k] / count[i][k];
            } 
          else{
            store2[i][4][k] = (double)store[i][4][k] / count[i][k];
            } 
          }
      }
    }

	}
	
void calcPriorProb(int labelTr[6670],double priorProb[]){
	//priorProb[10] = {0};

  for(int i = 0; i < 10; i++){
      for(int j = 0; j < 6670; j++){
        if(labelTr[j] == i)
            priorProb[i]++;
      }

  }
  for(int i = 0; i < 10; i++){
  	  priorProb[i] = priorProb[i]/6670;
      printf("%lf ", priorProb[i]);
 	  }
    printf("\n");
	
 }	

void load_testdata(int test[][200], int labelTe[]){
  
  FILE * file2;
  char ch ;
      file2 = fopen( "pp_tes.dat" , "r");
    int a , x=0 , y=0;

    int i,j;
    for(i = 0; i < 3333; i++){
        for(j = 0; j < 193; j++){
           if (!fscanf(file2, " %d", &test[i][j]))
               break;
           // printf("%d ",test[i][j]);
        }
        //break;
        // printf("\n\n");
    }
    for (int i = 0; i < 3333; ++i)
    {
      labelTe[i] = test[i][192];

    }
    fclose(file2);
}		


int largest(double arr[], int n){
    int i;
    
    int max = arr[0];
    int index = 0;
    for (i = 1; i < n; i++){
        if (arr[i] > max){
            max = arr[i];
            index = i;
            }
        }    
    return index;
    }

void finalComputation(int labelTe[], int test[][200], double store2[][5][10]){
    int count = 0;
    double mul = 0.0;
  	for(int p = 0; p < 3333 ; p++){
  	  double prob[10] = {0};
  		 //mul = 0.0;
  		// printf("%d ",p);
      for(int i = 0; i < 10; i++){
          mul = 0.0;
            for(int j =0; j< 5; j++){
              for(int k = 0; k < 192; k++){
                    if(test[p][k] == j)
                      mul = log(store2[k][j][i]) + mul;
                    
                     }
                   }
                  
                  prob[i] = mul + log(0.1);//priorProb[i];
                  //printf("%.4lf ***",prob[i]);
                  }
        //          printf("\n");

  		int max = largest(prob,10);
     
  		 //printf("%d %d \n", max, labelTe[p]);
      if(max == labelTe[p]){
        count++;
        }
       //printf("%d\n", count);
  		}
  	double percent;
  	percent = (count/3333.0)*100;
  	printf("Accuracy of Naive Classifier-%lf \n", percent);
	
	}	


int main(){
  int train[6670][200];
  int labelTr[6670];
  int store[192][5][10] = {0};
  double store2[192][5][10] = {0};  
  double priorProb[10] = {0};

  load_traindata(train,labelTr);
  matrixcode(train,store,store2,labelTr);
  // calcPriorProb(labelTr,priorProb);

// for(int k =0;k<192;k++){
//   for(int j =0;j<5;j++){
//     for(int i =0;i<10;i++){
//       printf("%lf ",store2[k][j][i]);
//     }
//     printf("\n");
//   }
//   printf("\n\n");
// }
  int test[3333][200] = {0};
  int labelTe[6670] = {0};
  load_testdata(test,labelTe);
  finalComputation(labelTe,test,store2);

  return 0;
  }
