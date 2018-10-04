#include "pch.h"
#include "../Treaps/Treap.h"


TEST(RotateLeft, NullPtr)
{
	TreapNode<char>* result;
	TreapNode<char>* a = new TreapNode<char>('a', 0);
	TreapNode<char>* b = new TreapNode<char>('b', 0, a);
	TreapNode<char>* X = new TreapNode<char>('X', 0, a);
	TreapNode<char>* Y = new TreapNode<char>('X', 0, b);
	TreapNode<char>* Z = new TreapNode<char>('X', 0, b);

	a->left = X;
	a->right = b;
	b->left = Y;
	b->right = Z;

	result = rotateLeft(a);

	ASSERT_EQ(result, b);

	ASSERT_EQ(b->parent, nullptr);
	ASSERT_EQ(b->left, a);
	ASSERT_EQ(b->right, Z);
	ASSERT_EQ(a->parent, b);
	ASSERT_EQ(a->left, X);
	ASSERT_EQ(a->right, Y);

	ASSERT_EQ(X->parent, a);
	ASSERT_EQ(X->left, nullptr);
	ASSERT_EQ(X->right, nullptr);
	ASSERT_EQ(Y->parent, a);
	ASSERT_EQ(Y->left, nullptr);
	ASSERT_EQ(Y->right, nullptr);
	ASSERT_EQ(Z->parent, b);
	ASSERT_EQ(Z->left, nullptr);
	ASSERT_EQ(Z->right, nullptr);


}

TEST(RotateRight, Reconnected)
{
	TreapNode<char>* result;
	TreapNode<char>* a = new TreapNode<char>('a', 0);
	TreapNode<char>* b = new TreapNode<char>('b', 0, a);
	TreapNode<char>* X = new TreapNode<char>('X', 0, b);
	TreapNode<char>* Y = new TreapNode<char>('X', 0, b);
	TreapNode<char>* Z = new TreapNode<char>('X', 0, a);

	a->left = b;
	a->right = Z;
	b->left = X;
	b->right = Y;

	result = rotateRight(a);

	ASSERT_EQ(result, b);

	ASSERT_EQ(b->parent, nullptr);
	ASSERT_EQ(b->right, a);
	ASSERT_EQ(b->left, X);
	ASSERT_EQ(a->parent, b);
	ASSERT_EQ(a->left, Y);
	ASSERT_EQ(a->right, Z);

	ASSERT_EQ(X->parent, b);
	ASSERT_EQ(X->left, nullptr);
	ASSERT_EQ(X->right, nullptr);
	ASSERT_EQ(Y->parent, a);
	ASSERT_EQ(Y->left, nullptr);
	ASSERT_EQ(Y->right, nullptr);
	ASSERT_EQ(Z->parent, a);
	ASSERT_EQ(Z->left, nullptr);
	ASSERT_EQ(Z->right, nullptr);
}


TEST(Treap, Search_EmptyTreap_DataNotFound)
{
	Treap<int> uut;

	ASSERT_FALSE(uut.search(2));
}

TEST(Treap, Search_OneElementInserted_DataFound) 
{
	Treap<int> uut;

	uut.insert(2);
	ASSERT_TRUE(uut.search(2));
}


TEST(Treap, Search_TwoElementsInserted_BothFound)
{
	Treap<int> uut;

	uut.insert(2);
	uut.insert(3);
	ASSERT_TRUE(uut.search(2));
	ASSERT_TRUE(uut.search(3));
}


TEST(Treap, Search_ThreeElementsInserted_AllFound)
{
	Treap<int> uut;

	uut.insert(0);
	uut.insert(1);
	uut.insert(2);
	uut.insert(3);
	ASSERT_TRUE(uut.search(0));
	ASSERT_TRUE(uut.search(1));
	ASSERT_TRUE(uut.search(2));
}


TEST(Treap, Search_100ElementsInserted_AllFound)
{
	Treap<int> uut;
	int n = 100;

	for(int i=0; i< n; i++)	uut.insert(i);

	for (int i = 0; i< n; i++)
		if (!uut.search(i)) ASSERT_TRUE(false);

	ASSERT_TRUE(true);
}