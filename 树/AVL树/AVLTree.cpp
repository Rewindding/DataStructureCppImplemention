#include <iostream>
#include <deque>

typedef struct AVLNode* Position;
typedef Position AVLTree; /* AVL树类型 */
typedef int ElementType;

using std::cin;
using std::cout;
using std::deque;

struct AVLNode {
	ElementType Data; 
	AVLTree Left;    
	AVLTree Right;    
	int Height;       
};

int Max(int a, int b)
{
	return a > b ? a : b;
}
int GetHeight(AVLTree t) {
	if (!t) return -1;/*null tree's height is defined to be -1,this is important*/
	return t->Height;
}

AVLTree SingleLeftRotation(AVLTree A)
{ /* 注意：A必须有一个左子结点B */
  /* 将A与B做左单旋，更新A与B的高度，返回新的根结点B */
	AVLTree B = A->Left;
	A->Left = B->Right;
	B->Right = A;
	A->Height = Max(GetHeight(A->Left), GetHeight(A->Right)) + 1;
	B->Height = Max(GetHeight(B->Left), A->Height) + 1;

	return B;
}
AVLTree SingleRightRotation(AVLTree A) {
	/*A must have a right child B*/
	AVLTree B = A->Right;
	A->Right = B->Left;
	B->Left = A;
	/*Update the height of A and B,the height of A must be first updated then B*/
	A->Height = Max(GetHeight(A->Left), GetHeight(A->Right)) + 1;
	B->Height = Max(GetHeight(B->Right), A->Height) + 1;
	return B;
}
AVLTree DoubleLeftRightRotation(AVLTree A)
{ /* 注意：A必须有一个左子结点B，且B必须有一个右子结点C */
  /* 将A、B与C做两次单旋，返回新的根结点C */

	/* 将B与C做右单旋，C被返回 */
	A->Left = SingleRightRotation(A->Left);
	/* 将A与C做左单旋，C被返回 */
	return SingleLeftRotation(A);
}
AVLTree DoubleRightLeftRotation(AVLTree A) {
	/*A must have a right child called B,and B must have a left child C*/
	A->Right = SingleLeftRotation(A->Right);
	return SingleRightRotation(A);
}
AVLTree findMax(AVLTree A) {
	while (A->Right) 
		A = A->Right;
	return A;
}
AVLTree findMin(AVLTree A) {
	while (A->Left)
		A = A->Left;
	return A;
}

AVLTree Insert(AVLTree T, ElementType X)
{ /* 将X插入AVL树T中，并且返回调整后的AVL树 */
	if (!T) { /* 若插入空树，则新建包含一个结点的树 */
		T = (AVLTree)malloc(sizeof(struct AVLNode));
		T->Data = X;
		T->Height = 0;
		T->Left = T->Right = NULL;
	} /* if (插入空树) 结束 */

	else if (X < T->Data) {
		/* 插入T的左子树 */
		T->Left = Insert(T->Left, X);
		/* 如果需要左旋 */
		if (GetHeight(T->Left) - GetHeight(T->Right) == 2)
			if (X < T->Left->Data)
				T = SingleLeftRotation(T);      /* 左单旋 */
			else
				T = DoubleLeftRightRotation(T); /* 左-右双旋 */
	} /* else if (插入左子树) 结束 */

	else if (X > T->Data) {
		/* 插入T的右子树 */
		T->Right = Insert(T->Right, X);
		/* 如果需要右旋 */
		if (GetHeight(T->Left) - GetHeight(T->Right) == -2)
			if (X > T->Right->Data)
				T = SingleRightRotation(T);     /* 右单旋 */
			else
				T = DoubleRightLeftRotation(T); /* 右-左双旋 */
	} /* else if (插入右子树) 结束 */

	/* else X == T->Data，无须插入 */

	/* 别忘了更新树高 */
	T->Height = Max(GetHeight(T->Left), GetHeight(T->Right)) + 1;

	return T;
}
AVLTree Delete(AVLTree A,ElementType x) {
	if (!A) return NULL;
	if (A->Data > x) {
		A->Left = Delete(A->Left, x);
		if (GetHeight(A->Right) - GetHeight(A->Left) == 2) {
			if (GetHeight(A->Right->Right) >= GetHeight(A->Right->Left)) A = SingleRightRotation(A);
			else A = DoubleRightLeftRotation(A);
		}
	}
	else if (A->Data < x) {
		A->Right = Delete(A->Right, x);
		if (GetHeight(A->Left) - GetHeight(A->Right) == 2) {
			/**/
			if (GetHeight(A->Left->Left) >= GetHeight(A->Left->Right)) A = SingleLeftRotation(A);
			else A = DoubleLeftRightRotation(A);
		}
	}
	else {//A is the node to be deleted
		if (A->Left && A->Right) {
			AVLTree tmp = findMin(A->Right);
			A->Data = tmp->Data;
			A->Right = Delete(A->Right, A->Data);
			if (GetHeight(A->Left) - GetHeight(A->Right) == 2) {
				if (GetHeight(A->Left->Left) >= GetHeight(A->Left->Right)) A = SingleLeftRotation(A);
				else A = DoubleLeftRightRotation(A);
			}
		}
		else {//the node to be deleted has no child or a single child
			AVLTree tmp = A;
			if (A->Left) A = tmp->Left;
			else if (A->Right) A = tmp->Right;
			else A = NULL;
			free(tmp);
		}
	}
	if (A) {
		A->Height = Max(GetHeight(A->Left), GetHeight(A->Right)) + 1;
	}
	return A;
}
void displayTree(AVLTree root) {
	deque<AVLTree> que;
	que.push_back(root);
	AVLTree last = root;
	while (!que.empty()) {
		AVLTree v = que.front(); que.pop_front();
		if (v) {
			cout << (v->Data) << " ";
			que.push_back(v->Left); que.push_back(v->Right);
		}
		else cout << "null ";
		if (que.empty()) break;
		if (v == last) {
			if(v)cout << '\n';
			last = que.back();
		}
	}
}
int main() {
	AVLTree root = NULL;
	char a; int b;
	while (1) {
		cin >> a;
		if (a == 'e') break;
		switch (a)
		{
		case 'i':cin >> b; root = Insert(root, b); break;
		case 'd':cin >> b; root = Delete(root, b); break;
		case 'D':displayTree(root);
		}
	}

	return 0;
}