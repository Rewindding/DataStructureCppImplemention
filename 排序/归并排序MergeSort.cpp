#include <iostream>
#include <cstring>

void Merge(int* arr,int*tmpArr, int L,int R,int Rend) {
	int Lend = R - 1, ElementNum = Rend - L + 1;
	int s = L, ptr = L;
	while (L <= Lend && R <= Rend) {
		if (arr[R] < arr[L]) tmpArr[ptr++] = arr[R++];
		else tmpArr[ptr++] = arr[L++];
	}
	while (L <= Lend)
		tmpArr[ptr++] = arr[L++];
	while (R <= Rend)
		tmpArr[ptr++] = arr[R++];
	memcpy(arr + s, tmpArr+s, sizeof(int) * ElementNum);
}
void MSort(int* arr,int*tmp,int L,int End) {
	if (L < End) {
		int Mid = (L + End) / 2;
		MSort(arr, tmp, L, Mid);
		MSort(arr, tmp, Mid + 1, End);
		Merge(arr, tmp, L, Mid + 1, End);
	}
}
void Merge_Sort(int*arr ,int n) {
	if (n <= 0) return;
	int* tmp = new int[n];
	MSort(arr, tmp, 0, n - 1);
	delete[]tmp;
}
/*非递归实现*/
void Merge1(int* arr, int* tmpArr, int L, int R, int Rend) {//将两个有序子列从arr归并到tmpArr
	int Lend = R - 1, ElementNum = Rend - L + 1;
	int s = L, ptr = L;
	while (L <= Lend && R <= Rend) {
		if (arr[R] < arr[L]) tmpArr[ptr++] = arr[R++];
		else tmpArr[ptr++] = arr[L++];
	}
	while (L <= Lend)
		tmpArr[ptr++] = arr[L++];
	while (R <= Rend)
		tmpArr[ptr++] = arr[R++];
}
void MergePass(int*arr,int*tmp,int N,int length) {//length是要归并的子列长度
	int i = 0;
	for (  ; i <= N - 2 * length; i += 2 * length)
		Merge1(arr, tmp, i, i + length, i + 2 * length - 1);
	//处理尾巴
	if (i + length < N)//还剩两个序列
		Merge1(arr, tmp, i, i + length, N - 1);
	else //只剩一个序列
		for (int j = i; j < N; ++j)tmp[j] = arr[j];
}
void MergeSort_NR(int* arr, int N) {//non recursive
	int* tmp = new int[N];
	int length = 1;
	while (length < N) {
		MergePass(arr, tmp, N, length);
		length *= 2;
		MergePass(tmp, arr, N, length);
		length *= 2;
	}
	delete[]tmp;
}
int main() {
	int arr[100];
	for (int i = 0; i < 100; ++i)
		arr[i] = rand() % 1000 + 100;
	Merge_Sort(arr, sizeof(arr) / 4);
	for (auto x : arr)
		std::cout << x << ' ';

	

	return 0;
}