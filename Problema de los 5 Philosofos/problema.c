#include <stdio.h>

#define n 4

int compltedPhilo = 0,i;

struct fork{
	int taken;
}ForkAvil[n];

struct philosp{
	int left;
	int right;
}Philostatus[n];

void goForDinner(int numfilosofo){
	if(Philostatus[numfilosofo].left == 10 && Philostatus[numfilosofo].right == 10)
		printf("filosofo %d completed his dinner\n",numfilosofo+1);
	//if already completed dinner
	else if(Philostatus[numfilosofo].left == 1 && Philostatus[numfilosofo].right == 1){
		//if just taken two forks
		printf("filosofo %d completed his dinner\n",numfilosofo+1);

		Philostatus[numfilosofo].left = Philostatus[numfilosofo].right = 10; //remembering that he completed dinner by assigning value 10
		int otherFork = numfilosofo-1;

		if(otherFork== -1)
		otherFork=(n-1);

		ForkAvil[numfilosofo].taken = ForkAvil[otherFork].taken = 0; //releasing forks
		printf("filosofo %d released fork %d and fork %d\n",numfilosofo+1,numfilosofo+1,otherFork+1);
		compltedPhilo++;
	}
	else if(Philostatus[numfilosofo].left==1 && Philostatus[numfilosofo].right==0){ //left already taken, trying for right fork
		if(numfilosofo==(n-1)){
			if(ForkAvil[numfilosofo].taken==0){ //KEY POINT OF THIS PROBLEM, THAT LAST filosofo TRYING IN reverse DIRECTION
			ForkAvil[numfilosofo].taken = Philostatus[numfilosofo].right = 1;
			printf("Fork %d taken by filosofo %d\n",numfilosofo+1,numfilosofo+1);
			}else{
			printf("filosofo %d is waiting for fork %d\n",numfilosofo+1,numfilosofo+1);
			}
		}else{ //except last filosofo case
			int dupphilID = numfilosofo;
			numfilosofo-=1;

			if(numfilosofo== -1)
			numfilosofo=(n-1);

			if(ForkAvil[numfilosofo].taken == 0){
				ForkAvil[numfilosofo].taken = Philostatus[dupphilID].right = 1;
				printf("Fork %d taken by filosofo %d\n",numfilosofo+1,dupphilID+1);
			}else{
				printf("filosofo %d is waiting for Fork %d\n",dupphilID+1,numfilosofo+1);
			}
		}
	}
	else if(Philostatus[numfilosofo].left==0){ //nothing taken yet
		if(numfilosofo==(n-1)){
			if(ForkAvil[numfilosofo-1].taken==0){ //KEY POINT OF THIS PROBLEM, THAT LAST filosofo TRYING IN reverse DIRECTION
			ForkAvil[numfilosofo-1].taken = Philostatus[numfilosofo].left = 1;
			printf("Fork %d taken by filosofo %d\n",numfilosofo,numfilosofo+1);
			}else{
			printf("filosofo %d is waiting for fork %d\n",numfilosofo+1,numfilosofo);
			}
		}else{ //except last filosofo case
			if(ForkAvil[numfilosofo].taken == 0){
				ForkAvil[numfilosofo].taken = Philostatus[numfilosofo].left = 1;
				printf("Fork %d taken by filosofo %d\n",numfilosofo+1,numfilosofo+1);
			}else{
				printf("filosofo %d is waiting for Fork %d\n",numfilosofo+1,numfilosofo+1);
			}
		}
	}else{}
}

int main(){
	for(i = 0; i< n ; i++)
		ForkAvil[i].taken = Philostatus[i].left = Philostatus[i].right = 0;

	while(compltedPhilo<n){
		for(i=0;i<n;i++)
			goForDinner(i);
		printf("\nTill now num of filosofos completed dinner are %d\n\n",compltedPhilo);
	}

	return 0;
}
