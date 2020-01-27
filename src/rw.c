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
	sem_init(&m_bSem->priv_r, 0, 0);
	sem_init(&m_bSem->priv_w, 0, 0);

}

void buffer_wait_read(struct BufferSem *bs) {
	sem_wait(&bs->m);

	if(bs->nw == 1 || bs->blockedW > 0) // check if buffer it's writing or not, or if there are writers waiting
		bs->blockedR++;		// broadcasting: guts, im blocking my ass here
	else {
		bs->nr++;
		sem_post(&bs->priv_r);	// acquiring reading sem		
	}

	sem_post(&bs->m);
	sem_wait(&bs->priv_r);
}	

void buffer_post_read(struct BufferSem *bs) {
	sem_wait(&bs->m);

	bs->nr--;				// I'm done with this shit
	if(bs->nr == 0 && bs->blockedW) { // if we all are done and there are writers in waiting
		bs->blockedW--;
		bs->nw = 1;
		sem_post(&bs->priv_w);	// then service them
	} 

	sem_post(&bs->m);	// otherwise, just relaise synch semaphore
}

void buffer_wait_write(struct BufferSem *bs) {
	sem_wait(&bs->m);

	if(bs->nr || bs->nw) 	// if there's at least one reader and writers
		bs->blockedW++;		// im blocking here
	else {
		bs->nw = 1;
		sem_post(&bs->priv_w);	// releasing that semaphore
	}

	sem_post(&bs->m);
	sem_wait(&bs->priv_w);
}

void buffer_post_write(struct BufferSem *bs){
	sem_wait(&bs->m);

	bs->nw = 0;
	while(bs->blockedR) {
		sem_post(&bs->priv_r);
		bs->blockedR--;
		bs->nr++;
	}
	if(bs->nr == 0 && bs->blockedW) {
		sem_post(&bs->priv_w);
		bs->blockedW--;
		bs->nw++;
	} 
	sem_post(&bs->m);
}

/*
 * INPUT BUFFER SEMAPHORES MANAGEMENT
 */

void inBuffer_wait_read(struct BufferSem *bs) {
	sem_wait(&bs->m);
	if (bs->blockedW)
		sem_post(&bs->priv_r);
	sem_post(&bs->m);
	sem_wait(&bs->priv_r);
}	

void inBuffer_post_read(struct BufferSem *bs) {
	sem_wait(&bs->m);

	bs->nr--;					// one reader has left the CS

	if(bs->nr == 0) 			// If this is the last reader 
								// leaving the CS
		sem_post(&bs->priv_w);	// then open the door for writer

	sem_post(&bs->m);
}

// void inBuffer_wait_write(struct BufferSem *bs) {
// 	sem_wait(&bs->priv_w);		// tries to enter to the 
// 								// critical section
// }

void inBuffer_post_write(struct BufferSem *bs){
	sem_wait(&bs->m);

	for (int i = 0; i < 2; i ++) {
		sem_post(&bs->priv_r);
		bs->nr++;
	}

	sem_post(&bs->m);
}

/*
 * OUTPUT BUFFER SEMAPHORES MANAGEMENT
 */

void outBuffer_wait_write(struct BufferSem *bs) {
	sem_wait(&bs->priv_w); // this has to start at 2
}

void outBuffer_post_write(struct BufferSem *bs) {
	sem_wait(&bs->m);
	
	bs->nw--;
	if(bs->nw == 0)
		sem_post(&bs->priv_r);
	
	sem_post(&bs->m);
}

void outBuffer_wait_read(struct BufferSem *bs) {
	sem_wait(&bs->priv_r);
}

void outBuffer_post_read(struct BufferSem *bs) {
	sem_wait(&bs->m);

	for(int i = 0; i < 2; i++) {
		sem_post(&bs->priv_w);
		bs->nw++;
	}

	sem_post(&bs->m);
}
