#ifndef _THREAD_HPP_
#define _THREAD_HPP_

#include <functional>


class thread {
  private:
  public:
    thread() noexcept = default;

    template<typename _Callable>
    explicit thread(_Callable&& __f, void* __args) {
          
    }

    ~thread() {

    }

    template<typename _Callable>
    struct _State_impl {
        _Callable		_M_func;	// 线程入口函数
    
        _State_impl(_Callable&& __f) : _M_func(std::forward<_Callable>(__f))
        { }

        void _M_run() { _M_func(); } // 执行线程入口函数
    };

    static void execute_native_thread_routine(const void*)
    {

    }

  }

#endif /* _THREAD_HPP_ */
