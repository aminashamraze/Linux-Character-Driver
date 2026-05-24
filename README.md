# Linux-Character-Driver
Linux character driver contains open(), release(), read() and write() from user space to kernel space.

# How to run?
Upload character driver in linux, then from inside the folder, run
>> make
after you see that make is successful, do.
>> sudo insmod simple_char_driver.ko
>> dmesg | tail -n 50
You will see some message like Simple char driver: loaded.
>> cat /proc/device | grep simple
You should see your simple device there.
>> ls -l /devices/simple_char_driver
You will see something like below,
crw------- 1 root root 550, 0 ... /dev/simple_char_driver
To rmmod, use the following command
>> sudo rmmod simple_char_driver.ko
>> dmesg | tail -n 50
check for a message saying simple_char_driver: unloaded.
