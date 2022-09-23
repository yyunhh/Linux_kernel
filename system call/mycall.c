#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/string.h>

asmlinkage long sys_mycall(char *str)
{
    
    if (!access_ok(VERIFY_WRITE, str, strlen(str))) {
        printk("error(can't access)\n");
        return -EFAULT;
    }

    char buf[110];
    if (strlen(str) > 100) {
        if (copy_from_user(buf, str, 100)) return -EFAULT;
        buf[100] = '\0';
    } else {
        if (copy_from_user(buf, str, strlen(str))) return -EFAULT;
        buf[strlen(str)] = '\0';
    }
    
    printk("%s\n", buf); 
    
    return 0;
}