/*
什么是线程的分离

对比进程，子进程在销毁的时候使用ｗａｉｔ和ｗａｉｔｐｉｄ函数，都是去等待子进程资源的回收，同时也有ｓｉｎｇａｌ去捕捉SIG_CHLD信号来进行调用
ｗａｉｔ函数去回收子进程，那么在线程中会不会也有这样的一个函数呢？或者类似的策略，让线程执行完毕之后，能够自动的监听检测．然后去释放其资源???
答案是：我目前还没有遇到这一快相关的知识积累．但是在线程中却提供了这样一个分离策略，就是在线程执行完毕之后，能够自动回收线程所占有的资源
定义：

线程分离策略就是当线程执行完毕之后，能够由系统自动去回收线程所占有的资源的这样的一个策略．

线程分离函数
#include<pthread.h>
int pthread_attr_getdetachstat(const pthread_attr_t *restrict attr,int *detachstate);
int pthread_attr_setdetachstat(const pthread_attr_t *attr,int detachstate);
返回:成功返回0,出错返回错误编号
detachstat取值：
PTHREAD_CREATE_JOINABLE(默认值)正常启动线程
PTHREAD_CREATE_DETACHED以分离状态启动线程

注意:
    以默认方式启动的线程，在线程结束后不会自动释放占有的系统资源，要在主控线程中调用pthread_join()后才会释放
    以分离状态启动的线程，在线程结束后会自动释放所占有的系统资源,这个时候就不需要调用pthread_join方法了
    分离属性在网络通讯中使用的比较多；
    以分离状态创建的线程就不需要去调用pthread_join了，同时以分离状态去创建的线程，是不能够获取线程返回的结果

*/

/*
 * ===========================================================================
 *
 *       Filename:  pthread_detach.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2017年03月27日 22时02分37秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:   (), 
 *        Company:  
 *
 * ===========================================================================
 */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void * th_function(void* argv){
  int *agv = (int *) argv;
  printf("agv:%d\n",agv);
  int i=0;
  for(i =0; i<agv; i++){
     printf("thread:%lx,i:%d\n",pthread_self(),i);
     sleep(1);
  }
  return (void*)0;
}

void outDeched(pthread_attr_t* attr){
  int result_code;
  int detach;
  if((result_code = pthread_attr_getdetachstate(attr,&detach))!=0){
      perror("get error");
  }else{
    if(detach == PTHREAD_CREATE_JOINABLE){
      printf("PTHREAD_CREATE_JOINABLE\n");
    }else if(detach ==PTHREAD_CREATE_DETACHED ){
      printf("PTHREAD_CREATE_DETACHED\n");
    }else{
      printf("other detach");
  }
  }
}


int main(int argc,char * argv[]){
  pthread_attr_t attr;
  pthread_t turtle ,rabbit;

  int err;
  int entach;
  if((err = pthread_attr_init(&attr)) != 0){
      perror("create attr error");
  }

  int result_code;
  //获得响应的分离策略，一般的是默认策略，即不分离状态，需要使用pthread_join去阻塞回收
  if((result_code = pthread_attr_getdetachstate(&attr,&entach))!=0){
      perror("get error");
  }else{
  } 
  outDeched(&attr);

  int create_result;
  if((create_result = pthread_create(&turtle,&attr,th_function,(void*)50))!=0){
      perror("creat turtle error");
  }
  printf("==========================");
  //给rabbit去设置detached的属性
  if((result_code = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED))!=0){
      perror("set error");
  }
  outDeched(&attr);

  if((create_result = pthread_create(&rabbit,&attr,th_function,(void*)40))!=0){
      perror("create rabbit error");  
  }

  //销毁这样的一个pthread_attr_t结构体
  if((err = pthread_attr_destroy(&attr))!=0){
      perror("destory pthread error");
  }

  //因为turtle是默认的join方式进行创建的，所以其需要调用pthread_join去释放
  pthread_join(turtle,NULL);
  printf("main thread:%lx ended\n",pthread_self());
  return 0;
}

/*
如上所陈述：turtle采用的是默认的ＪＯＩＮ方式去启动，而ｒａｂｂｉｔ采用的是ＤＥＴＡＣＨＥＤ的方式去启动，注意：
　detached的方式去启动的时候，是不需要通过pthread_join方法去阻塞等待回收的，并且detached的方式启动的，这个时候线程是不会有返回值返回来的
*/
