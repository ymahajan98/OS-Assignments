#include<context.h>
#include<memory.h>
#include<schedule.h>
static u64 numticks;

static void save_current_context()
{
  /*Your code goes in here*/ 
} 

static void schedule_context(struct exec_context *next)
{
  /*Your code goes in here. get_current_ctx() still returns the old context*/
 struct exec_context *current = get_current_ctx();
 printf("schedluing: old pid = %d  new pid  = %d\n", current->pid, next->pid); /*XXX: Don't remove*/
/*These two lines must be executed*/
 set_tss_stack_ptr(next);
 set_current_ctx(next);
/*Your code for scheduling context*/
 return;
}

static struct exec_context *pick_next_context(struct exec_context *list)
{
  /*Your code goes in here*/
  
   return NULL;
}
static void schedule()
{
/* 
 struct exec_context *next;
 struct exec_context *current = get_current_ctx(); 
 struct exec_context *list = get_ctx_list();
 next = pick_next_context(list);
 schedule_context(next);
*/     
}

static void do_sleep_and_alarm_account()
{
 /*All processes in sleep() must decrement their sleep count*/ 
}

/*The five functions above are just a template. You may change the signatures as you wish*/
void handle_timer_tick()
{
 /*
   This is the timer interrupt handler. 
   You should account timer ticks for alarm and sleep
   and invoke schedule
 */
  asm volatile("cli;"
                :::"memory");
  asm volatile("push %rax;""push %rbx;""push %rcx;""push %rdx;""push %r8;""push %r9;""push %r10;""push %r11;""push %r12;""push %r13;""push %r14;""push %r15;""push %rsi;""push %rdi;");
  printf("Got a tick. #ticks = %u\n", numticks++);   /*XXX Do not modify this line*/ 
  struct exec_context *current=get_current_ctx();
  current->ticks_to_alarm=current->ticks_to_alarm-1;
  if(current->ticks_to_alarm==0){
    current->ticks_to_alarm=current->alarm_config_time;
  }
  ack_irq();  /*acknowledge the interrupt, before calling iretq */
  asm volatile("pop %rdi;""pop %rsi;""pop %r15;""pop %r14;""pop %r13;""pop %r12;""pop %r11;""pop %r10;""pop %r9;""pop %r8;""pop %rdx;""pop %rcx;""pop %rbx;""pop %rax;");
  asm volatile("mov %%rbp, %%rsp;"
               "pop %%rbp;"
               "iretq;"
               :::"memory");
}

void do_exit()
{
  /*You may need to invoke the scheduler from here if there are
    other processes except swapper in the system. Make sure you make 
    the status of the current process to UNUSED before scheduling 
    the next process. If the only process alive in system is swapper, 
    invoke do_cleanup() to shutdown gem5 (by crashing it, huh!)
    */
    do_cleanup();  /*Call this conditionally, see comments above*/
}

/*system call handler for sleep*/
long do_sleep(u32 ticks)
{
   
}

/*
  system call handler for clone, create thread like 
  execution contexts
*/
long do_clone(void *th_func, void *user_stack)
{
   
}

long invoke_sync_signal(int signo, u64 *ustackp, u64 *urip)
{
   /*If signal handler is registered, manipulate user stack and RIP to execute signal handler*/
   /*ustackp and urip are pointers to user RSP and user RIP in the exception/interrupt stack*/ 
    printf("Called signal with ustackp=%x urip=%x\n", *ustackp, *urip);
   /*Default behavior is exit( ) if sighandler is not registered for SIGFPE or SIGSEGV.
    Ignore for SIGALRM*/
    struct exec_context *current=get_current_ctx();
    if(current->sighandlers[signo]){
       u64 *temp=ustackp-1;
       *temp=urip;
       ustackp=temp;
       urip=(u64 *)current->sighandlers[signo];
    }
    if(signo != SIGALRM)
      do_exit();
}
/*system call handler for signal, to register a handler*/
long do_signal(int signo, unsigned long handler){
    struct exec_context *current=get_current_ctx();
    current->sighandlers[signo]=(void *)handler;
}

/*system call handler for alarm*/
long do_alarm(u32 ticks){
    struct exec_context *current=get_current_ctx();
    current->alarm_config_time=ticks;
    current->ticks_to_alarm=ticks;
}
