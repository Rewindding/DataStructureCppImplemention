#include <iostream>
#include <cassert>
using namespace std;
typedef int ElemType;
/*
2020年1月18日
红黑树
删除尚未完成
*/
enum Color
{
	Black, Red
};

struct RBNode {
	ElemType Data;
	Color color;
	RBNode* Left;
	RBNode* Right;
	RBNode* P;
};
struct RBTree {
	RBNode* Root;
	RBNode* NIL;//哨兵
	RBTree() {
		NIL = new RBNode();
		NIL->color = Black;
		NIL->P = NIL->Left = NIL->Right = NULL;
		NIL->Data = 666;
		Root = RBTree::NIL;
	}
};
void insert(RBTree* T, ElemType X);
void insertFixUp(RBTree* T, RBNode* z);
void LeftRotation(RBTree* T, RBNode* z);
void RightRotation(RBTree* T, RBNode* z);

int main() {
	int arr[] = { 11,2,1,7,8,6,3,5,2 };
	RBTree* T = new RBTree();
	for (int n : arr) {
		insert(T, n);
	}
	return 0;
}
void LeftRotation(RBTree* T, RBNode* z) {
	auto B = z->Right;
	if (!B) return;//z必须有一个右儿子才能左旋
	if (T->Root == z) {//要更新T的根结点
		T->Root = B;//提前先把root更新了
	}
	z->Right = B->Left;
	if (B->Left != T->NIL)//注意红黑树的性质，不能写==NULL;
		B->Left->P = z;
	B->P = z->P;
	if (z == z->P->Left)
		z->P->Left = B;
	else z->P->Right = B;

	B->Left = z;
	z->P = B;
}
void RightRotation(RBTree* T, RBNode* z) {
	auto B = z->Left;
	if (!B) return;
	if (T->Root == z) {
		T->Root = B;
	}
	z->Left = B->Right;
	if (B->Right != T->NIL)
		B->Right->P = z;
	B->P = z->P;
	if (z == z->P->Left)
		z->P->Left = B;
	else z->P->Right = B;
	B->Right = z;
	z->P = B;
}
void insertFixUp(RBTree* T, RBNode* z) {

	while (z->P->color == Red) {
		if (z->P == z->P->P->Left) {
			//case1
			if (z->P->P->Right->color == Red) {
				z->P->P->color = Red;
				z->P->P->Right->color = Black;
				z->P->color = Black;
				z = z->P->P;
			}
			else {
				//case2
				if (z == z->P->Right) {
					z = z->P;
					LeftRotation(T, z);
				}
				//case3
				z->P->color = Black;
				z->P->P->color = Red;
				RightRotation(T, z->P->P);
			}
		}
		else {
			//case4
			if (z->P->P->Left->color == Red) {
				z->P->color = Black;
				z->P->P->Left->color = Black;
				z->P->P->color = Red;
				z = z->P->P;
			}
			else {
				//case5
				if (z == z->P->Left) {
					z = z->P;
					RightRotation(T, z);
				}
				//case6
				z->P->color = Black;
				z->P->P->color = Red;
				LeftRotation(T, z->P->P);
			}
		}
	}
	T->Root->color = Black;
}
void insert(RBTree* T, ElemType X) {
	auto p = T->Root;
	auto y = T->NIL;
	while (p != T->NIL) {
		y = p;
		if (X < p->Data)p = p->Left;
		else if (X > p->Data) p = p->Right;
		else return;
	}
	auto z = new RBNode();
	z->Data = X;
	z->color = Red;
	z->Left = z->Right = T->NIL;
	z->P = y;
	if (y == T->NIL) T->Root = z;
	else if (y->Data > X)y->Left = z;
	else y->Right = z;
	insertFixUp(T, z);
}