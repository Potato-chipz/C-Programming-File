#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

#define NUM_THREADS 2

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t;
} thread_arg_t; //when declare, knows the momroy allocation already 

/* the main function of threads */
static void * thread_main(void * p_arg)
{
    // TODO
    //create a thread data stucture called data_agent 
    thread_arg_t * data_agent = p_arg; //contains allthe information includes m,n,t,id

    //two threads, each take half of the task to sum up matrix 
    for (int a = data_agent->id; a < data_agent->m->nrows; a += NUM_THREADS) //skip rows
    {
        for(int b = 0; b < data_agent->m->ncols; b++) //skip coloumns
        {
            //sum the two matrix together
            data_agent->t->data[a][b] = data_agent->m->data[a][b] + data_agent->n->data[a][b];
        }
    }

    //exit the thread when done, just like fork 
    pthread_exit(0);

    return NULL;

}

/* Return the sum of two matrices. The result is in a newly created matrix. 
 *
 * If a pthread function fails, report error and exit. 
 * Return NULL if something else is wrong.
 *
 * Similar to addMatrix, but this function uses 2 threads.
 */
TMatrix * addMatrix_thread(TMatrix *m, TMatrix *n)
{
    if ( m == NULL || n == NULL
         || m->nrows != n->nrows || m->ncols != n->ncols )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, m->ncols);
    if (t == NULL)
        return t;

    // TODO
    //need to create thread assigning id 
    pthread_t my_threads[NUM_THREADS];

    thread_arg_t thread_info[NUM_THREADS];
    
    int x, rc; 

    for(x = 0; x < NUM_THREADS; x++)
    {
        thread_info[x].id = x;
        thread_info[x].m = m; 
        thread_info[x].n = n; 
        thread_info[x].t = t; 

        rc = pthread_create(&my_threads[x], NULL, thread_main, &thread_info[x]);

        if(rc) //a return of 0 indicates success, otherwise error 
        {
            printf("Warning, resut from pthread_create() is %d\n",rc);
            
            //exit the thread program 
            exit(-1); 
        }
    }

    //conquer 
    for (x = 0; x < NUM_THREADS; x++)
    {
        rc = pthread_join(my_threads[x],NULL);

        if(rc)
        {
            printf("Warning, resut from pthread_join() is %d\n",rc);
            
            //exit the thread program 
            exit(-1); 
        } 
    }

    return t; //return the time

    //
   
    
}