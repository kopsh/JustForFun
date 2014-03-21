/*************************************************************************
    > File Name: btree.cpp
    > Author: kopsh
    > Mail: kopbupt@126.com
    > Created Time: Thu 20 Mar 2014 07:46:43 PM CST
 ************************************************************************/

#include <iostream>
#include <vector>
#include <queue>
#include <limits.h>
using namespace std;

#define ORDER 2
#define MAX (ORDER*2-1)

struct TreeNode {
	vector<int> keys;
	vector<TreeNode*> children;
	TreeNode* parent;
	TreeNode() : parent(NULL) {}
	~TreeNode() {
		for(int i=0; i < children.size(); ++i)
			delete children[i];
	}
	int key_count() {
		return keys.size();
	}
	int children_count() {
		return children.size();
	}
	bool is_leaf() {
		return (children.size() == 0);
	}
	bool is_full() {
		return (keys.size() == MAX);
	}
	int get_key(int i) {
		if(i >= 0 && i < keys.size())
			return keys[i];
		return INT_MAX;
	}
	TreeNode*  get_child(int i) {
		if(i >= 0 && i < children.size())
			return children[i];
		return NULL;
	}

};

TreeNode *root;    //根结点

TreeNode* spilt(TreeNode *node, int val);
void print_tree(TreeNode *root);

bool insert_node(TreeNode *node, int val)
{
	if(node->is_leaf())
	{
		if(!node->is_full())	//非满叶结点直接插入
		{
			vector<int> &k = node->keys;
			for(vector<int>::iterator iter = k.begin(); iter != k.end(); ++iter)
			{
				if(val < *iter)
				{
					k.insert(iter, val);
					return true;
				}
				else if(val == *iter)
					return false;
			}
			k.push_back(val);
			return true;
		}
		else
		{
			node = spilt(node, val);
			insert_node(node, val);
		}
	}
	else
	{
		if(!node->is_full())	//继续向下查找
		{
			vector<int> &k = node->keys;
			int pos = 0;
			for(;pos < k.size(); ++pos)
			{
				if(k[pos] == val)
					return false;
				else if(k[pos] > val)
					break;
			}
			return insert_node(node->get_child(pos), val);
		}
		else	//分裂后继续查找
		{
			int mid = node->get_key((MAX-1)/2);
			if(val == mid)
				return false;
			node = spilt(node, val);
			insert_node(node, val);
		}
	}
}

TreeNode* spilt(TreeNode *node, int val)
{
	int mid = node->get_key((MAX-1)/2);
	if(node->parent == NULL)	//若结点为根结点，则高度加1
	{
		node->parent = new TreeNode();
		(node->parent->children).push_back(node);
		root = node->parent;
	}
	TreeNode* temp_node = new TreeNode();
	temp_node->parent = node->parent;
	(node->parent->children).push_back(temp_node);
	(node->parent->keys).push_back(mid);

	vector<int> &k1 = node->keys;
	vector<int> &k2 = temp_node->keys;
	if(!node->is_leaf())
    {
        vector<TreeNode*> &c1 = node->children;
        vector<TreeNode*> &c2 = temp_node->children;

        int key = *(k1.end()-1);
        TreeNode* child = *(c1.end()-1);

        while(key != mid)	//将原结点关键字以及孩子的后半部分“剪切-粘贴”到新结点上
        {
            k2.insert(k2.begin(), key);
            c2.insert(c2.begin(), child);

            k1.pop_back();
            c1.pop_back();

            key = *(k1.end()-1);
            child = *(c1.end()-1);
        }

        k1.pop_back();
        c2.insert(c2.begin(), child);
        c2.pop_back();
    }
    else
    {
        int key = *(k1.end()-1);

        while(key != mid)	//将原结点关键字的后半部分“剪切-粘贴”到新结点上
        {
            k2.insert(k2.begin(), key);
            k1.pop_back();
            key = *(k1.end()-1);
        }
        k1.pop_back();
    }

	if(val > mid)
		return temp_node;
	return node;
}

void del_node(TreeNode *root, int val)
{

}

void print_tree(TreeNode *root)
{
	if(root == NULL || root->key_count() == 0)
	{
		cout << "树为空" << endl;
		return;
	}

	queue<TreeNode*> q;
	q.push(root);
	q.push(NULL);

	while(!q.empty())
	{
		TreeNode *top = q.front();
		q.pop();

		while(top != NULL)
		{
			for(int i = 0; i < top->key_count(); ++i)
				cout << top->get_key(i) << " ";
			for(int i = 0; i < top->children_count(); ++i)
				q.push(top->get_child(i));

			top = q.front();
			q.pop();
			if(top == NULL)
			{
				q.push(NULL);
				cout << endl;
			}
			else
				cout << " | ";
		}
	}
}

int main()
{
    root = new TreeNode();
	insert_node(root, 1);
	insert_node(root, 2);
	insert_node(root, 3);
	insert_node(root, 4);
	insert_node(root, 5);
	insert_node(root, 6);
	insert_node(root, 7);
	insert_node(root, 8);
	insert_node(root, 9);
	insert_node(root, 10);
	insert_node(root, 11);
	insert_node(root, 12);
	print_tree(root);
	delete root;
}
