#ifndef UTILITIES_HPP
#define UTILITIES_HPP
#include <string>

//Converts a string to it's lowercase equivalent.
std::string upperConvert(std::string data);

//Return's true if the user enters 'y' or 'Y' and false if 'N' or 'n'.
bool getYesOrNo( );

int getValidInt( );

//Sorts an array** of data in ascending order.
template <class Type>
void selectionSortUp(Type** a, int size) {
	int minIndex;
	for (int i = 0; i < size - 1; ++i) {
		minIndex = i;
		for (int j = i + 1; j < size; ++j) {
			if (*a[j] < *a[minIndex]) {
				minIndex = j;
			}
		}
		if (i != minIndex) {
			swap(a[i], a[minIndex]);
		}
	}
}

//Sorts an array** of data in descending order.
template <class Type>
void selectionSortDown(Type** a, int size) {
	int maxIndex;
	for (int i = 0; i < size - 1; ++i) {
		maxIndex = i;
		for (int j = i + 1; j < size; ++j) {
			if (*a[j] > *a[maxIndex]) {
				maxIndex = j;
			}
		}
		if (i != maxIndex) {
			swap(a[i], a[maxIndex]);
		}
	}
}

template <class Type>
void swap(Type& a, Type& b) {
	Type temp = a;
	a = b;
	b = temp;
}
#endif

