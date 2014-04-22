#ifndef _BSTREE_H
#define _BSTREE_H

#include <cstddef> //size_t

template <typename T>
class TreeNode
{
public:
	TreeNode(T val): _left(0), _right(0), _value(val){};
	TreeNode<T>* _left;
	TreeNode<T>* _right;
	T _value;
};

template<typename T>
class BSTree
{
public:
	~BSTree();
	BSTree();
	BSTree(const BSTree<T> & other);
	BSTree& operator=(const BSTree<T>& other);
	size_t size() const;
	bool empty() const;
	void clear();
	bool retrieve(const T& key) const;
	bool insert(const T& key);

	template<typename Iterator>
	void preorderTraverse(Iterator itr) const;

	template<typename Iterator>
	void inorderTraverse(Iterator itr) const;

	template<typename Iterator>
	void postorderTraverse(Iterator itr) const;

private:
	TreeNode<T>* _head;
	std::size_t _size;

	void copyRecurse(TreeNode<T>* from, TreeNode<T>** toPtr);

	template<typename Iterator>
	void postorderRecurse(TreeNode<T>* node, Iterator& itr) const;

	template<typename Iterator>
	void inorderRecurse(TreeNode<T>* node, Iterator& itr) const;

	template<typename Iterator>
	void preorderRecurse(TreeNode<T>* node, Iterator& itr) const;

	void clear_recurse(TreeNode<T>* node);
	bool insert_recurse(TreeNode<T>* node, const T& key);
	TreeNode<T>* find_key(const T& key) const;
};

/*--------------------------------------------------------------*/
/* PUBLIC FUCNTIONS */
/*--------------------------------------------------------------*/

/*
 * BSTree<T>::~BSTree: Deallocate the Binary Search Tree
 * Pre: None
 * Post: None
 * Exception Safety: No-Throw
 */
template<typename T>
BSTree<T>::~BSTree()
{
	if(_size)
		clear_recurse(_head);
}

/*
 * BSTree<T>::BSTree(): Allocates a default BSTree with size of 0
 * Pre: None
 * Post: None
 * Exception Safety: No-Throw
 */
template<typename T>
BSTree<T>::BSTree(): _head(0), _size(0){};

/*
 * BSTree<T>::BSTree(const BSTree<T> & other): Creates a copied BSTree from other
 * Pre: None
 * Post: This now contains the copied contents of other
 * Exception Safety: Basic
 */
template<typename T>
BSTree<T>::BSTree(const BSTree<T> & other)
{
	_size = other._size;
	copyRecurse(other._head, &_head);
}

/*
 * BSTree<T>& BSTree<T>::operator= : copies the contents of other in to this
 * Pre: None
 * Post: This now contains the copied contents of other
 * Exception Safety: Basic
 */
template<typename T>
BSTree<T>& BSTree<T>::operator =(const BSTree<T> & other)
{
	if(&other == this)
		return *this;
	clear();
	_size = other._size;
	copyRecurse(other._head, &_head);
	return *this;
}

/*
 * size_t BSTree<T>::size() : Gets the size of the BSTree
 * Pre: None
 * Post: Returns count of items in the tree
 * Exception Guarantee: No-Throw
 */
template<typename T>
size_t BSTree<T>::size() const {return _size;}

/*
 * bool BSTree<T>::empty() : Returns if the BSTree is empty
 * Pre: None
 * Post: Returns true if this is empty, else false
 * Exception Guarantee: No-Throw
 */
template<typename T>
bool BSTree<T>::empty() const {return _size == 0;}

/*
 * void BSTree<T>::clear() : Clears the BSTree
 * Pre: None
 * Post: This now contains no items
 * Exception Guarantee: No-Throw
 */
template<typename T>
void BSTree<T>::clear()
{
	if(_size)
	{
		clear_recurse(_head);
		_size = 0;
		_head=NULL;
	}
}

/*
 * bool BSTree<T>::retrieve(const T& key) : Returns if the given item is in the BSTree
 * Pre: None
 * Post: Returns true if key is in the tree, else false
 * Exception Guarantee: No-Throw
 */
template<typename T>
bool BSTree<T>::retrieve(const T& key) const
{
	return find_key(key);
}

/*
 * bool BSTree<T>::insert(const T& key): inserts the given key in to the tree
 * Pre: None
 * Post: The tree now contains a copy of key
 * Exception Guarantee: Basic
 */
template<typename T>
bool BSTree<T>::insert(const T& key)
{
	if(!_head)
	{
		_head = new TreeNode<T>(key);
		_size++;
		return true;
	}
	bool result = !insert_recurse(_head, key);
	if(result)
	{
		_size++;
	}
	return result;
}

/*
 * void BSTree<T>::preorderTraverse(Iterator itr): Outputs the contents to the tree in preorder
 * Pre: None
 * Post: Outputs the contents of the tree in preorder to the range starting with itr
 * Exception Safety: Strong
 */
template<typename T>
template<typename Iterator>
void BSTree<T>::preorderTraverse(Iterator itr) const
{
	preorderRecurse(_head, itr);
}

/*
 * void BSTree<T>::inorderTraverse(Iterator itr): Outputs the contents to the tree in inorder
 * Pre: None
 * Post: Outputs the contents of the tree in inorder to the range starting with itr
 * Exception Safety: Strong
 */
template<typename T>
template<typename Iterator>
void BSTree<T>::inorderTraverse(Iterator itr) const
{
	inorderRecurse(_head, itr);
}

/*
 * void BSTree<T>::postorderTraverse(Iterator itr): Outputs the contents to the tree in postorder
 * Pre: None
 * Post: Outputs the contents of the tree in postorder to the range starting with itr
 * Exception Safety: Strong
 */
template<typename T>
template<typename Iterator>
void BSTree<T>::postorderTraverse(Iterator itr) const
{
	postorderRecurse(_head, itr);
}

/*--------------------------------------------------------------*/
/* PRIVATE FUCNTIONS */
/*--------------------------------------------------------------*/

/*
 * void BSTree<T>::copyRecurse(TreeNode<T>* from, TreeNode<T>** toPtr): Copies a (sub)tree starting at from to the node at toPtr
 */
template<typename T>
void BSTree<T>::copyRecurse(TreeNode<T>* from, TreeNode<T>** toPtr)
{
	TreeNode<T>* to = *toPtr;
	if(!from)
	{
		to = NULL;
		return;
	}

	to = new TreeNode<T>(from->_value);
	*toPtr = to;

	try
	{
	copyRecurse(from->_left, &to->_left);
	copyRecurse(from->_right, &to->_right);
	}
	catch(...)
	{
		//Clean up if child creation fails
		delete to;
		throw;
	}
}

/*
 * void BSTree<T>::postorderRecurse(TreeNode<T>* node, Iterator& itr): Recursive call to traverse a tree in postorder
 */
template<typename T>
template<typename Iterator>
void BSTree<T>::postorderRecurse(TreeNode<T>* node, Iterator& itr) const
{
	if(node == NULL)
		return;
	postorderRecurse(node->_left, itr);
	postorderRecurse(node->_right, itr);
	*itr++=node->_value;
}

/*
 * void BSTree<T>::inorderRecurse(TreeNode<T>* node, Iterator& itr): Recursive call to traverse a tree in inorder
 */
template<typename T>
template<typename Iterator>
void BSTree<T>::inorderRecurse(TreeNode<T>* node, Iterator& itr) const
{
	if(node == NULL)
		return;
	inorderRecurse(node->_left, itr);
	*itr++=node->_value;
	inorderRecurse(node->_right, itr);
}

/*
 * void BSTree<T>::preorderRecurse(TreeNode<T>* node, Iterator& itr): Recursive call to traverse a tree in preorder
 */
template<typename T>
template<typename Iterator>
void BSTree<T>::preorderRecurse(TreeNode<T>* node, Iterator& itr) const
{
	if(node == NULL)
		return;
	*itr++=node->_value;
	preorderRecurse(node->_left, itr);
	preorderRecurse(node->_right, itr);
}

/*
 * void BSTree<T>::clear_recurse(TreeNode<T>* node): Recursive call to clear a BSTree
 */
template<typename T>
void BSTree<T>::clear_recurse(TreeNode<T>* node)
{
	if(node->_left)
		clear_recurse(node->_left);
	if(node->_right)
		clear_recurse(node->_right);
	delete node;
}

/*
 * bool BSTree<T>::insert_recurse(TreeNode<T>* node): Recursive call to insert into a BSTree
 */
template<typename T>
bool BSTree<T>::insert_recurse(TreeNode<T>* node, const T& key)
{
	if(key == node->_value)
		return true;
	if(key < node->_value)
	{
		if(node->_left == NULL)
		{
			node->_left = new TreeNode<T>(key);
		}
		else
		{
			return insert_recurse(node->_left, key);
		}
	}
	else//If its greater
	{
		if(node->_right == NULL)
		{
			node->_right = new TreeNode<T>(key);
		}
		else
		{
			return insert_recurse(node->_right, key);
		}
	}
	return false;
}

/*
 * TreeNode<T>* BSTree<T>::find_key(const T& key): Recursive call to check if a key exists in a tree
 */
template<typename T>
TreeNode<T>* BSTree<T>::find_key(const T& key) const
{
	TreeNode<T>* cur = _head;
	while(cur)
	{
		if(key == cur->_value)
			return cur;
		if(key < cur->_value)
			cur = cur->_left;
		else
			cur = cur->_right;
	}
	return cur;
}


#endif
