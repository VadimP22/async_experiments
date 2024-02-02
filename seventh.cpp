#include <iostream>
#include <type_traits>

template <typename Fn, typename Arg, typename ... Other> struct Async {
    Arg arg;
    Async<Other...> next;

    Async(Fn fn, Arg arg) : arg(arg) {}
    Async(Fn fn) : arg(arg) {}
    Async() {}

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
    
    Async(Fn fn, Arg arg) : arg(arg) {}
    Async(Fn fn) : arg(arg) {}
    Async() {}

    void Execute() {
        Fn fn;
        fn(arg);
    }

    template<typename ArgT> auto Then(auto fn) -> Async<Fn, Arg, decltype(fn), ArgT> {
        return Async<Fn, Arg, decltype(fn), ArgT>();
    }
};

int main() {

    auto a = Async([](auto a){
        std::cout << "lambda 1 taskes " << a << std::endl;
        return 123;
    }, 33).Then<int>([](auto val){
        std::cout << "lambda 2 taskes " << val << std::endl;
        return "fwe";
    }).Then<const char*>([](auto value){
        std::cout << "lambda 3 taskes " << value << std::endl;
        return 234;        
    });

    a.Execute();
    a.next.Execute();
    a.next.next.Execute();

    std::cout << sizeof(a) << std::endl;

    return 0;
}
