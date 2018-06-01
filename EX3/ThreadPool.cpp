//
// Created by shawn on 1/06/2018.
//

#include ThreadPool.h

ThreadPool::ThreadPool (int numOftThreads) : shutdown_ (false)
{
    vectorOfThreads.reserve (numOftThreads); //frees enough space in memory for numOfThread elements
    for (int i = 0; i < numOftThreads; ++i)
        vectorOfThreads.emplace_back (std::bind (&ThreadPool::threadEntry, this, i));//like "push_back" but different
        //function "bind" binds the function "threadEntry" to the input variables that we send, "i"
}

~ThreadPool ()
{
    {
        // Unblock any threads and tell them to stop
        std::unique_lock <std::mutex> l (lock_);
        shutdown_ = true;
        condVar_.notify_all();
    }
    for (auto& thread : vectorOfThreads)
        thread.join();
}

void doJob (std::function <void (void)> func)
{
    // Place a job on the queue and unblock a thread
    std::unique_lock <std::mutex> l (lock_);
    jobs_.emplace (std::move (func));
    condVar_.notify_one();
}

void threadEntry (int i)
{
    std::function <void (void)> job;
    while (1)
    {
        std::unique_lock <std::mutex> l (lock_);
        while (! shutdown_ && jobs_.empty())
            condVar_.wait (l);

        if (jobs_.empty ())
        {
            // No jobs to do and we are shutting down
            std::cerr << "Thread " << i << " terminates" << std::endl;//todo delete this
            return;
        }

        std::cerr << "Thread " << i << " does a job" << std::endl;//todo delete this
        job = std::move (jobs_.front ());
        jobs_.pop();

        // Do the job without holding any locks
        job ();
    }

}
