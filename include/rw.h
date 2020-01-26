/* 
 * file name: rw.h
 * Created Jan 22, 2020 @ 3:35PM
 * Author: ckevar
 */

#ifndef RW_H
#define RW_H 

#include <semaphore.h>

struct BufferSem {
	sem_t m;
	sem_t priv_r;
	sem_t priv_w;

	unsigned nr;
	unsigned nw;
	unsigned blockedR;
	unsigned blockedW;
};

void bufferSem_init(struct  BufferSem *m_bSem);


/*
 * GENERAL WRITE/READ BUFFER SEMAPHORES MANAGEMENT
*/
void buffer_wait_read(struct BufferSem *bs);
void buffer_post_read(struct BufferSem *bs);
void buffer_wait_write(struct BufferSem *bs);
void buffer_post_write(struct BufferSem *bs);

/*
 * INPUT BUFFER SEMAPHORES MANAGEMENT
 */
void inBuffer_wait_read(struct BufferSem *bs);
void inBuffer_post_read(struct BufferSem *bs);
void inBuffer_wait_write(struct BufferSem *bs);
void inBuffer_post_write(struct BufferSem *bs);

/*
 * OUTPUT BUFFER SEMAPHORES MANAGEMENT
 */
void outBuffer_wait_write(struct BufferSem *bs);
void outBuffer_post_write(struct BufferSem *bs);
void outBuffer_wait_read(struct BufferSem *bs);
void outBuffer_post_read(struct BufferSem *bs);

#endif