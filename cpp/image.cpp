#include <complex>
#include <exception>
#include <tuple>
#include <iostream>
#include <cpp/gmp_complex.h>
#include <cpp/image.h>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <stack>
#include <vector>
#include <atomic>

struct ThreadWork {
    ThreadWork(IMG::iter_t it) : x_coord(it) {
    }
    IMG::iter_t x_coord;
};

template <class T>
class ThreadSafeStack {
public:
    template <class... U>
    void Emplace(U&&... args) {
        size_.fetch_add(1);
        stack_.emplace(std::forward<U>(args)...);
    }

    void Pop() {
        size_.fetch_add(-1);
        stack_.pop();
    }

    const T& Top() const {
        return stack_.top();
    }

    size_t Size() const {
        return size_.load();
    }

    std::unique_lock<std::mutex> GetLock() const {
        return std::unique_lock(mutex_);
    }

private:
    mutable std::mutex mutex_;
    std::stack<T> stack_;
    std::atomic<size_t> size_{0};
};

void ThreadWorker(ThreadSafeStack<ThreadWork>* stack_p,
                  const IMG::ImageParams<IMG::rsize_t>& params, IMG::Image<IMG::iter_t>* image_p,
                  IMG::iter_t max_iter);

namespace IMG {
iter_t iter_function(const ComplexT& c, iter_t max_iter) {
    iter_t num_iter = 0;
    ComplexT z = {0, 0};
    while (num_iter < max_iter) {
        z = z * z + c;
        if (z.Abs2() > 4) {
            break;
        }
        ++num_iter;
    }
    return num_iter;
}

ComplexT get_complex(const ImageParams<IMG::rsize_t>& params, rsize_t x, rsize_t y) {
    const auto ur_corner_centered = params.ur_corner - params.ll_corner;

    const auto x_skip = ur_corner_centered.Real() / (params.x_res);
    const auto y_skip = ur_corner_centered.Imag() / (params.y_res);

    return {x_skip * x + params.ll_corner.Real(), y_skip * y + params.ll_corner.Imag()};
}

void build_image(Image<iter_t>* image, iter_t max_iter, ExecutionPolicy policy) {
    const ImageParams<IMG::rsize_t>& params = image->GetParams();

    if (policy == ExecutionPolicy::kSingleThread) {
        for (iter_t x = 0; x < params.x_res; ++x) {
            for (iter_t y = 0; y < params.y_res; ++y) {
                auto z = get_complex(params, x, y);
                image->At(x, y) = iter_function(z, max_iter);
            }
        }
    } else if (policy == ExecutionPolicy::kMultiThread) {
        ThreadSafeStack<ThreadWork> stack;
        for (iter_t x = 0; x < params.x_res; ++x) {
            stack.Emplace(x);
        }

        std::vector<std::thread> workers;
        for (unsigned int i = 0; i < std::thread::hardware_concurrency(); ++i) {
            workers.emplace_back([&] { ThreadWorker(&stack, params, image, max_iter); });
        }

        for (auto& worker : workers) {
            worker.join();
        }
    } else {
        throw std::runtime_error("Unknown execution policy");
    }
}
}  // namespace IMG

void ThreadWorker(ThreadSafeStack<ThreadWork>* stack_p,
                  const IMG::ImageParams<IMG::rsize_t>& params, IMG::Image<IMG::iter_t>* image_p,
                  IMG::iter_t max_iter) {
    while (stack_p->Size()) {
        IMG::iter_t current_x;
        {
            auto lock = stack_p->GetLock();
            if (stack_p->Size()) {
                current_x = stack_p->Top().x_coord;
                stack_p->Pop();
            } else {
		return;
	    }
        }
        for (IMG::iter_t y = 0; y < params.y_res; ++y) {
            auto z = get_complex(params, current_x, y);
            image_p->At(current_x, y) = IMG::iter_function(z, max_iter);
        }
    }
}
