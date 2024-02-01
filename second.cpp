#include <coroutine>
#include <iostream>

template <typename T> class MyTask {
    public:
    MyTask(T&& t) {
        std::cout << "StupidTask ctor" << std::endl;
        value_ = t;
    }

    T Value() { return value_; }

    struct promise_type {
    //     promise_type() {
    //         std::cout << "Primise type ctor" << std::endl;
    //     }
        
    //     MyTask<T> get_return_object() {
    //         return MyTask<T>(33);
    //     }

    //     void return_value(MyTask<T> val) {}
        
    //     std::suspend_never initial_suspend() const noexcept { return {}; }
    //     std::suspend_never final_suspend() const noexcept { return {}; }
    //     void unhandled_exception() noexcept {}
    };

    bool await_ready() {
        std::cout << "await_ready" << std::endl;
        return true;
    }
    void await_suspend(auto t) {
        std::cout << "await_suspend" << std::endl;
        // g_handle = std::move(t);
    }

    T await_resume() {
        std::cout << "await_resume" << std::endl;
        return value_;
    }

    ~MyTask() {
        std::cout << "StupidTask dtor" << std::endl;
    }

    private:
    T value_;
};

MyTask<int> GetValue() {
    return MyTask(44);
}

MyTask<int> GetUsingAwait() {
   auto a = co_await GetValue();
   return GetValue(); 
}

int main() {
    auto a = GetValue();    
    a.Value();
}
