#include <iostream>
#include <time.h>

using namespace std;

int calcFitness(int arr[], int size) {
	int ans = 0;
	for (int i = 0; i < size-1; i++) {
		for (int j = i+1; j < size; j++) {
			if (arr[i] == arr[j] || abs(arr[i] - arr[j]) == (j - i)) {
				ans++;
				break;
			}
		}
	}
	return size-ans;
}

int maxFitIndex(int fitness[], int size, int popSize) {
	int sum = 0;
	for (int i = 0; i < popSize ; i++) {
		if (fitness[i] == size) {
			return i;
		}
	}
	return -1;
}

template<typename T, size_t n>
void print_array(T const(&arr)[n])
{
	for (size_t i = 0; i < n; i++)
		std::cout << arr[i] << ' ';
}

int main()
{

	int temp[] = { 4, 2, 0, 6, 5, 7, 2, 3 };


	//exit(0);

	srand(time(0));

	int size;
	int popSize;

	//cout << "Enter board size: ";
	//cin >> size;
	size = 8;


	//cout << "Enter population size: ";
	//cin >> popSize;

	popSize = 1000000;

	int** rowPos = new int*[popSize];
	int* fitness = new int[popSize];
	
	int realSize = 0;
	for (int i = 0; i < popSize; i++) {
		rowPos[i] = new int[size];

		for (int j = 0; j < size; j++) {
			rowPos[i][j] = rand() % size;
		}
		
		fitness[i] = calcFitness(rowPos[i], size);

	}

	while (maxFitIndex(fitness, size, popSize) == -1) {

		int** childrowPos = new int* [popSize];
		for (int i = 0; i < popSize; i++) {
			childrowPos[i] = new int[size];

		}


		int crossIndex = rand() % size;

		for (int j = 0; j < popSize; j++) {
			for (int i = 0; i < crossIndex; i++) {
				childrowPos[j][i] = rowPos[j][i];
			}
			
			for (int i = crossIndex; i < size; i++) {
				childrowPos[j][i] = rowPos[(j+1)%popSize][i];
			}

			childrowPos[j][rand() % size] = rand() % size;

			fitness[j] = calcFitness(childrowPos[j], size);

			
		}

		rowPos = childrowPos;

		
	}

	cout << "Place at following rows for each column:" << endl;
	int index = maxFitIndex(fitness, size, popSize);

	for (int i = 0; i < size; i++) {
		cout << rowPos[index][i]+1 << ", ";
	}

	
	getchar();

	return 0;
	
}


