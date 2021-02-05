/*
 * file name: rw.c
 * Author: ckevar 
 * created Jan 22, 2019 at 4:20 PM
 */ 
#include "rw.h"

void bufferSem_init(struct  BufferSem *m_bSem) {
	m_bSem->nr = 0;
	m_bSem->nw = 0;
	m_bSem->blockedR = 0;
	m_bSem->blockedW = 0;

	sem_init(&m_bSem->m, 0, 1);
	sem_init(&m_bSem->priv_r[0], 0, 0); // Left Channel
	sem_init(&m_bSem->priv_r[1], 0, 0);	// Right Channel
	sem_init(&m_bSem->priv_w, 0, 0);
}

/*
 * INPUT BUFFER SEMAPHORES MANAGEMENT
 */

void inBuffer_post_read(struct BufferSem *bs) {
	sem_wait(&bs->m);

	bs->nr--;					// one reader has left the CS
    
	if(bs->nr == 0) 			// If this is the last reader 
								// leaving the CS
		sem_post(&bs->priv_w);	// then unlock the writer

	sem_post(&bs->m);
}

void inBuffer_post_write(struct BufferSem *bs){
	sem_wait(&bs->m);

	for (int i = 0; i < 2; i ++) {
		sem_post(&bs->priv_r[i]);
		bs->nr++;
	}

	sem_post(&bs->m);
}

/*
 * OUTPUT BUFFER SEMAPHORES MANAGEMENT
 */

void outBuffer_post_write(struct BufferSem *bs) {
	sem_wait(&bs->m);
	
	bs->nw--;
	if(bs->nw == 0)
		sem_post(bs->priv_r); //only 1 out of 2 semaphores is being used
	
	sem_post(&bs->m);
}

void outBuffer_post_read(struct BufferSem *bs) {
	sem_wait(&bs->m);

	for(int i = 0; i < 2; i++) {
		sem_post(&bs->priv_w);
		bs->nw++;
	}

	sem_post(&bs->m);
}
