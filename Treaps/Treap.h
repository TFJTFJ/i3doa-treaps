#pragma once
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>
#include<random>


template <typename T>
class Treap
{
private:
	// internal class to represent a Treap node
	template<typename T>
	class Node
	{
	public:
		T data;
		int pri;
		Node* parent;
		Node* left;
		Node* right;



		Node(const T& d = T(), const int pri = 0, Node* p = nullptr, Node* l = nullptr,
			Node* r = nullptr) :
			data(d), pri(pri), parent(p), left(l), right(r)
		{
		}
	};

	std::mt19937_64 randomizer;

	// rotateLeft()/rotateRight()
	// Parameter: Pointer to node to rotate (RR: b, LR: C)
	// Returns: Pointer to new root after rotation (RR: c, LR: b)
	//
	//	       b       R      c
	//	      / \     ->     / \
	//	     c   X          Y   b
	//	    / \        L       / \
	//	   Y   Z      <-      Z   X
	//
	//
	template<typename T>
	Node<T>* rotateLeft(Node<T>* b)
	{
		if (b == nullptr || b->right == nullptr) return b;
		Node<T>* c = b->right;
		b->right = c->left;
		if (b->right) b->right->parent = b;
		c->left = b;
		c->parent = b->parent;
		b->parent = c;

		return c;
	}


	template<typename T>
	Node<T>* rotateRight(Node<T>* b)
	{
		if (b == nullptr || b->left == nullptr) return b;
		Node<T>* c = b->left;
		b->left = c->right;
		if (b->left) b->left->parent = b;
		c->right = b;
		c->parent = b->parent;
		b->parent = c;

		return c;
	}


	Node<T>* find(const T& data)
	{
		Node<T>* cursor = root;
		while (true)
		{
			if (cursor == nullptr) return nullptr;
			if (cursor->data == data) return cursor;
			if (data < cursor->data) cursor = cursor->left;
			else cursor = cursor->right;
		}
	}




	Node<T>* root;

	int recDepth(Node<T>* root) 
	{
		return root ? 1 + std::max(recDepth(root->left), recDepth(root->right)) : 0;
	}

	int recCount(Node<T>* root)
	{
		return root ? 1 + recCount(root->left) + recCount(root->right) : 0;
	}

	void deleteTree(Node<T>* root)
	{
		if (root)
		{
			deleteTree(root->left);
			deleteTree(root->right);
			delete root;
		}
	}

	
	int generatePriority() { return static_cast<int>(randomizer()); }
	bool isLeftChild(Node<T>* n) { return n != root && n->parent->left == n; }
	bool isLeaf(Node<T>* n) { return n->left == nullptr && n->right == nullptr; }

public:

	int depth() 	{	return recDepth(root);	}
	int count()		{	return recCount(root);	}
	~Treap()		{	deleteTree(root);		}
	Treap()	: randomizer(std::random_device{}()), root(nullptr)	{	}

	void insert(const T& data)
	{
		int pri = generatePriority();

		// If treap is empty, insert data at root
		if (!root)
		{
			root = new Node<T>(data, pri);
			return;
		}

		// Find place to insert data and insert it
		Node<T>* cursor = root;
		Node<T>* child;

		while (true)
		{
			if (data < cursor->data)
			{
				if (!cursor->left) // Insertion point (leaf) reached
				{
					cursor->left = new Node<T>(data, pri, cursor);
					child = cursor->left;
					break;
				}
				cursor = cursor->left; // Keep looking
			}
			else if(data > cursor->data)
			{
				if (!cursor->right) // Insertion point (leaf) reached
				{
					cursor->right = new Node<T>(data, pri, cursor);
					child = cursor->right;
					break;
				}
				cursor = cursor->right; // Keep looking
			}
			else 
				return;	// duplicates not allowed
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
				Node<T>* cursorParent = cursor->parent;
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
		Node<T>* toKill = find(data);
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
			Node<T>* singleChild = (toKill->left != nullptr ? toKill->left : toKill->right);

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
			Node<T>* toKillParent = toKill->parent;

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
		Node<T>* singleChild = (toKill->left != nullptr ? toKill->left : toKill->right);

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
