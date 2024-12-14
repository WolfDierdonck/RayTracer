#include <iostream>

#include "RenderingThreadPool.hpp"

RenderingThreadPool::RenderingThreadPool(size_t num_threads, size_t width_, size_t height_)
    : width(width_), height(height_)
{
    threads.reserve(num_threads);
}

void RenderingThreadPool::process(std::function<void(uint32_t, uint32_t)> func)
{
    pixel_function = std::move(func);

    for (size_t i = 0; i < threads.capacity(); ++i)
    {
        threads.emplace_back([this]()
                             {
            while (true)
            {
                // Get next row atomically
                size_t y = current_row.fetch_add(1);
                
                // Exit if no more rows
                if (y >= height) {
                    break;
                }

                if (y % (height / 10) == 0)
                {
                    std::cout << "Progress: " << (y / (height / 10)) * 10 << "%" << " for y = " << y << std::endl;
                }
                
                // Process entire row
                for (uint32_t x = 0; x < width; ++x) {
                    pixel_function(x, y);
                }
            } });
    }
}

RenderingThreadPool::~RenderingThreadPool()
{
    for (auto &thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}
