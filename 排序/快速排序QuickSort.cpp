#include <iostream>
#include <algorithm>
using namespace std;
typedef int  ElemType;
const int cutoff = 100;

ElemType Median3(ElemType* arr, int left, int right) {//return pivort
	int center = (left + right) / 2;
	if (arr[left] > arr[center]) swap(arr[left], arr[center]);
	if (arr[center] > arr[right]) swap(arr[center], arr[right]);
	if (arr[left] > arr[center]) swap(arr[left], arr[center]);

	swap(arr[center], arr[right - 1]);//hide the pivort
	return arr[right - 1];
}
void quickSort(ElemType* arr, int left, int right) {
	if (left >= right) return;
	if (left - right < cutoff) {
		sort(arr + left, arr + right+1);
		return;
	}
	ElemType pivort = Median3(arr, left, right);
	int i = left, j = right - 1;
	while (true) {
		while(i<j&&arr[++i]<pivort){}
		while(i<j&&arr[--j]>pivort){}
		if (i < j) swap(arr[i], arr[j]);
		else break;
	}
	//put pivort to the correct place
	swap(arr[i], arr[right - 1]);
	quickSort(arr, left, i - 1);
	quickSort(arr, i + 1, right);
}
void Quick_Sort(ElemType* arr, int N) {
	quickSort(arr, 0, N - 1);
}
int main() {
	int arr[20];
	for (auto& n : arr) {
		n = rand() % 100;
		printf("%d ", n);
	}
	printf("\n");
	Quick_Sort(arr, 20);
	for (int i = 0; i < 20; ++i)
		cout << arr[i] << ' ';



	return 0;
}