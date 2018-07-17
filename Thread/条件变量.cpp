//---------------
条件变量需要配合互斥量一起使用，互斥量作为参数传入wait函数，函数把调用线程放到等待条件的线程列表上，然后对互斥量解锁，这两个是原子操作。
当线程等待到条件，从wait函数返回之前，会再次锁住互斥量。

1.Lock

2.Unlock
3.等待
4.Lock

5.Unlock

第2,3,4步是wait的内部操作

在wait被唤醒后，还需要在while中去检查条件，这是为了防止“惊群效应”，比如有两个线程同时阻塞在wait，先后醒来，快的线程做完处理然后把条件reset了，
并且对互斥量解锁，此时慢的线程在wait里获得了锁（即第4步）返回，还再去做处理就会出问题。

//------------
pthread_cond_signal()的一个简单的例子。
pthread_mutex_t count_lock;
pthread_cond_t count_nonzero;
unsigned count;
decrement_count () {
    pthread_mutex_lock (&count_lock);
    while(count==0) 
        pthread_cond_wait( &count_nonzero, &count_lock);
    count=count -1;
    pthread_mutex_unlock (&count_lock);
}

increment_count(){
    pthread_mutex_lock(&count_lock);
    if(count==0)
        pthread_cond_signal(&count_nonzero);
    count=count+1;
    pthread_mutex_unlock(&count_lock);
}
