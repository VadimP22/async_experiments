#include <coroutine>
#include <exception>
#include <iostream>
#include <memory>

std::coroutine_handle<> g_handle;

template <typename T> class StupidTask {
    public:
    StupidTask(T* t) {
        std::cout << "StupidTask ctor" << std::endl;
        pval_ = t;
    }

    T Value() { return *pval_; }
    
    class promise_type {
        public:
        promise_type() {
            std::cout << "promise_type ctor" << std::endl;
        }
        
        StupidTask<T> get_return_object() {
            std::cout << "get_return_object" << std::endl;
            pval_ = new T;
            return StupidTask<T>(pval_);
        }    
        std::suspend_never initial_suspend() const noexcept {
            return {};
        }

        std::suspend_never final_suspend() const noexcept {
            return {};
        }

        void return_value(T val) {
            std::cout << "return_value" << std::endl;
            *pval_ = val; 
        }

        void unhandled_exception() noexcept {
            std::terminate();
        }

        ~promise_type() {
            std::cout << "promise_type dtor" << std::endl;            
        }

        T* pval_ = nullptr;
    };

    bool await_ready() {
        std::cout << "await_ready" << std::endl;
        return true;
    }
    void await_suspend(auto t) {
        std::cout << "await_suspend" << std::endl;
        g_handle = std::move(t);
    }

    T await_resume() {
        std::cout << "await_resume" << std::endl;
        return *pval_;
    }

    ~StupidTask() {
        std::cout << "StupidTask dtor" << std::endl;
    }

    private:
    T* pval_;
};

template <typename F> void CallSomeFn(F fn) {
    F f;
    f();
}

void hello() {}

StupidTask<int> GetValue() {
    std::cout << "[GetValue] begin" << std::endl;
    co_return 444;
    std::cout << "[GetValue] after co_return" << std::endl;
}

StupidTask<int> Lol() {
    std::cout << "[Lol] begin" << std::endl;
    auto a = co_await GetValue();
    std::cout << "[Lol] after co_await" << std::endl;
    // co_yield GetValue();
    co_return a;
}



int main() {
    std::cout << "[main] begin" << std::endl;

    // auto b = GetValue();
    auto a = Lol();

    std::cout << "[main] after GetValue/Lol" << std::endl;
    
    std::cout << a.Value() << std::endl;

    // std::cout << "[main] call handle" << std::endl;
    // g_handle.resume();
    // std::cout << "[main] call handle finished " << std::endl;
    // std::cout << "[main] call handle.done()" << std::endl;
    // std::cout << "[main] done: " << g_handle.done();
    // std::cout << "[main] call handle.done finished" << std::endl;

    return 0;
}
