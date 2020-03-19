#include <iostream>
#include <cassert>
using namespace std;
typedef int ElemType;

/**
 *2020年1月18日
 *2020年3月19日
 *红黑树
 **/
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
		NIL->Data = 1<<30;
		Root = RBTree::NIL;
	}
};
void insert(RBTree* T, ElemType X);
void insertFixUp(RBTree* T, RBNode* z);
void LeftRotation(RBTree* T, RBNode* z);
void RightRotation(RBTree* T, RBNode* z);
void TransPlant(RBTree* T, RBNode* u, RBNode* v);
void Delete(RBTree* T, RBNode* z);
void DeleteFixUp(RBTree* T, RBNode* z);
RBNode* MinNode(RBTree* T, RBNode* z);
RBNode* Find(RBTree* T, ElemType x);

int main() {
	int arr[] = { 11,2,1,7,8,6,3,5,2 };
	RBTree* T = new RBTree();
	for (int n : arr) {
		insert(T, n);
	}
	for (int n : arr) {
		Delete(T, Find(T, n));
		printf("%d deleted\n", n);
	}
	cout << "\n";
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

void Delete(RBTree* T, RBNode* z) {
	if (!z) return;
	auto y = z;//y 指向要删除的结点，或者是要移动到树内部的结点
	RBNode* x = NULL;
	Color yOriginalColor = y->color;
	if (z->Left == T->NIL) {
		x = z->Right;
		TransPlant(T, z, z->Right);
	}
	else if (z->Right == T->NIL) {
		x = z->Left;
		TransPlant(T, z, z->Left);
	}
	else {
		y = MinNode(T, z->Right);
		x = y->Right;
		if (y->P != z) {
			TransPlant(T, y, y->Right);
			y->Right = z->Right;
			z->Right->P = y;
		}
		TransPlant(T, z, y);
		y->Left = z->Left;
		z->Left->P = y;
		y->color = z->color;
	}
	if (yOriginalColor == Black) {
		DeleteFixUp(T, x);
	}
	delete z;
}
void DeleteFixUp(RBTree* T, RBNode* x) {
	//node x has 'double black' or 'red-black' attribute
	while (x != T->Root && x->color == Black) {
		if (x->P->Left == x) {
			auto w = x->P->Right;
			if (w->color == Red) {//case1: w is red 
				//transfer to case 2/3/4
				w->color = Black;
				w->P->color = Red;//swap colors to make w black
			}
			if (w->Left->color == Black && w->Right->color == Black) {//case2: w is black,and its sublings are all black
				w->color = Red;
				x = x->P;
			}
			else if (w->Right->color == Black) {//case3: w left is red and right is black
				//after excute this, will transfer to case 4 directly
				w->Left->color = Black;
				w->color = Red;
				RightRotation(T, w);
				w = x->P->Right;
			}
			if (w->Right->color == Red) {//case4: w has a red right child
				w->Right->color = Black;
				w->color = x->P->color;
				x->P->color = Black;
				LeftRotation(T, x->P);
				x = T->Root;
			}
		}
		else if (x->P->Right == x) {//symmitric case
			auto w = x->P->Left;
			if (w->color == Red) {//case1':
				w->color = Black;
				w->P->color = Red;
			}
			if (w->Left->color == Black && w->Right->color == Black) {//case2':
				w->color = Red;
				x = x->P;
			}
			else	if (w->Left->color == Black) {//case3':
					w->color = Red;
					w->Right->color = Black;
					LeftRotation(T, w);
					w = x->P->Left;
			}
			if (w->Left->color == Red) {//case4':
				w->color = x->P->color;
				x->P->color = Black;
				w->Left->color = Black;
				RightRotation(T, x->P);
				x = T->Root;
			}
			
		}
	}
	x->color = Black;
}
void TransPlant(RBTree* T, RBNode* u, RBNode* v) {
	//把v结点替换到u
	if (u == T->Root)
		T->Root = v;
	else if (u == u->P->Left) {
		u->P->Left = v;
	}
	else u->P->Right = v;
	v->P = u->P;
}
RBNode* MinNode(RBTree* T, RBNode* z) {
	if (!z) return z;
	while (z->Left!=T->NIL) z = z->Left;
	return z;
}
RBNode* Find(RBTree* T, ElemType x) {
	auto p = T->Root;
	while (p!=T->NIL) {
		if (x > p->Data) p = p->Right;
		else if (x < p->Data) p = p->Left;
		else return p;
	}
	return NULL;
}