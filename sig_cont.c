#include <signal.h>
#include "sig.h"

void sig_contblock() { sig_block(SIGCONT); }
void sig_contunblock() { sig_unblock(SIGCONT); }
void sig_contcatch(f) void (*f)(); { sig_catch(SIGCONT,f); }
void sig_contdefault() { sig_catch(SIGCONT,SIG_DFL); }
