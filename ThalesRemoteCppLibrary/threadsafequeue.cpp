/******************************************************************
 *  ____       __                        __    __   __      _ __
 * /_  / ___ _/ /  ___  ___ ___________ / /__ / /__/ /_____(_) /__
 *  / /_/ _ `/ _ \/ _ \/ -_) __/___/ -_) / -_)  '_/ __/ __/ /  '_/
 * /___/\_,_/_//_/_//_/\__/_/      \__/_/\__/_/\_\\__/_/ /_/_/\_\
 *
 * Copyright 2024 ZAHNER-elektrik I. Zahner-Schiller GmbH & Co. KG
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
 * THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "threadsafequeue.h"

ThreadsafeQueue::ThreadsafeQueue()
{
    dataAvailable.lock();
}

ThreadsafeQueue::~ThreadsafeQueue() { }

bool ThreadsafeQueue::empty() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return queue.empty();
}

unsigned long ThreadsafeQueue::size() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return queue.size();
}

std::vector<uint8_t> ThreadsafeQueue::pop()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (queue.empty()) {
        return {};
    }
    std::vector<uint8_t> tmp = queue.front();
    queue.pop();
    return tmp;
}

void ThreadsafeQueue::put(const std::vector<uint8_t> &item)
{
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(item);
    dataAvailable.unlock();
}

std::vector<uint8_t> ThreadsafeQueue::get(const bool blocking, const std::chrono::duration<int, std::milli> timeout)
{
    std::vector<uint8_t> retval = {};
    if(blocking == false)
    {
        retval = this->pop();
    }
    else
    {
        std::chrono::milliseconds startTime = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
        std::chrono::duration<int, std::milli> remainingTime;
        std::chrono::duration<int, std::milli> elapsedTime;

        while (this->empty() == true)
        {
            elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch()) - startTime;
            if (elapsedTime > timeout)
            {
                break;
            }
            remainingTime = timeout - elapsedTime;

            dataAvailable.try_lock_for(remainingTime);
        }
        retval = this->pop();
    }
    return retval;
};
