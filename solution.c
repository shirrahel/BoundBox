//Shir Machlev

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int** getarray(int *size);//get num of objects and create array in this size
int** CreateArrOfObj(int NumOfObj);//scan objects to array
int** CreateArrOfSeperateObj(int **arr, int* NumOfObj);//create array of separate (and valid) objects
int ValidObj(int*a);//check if object valid
int InList(int i, int* ArrOfFalseIndex, int k);//check if object in the 'Black list' (list of failure objects)
int WhatIsTheRelation(int *a, int *b);//check if 2 objects separate or congruent
int** CreateSortArrOfSeperate(int **arr, int NumofObj);//create sorted array of separate objects
int CalculateArea(int *a);//calculate area of object
void quicksort(int **number, int first, int last);//sort objects by their area size


void main()
{

		
	int NumOfObj;

	int **Fullarr = getarray(&NumOfObj);
	Fullarr = CreateArrOfObj(NumOfObj);

	int **SeparateArr = (int**)malloc(NumOfObj * sizeof(int*));
	SeparateArr = CreateArrOfSeperateObj(Fullarr, &NumOfObj);

	if (NumOfObj > 1) {
		int **SortArr = CreateSortArrOfSeperate(SeparateArr, NumOfObj);

		printf("List of separate bound boxes (Sort by area size):\n");

		for (int i = 0; i < NumOfObj; i++) {

			for (int j = 0; j < 4; j++) {
				printf("%d ", SortArr[i][j]);

			}
			printf("\n");

			//free(SortArr[i]);

		}
		free(SortArr);


	}




	for (int i = 0; i < NumOfObj; i++) {
		//free(SortArr[i]);
		free(Fullarr[i]);
		free(SeparateArr[i]);
	}

	free(Fullarr);
	free(SeparateArr);

	system("pause");
}





int** getarray(int *size) {
	int NumOfObj = 0;

	

	int **arr = (int**)malloc(NumOfObj * sizeof(int*));;
	

	//printf("How many objects would you like to insert?");
	//scanf("%d", &NumOfObj);

	//file
	FILE * fp;
	fopen_s(&fp, "BoundBoxesList.txt", "r");
	fscanf(fp, "%d\n", &NumOfObj);
	fclose(fp);

	printf("Num of objects=%d\n", NumOfObj);
	*size = NumOfObj;
	 

	return arr;
}

int** CreateArrOfObj(int NumOfObj) {

	int **ArrOfObjs;
	ArrOfObjs = (int **)malloc(NumOfObj * sizeof(int*));

	//file
	FILE * fp;
	fopen_s(&fp, "BoundBoxesList.txt", "r");

	fseek(fp, 1, SEEK_SET);


	for (int i = 0; i < NumOfObj; i++) {

		ArrOfObjs[i] = malloc(4 * sizeof(int));

		//printf("Enter 4 values (integers) of boundries to BoundBox %d (L,B,R,T):", i + 1);
		printf("4 values (integers) of boundries to BoundBox %d (L,B,R,T):", i + 1);

		for (int j = 0; j < 4; j++) {
			//scanf("%d", &ArrOfObjs[i][j]);
			fscanf(fp, "%d", &ArrOfObjs[i][j]);//scan from file
			printf(" %d", ArrOfObjs[i][j]);
		}
		printf("\n");

	}

	fclose(fp);//file
		return ArrOfObjs;
	
}

int **CreateArrOfSeperateObj(int **arr, int *NumOfObj) {
	int counter;
	int **SeparatObjs;
	int k = 0;
	int m = 0;

	int* ArrOfFalseIndex = (int*)malloc(sizeof(int)*(*NumOfObj));/*Create array of false indexes
	(indexes of pointers that hold congruence objects or invalid BoundBox),so we won't run over these indexes again (i)*/

	SeparatObjs = (int **)malloc(*NumOfObj * sizeof(int*));

	for (int i = 0; i < *NumOfObj; i++) {//-1

		if (ValidObj(arr[i]) == 1) {//if object[i] valid continue

			if (InList(i, ArrOfFalseIndex, m) == 0) {//if this index in the array 'ArrOfFalseIndex' skip 

				counter = 0;


				for (int j = 0; j < *NumOfObj; j++) {//check what is the relation

					if (i != j) {//don't check realtion between object and itself

						if (WhatIsTheRelation(arr[i], arr[j]) == 0)//if separate
						{
							counter++;
						}
						else if ((WhatIsTheRelation(arr[i], arr[j]) == 1)) {//if not separate 

							if (InList(j, ArrOfFalseIndex, m) == 0) {//if object[j] not in the array 'ArrOfFalseIndex' then insert it
								ArrOfFalseIndex[m] = j;
								m++;
							}
						}
					}
				}


				if (counter == ((*NumOfObj) - 1))//if object separate then insert to 'SeparatObjs'
				{
					SeparatObjs[k] = malloc(4 * sizeof(int));
					for (int l = 0; l < 4; l++) {
						SeparatObjs[k][l] = arr[i][l];
					}
					k++;
				}
			}

		}
		else {
			if (InList(i, ArrOfFalseIndex, m) == 0) {//if object[i] not in the array 'ArrOfFalseIndex' then insert it
				ArrOfFalseIndex[m] = i;
				m++;
			}
		}
	}
	*NumOfObj = k;//Update num of objects, num of seperate objects
	return SeparatObjs;
}

int ValidObj(int*a) {

	if ((a[3] >= a[1]) && (a[2] >= a[0])) {
		return 1;
	}
	else {
		return 0;
	}
}

int InList(int i, int* ArrOfFalseIndex, int k) {

	for (int j = 0; j < k; j++) {

		if (i == ArrOfFalseIndex[j]) {
			return 1;
		}
	}
	return 0;
}

int WhatIsTheRelation(int *a, int *b) {


	if ((((a[0] >= b[0]) && (a[0] <= b[2])) || ((a[2] >= b[0]) && (a[2] <= b[2]))) &&
		(((a[1] >= b[1]) && (a[1] <= b[3])) || ((a[3] >= b[1]) && (a[3] <= b[3]))))//FIX!!!

	{
		return 1;//intersect
	}

	else {
		return 0;//separate
	}
}

int** CreateSortArrOfSeperate(int **arr, int NumofObj) {

	int area = 0;

	int** SortArr = (int**)malloc(sizeof(int*)*NumofObj);
	SortArr[NumofObj - 1] = (int*)malloc(sizeof(int*));

	int Low = CalculateArea(arr[0]);
	int High = CalculateArea(arr[NumofObj - 1]);



	for (int i = 0; i < NumofObj; i++) {

		SortArr[i] = (int*)malloc(sizeof(int) * 4);
	}


	quicksort(arr, 0, NumofObj - 1);


	for (int i = 0; i < NumofObj; i++) {

		SortArr[i] = arr[i];
	}



	return SortArr;

}

int CalculateArea(int *a) {

	int Area;
	Area = ((a[2] - a[0])*(a[3] - a[1]));
	return Area;

}

void quicksort(int **number, int first, int last) {
	int i, j, pivot;
	int* temp;

	if (first < last) {
		pivot = first;
		i = first;
		j = last;

		while (i < j) {
			while (((CalculateArea(number[i])) <= (CalculateArea(number[pivot]))) && (i < last))
				i++;
			while (CalculateArea(number[j]) > CalculateArea(number[pivot]))
				j--;
			if (i < j) {
				temp = number[i];
				number[i] = number[j];
				number[j] = temp;
			}
		}

		temp = number[pivot];
		number[pivot] = number[j];
		number[j] = temp;
		quicksort(number, first, j - 1);
		quicksort(number, j + 1, last);

	}
}

