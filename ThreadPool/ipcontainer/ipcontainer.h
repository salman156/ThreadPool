#include <vector>
#include "../threadsafe_queue/atomic_threadsafe_queue/threadsafe_queue.h"
#include <memory>

template<typename T>
class ipcontainer{
    static std::vector<T> container;
    static threadsafe_queue<int,64> queue;
    static bool init = Init();
    static int Init(){
        container = vector<T>();
        return 0;
    }
public:
    static int newIP(T val){

        int pos = 0;

        if(auto ptr_item = queue.try_pop(); ptr_item)
        {
            container[*ptr_item] = val;
            pos = *ptr_item;
        }
        else
            container.push_back(val);

        return pos;
    }
    static int deleteIP(int pos){
        queue.push(pos);
    }
    
    T& operator[](int pos){
        return container[pos];
    }
};