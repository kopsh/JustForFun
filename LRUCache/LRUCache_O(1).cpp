#include <iostream>
#include <vector>
#include <map>
#include <ext/hash_map>
using namespace __gnu_cxx;
using namespace std;


class LRUCache{
public:
    struct Node {
        int key;
        int val;
        Node *pre;
        Node *next;
        Node(int m, int n): key(m), val(n), pre(NULL), next(NULL) {}
    };

    LRUCache(int capacity) {
        this->capacity = capacity;
        size = 0;
    }

    int get(int key) {
        if(cache.count(key) == 0)
            return -1;
        Node *node = cache[key];

        if(node != first)
        {
            if(node == last)
            {
                last = node->pre;
            }
            else
            {
                node->next->pre = node->pre;
            }
            node->pre->next = node->next;
            node->next = first;
            first->pre = node;
            first = node;
        }
        return first->val;
    }

    void set(int key, int value) {
        if(size == 0)
        {
			Node *node = new Node(key, value);
            first = node;
            last = node;
            ++size;
            cache[key] = node;
        }
        else
        {
			//key在cache中不存在
			if(cache.count(key) == 0)
			{
                Node *node;
				//若cache不满，需要新建结点
				if(size != capacity)
				{
					node = new Node(key, value);
					node->next = first;
					first->pre = node;
					first = node;
					++size;
				}
				//修改最后一个结点
				else
				{
					node = last;
					cache.erase(last->key);
					node->key = key;
					node->val = value;

					if(node != first)
					{
					    last = node->pre;
						node->pre->next = node->next;
						node->next = first;
						first->pre = node;
						node->pre = NULL;
						first = node;
					}
				}
				cache[key] = node;
			}
			//直接修改存储key的结点
			else
			{
				Node *node = cache[key];
				node->val = value;

				if(node != first)
				{
					if(node != last)
						node->next->pre = node->pre;
					else
					    last = node->pre;
					node->pre->next = node->next;
					node->next = first;
					first->pre = node;
					node->pre = NULL;
					first = node;
				}
			}
		}
    }
    Node *first;
    Node *last;
    int size;
private:
    hash_map<int, Node*> cache;
    int capacity;
};