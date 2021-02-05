/* 
 * file name: rw.h
 * Author: ckevar
 * Created: Jan 22, 2020 at 3:35PM
 */

#ifndef RW_H
#define RW_H 

#include <semaphore.h>

struct BufferSem {
	sem_t m;
	sem_t priv_r[2];
	sem_t priv_w;

	unsigned nr;
	unsigned nw;
	unsigned blockedR;
	unsigned blockedW;
};

void bufferSem_init(struct  BufferSem *m_bSem);

/*
 * INPUT BUFFER SEMAPHORES MANAGEMENT
 */
void inBuffer_post_read(struct BufferSem *bs);
void inBuffer_post_write(struct BufferSem *bs);

/*
 * OUTPUT BUFFER SEMAPHORES MANAGEMENT
 */
void outBuffer_post_read(struct BufferSem *bs);
void outBuffer_post_write(struct BufferSem *bs);

#endif