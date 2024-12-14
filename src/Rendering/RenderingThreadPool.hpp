#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>

class RenderingThreadPool
{
private:
    std::vector<std::thread> threads;
    std::atomic<size_t> current_row{0};
    const size_t width;
    const size_t height;
    std::function<void(uint, uint)> pixel_function;

public:
    RenderingThreadPool(size_t num_threads, size_t width_, size_t height_);

    void process(std::function<void(uint32_t, uint32_t)> func);

    ~RenderingThreadPool();
};