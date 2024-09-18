#include <queue>
#include <fstream>
#include <vector>

struct Task;
struct Queue;
struct Worker;


typedef std::vector<Queue>::iterator QueueIt;
typedef std::vector<Worker>::iterator WorkerIt;
typedef std::vector<Task>::iterator TaskIt;

struct Task
{
    uint64_t id;
    uint64_t whenToProcess;
    uint64_t queueId;
    uint64_t executionTime;
};

struct Queue
{
    uint64_t id;
    uint64_t lastTimeUsed;
    std::queue<TaskIt> tasks;
};

struct Worker
{
    uint64_t id;
    uint64_t deadline;
};


struct QueueItGreaterComparator
{
    bool operator() (QueueIt l, QueueIt r)
    {
        if(l->lastTimeUsed == r->lastTimeUsed) return l->id > r->id;

        return l->lastTimeUsed > r->lastTimeUsed;
    }
};

struct FreeWorkerItGreaterComparator
{
    bool operator() (WorkerIt l, WorkerIt r)
    {
        return l->id > r->id;
    }
};

struct BusyWorkerItGreaterComparator
{
    bool operator() (WorkerIt l, WorkerIt r)
    {
        return l->deadline > r->deadline;
    }
};

typedef std::priority_queue<QueueIt, std::vector<QueueIt>, QueueItGreaterComparator> PriorityQueue;
typedef std::priority_queue<WorkerIt, std::vector<WorkerIt>, FreeWorkerItGreaterComparator> FreeWorkerQueue;
typedef std::priority_queue<WorkerIt, std::vector<WorkerIt>, BusyWorkerItGreaterComparator> BusyWorkerQueue;

struct TaskSummary
{
    uint64_t workerId;
    uint64_t timeStarted;
};

typedef std::vector<TaskSummary> WorkResult;

class TaskManager
{
public:
    TaskManager(std::vector<Task> p_tasks, size_t queueCount, size_t workerCount);
    WorkResult DoWork();

private:
    std::vector<Queue> m_queues;
    std::vector<Worker> m_workers;
    std::vector<Task> m_tasks;

    PriorityQueue busyQueues;
    FreeWorkerQueue freeWorkers;
    BusyWorkerQueue busyWorkers;
};

TaskManager::TaskManager(std::vector<Task> p_tasks, size_t queueCount, size_t workerCount)
    : 
    m_queues(queueCount), 
    m_workers(workerCount), 
    m_tasks(std::move(p_tasks))   
{
    for(size_t i = 0; i < queueCount; ++i)
    {
        m_queues[i].id = i;
        m_queues[i].lastTimeUsed = 0;
    }

    WorkerIt workerIt = m_workers.begin();

    for(size_t i = 0; i < workerCount; ++i)
    {
        m_workers[i].id = i;
        m_workers[i].deadline = 0;

        freeWorkers.push(workerIt);
        workerIt = std::next(workerIt);
    }
}

WorkResult TaskManager::DoWork()
{
    WorkResult res(m_tasks.size());

    uint64_t currentSecond = 1;
    size_t nextTask = 0;

    while(!busyQueues.empty() || !busyWorkers.empty() || nextTask < m_tasks.size()) 
    {
        while(nextTask < m_tasks.size() && m_tasks[nextTask].whenToProcess == currentSecond) 
        {
            Task const& task = m_tasks[nextTask];
            uint64_t queueId = task.queueId;
            auto& queueTasks = m_queues[queueId].tasks;

            queueTasks.push(std::next(m_tasks.begin(), nextTask));

            if(queueTasks.size() == 1) 
            {
                busyQueues.push(std::next(m_queues.begin(), queueId));
            }

            ++nextTask;             
        }

        while(!busyWorkers.empty() && busyWorkers.top()->deadline == currentSecond) {

            WorkerIt workerIt = busyWorkers.top();
            workerIt->deadline = 0;

            freeWorkers.push(workerIt);
            busyWorkers.pop();
        }

        while(!freeWorkers.empty() && !busyQueues.empty())
        {
            WorkerIt workerIt = freeWorkers.top();
            QueueIt queueIt = busyQueues.top();
            TaskIt taskIt = queueIt->tasks.front();

            workerIt->deadline = currentSecond + taskIt->executionTime;
            busyWorkers.push(workerIt);
            queueIt->lastTimeUsed = currentSecond;

            res.at(taskIt->id).timeStarted = currentSecond;
            res.at(taskIt->id).workerId = workerIt->id + 1;

            queueIt->tasks.pop();
            busyQueues.pop();
            freeWorkers.pop();

            if(!queueIt->tasks.empty()) {
                busyQueues.push(queueIt);
            }

        }

        ++currentSecond;
    }

    return res;
}

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    size_t taskCount;
    size_t workerCount;
    size_t queueCount;

    input >> taskCount >> workerCount >> queueCount;

    std::vector<Task> tasks;
    tasks.reserve(taskCount);

    for(size_t i = 0; i < taskCount; ++i) 
    {
        uint64_t second, queueId, timeToExec;
        input >> second >> queueId >> timeToExec;
        tasks.push_back(Task 
        {
            .id = i,
            .whenToProcess = second,
            .queueId = queueId - 1,
            .executionTime = timeToExec
        });
    }

    TaskManager manager(std::move(tasks), queueCount, workerCount);
    WorkResult res = manager.DoWork();

    for(TaskSummary const& summary : res)
    {
        output << summary.workerId << " " << summary.timeStarted << std::endl;
    }

    return 0;
}