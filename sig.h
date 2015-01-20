#ifndef SIG_H
#define SIG_H

extern void sig_catch();
extern void sig_block();
extern void sig_unblock();

extern void sig_dfl();

extern void sig_pipeignore();
extern void sig_pipedefault();

extern void sig_contblock();
extern void sig_contunblock();
extern void sig_contcatch();
extern void sig_contdefault();

#endif
