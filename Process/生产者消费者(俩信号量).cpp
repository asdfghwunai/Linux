/*
int full=0;
int empty=5;    //空槽个数
int lock=1;   //锁

生产者和消费者都是互斥访问的，消费者也不能一起消费
是忙等还是阻塞取决于signal和wait实现

生产者:
while(1)
{
    wait(empty);  //取得空槽
    wait(lock);    //上锁;
    
    //放东西
    
    signal(lock);
    signal(full);
    
    
}

消费者:
while(1)
{
    wait(full);   //等待有东西
    wait(lock);
    
    //拿东西
    
    signal(lock);
    signal(empty);
    
}




*/
