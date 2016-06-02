#ifndef __SBTREE_H__
#define __SBTREE_H__

template<class K,class V>
struct SBTNode
{
	SBTNode* _left;
	SBTNode* _right;
	K _key;
	V _value;

	SBTNode(const K& key, const V& value)
		:_left(NULL)
		, _right(NULL)
		, _key(key)
		, _value(value)
	{}

};


template<class K,class V>
class SBTree
{
	typedef SBTNode<K, V> Node;

public:
	SBTree()
		:_root(NULL)
	{}

	bool Insert(const K& key, const V& value)
	{
		if (_root == NULL)
		{
			_root = new Node(key, value);
		}

		Node* cur = _root;
		Node* parent = NULL;
		while (cur)
		{
			if (cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}

			else
			{
				return false;
			}
		}

		if (parent->_key > key)
		{
			parent->_left = new Node(key, value);
		}
		else
		{
			parent->_right = new Node(key, value);
		}

		return true;
	}

	Node* Find(const K&key)
	{
		Node* cur = _root;
		while (cur)
		{
			if (cur->_key > key)
			{
				cur = cur->_left; 
			}
			else if (cur->_key < key)
			{
				cur = cur->_right;
			}
			else
			{
				return cur;
			}
		}

		return NULL;
	}

	bool Remove(const K&key)
	{
		if (_root == NULL)
			return false;

		if (_root->_left == NULL && _root->_right == NULL)
		{
			if (_root->_key == key)
			{
				delete _root;
				_root = NULL;
				return true;
			}

			return false;
		}

		Node* parent = NULL;
		Node* cur = _root;
		while (cur)
		{
			Node* del = NULL;
			if (cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				if (cur->_left == NULL)
				{
					if (parent->_key >cur->_key)
					{
						del = cur;
						parent->_left = cur->_right;
						delete del;
						return true;
					}
					else if (parent->_key < key)
					{
						del = cur;
						parent->_right = cur->_right;
						delete del;
						return true;
					}
				}
				else if (cur->_right == NULL)
				{
					if (parent->_key >cur->_key)
					{
						del = cur;
						parent->_left = cur->_left;
						delete del;
						return true;
					}
					else if (parent->_key < cur->_key)
					{
						del = cur;
						parent->_right = cur->_left;
						delete del;
						return true;
					}
				}
				else
				{
					Node* del=cur;
					Node* parent = NULL;
					Node* RightFirst = cur->_right;
					if (RightFirst->_left ==NULL)
					{
						swap(RightFirst->_key, cur->_key);
						swap(RightFirst->_value, cur->_value);
						del = RightFirst;
						cur->_right = RightFirst->_right;
						delete del;
						return true;
					}

					while (RightFirst->_left)
					{
						parent = RightFirst;
						RightFirst = RightFirst->_left;
					}

					swap(RightFirst->_key, cur->_key);
					swap(RightFirst->_value, cur->_value);

					del = RightFirst;
					parent->_left = RightFirst->_right;
					delete del;
					return true;

				}
			}
		}
		return false;
	}

	bool Insert_R(const K&key,const V&value)
	{
		return _Insert_R(_root,key, value);
	}

	Node* Find_R(const K& key)
	{
		return _Find_R(_root, key);
	}
	

	bool Remove_R(const K&key)
	{
		return _Remove_R(_root, key);
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

protected:
	bool _Remove_R(Node*& root, const K& key)
	{
		//没有节点
		if (root == NULL)
			return false;

		//只有一个根节点，判断是不是然后删除
		if (root->_left == NULL && root->_right == NULL)
		{
			if (root->_key == key)
			{
				delete root;
				root = NULL;
				return true;
			}
			else
				return false;
		}

		if (root->_key > key)
		{
			_Remove_R(root->_left, key);
		}
		else if (root->_key < key)
		{
			_Remove_R(root->_right, key);
		}
		else
		{
			Node* del = NULL;
			if (root->_left == NULL)
			{
				del = root;
				root = root->_right;
				delete del;
				del = NULL;
				return true;
			}
			else if (root->_right == NULL)
			{
				del = root;
				root = root->_left;
				delete del;
				del = NULL;
				return true;
			}
			else
			{
				Node* RightFirst = root->_right;
				while (RightFirst->_left)
				{
					RightFirst = RightFirst->_left;
				}

				swap(root->_key, RightFirst->_key);
				swap(root->_value, RightFirst->_value);



				//删除的时候为什么选择root->_right，当其为作为空时，就相当于把要删除的节点的前一个节点传过去了，为什么不传RightFirst这个变量呢，不具有引用的传递性，从而选择了root->_right代替，能够衔接上引用的传递，该节点的root就是上一个节点的root->_right或者root->_left
				//为什么不用root作为删除节点的根？因为交换以后root的值比要删除的这个值大，会往左边去找，从而出错，实际上交换以后在右边的第一个的最左边。
				_Remove_R(root->_right, key);

				return true;
			}
		}
	}

	Node* _Find_R(Node* root, const K& key)
	{
		if (root == NULL)
			return NULL;

		if (root->_key > key)
		{
			return _Find_R(root->_left, key);
		}
		else if (root->_key < key)
		{
			return _Find_R(root->_right, key);
		}

		else
		{
			return root;
		}
	}

	bool _Insert_R(Node*& root,const K&key, const V&value)
	{
		if (root == NULL)
		{
			root = new Node(key, value);
			return true;
		}

		if (root->_key > key)
			return _Insert_R(root->_left, key, value);

		else if (root->_key < key)
			return _Insert_R(root->_right, key, value);

		else
			return false;
	}

	void _InOrder(Node* root)
	{
		if (root == NULL)
			return;

		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}

protected:
	Node* _root;
};




void Test1()
{
	SBTree<int, int> s;
	s.Insert_R(5, 1);
	s.Insert_R(4, 1);
	s.Insert_R(3, 1);
	s.Insert_R(6, 1);
	s.Insert_R(1, 1);
	s.Insert_R(2, 1);
	s.Insert_R(9, 1);
	s.Insert_R(8, 1);
	s.Insert_R(7, 1);
	s.Insert_R(0, 1);
	s.InOrder();
	/*cout << s.Find_R(9)->_key << endl;*/
	s.Remove_R(4);
	s.Remove_R(7);
	s.Remove_R(8);
	s.Remove_R(5);
	s.Remove_R(6);
	s.Remove_R(3);
	s.Remove_R(1);
	s.Remove_R(2);
	s.Remove_R(0);
	s.Remove_R(9);
	s.InOrder();


}

#endif //__SBTREE_H__