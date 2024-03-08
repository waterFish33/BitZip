#pragma once
#include <queue>
#include "FCHuffMan.h"

using namespace std;

	template <class T>
	struct HuffmanTreeNode {
	
		HuffmanTreeNode<T>* _left;
		HuffmanTreeNode<T>* _right;
		T _weight;
	
		HuffmanTreeNode(T weight=T()) :_left(nullptr), _right(nullptr), _weight(weight) {

		}
		HuffmanTreeNode() {

		}

	};
	
	template <class T>
	class HuffmanTree {
		typedef HuffmanTreeNode<T> Node;
		class Compare {
		public:
			bool operator()(const Node* left, const Node* right)const {
				return left->_weight > right->_weight;
			}
		};

	private:
		Node* _root;
		void Destroy(Node*& root) {
			if (root) {
				Destroy(root->_left);
				Destroy(root->_right);
				delete root;
				root = nullptr;
			}
		}
	public:
		HuffmanTree() :_root(nullptr) {
				   
		}
		~HuffmanTree() {
			Destroy(_root);
		}
		HuffmanTree(const vector<T>& vt,const T& valid) {
			 //1.把所有权值构建成只有根节点的二叉树森林（用堆进行存储
			priority_queue<Node*,vector<Node*>,Compare> pr;
			for (auto& e : vt) {
				if(valid!=e)
					pr.push(new Node(e));
			}
			while (pr.size() > 1) {
				Node* left = pr.top();
				pr.pop();
				Node* right = pr.top();
				pr.pop();
				//创建一颗新子树，left和right作为左右子树
				Node* parent = new Node(left->_weight + right->_weight);
				parent->_left = left;
				parent->_right = right;
				//将parent放回二叉树森林
				pr.push(parent);
			}
			_root = pr.top();
		}
		
		Node* getroot() {
			return _root;
		}
	};
	


