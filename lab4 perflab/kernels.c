/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include <pthread.h>
#include <semaphore.h>
#include <algorithm>
#include <string.h>
#include "helper.h"

pthread_t threads[32];


/* 
 * Please fill in the following team struct 
 */
user_t user = {
    (char*)  "104850256",            /* UID */
    (char*)  "Yutong Sun",          /* Full name */
    (char*)  "ysun52dd@ucla.edu",  /* Email */
};

//If you worked with anyone in this project, please indicate that here:
// I worked with  "... , ... ".

// Of course, your entire file should be hand-written by you.  You are free to
// look at tutorials and academic literature for radix-sort based sorting.  You
// are not allowed to look at or copy from implementations on online code repositories
// like github.


//  You should modify only this file, but feel free to modify however you like!


/*
 * setup - This function runs one time, and will not be timed.
 *         You can do whatever initialization you need here, but
 *         it is not required -- don't use if you don't want to.  ^_^
 */
void setup() {
  //..

  // So, in my experiments, it take fewer cycles for each run if we 
  // waste some time doing absolute nothing in particular
  // at the begining of the program.  It might be because of Intel's Turbo
  // mode and DVFS somehow??  TBH IDK, but I would leave this in if I were
  // you.
  for(int i = 0; i < 50000;++i) {
     do_nothing(i);
  }
}

/******************************************************
 * Your different versions of the singlethread kernel go here
 ******************************************************/
bool kvp_compare(kvp lhs, kvp rhs) { 
  return lhs.key < rhs.key; 
}

//inline void move_kvp(kvp* dst, kvp* src, int src_i, int dst_i) {
//    dst[dst_i].key = src[src_i].key;
//    dst[dst_i].value = src[src_i].value;
//}

inline int gen_shift(int val, int shift, int mask){
    return ((val >> shift) & mask);
}
/*
 * naive_singlethread - The naive baseline version of singlethread 
 */
char naive_singlethread_descr[] = "naive_singlethread: Naive baseline implementation";
void naive_singlethread(int dim, kvp *src, kvp *dst) 
{
    //This is the built-in stable sort if you want to try it
    //memcpy(dst, src, dim*sizeof(kvp));
    //std::stable_sort(dst, dst+dim, kvp_compare);
    //return;

    int log_radix=8; //Radix of radix-sort is 2^8
    int iters=(sizeof(unsigned int)*8/log_radix);

    // 256 buckets for 2^8 bins, one for each iteration 
    unsigned long long buckets[256+1][iters];
    unsigned long long sum[256+1][iters];

    for(int iter = 0; iter < iters; ++iter) {
      for(int i = 0; i < bucket_size(log_radix); ++i) {
        buckets[i][iter]=0;
        sum[i][iter]=0;
      }

      //1. Generate the bucket count
      for(int i = 0; i < dim; ++i) {
        int index = gen_shift(src[i].key,iter*log_radix,
                              (bucket_size(log_radix)-1))+1;
        buckets[index][iter]++;
      }

      //2. Perform scan
      for(int i = 1; i < bucket_size(log_radix); ++i) {
        sum[i][iter] = buckets[i][iter] + sum[i-1][iter];
      }

      //3. Move Data items
      for(int i = 0; i < dim; ++i) {
        int index = gen_shift(src[i].key,iter*log_radix,
                              bucket_size(log_radix)-1);
        int out_index = sum[index][iter];
        move_kvp(dst,src,i,out_index);
        sum[index][iter]++;
      }

       //Move dest back to source
      for(int i = 0; i < dim; ++i) {
        move_kvp(src,dst,i,i);
      }

    }
}


/*
 * singlethread - Your current working version of singlethread. 
 * IMPORTANT: This is the version you will be graded on
 */


char singlethread_descr[] = "singlethread: Current working version";
void singlethread(int dim, kvp *src, kvp *dst)
{
    
    int log_radix=8; //Radix of radix-sort is 2^8
    int iters=(4*8/log_radix);
    int b_size=bucket_size(log_radix);
    int shift;
    unsigned long long* b;

    int index0[4];
    int index1[4];
    int index2[4];
    int index3[4];
    
    
    // 256 buckets for 2^8 bins, one for each iteration
    unsigned long long buckets0[257]={0};
    unsigned long long buckets1[257]={0};
    unsigned long long buckets2[257]={0};
    unsigned long long buckets3[257]={0};
   


    for(int j = 0; j < dim; j+=16) {
        for(int i=j;i<j+16;i+=4){
            //1. Generate the bucket count iter=0, shift=0
            index0[0] = gen_shift(src[i].key,0,255)+1;//&11111111
            index0[1]=gen_shift(src[i+1].key,0,255)+1;
            index0[2]=gen_shift(src[i+2].key,0,255)+1;
            index0[3]=gen_shift(src[i+3].key,0,255)+1;
            
            (*(buckets0+index0[0]))++;
            (*(buckets0+index0[1]))++;
            (*(buckets0+index0[2]))++;
            (*(buckets0+index0[3]))++;
            
            index1[0] = gen_shift(src[i].key,8,255)+1;//&11111111
            index1[1]=gen_shift(src[i+1].key,8,255)+1;
            index1[2]=gen_shift(src[i+2].key,8,255)+1;
            index1[3]=gen_shift(src[i+3].key,8,255)+1;
            
            (*(buckets1+index1[0]))++;
            (*(buckets1+index1[1]))++;
            (*(buckets1+index1[2]))++;
            (*(buckets1+index1[3]))++;
            //1. Generate the bucket count iter=2, shift=16
            index2[0] = gen_shift(src[i].key,16,255)+1;//&11111111
            index2[1]=gen_shift(src[i+1].key,16,255)+1;
            index2[2]=gen_shift(src[i+2].key,16,255)+1;
            index2[3]=gen_shift(src[i+3].key,16,255)+1;
            
            (*(buckets2+index2[0]))++;
            (*(buckets2+index2[1]))++;
            (*(buckets2+index2[2]))++;
            (*(buckets2+index2[3]))++;
            
            //1. Generate the bucket count iter=3, shift=24
            index3[0] = gen_shift(src[i].key,24,255)+1;//&11111111
            index3[1]=gen_shift(src[i+1].key,24,255)+1;
            index3[2]=gen_shift(src[i+2].key,24,255)+1;
            index3[3]=gen_shift(src[i+3].key,24,255)+1;
            
            (*(buckets3+index3[0]))++;
            (*(buckets3+index3[1]))++;
            (*(buckets3+index3[2]))++;
            (*(buckets3+index3[3]))++;


 
        }
    }


    

   //2. Perform scan iter=1
    for(int i = 1; i < 256; i+=4) {
      //2. scan iter=0
    buckets0[i] = buckets0[i] + buckets0[i-1];
    buckets0[i+1] = buckets0[i+1] + buckets0[i];
    buckets0[i+2] = buckets0[i+2] + buckets0[i+1];
    buckets0[i+3] = buckets0[i+3] + buckets0[i+2];
    
    //iter=1
    buckets1[i] = buckets1[i] + buckets1[i-1];
    buckets1[i+1] = buckets1[i+1] + buckets1[i];
    buckets1[i+2] = buckets1[i+2] + buckets1[i+1];
    buckets1[i+3] = buckets1[i+3] + buckets1[i+2];
    //iter=2
    buckets2[i] = buckets2[i] + buckets2[i-1];
    buckets2[i+1] = buckets2[i+1] + buckets2[i];
    buckets2[i+2] = buckets2[i+2] + buckets2[i+1];
    buckets2[i+3] = buckets2[i+3] + buckets2[i+2];
    //iter=3
    buckets3[i] = buckets3[i] + buckets3[i-1];
    buckets3[i+1] = buckets3[i+1] + buckets3[i];
    buckets3[i+2] = buckets3[i+2] + buckets3[i+1];
    buckets3[i+3] = buckets3[i+3] + buckets3[i+2];
    }
    
    for(int iter = 0; iter < iters; ++iter) {
        shift=iter*8;

        switch (iter) {
            case 0:
                b=buckets0;
                break;
            case 1:
                b=buckets1;
                break;
            case 2:
                b=buckets2;
                break;
            case 3:
                b=buckets3;
                break;
                
        }
        //3. Move Data items
        for(int i = 0; i < dim; i++) {

     
            int index = gen_shift(src[i].key,shift,
                                  255);//&11111111
         
                    *(dst+b[index])=*(src+i);
            b[index]++;
            
          
        }
        
        // Move dest back to source
        
      
        if(iter!=3)
        {

            memcpy(src,dst,dim*8);
        }
    }
  
}




/********************************************************************* 
 * register_singlethread_functions - Register all of your different versions
 *     of the singlethread kernel with the driver by calling the
 *     add_singlethread_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_singlethread_functions() {
    add_singlethread_function(&naive_singlethread, naive_singlethread_descr);
    add_singlethread_function(&singlethread, singlethread_descr);
    //add_singlethread_function(&singlethread2, singlethread_descr2);
    /* ... Register additional test functions here */
}



// ----------------------- do multi-threaded versions here ------------------


// I'm kind of cheating to pass global variables to my thread function
// There are nicer ways to do this, but w/e
pthread_barrier_t mybarrier;
int gdim;
kvp* gsrc;
kvp* gdst;
unsigned long long buckets[5][257];//iter=0 to store part1 part2 part3 part4
//unsigned long long buckets2[9][257];//iter=0 to store part1 part2 part3 part4



void *do_sort2(void* threadid) {
    long tid = (long)threadid;
    //printf("Hello from thread %d\n", tid);
    int dim = gdim;
    kvp* src = gsrc;
    kvp* dst = gdst;
    
    singlethread(dim,src,dst);
    return 0;
}

void *do_sort(void* threadid)
{
    long tid = (long)threadid;
    unsigned long long* b;
    int offset;
    unsigned long long offset_b;
    int index;
    int index0[4];

    int dim = gdim;
    kvp* src = gsrc;
    kvp* dst = gdst;

    int shift;
    int iter;
    for(iter = 0; iter < 4; ++iter) {
        shift=iter*8;
  switch (tid) {
      case 0:
          b=*(buckets);//thread1 iter0
          offset=0;
            break;
        case 1:
          b=*(buckets+1);//thread2 iter0
          offset=64;
            break;
        case 2:
          b=*(buckets+2);

          offset=128;
            break;
        case 3:
          b=*(buckets+3);

          offset=192;
            break;

    }
 
        //pthread_barrier_wait(&mybarrier);
        int part_start=(dim/4)*tid;
        int part_end=part_start+(dim/4);
   for(int j = part_start; j < part_end; j+=16) {
        for(int i=j;i<j+16;i+=4){
            
            //1. Generate the bucket count iter=0, shift=0


            buckets[tid][gen_shift(src[i].key,shift,255)+1]++;
            buckets[tid][gen_shift(src[i+1].key,shift,255)+1]++;
            buckets[tid][gen_shift(src[i+2].key,shift,255)+1]++;
            buckets[tid][gen_shift(src[i+3].key,shift,255)+1]++;

        }
   }
 
          pthread_barrier_wait(&mybarrier);

    int start=1+offset;
    int end=64+offset;
       
            for(int i = start; i < end; i+=4) {

                
                buckets[1][i]+=buckets[0][i];
                buckets[2][i]+=buckets[1][i];
                buckets[3][i]+=buckets[2][i];
                buckets[4][i]+=buckets[3][i];
                
                buckets[1][i+1]+=buckets[0][i+1];
                buckets[2][i+1]+=buckets[1][i+1];
                buckets[3][i+1]+=buckets[2][i+1];
                buckets[4][i+1]+=buckets[3][i+1];
                
                buckets[1][i+2]+=buckets[0][i+2];
                buckets[2][i+2]+=buckets[1][i+2];
                buckets[3][i+2]+=buckets[2][i+2];
                buckets[4][i+2]+=buckets[3][i+2];
                
                buckets[1][i+3]+=buckets[0][i+3];
                buckets[2][i+3]+=buckets[1][i+3];
                buckets[3][i+3]+=buckets[2][i+3];
                buckets[4][i+3]+=buckets[3][i+3];
 
            }
    
    
   

    pthread_barrier_wait(&mybarrier);
   
    //do prefix sum for the last row
    if(tid==0){
       // printf("\nstart last row");
for(int i = 1; i < 256; i+=4) {
  
    buckets[4][i]=buckets[4][i]+buckets[4][i-1];
    buckets[4][i+1]=buckets[4][i+1]+buckets[4][i];
    buckets[4][i+2]=buckets[4][i+2]+buckets[4][i+1];
    buckets[4][i+3]=buckets[4][i+3]+buckets[4][i+2];
}
    }
  
pthread_barrier_wait(&mybarrier);

        
        //3. Move Data items

         for(int i = part_end; i < part_start; i--) {
            index = gen_shift(src[i].key,shift,
                                  255)+1;//&11111111
            
             if(index==0)
                 offset_b=0;
             else
             offset_b=buckets[4][index-1];
           
             buckets[tid][index]--;
            (*(dst+offset_b+buckets[tid][index]))=(*(src+i));
             
         }
    
       

        
        pthread_barrier_wait(&mybarrier);
        if(tid==0 && iter!=3)
        {
        
  
            memcpy(src,dst,dim*8);
            memset(buckets,0,sizeof(buckets));
            
        }
       
        pthread_barrier_wait(&mybarrier);

    }
   
  
   return 0;
}


/*
 * naive_multithread - The naive baseline version of multithread 
 */
char naive_multithread_descr[] = "naive_multithread: Naive baseline implementation";
void naive_multithread(int dim, kvp *src, kvp *dst) 
{
    gdim=dim;
    gsrc=src;
    gdst=dst;
    //pthread_t threads[4];
    
    //call one thread to do our work -- I'm sure that will make things go faster
    
    pthread_create(&threads[0], NULL, do_sort2, (void *)0 /*tid*/);
    
    void** ret=0;
    pthread_join(threads[0],ret);
}


/* 
 * multithread - Your current working version of multithread
 * IMPORTANT: This is the version you will be graded on
 */
char multithread_descr[] = "multithread: Current working version";
void multithread(int dim, kvp *src, kvp *dst) 
{
    
    int i;
    gdim=dim;
    gsrc=src;
    gdst=dst;
    memset(buckets,0,sizeof(buckets));
   

    if(dim<=16384)
        singlethread(dim,src,dst);
    else
    {
    pthread_barrier_init(&mybarrier, NULL, 4);


    pthread_t threads[4];
    
    //call one thread to do our work -- I'm sure that will make things go faster
    
    for(i=0; i<4; i++)
    {
    pthread_create(&threads[i], NULL, do_sort, (void *)i /*tid*/);
        
    }
    
   // #  pthread_barrier_wait(&mybarrier);
    
    void** ret=0;
    
    //pthread_mutex_init(&list_mutex, NULL);
    for(i=0; i<4;i++)
    pthread_join(threads[i],ret);
   
    
    pthread_barrier_destroy(&mybarrier);
    }
    
}

/*********************************************************************
 * register_multithread_functions - Register all of your different versions
 *     of the multithread kernel with the driver by calling the
 *     add_multithread_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_multithread_functions() 
{
    add_multithread_function(&naive_multithread, naive_multithread_descr);   
    add_multithread_function(&multithread, multithread_descr);   
    /* ... Register additional test functions here */
}

