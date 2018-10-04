#pragma once
#include <cstdlib>
#include <ctime>

// internal class to represent a Treap node
template<typename T>
class TreapNode
{
public:
	T data;
	int pri;
	TreapNode* parent;
	TreapNode* left;
	TreapNode* right;

	TreapNode(const T& d = T(), const int pri = 0, TreapNode* p = nullptr, TreapNode* l = nullptr,
		TreapNode* r = nullptr) :
		data(d), pri(pri), parent(p), left(l), right(r)
	{
	}
};

template<typename T>
TreapNode<T>* rotateLeft(TreapNode<T>* b)
{
	if (b == nullptr || b->right == nullptr) return b;
	TreapNode<T>* c = b->right;
	b->right = c->left;
	if (b->right) b->right->parent = b;
	c->left = b;
	c->parent = b->parent;
	b->parent = c;

	return c;
}


/* Returns pointer to c (new root after rotation)
*
*	       b              c
*	      / \            / \
*	     c   X    ->    Y   b
*	    / \                / \
*	   Y   Z              Z   X
*
*/

template<typename T>
TreapNode<T>* rotateRight(TreapNode<T>* b)
{
	if (b == nullptr || b->left == nullptr) return b;
	TreapNode<T>* c = b->left;
	b->left = c->right;
	if (b->left) b->left->parent = b;
	c->right = b;
	c->parent = b->parent;
	b->parent = c;

	return c;
}


template <typename T>
class Treap
{
public:
	int pris[4] = { 10,25,5,56 };
	int generatePriority()
	{
		return rand() % (MAX_PRI - MIN_PRI + 1) + MIN_PRI;
		//return pris[count++];
	}

	int count = 0;
	static const int MIN_PRI = 1;
	static const int MAX_PRI = 100;


	TreapNode<T>* root;


	/* Returns pointer to c (new root after rotation)
	 *
	 *	       b                c
	 *		  / \		->     / \
	 *		 X   c            b   Z
	 *			/ \		     / \
	 *		   Y   Z        X   Y
	 */

public:
	Treap()
	{
		root = nullptr;
		srand(time(nullptr));
	}

	void insert(const T& data)
	{
		int pri = generatePriority();

		// If treap is empty, insert data at root
		if (!root)
		{
			root = new TreapNode<T>(data, pri);
			return;
		}

		// Find place to insert data and insert it
		TreapNode<T>* cursor = root;
		TreapNode<T>* child;

		while (true)
		{
			if (data == cursor->data) return; // Duplicates not allowed
			if (data < cursor->data)
			{
				if (!cursor->left) // Insertion point (leaf) reached
				{
					cursor->left = new TreapNode<T>(data, pri, cursor);
					child = cursor->left;
					break;
				}
				cursor = cursor->left; // Keep looking
			}
			else
			{
				if (!cursor->right) // Insertion point (leaf) reached
				{
					cursor->right = new TreapNode<T>(data, pri, cursor);
					child = cursor->right;
					break;
				}
				cursor = cursor->right; // Keep looking
			}
		}

		// INV: Node inserted as leaf. cursor points to the recently inserted node
		while(true)
		{
			// INV: Cursor points to parent of new node containing data.
			// Rotate node upwards as necessary
			if (cursor == root)
			{
				if (root->pri < pri)
				{
					if (child == root->right)
					{
						root = rotateLeft(root);
					}

					else
					{
						root = rotateRight(root);
					}
				}
				return;
			}


			if (cursor->pri < pri)
			{
				if (child == cursor->right)	// Need left rotation
				{
					if (cursor == cursor->parent->right) cursor->parent->right = rotateLeft(cursor);
					else cursor->parent->left = rotateLeft(cursor);
				}
				else // Need right rotation
				{
					if (cursor == cursor->parent->right) cursor->parent->right = rotateRight(cursor);
					else cursor->parent->left = rotateRight(cursor);
				}
				cursor = cursor->parent->parent;
			}
			else
				break;
		}
	}


	void remove(const T& data)
	{
	}


	bool search(const T& data)
	{
		TreapNode<T>* cursor = root;
		{
			while (cursor)
			{
				if (cursor->data == data) return true;
				if (data < cursor->data) cursor = cursor->left;
				else cursor = cursor->right;
			}
			return false;
		}
	}
};
