#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>

#include <sys/utsname.h>
#include <sys/types.h>
// mm
#include <asm/page.h>
#include <asm/mman.h>
#include <asm/pgtable.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <unistd.h>

struct task_struct *task;

#define MAX_PROC_SIZE 100
static char proc_data[MAX_PROC_SIZE];
char *endp;
int ipid=0;
pid_t p_pid;
static struct proc_dir_entry *proc_write_entry;

int read_proc(char *buf,char **start,off_t offset,int count,int *eof,void *data )
{
    int len=0;
    for_each_process(task){
        if(task->pid == ipid){
            len = sprintf(buf,"\n%d %d OK\n",task->pid,task->mm->map_count);
        }
    }

    //   len = sprintf(buf,"\n%s\n",proc_data);

    // find parent pid
    pid_t p_pid = getppid();
    printk("parent pid = %d\n", p_pid);

    // mm info
    // struct task_struct *task;
    struct mm_struct *cur_mm;
    // task = pid_task(find_vpid(ipid), PIDTYPE_PID);
    cur_mm = task->active_mm;

    printk("Address of code : 0x%lx ~ 0x%lx\n", cur_mm->start_code, cur_mm->end_code);
    printk("Address of data : 0x%lx ~ 0x%lx\n", cur_mm->start_data, cur_mm->end_data);
    printk("Address of heap : 0x%lx ~ 0x%lx\n", cur_mm->start_brk, cur_mm->brk);

    // kernel version
    struct utsname unameData;
    uname(&unameData);
    printk("the kerenel version is %s", unameData.sysname);

    // find # of child
    struct list_head* traverse_ptr;
    int num_counter = 0;
    list_for_each(traverse_ptr, &(task->children));{
        num_counter++;
    }

    printk("# of child = %d", num_counter);

    return len;
}

int write_proc(struct file *file,const char *buf,int count,void *data )
{
    // find current pid
    int ret;
    if(count > MAX_PROC_SIZE)
        count = MAX_PROC_SIZE;
    if(copy_from_user(proc_data, buf, count))
        return -EFAULT;
        ipid=simple_strtol(proc_data,&endp,10);

    // printk(KERN_INFO "current pid is %d ret %d :",ipid, ret);
    printk(KERN_INFO "current pid =  %d\n", ipid);
    

    return count;
}

void create_new_proc_entry()
{
    proc_write_entry = create_proc_entry("myproc",0666,(void *)NULL);
    if(!proc_write_entry)
        {
        printk(KERN_INFO "Error creating proc entry");
        return;
    //    return -ENOMEM;
        }
    proc_write_entry->read_proc = (void *)read_proc ;
    proc_write_entry->write_proc = (void *)write_proc;
    printk(KERN_INFO "proc initialized");
}

int proc_init (void) {
    create_new_proc_entry();
    return 0;
}

void proc_cleanup(void) {
    printk(KERN_INFO " Inside cleanup_module\n");
    remove_proc_entry("myproc",NULL);
}

MODULE_LICENSE("GPL");
module_init(proc_init);
module_exit(proc_cleanup);