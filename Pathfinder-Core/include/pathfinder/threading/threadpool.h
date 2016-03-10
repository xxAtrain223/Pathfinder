#ifndef PATHFINDER_THREADPOOL_H_DEF
#define PATHFINDER_THREADPOOL_H_DEF

// Threadpool code largely copied / inspired by https://github.com/Pithikos/C-Thread-Pool

typedef struct thpool* threadpool;

threadpool thpool_init(int num_threads);
int thpool_add_work(threadpool, void *(*function_p)(void*), void* arg_p);
void thpool_wait(threadpool);
void thpool_pause(threadpool);
void thpool_resume(threadpool);
void thpool_destroy(threadpool);

#endif