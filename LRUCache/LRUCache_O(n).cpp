#include <iostream>
#include <vector>
#include <map>
#include<ext/hash_map>
using namespace __gnu_cxx;
using namespace std;

class LRUCache{
public:
    LRUCache(int capacity) {
        this->capacity = capacity;
    }

    int get(int key) {
        bool hit = false;
        vector<int>::iterator target;
        for(vector<int>::iterator iter = cache.begin(); iter != cache.end(); ++iter)
            if(*iter == key)
            {
                hit = true;
                target = iter;
                break;
            }
        if(!hit)
            return -1;
        cache.erase(target);
        cache.insert(cache.begin(), key);
        return memory[key];
    }

    void set(int key, int value) {
        if(cache.size() == capacity)
        {
            memory.erase(*(cache.end()-1));
            cache.pop_back();
        }
        cache.insert(cache.begin(), key);
        memory[key] = value;
    }
private:
    map<int> cache;
    hash_map<int, int> memory;
    int capacity;
};

int main()
{
    LRUCache c = LRUCache(2048);
    c.set(1178,3401);
    c.set(903,6060);
    cout << c.get(1178);

    system("pause");
    return 0;
}

