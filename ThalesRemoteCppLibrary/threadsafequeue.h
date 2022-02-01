/******************************************************************
 *  ____       __                        __    __   __      _ __
 * /_  / ___ _/ /  ___  ___ ___________ / /__ / /__/ /_____(_) /__
 *  / /_/ _ `/ _ \/ _ \/ -_) __/___/ -_) / -_)  '_/ __/ __/ /  '_/
 * /___/\_,_/_//_/_//_/\__/_/      \__/_/\__/_/\_\\__/_/ /_/_/\_\
 *
 * Copyright 2022 ZAHNER-elektrik I. Zahner-Schiller GmbH & Co. KG
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
#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <queue>
#include <mutex>
#include <vector>
#include <cstring>

/** Class which implements a thread-safe FIFO queue.
 *
 */
class ThreadsafeQueue
{
    std::queue< std::vector<uint8_t> > queue;
    mutable std::mutex mutex;
    std::timed_mutex dataAvailable;

public:
    ThreadsafeQueue();
    ThreadsafeQueue(const ThreadsafeQueue &) = delete ;
    ThreadsafeQueue& operator=(const ThreadsafeQueue &) = delete ;

    virtual ~ThreadsafeQueue();

    /** Check if the queue is empty.
     *
     * @return true if the queue is empty.
     */
    bool empty() const;

    /** Returns the number of elements in the queue.
     *
     *  The number of byte arrays in the queue.
     *
     * @return Number of elements in the queue.
     */
    unsigned long size() const;

    /** Adding an element to the queue.
     *
     * @param item The element to add.
     */
    void put(const std::vector<uint8_t> &item);

    /** Non-blocking read from the queue.
     *
     * If the queue is empty, a vector with length 0 is returned.
     *
     * @return An element of the queue.
     */
    std::vector<uint8_t> pop();

    /** Blocking and non-blocking read from the queue.
     *
     * If blocking is false the pop method is executed.
     * If blocking is true it will wait for the timeout time to be read if there are no elements in the queue.
     * After a timeout a vector with length 0 is returned.
     *
     * @param blocking true to wait for timeout time.
     * @param timeout Time to wairt for data.
     * @return An element of the queue.
     */
    std::vector<uint8_t> get(const bool blocking = true, const std::chrono::duration<int, std::milli> timeout = std::chrono::duration<int, std::milli>::max());
};

#endif // THREADSAFEQUEUE_H
