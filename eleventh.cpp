
#include <iostream>
#include <tuple>
#include <type_traits>

template <typename Fn, typename Arg> class BasicTask {
    public:

    using Ret = std::invoke_result_t<Fn, Arg>;

    void Execute(Arg arg) {
        Fn fn;
        result_ = fn(arg);
        is_ready_ = true;
    }

    bool IsReady() {
        return is_ready_;
    }

    Ret GetResult() {
        return result_;
    }

    private:
    bool is_ready_ = false;
    Ret result_;
};

template <typename Task, typename ... Tasks> class StatelessPollable {
    public:

    template <typename ... Args> bool PollWithArgs(Args ... args) {
        if (task_.IsReady())
            return next_.PollWithArgs(task_.GetResult());
        else
            task_.Execute(args...);

        return false;
    }

    // private:
    Task task_;
    StatelessPollable<Tasks...> next_;
};

template <typename Task> class StatelessPollable<Task> {
    public:
    template <typename ... Args> bool PollWithArgs(Args ... args) {
        if (!task_.IsReady())
            task_.Execute(args...);

        return true;
    }

    // private:
    Task task_;
};

template <typename Pollable, typename Arg> class StatefullPollable {
    public:
    StatefullPollable(Arg arg) : arg_(arg) {}
    StatefullPollable() {}

    void SetArg(Arg arg) {
        arg_ = arg;
    }
    
    bool Poll() {
        return stateless_pollable_.PollWithArgs(arg_);
    }
    
    private:
    Pollable stateless_pollable_;
    Arg arg_;
};

template <typename Fn, typename Arg, typename ... Other> class BasicAsync {
    public:

    BasicAsync() {}
    
    template <typename NextArg> auto Then(auto nextfn) {
        return BasicAsync<Fn, Arg, Other..., decltype(nextfn), NextArg>();
    }

    private:
    // Pollable<TaskType> pollable_;
};

template <typename Fn, typename Arg> class BasicAsync<Fn, Arg> {
    public:

    BasicAsync() {}

    template <typename NextArg> auto Then(auto nextfn) {
        return BasicAsync<Fn, Arg, decltype(nextfn), NextArg>();
    }

    private:
    // Pollable<TaskType> pollable_;
};

template<typename ArgT> auto Async(auto fn) {
    return BasicAsync<decltype(fn), ArgT>();
}

// TODO Make it recursive
template <typename ... Args> auto MakeTask(BasicAsync<Args...> async, auto p_arg) {
    auto args = std::tuple<Args...>();
    auto [fn1, arg1, fn2, arg2, fn3, arg3] = args;
    StatelessPollable<BasicTask<decltype(fn1), decltype(arg1)>, BasicTask<decltype(fn2), decltype(arg2)>, BasicTask<decltype(fn3), decltype(arg3)>> stateless;
    StatefullPollable<decltype(stateless), decltype(p_arg)> statefull(p_arg);
    return statefull;
}

int main() {
    auto logint = [](int i) {
        std::cout << "log value is " << i << std::endl;
        return i + 1;
    };

    auto logintretstr = [](int i) {
        std::cout << "log value is " << i << std::endl;
        return "hello";
    };

    auto logstr = [](const char* str) {
        std::cout << str << std::endl;
        return 0;
    };
    
    // BasicTask<decltype(logint), int> task1;
    // BasicTask<decltype(logint), int> task2;
    // BasicTask<decltype(logintretstr), int> task3;
    // BasicTask<decltype(logstr), const char*> task4;

    // StatelessPollable<decltype(task1), decltype(task2), decltype(task3), decltype(task4)> async;
    // StatefullPollable<decltype(async), int> async1(100);

    // while (!async1.Poll()) {}

    auto a = Async<int>([](int i){
        std::cout << "log value is " << i << std::endl;
        return i + 1; 
    }).Then<int>([](int i){
        std::cout << "log value is " << i << std::endl;
        return "hello";
    }).Then<const char*>([](const char* str){
        std::cout << str << std::endl;
        return 0;
    });

    auto b = MakeTask(a, 500);

    while (!b.Poll()) {}

    return 0;
}
