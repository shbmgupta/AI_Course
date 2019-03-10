/*Soumik Mandal 201601091
Shubham Gupta 201601088*/
/*Test Cases :
0       1       2       3
5       6       7       4
9       10      11      8
13      14      15      12

1       2       3       4
5       6       0       8
9       10      7       11
13      14      15      12

5       8       7       11
1       6       12      2
9       0       13      10
14      3       4       15

1       2       3       4
5       6       7       8
9       10      11      12
13      14      15      0
*/
#include<stdio.h>
#include<string.h>
#include<limits.h>
#include<math.h>
#include<stdbool.h>
#include<stdlib.h>

int m=0,n=4;

typedef struct state{
	int matrix[4][4];
	struct state* parent;
	int cost;int level;
	int x;int y;
}STATE;

#define N 4

typedef struct node{
	STATE* state;
	struct node *next;
}node;

void printpath(STATE* head)
{
    if(head==NULL)
    {
        return ;
    }
        else{
            printpath(head->parent);
            //printmatrix(head->matrix);
        }


}
int check_duplicate(node *temp,STATE *S)
{
    node* start;
    start=temp;
	while(start!=NULL){
		int i,j,f=0;
		for(i=0;i<4;i++){
			for(j=0;j<4;j++){
				if(S->matrix[i][j]!=start->state->matrix[i][j]){
					f=1;
					break;}}
			if(f==1)
				break;
			}
		if(f==0 ){
			if(S->cost+S->level < start->state->cost+start->state->level)
				start->state=S;

			return 0; }
		start=start->next;
	}
	return 1;


}
int getInvCount(int arr[])
{
    int inv_count = 0;int i=0,j=0;
    for (i = 0; i < n * n - 1; i++)
    {
        for (j = i + 1; j < n * n; j++)
        {
            // count pairs(i, j) such that i appears
            // before j, but i > j.
            if (arr[j] && arr[i] && arr[i] > arr[j])
                inv_count++;
        }
    }
    return inv_count;
}
int findXPosition(int puzzle[][n])
{
    // start from bottom-right corner of matrix
    int i,j;
	for (i = n - 1; i >= 0; i--)
        for (j = n - 1; j >= 0; j--)
            if (puzzle[i][j] == 0)
                return n - i;
}
bool if_valid(int puzzle[][10])
{
    // Count inversions in given puzzle
    int invCount = getInvCount((int*)puzzle);
 
    // If grid is odd, return true if inversion
    // count is even.
    if (n & 1)
        return !(invCount & 1);
 
    else     // grid is even
    {
        int pos = findXPosition(puzzle);
        if (pos & 1)
            return !(invCount & 1);
        else
            return invCount & 1;
    }
}

//Misplaced title heuristic.
int mod(int a){
    if(a>0)
        return a;
    else
        return -a;
}
int cal(int mat[][10],int final[][10]){
    int count=0;
    int i,j,r,c;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++)
        {
            if(mat[i][j]!=0){
            r=(mat[i][j]-1)/4;
            c=(mat[i][j]-1)%4;
            count+=mod(r-i)+mod(c-j);                
        }
        }}    
                
    return count;
}

int cal2(int temp[][10],int t[][10]){
	
}	

STATE* createState(int mat[][4],STATE* parent,int level,int x,int y,int x1,int y1,int final[][4])
{
	//copy_matrix(temp->matrix,mat,x,y,x1,y1,4);
	STATE *temp=(STATE*)malloc(sizeof(STATE));
	//copy_matrix(temp->matrix,mat,x,y,x1,y1,4);



	temp->parent=parent;
	//temp->cost=compute_cost(mat,final);
	temp->level=level;
	temp->x=x1;
	temp->y=y1;
}
int check(int a[10][10],int t[10][10]){
	int i,j,f=1;
	
	for(i=0;i < n;i++)
		for(j=0;j < n;j++)
			if(a[i][j]!=t[i][j])
				f=0;
		return f;
}


int main(){
	int p,i,j,n=4,a[10][10],t[10][10],temp[10][10],r[10][10];
	int m=0,x=0,y=0,d=1000,dmin=0,l=0;
	int no=0;
	
	/*-----------------------------------------------------------------------------*/
	
	printf("\nEnter the initial state of the puzzle\n");
	
	for(i=0;i < n;i++)
		for(j=0;j < n;j++)
			scanf("%d",&a[i][j]);

	//printf("\nEnter the target matrix,space with zero :\n");
	//Defining the target matrix.
	int k=1;
	
	for(i=0;i < n;i++)	{
		for(j=0;j < n;j++){
			if(k<=15){
			t[i][j]=k;k++;}
			else 
				break;
			}
		}
			t[3][3]=0;
	printf("\nEntered Matrix is :\n");
	
	for(i=0;i < n;i++){
		for(j=0;j < n;j++)
			printf("%d\t",a[i][j]);
		printf("\n");
		}

	printf("\nTarget Matrix is :\n");
	
	for(i=0;i < n;i++){
		for(j=0;j < n;j++)
			printf("%d\t",t[i][j]);
		printf("\n");
		}
		
	
	bool status=if_valid(a);
	status=1;
	printf("%d\n", status);
	if(status== true){
		printf("Solvable\n");
	
	
	while(!(check(a,t)) && status==true && l<=25){
		l++;
		d=1000;
		for(i=0;i < n;i++)
			for(j=0;j < n;j++){
				if(a[i][j]==0){
					x=i;
					y=j;
					}//Copying coordinates of the blank.
				}
				

		//To move upwards
		for(i=0;i < n;i++)
			for(j=0;j < n;j++)
				temp[i][j]=a[i][j];

		if(x!=0){
			p=temp[x][y];
			temp[x][y]=temp[x-1][y];
			temp[x-1][y]=p;
			}
	
		m=cal(temp,t);
		dmin=l+m;
	
		if(dmin < d){
			d=dmin;
			for(i=0;i < n;i++)
				for(j=0;j < n;j++)
					r[i][j]=temp[i][j];
			}

		//To move downwards
		for(i=0;i < n;i++)
			for(j=0;j < n;j++)
				temp[i][j]=a[i][j];
		if(x!=n-1){
			p=temp[x][y];
			temp[x][y]=temp[x+1][y];
			temp[x+1][y]=p;
			}
		
		m=cal(temp,t);
		dmin=l+m;
		
		if(dmin < d){
			d=dmin;
			for(i=0;i < n;i++)
				for(j=0;j < n;j++)
					r[i][j]=temp[i][j];
			}

		//To move right side
		for(i=0;i < n;i++)
			for(j=0;j < n;j++)
				temp[i][j]=a[i][j];
		if(y!=n-1){
			p=temp[x][y];
			temp[x][y]=temp[x][y+1];
			temp[x][y+1]=p;
			}
		
		m=cal(temp,t);
		dmin=l+m;
		
		if(dmin < d){
			d=dmin;
			for(i=0;i < n;i++)
				for(j=0;j < n;j++)
					r[i][j]=temp[i][j];
			}

		//To move left
		for(i=0;i < n;i++)
			for(j=0;j < n;j++)
				temp[i][j]=a[i][j];
		if(y!=0){
			p=temp[x][y];
			temp[x][y]=temp[x][y-1];
			temp[x][y-1]=p;
			}
		
		m=cal(temp,t);
		dmin=l+m;
		
		if(dmin < d){
			d=dmin;
			for(i=0;i < n;i++)
				for(j=0;j < n;j++)
					r[i][j]=temp[i][j];
			}

		printf("\nCalculated Intermediate Matrix Value :\n");
		for(i=0;i < n;i++)
		{
			for(j=0;j < n;j++)
			  printf("%d\t",r[i][j]);
			printf("\n");
		}
		
		for(i=0;i < n;i++)
			for(j=0;j < n;j++){
			  a[i][j]=r[i][j];
			  temp[i][j]=0;
			}
		no++;
		//printf("Minimum cost : %d\n",d);
		}
		int ex=pow(4,no);
		int closed=1;
		printf("No of nodes in closed: %d\n",l);
		printf("The total nodes expanded/closed:%d",ex);
		printf("\nMemory : %d\n",closed);
	}
	else{
		printf("Invalid Input\n");
	}
	return 0;
}
