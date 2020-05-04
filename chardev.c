#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<asm/uaccess.h>
#include "chardev.h"
#define SUCCESS 0
#define DEVICE_NAME "char_dev"
#define BUF_LEN 80

static int Device_Open = 0;
static char *Message_ptr;

static int device_open(struct inode *inode, struct file *file){
    #ifdef DEBUG
        printk(KERN_INFO "device_open(%p)\n", file);
    #endif

    if(Device_Open)
        return -EBUSY;
    Device_Open++;
    Message_ptr = Message;
    try_module_get(THIS_MODULE);
    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file){
    #ifdef DEBUG
        printk(KERN_INFO "device_release(%p %p)\n", inode, file);
    #endif
        Device_Open--;
        rmodule_put(THIS_MODULE);
        return SUCCESS;
}

static ssize_t device_read(struct file *file, char __user *buffer, size_t length, loff_t *offset){
    int bytes_read = 0;

    #ifdef DEBUG
        printk(KERN_INFO "device_read(%p %p %p)\n", file, buffer, length);
    #endif
    if(*Message_ptr == 0)
        return 0;

    while(length && *Message_ptr){
        put_user(*(Message_ptr++), buffer++);
        length--;
        bytes_read++;
    }
    return bytes_read;
}

static ssize_t device_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset){
    int i;
    char Message[100];
    for(i = 0; i < length && i < BUF_LEN; i++)
        get_user(Message[i], buffer + i);
    Message_ptr = Message;
    return i;  
}

int device_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param){
    int i;
    char *temp;
    char ch;
    switch(ioctl_num){
        case IOCTL_SET_MSG:
            temp = (char*)ioctl_param;
            get_user(ch, temp);
            for(i = 0; ch && i < BUF_LEN; i++, temp++)
                get_user(ch, temp);
            device_write(file, (char *)ioctl_param, i, 0);
            break;
        case IOCTL_GET_MSG:
            i = device_read(file, (char *)ioctl_param, 99, 0);
            put_user('\0', (char *)ioctl_param + i);
            break;
        case IOCTL_GET_NTH_BYTE:
            return Message[ioctl_param];
            break;
    }
    return SUCCESS;
}

struct file_operations Fops = {
    .read = device_read,
    .write = device_write,
    .ioctl = device_ioctl,
    .open = device_open,
    .release = device_release
};

int init_module(){
    int ret_val;
    ret_val = register_chrdev(MAJOR_NUM, DEVICE_NAME, &Fops);
    if(ret_val < 0){
        printk(KERN_ALERT "%s failed with %d\n", "Sorry", ret_val);
        return ret_val;
    }
    return 0;
}

void cleanup_module(){
    int ret;
    ret = unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
    if(ret < 0)
        printk(KERN_ALERT "Error : unregister_chrdev %d\n", ret);
}