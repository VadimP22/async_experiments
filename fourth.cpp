#include <iostream>
template <typename Fn, typename ... Next> struct Container {
    // bool is_current = false;

    Container(Fn fn) {}
    Container() {}

    Container<Next...> next;

    auto Execute() {
        Fn fn;
        return fn(); 
    }

    template<typename NextFn> auto Then(NextFn fn) -> Container<Fn, Next..., NextFn> {
        return Container<Fn, Next..., NextFn>();
    }
};

template <typename Fn> struct Container<Fn> {
    // bool is_current = false;

    Container(Fn fn) {}
    Container() {}

    template<typename NextFn> auto Then(NextFn fn) -> Container<Fn, NextFn> {
        return Container<Fn, NextFn>();
    }

    auto Execute() {
        Fn fn;
        return fn(); 
    }
};

int main() {
    auto a = Container ([](){
        std::cout << "lambda 1" << std::endl;
    }).Then([](){
        std::cout << "lambda 2" << std::endl;
    }).Then([](){
        std::cout << "lambda 3" << std::endl;        
    }).Then([](){
        std::cout << "lambda 4" << std::endl;        
    }).Then([](){
        std::cout << "lambda 5" << std::endl;        
    });

    a.Execute();
    a.next.Execute();
    a.next.next.Execute();
    a.next.next.next.Execute();
    a.next.next.next.next.Execute();

    std::cout << "sizeof container " << sizeof(a) << std::endl;

}
