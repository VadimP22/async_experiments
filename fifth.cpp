#include <iostream>

template <typename Fn, typename ArgT = int, typename ... Next> struct Container {
    // bool is_current = false;
    ArgT arg;

    Container(Fn fn, ArgT arg) {}
    Container(Fn fn) {}
    Container() {}

    Container<Next...> next;

    void Execute() {
        Fn fn;
        next.arg = fn(arg); 
    }

    template<typename NextFn, typename NextArgT> auto Then(NextFn fn, ArgT arg) -> Container<Fn, ArgT, Next..., NextFn, NextArgT> {
        return Container<Fn, ArgT, Next..., NextFn, NextArgT>();
    }

    template<typename NextFn> auto Then(NextFn fn) -> Container<Fn, ArgT, Next..., NextFn, int> {
        return Container<Fn, ArgT, Next..., NextFn, int>();
    }

};

template <typename Fn, typename ArgT> struct Container<Fn, ArgT> {
    // bool is_current = false;
    ArgT arg;

    Container(Fn fn, ArgT arg) {}
    Container(Fn fn) {}
    Container() {}

    template<typename NextFn, typename NextArgT> auto Then(NextFn fn, ArgT arg) -> Container<Fn, ArgT, NextFn, NextArgT> {
        return Container<Fn, ArgT, NextFn, NextArgT>();
    }

    template<typename NextFn> auto Then(NextFn fn) -> Container<Fn, ArgT, NextFn, int> {
        return Container<Fn, ArgT, NextFn, int>();
    }

    void Execute() {
        Fn fn;
        fn(arg); 
    }
};

int main() {
    auto a = Container([](auto f){
        std::cout << "[lambda 1] takes " << f << std::endl;
        return 1234;
    }, 4).Then([](auto f){
        std::cout << "[lambda 2] taskes " << f << std::endl;
        return 4321;
    }).Then([](auto f){       
        std::cout << "[lambda 3] taskes " << f << std::endl;
    });

    // auto b = Container([](){});

    a.Execute();
    a.next.Execute();
    a.next.next.Execute();
    // a.next.next.next.Execute();
    // a.next.next.next.next.Execute();

    // std::cout << "sizeof container " << sizeof(a) << std::endl;

    return 0;
}
