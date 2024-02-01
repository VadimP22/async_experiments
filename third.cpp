
#include <iostream>
#include <cstdint>
#include <tuple>


template <typename Arg, typename ... Args> struct RawContainer {
    
    Arg value = Arg({});
    bool is_current = false;

    int next_size = sizeof...(Args);
    RawContainer<Args...> next;
};

template <typename Arg> struct RawContainer<Arg> {
    Arg value = Arg({});
    bool is_current = false;
};

template <typename Fn, typename ... Args> struct PollableTaskStage {
    std::tuple<Args...> args;

    bool finished = false;

    PollableTaskStage(Fn func, Args ... arguments) : args(arguments...) {};

    void Execute() {
        Fn fn;
        // result = std::apply(fn, args);
    }
};
 
int main() {
    PollableTaskStage first([](){
        return 0;
    });
    PollableTaskStage second([](){
        return 1;
    });
    RawContainer<decltype(first), decltype(second)> container;

    container.value.Execute();
    container.next.value.Execute();
    // std::cout << "size is " << sizeof(a) << std::endl;
}
