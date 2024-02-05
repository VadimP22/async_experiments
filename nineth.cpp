
#include <iostream>
#include <type_traits>

template <typename Fn, typename ... Args> class BasicTask {
    public:

    using Ret = std::invoke_result_t<Fn, Args...>;

    void Execute(Args ... args) {
        Fn fn;
        result_ = fn(args...);
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

// int LogInt(int i) {
//     std::cout << "log value is " << i << std::endl;
//     return i + 1;
// }

int main() {
    auto logint = [](int i) {
        std::cout << "log value is " << i << std::endl;
        return i + 1;
    };
    
    BasicTask<decltype(logint), int> task1;
    BasicTask<decltype(logint), int> task2;
    BasicTask<decltype(logint), int> task3;

    StatelessPollable<decltype(task1), decltype(task2), decltype(task3)> async;
    StatefullPollable<decltype(async), int> async1(100);

    while (!async1.Poll()) {}

    // task1.Execute(123);

    // if (task1.IsReady())
        // std::cout << "returned is " << task1.GetResult() << std::endl;

    return 0;
}
