#pragma once
#include <cstdlib>
#include <ctime>
#include <ostream>


template <typename T>
class Treap
{
private:
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

	// rotateRight()
	// Returns pointer to c (new root after rotation)
	//
	//	       b              c
	//	      / \            / \
	//	     c   X    ->    Y   b
	//	    / \                / \
	//	   Y   Z              Z   X
	//
	//
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


	TreapNode<T>* find(const T& data)
	{
		TreapNode<T>* cursor = root;
		while (true)
		{
			if (cursor == nullptr) return nullptr;
			if (cursor->data == data) return cursor;
			if (data < cursor->data) cursor = cursor->left;
			else cursor = cursor->right;
		}
	}

	int p = 0;


	int generatePriority() 	{
		int pri = rand() % (MAX_PRI - MIN_PRI + 1) + MIN_PRI;
		return pri;
		//return (p*p)%9;
	}

	int count = 0;
	const int MIN_PRI = 1;
	const int MAX_PRI = 100;


	bool isLeftChild(TreapNode<T>* n) { return n != root && n->parent->left == n; }
	bool isLeaf(TreapNode<T>* n) { return n->left == nullptr && n->right == nullptr;}

public:

	TreapNode<T>* root;

	Treap()
	{
		root = nullptr;
		srand(static_cast<int>(time(nullptr)));
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
				TreapNode<T>* cursorParent = cursor->parent;
				if (child == cursor->right)	// Need left rotation
				{
					if (isLeftChild(cursor)) 
						cursorParent->left = rotateLeft(cursor); 
					else 
						cursorParent->right = rotateLeft(cursor);
				}
				else // Need right rotation
				{
					if (isLeftChild(cursor)) 
						cursorParent->left = rotateRight(cursor);
					else 
						cursorParent->right = rotateRight(cursor);
				}
				cursor = cursor->parent;
			}
			else
				break;
		}
	}


	void remove(const T& data)
	{
		TreapNode<T>* toKill = find(data);
		if(toKill == nullptr) return;

		//INV: toKill points to node to remove

		if (isLeaf(toKill))
		{
			if (toKill == root)
			{
				root = nullptr;
				delete toKill;
				return;
			}
			// INV: toKill is not root
			if (isLeftChild(toKill)) toKill->parent->left = nullptr;
			else toKill->parent->right = nullptr;
			delete toKill;
			return;
		}

		// INV: toKill is non-leaf
		if (toKill->left == nullptr || toKill->right == nullptr)
		{
			// INV: toKill has a single child
			TreapNode<T>* singleChild = (toKill->left != nullptr ? toKill->left : toKill->right);

			if (toKill == root) root = singleChild;
			else
			{
				if (isLeftChild(toKill)) toKill->parent->left = singleChild;
				else toKill->parent->right = singleChild;
				singleChild->parent = toKill->parent;
			}
			delete toKill;
			return;
		}
			


		// INV: toKill has two children
		if (toKill == root)
			// INV: toKill is root
			root = (toKill->left->pri > toKill->right->pri ? rotateRight(toKill) : rotateLeft(toKill));

		
		while(toKill->left != nullptr && toKill->right != nullptr)	// toKill has two children - rotate further down in treap, always AWAY from higher-priority child
		{
			TreapNode<T>* toKillParent = toKill->parent;

			// INV: toKill is non-root
			if (toKill->left->pri > toKill->right->pri)
			{
				// Left child must become root => RIGHT rotation needed
				if (isLeftChild(toKill)) toKillParent->left = rotateRight(toKill);
				else toKillParent->right = rotateRight(toKill);
			}
			else
			{
				// Right child must become root => LEFT rotation needed
				if (isLeftChild(toKill)) toKillParent->left = rotateLeft(toKill);
				else toKillParent->right = rotateLeft(toKill);
			}
		}
		
		// Reconnect single child and remove
		TreapNode<T>* singleChild = (toKill->left != nullptr ? toKill->left : toKill->right);

		if (isLeftChild(toKill))
			toKill->parent->left = singleChild;
		else 
			toKill->parent->right = singleChild;

		singleChild->parent = toKill->parent;
		delete toKill;
	}


	bool search(const T& data)
	{
		return find(data) != nullptr;
	}



};
