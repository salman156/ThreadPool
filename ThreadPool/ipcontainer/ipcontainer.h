#include <vector>
#include "../threadsafe_queue/atomic_threadsafe_queue/threadsafe_queue.h"
#include <memory>
#include <mutex>

template<typename T>
class ipcontainer{
    static std::vector<T> container;
    static threadsafe_queue<int,64> queue;
    static std::mutex alloc_mutex;
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
        else{
            std::scoped_lock<std::mutex> lock(alloc_mutex);
            container.push_back(val);
        }

        return pos;
    }
    static int deleteIP(int pos){
        queue.push(pos);
    }

    T& operator[](int pos){
        std::scoped_lock<std::mutex> lock(alloc_mutex);
        return container[pos];
    }
};