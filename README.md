

````markdown
# Linux Character Driver

This project implements a simple Linux character device driver with the basic file operations:

- `open()`
- `release()`
- `read()`
- `write()`

The driver creates a character device that can be accessed from user space through a `/dev` node. It demonstrates how user-space programs interact with kernel-space driver code using standard file operations.

## What This Driver Demonstrates

This driver shows the basic structure of a Linux character driver:

- Loading and unloading a kernel module
- Registering a character device with a major and minor number
- Creating a `/dev` device node
- Handling user-space `open()`, `read()`, `write()`, and `close()` calls
- Copying data between user space and kernel space using `copy_to_user()` and `copy_from_user()`

## Build Instructions

From inside the project directory, run:

```bash
make
````

If the build is successful, a kernel module file should be generated:

```bash
simple_char_driver.ko
```

## Loading the Driver

Load the kernel module using:

```bash
sudo insmod simple_char_driver.ko
```

Check the kernel log:

```bash
dmesg | tail -n 50
```

You should see a message similar to:

```text
simple_char_driver: loaded
```

## Checking the Registered Device

To check whether the character device was registered, run:

```bash
cat /proc/devices | grep simple
```

You should see an entry for the simple character driver, along with its assigned major number.

## Checking the Device Node

If the driver creates the device node automatically, check `/dev`:

```bash
ls -l /dev/simple_char_driver
```

You should see something similar to:

```text
crw------- 1 root root 550, 0 ... /dev/simple_char_driver
```

The `c` at the beginning means this is a character device.

The two numbers, such as:

```text
550, 0
```

represent the major and minor numbers.

## Testing the Driver

Write data to the device:

```bash
echo "hello kernel" | sudo tee /dev/simple_char_driver
```

Read data from the device:

```bash
cat /dev/simple_char_driver
```

You can also check the kernel log after read/write operations:

```bash
dmesg | tail -n 50
```

## Unloading the Driver

Remove the kernel module using:

```bash
sudo rmmod simple_char_driver
```

Then check the kernel log again:

```bash
dmesg | tail -n 50
```

You should see a message similar to:

```text
simple_char_driver: unloaded
```

## Clean Build Files

To remove generated build files, run:

```bash
make clean
```

## Notes

This is a beginner-level Linux character driver intended for learning how user-space file operations connect to kernel-space driver callbacks.

The driver is not intended for production use. It is meant to demonstrate the basic Linux character driver flow.



