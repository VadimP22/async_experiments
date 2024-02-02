#include <iostream>

template <typename Fn, typename Arg, typename ... Other> struct Async {
    Arg arg;
    Async<Other...> next;
    bool is_executed = false;

    Async(Fn fn) {}
    Async(Fn fn, Arg arg) : arg(arg) {}
    Async() {}

    template <typename T> bool Poll(T val) {
        if (is_executed) {
            return next.Poll();
        } else {
            arg = val;
            Execute();
            is_executed = true;
        }
        return false;
    }

    bool Poll() {
        if (is_executed) {
            return next.Poll();
        } else {
            Execute();
            is_executed = true;
        }
        return false;
    }

    void Execute() {
        Fn fn;
        next.arg = fn(arg);
    }

    template<typename ArgT> auto Then(auto fn) -> Async<Fn, Arg, Other..., decltype(fn), ArgT> {
        return Async<Fn, Arg, Other..., decltype(fn), ArgT>();
    }
};

template <typename Fn, typename Arg> struct Async <Fn, Arg> {
    Arg arg;
    bool is_executed = false;
    
    Async(Fn fn) {}
    Async(Fn fn, Arg arg) : arg(arg) {}
    Async() {}

    template <typename T> bool Poll(T val) {
       if (!is_executed) {
            // arg = val;
            Execute();
            is_executed = true;     
        } 
        return true;
    }

    bool Poll() {
        if (!is_executed) {
            // arg = val;
            Execute();
            is_executed = true;     
        } 
        return true;
    }

    void Execute() {
        Fn fn;
        fn(arg);
    }

    template<typename ArgT> auto Then(auto fn) -> Async<Fn, Arg, decltype(fn), ArgT> {
        return Async<Fn, Arg, decltype(fn), ArgT>();
    }
};

int main() {


    auto a = Async([](auto arg){
        std::cout << "lambda 1 takes " << arg << std::endl;
        return 123;
    }, "").Then<int>([](auto val){
        std::cout << "lambda 2 takes " << val << std::endl;
        return "fwe";
    }).Then<const char*>([](auto value){
        std::cout << "lambda 3 takes " << value << std::endl;
        return 234;        
    });

    // a.Poll();
    while (!a.Poll("hello")) {}
    std::cout << sizeof(a) << std::endl;

    return 0;
}
