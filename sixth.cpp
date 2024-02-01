#include <iostream>

template <typename Fn, typename ArgT = int, typename ... Next> struct Async {
    ArgT arg;
    bool is_executed = false;
    Async<Next...> next;

    Async(Fn fn, ArgT arg) : arg(arg) { }
    Async(Fn fn) {}
    Async() {}
   
    void Execute() {
        Fn fn;
        next.arg = fn(arg); 
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

    template<typename NextFn> auto Then(NextFn fn) -> Async<Fn, ArgT, Next..., NextFn, int> {
        Fn f;
        return Async<Fn, ArgT, Next..., NextFn, int>(f, arg);
    }

};

template <typename Fn, typename ArgT> struct Async<Fn, ArgT> {
    ArgT arg;
    bool is_executed = false;

    Async(Fn fn, ArgT arg) : arg(arg) { }
    Async(Fn fn) {}
    Async() {}

    template<typename NextFn> auto Then(NextFn fn) -> Async<Fn, ArgT, NextFn, int> {
        Fn f;
        return Async<Fn, ArgT, NextFn, int>(f, arg);
    }

    void Execute() {
        Fn fn;
        fn(arg); 
    }

    bool Poll() {
        if(!is_executed) {
            Execute();
            is_executed = true;
        }

        return true;
    }

};

int main() {
    auto a = Async([](auto f){
        std::cout << "[lambda 1] takes " << f << std::endl;
        return 1234;
    }, 4).Then([](auto f){
        std::cout << "[lambda 2] takes " << f << std::endl;
        return 4321;
    }).Then([](auto f){       
        std::cout << "[lambda 3] takes " << f << std::endl;
    });

    // auto b = Contaioner([](){});
    while (!a.Poll()) {}

    std::cout << sizeof(a) << std::endl;
    // a.Poll();

    // a.Execute();
    // a.next.Execute();
    // a.next.next.Execute();
    // a.next.next.next.Execute();
    // a.next.next.next.next.Execute();

    // std::cout << "sizeof container " << sizeof(a) << std::endl;

    return 0;
}
