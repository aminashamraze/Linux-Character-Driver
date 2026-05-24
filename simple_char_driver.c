
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "simple_char"
#define CLASS_NAME  "simple_class"
#define BUFFER_SIZE 256

static dev_t dev_num;
static struct cdev simple_cdev;
static struct class *simple_class;
static struct device *simple_device;

static char device_buffer[BUFFER_SIZE];
static size_t buffer_size = 0;

static int simple_driver_open(struct inode *inode, struct file *file)
{
    pr_info("simple_char: device opened\n");
    return 0;
}

static int simple_release(struct inode *inode, struct file *file)
{
    pr_info("simple_char: device closed\n");
    return 0;
}

static ssize_t simple_read(struct file *file, char __user *user_buffer,
                           size_t count, loff_t *offset)
{
    size_t bytes_available;
    size_t bytes_to_read;

    if (*offset >= buffer_size)
        return 0;

    bytes_available = buffer_size - *offset;

    if (count < bytes_available)
        bytes_to_read = count;
    else
        bytes_to_read = bytes_available;

    if (copy_to_user(user_buffer, device_buffer + *offset, bytes_to_read))
        return -EFAULT;

    *offset += bytes_to_read;

    pr_info("simple_char: read %zu bytes\n", bytes_to_read);

    return bytes_to_read;
}

static ssize_t simple_write(struct file *file, const char __user *user_buffer,
                            size_t count, loff_t *offset)
{
    size_t bytes_to_write;

    if (count >= BUFFER_SIZE)
        bytes_to_write = BUFFER_SIZE - 1;
    else
        bytes_to_write = count;

    if (copy_from_user(device_buffer, user_buffer, bytes_to_write))
        return -EFAULT;

    device_buffer[bytes_to_write] = '\0';
    buffer_size = bytes_to_write;

    pr_info("simple_char: wrote %zu bytes: %s\n", bytes_to_write, device_buffer);

    return bytes_to_write;
}

static struct file_operations simple_fops = {
    .owner = THIS_MODULE,
    .open = simple_driver_open,
    .release = simple_release,
    .read = simple_read,
    .write = simple_write,
};

static int __init simple_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("simple_char: failed to allocate device number\n");
        return ret;
    }

    cdev_init(&simple_cdev, &simple_fops);
    simple_cdev.owner = THIS_MODULE;

    ret = cdev_add(&simple_cdev, dev_num, 1);
    if (ret < 0) {
        pr_err("simple_char: failed to add cdev\n");
        unregister_chrdev_region(dev_num, 1);
        return ret;
    }

    simple_class = class_create(CLASS_NAME);
    if (IS_ERR(simple_class)) {
        pr_err("simple_char: failed to create class\n");
        cdev_del(&simple_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(simple_class);
    }

    simple_device = device_create(simple_class, NULL, dev_num, NULL, DEVICE_NAME);
    if (IS_ERR(simple_device)) {
        pr_err("simple_char: failed to create device\n");
        class_destroy(simple_class);
        cdev_del(&simple_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(simple_device);
    }

    pr_info("simple_char: module loaded\n");
    pr_info("simple_char: major=%d minor=%d\n", MAJOR(dev_num), MINOR(dev_num));

    return 0;
}

static void __exit simple_exit(void)
{
    device_destroy(simple_class, dev_num);
    class_destroy(simple_class);
    cdev_del(&simple_cdev);
    unregister_chrdev_region(dev_num, 1);

    pr_info("simple_char: module unloaded\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Amina");
MODULE_DESCRIPTION("Simple Linux character driver with read and write support");
MODULE_VERSION("1.0");
