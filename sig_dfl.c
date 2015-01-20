#include <signal.h>
#include "sig.h"
#include "hassgact.h"
#include "hassgprm.h"

/* tnx to Chris Torek for some improvements in previous versions */

static int flagcont;

static void sigcont() { flagcont = 1; }

void sig_dfl(sig)
int sig;
{
#ifdef HASSIGPROCMASK
  sigset_t ssorig;
  sigset_t ssnew;
#else
  int maskorig;
  int masknew;
#endif
#ifdef HASSIGACTION
  struct sigaction saorig;
  struct sigaction sanew;
#else
  struct sigvec svorig;
  struct sigvec svnew;
#endif

  if (sig == SIGCONT) return;

#ifdef HASSIGPROCMASK
  sigfillset(&ssnew);
  sigprocmask(SIG_SETMASK,&ssnew,&ssorig);
#else
  masknew = ~0;
  maskorig = sigsetmask(masknew);
#endif

#ifdef HASSIGACTION
  sanew.sa_handler = SIG_DFL;
  sanew.sa_flags = 0;
  sigemptyset(&sanew.sa_mask);
  sigaction(sig,&sanew,&saorig);
  sanew.sa_handler = sigcont;
  sigaction(SIGCONT,&sanew,(struct sigaction *) 0);
#else
  svnew.sv_handler = SIG_DFL;
  svnew.sv_flags = 0;
  svnew.sv_mask = 0;
  sigvec(sig,&svnew,&svorig);
  svnew.sv_handler = sigcont;
  sigvec(SIGCONT,&svnew,(struct sigvec *) 0);
#endif

  flagcont = 0;
  if (kill(getpid(),sig) == 0) {
#ifdef HASSIGPROCMASK
    sigdelset(&ssnew,sig);
    sigdelset(&ssnew,SIGCONT);
    while (!flagcont)
      sigsuspend(&ssnew);
#else
    masknew &= ~(1 << (sig - 1));
    masknew &= ~(1 << (SIGCONT - 1));
    while (!flagcont)
      sigpause(masknew);
#endif
  }

#ifdef HASSIGACTION
  sigaction(sig,&saorig,(struct sigaction *) 0);
#else
  sigvec(sig,&svorig,(struct sigvec *) 0);
#endif

#ifdef HASSIGPROCMASK
  sigprocmask(SIG_SETMASK,&ssorig,(sigset_t *) 0);
#else
  sigsetmask(maskorig);
#endif
}
