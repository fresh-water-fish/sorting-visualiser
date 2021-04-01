#include <iostream>
#include <stdlib.h>

using namespace std;

// declare functions
int main();
void initialise();

// global variables
int* sortArray;

int main()
{
	cout << "   ---   Visualiser Prgram   ---   " << endl;
	cout << "Select a sorting algorithm:" << endl;
	cout << "1. Bubble Sort" << endl; 
	cout << "2. Insertion Sort" << endl;
	cout << "3. Merge Sort" << endl;
	
	initialise();
	bool valid = false;
	char userInput = ' ';
	// validate and get the sorting algorithm selected
	while (!valid)
	{
		cin >> userInput;
		if (isdigit(userInput))
		{
			if (userInput > 0 && userInput < 4) { valid = true; } 
		}
	}
	
	switch (userInput)
	{
	case 1:
		// bubble sort
		break;
	case 2:
		// insertion sort
		break;
	case 3:
		// merge sort
		break;
	}
	return 0;
}

void initialise()
{
	// initiate staggered array to assist 'randomness'
	int i = 0;
	for (i = 0; i < 100; i++)
	{
		if (i % 2 == 0) { sortArray[i] = i + 1; }
		else { sortArray[100 - i] = i + 1; }
	}
	// alter array to 'randomly' sorted different integers 
	int tmpint;
	int randSeed = 0;
	for (i = 0; i < 100; i++)
	{
		randSeed = rand() % 100;
		tmpint = sortArray[i];
		sortArray[i] = sortArray[randSeed];
		sortArray[randSeed] = tmpint;
	}
}

void sortSub()
