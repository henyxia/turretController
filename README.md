# Turret Controller

This program allows you to control your turret with your terminal. It's also possible to control them from an Arduino (or any other serial device).

### Is my turret compatible ?

Currently, only 3 devices are supported. To determine if your device is compatible, run `lsusb`. Supported ID are :

* 0a81:ff01 : Tenx turrets
* 0416:9391 : Winbond turrets
* 1130:0202 : Chesen wireless turrets

### How do I compile it ?

First, you need to check few requirements. You need to have :

* make
* gcc
* libusb > 1.0.19

Then just run `make`

### What can i do now ?

Basically, if you don't know what you can do, just run `help`. Here are a list of the available commands :

* `clear` Delete log
* `help` Display help screen
* `stop N` Make the turret N stop
* `top N` Make the turret N go to the top
* `bottom N` Make the turret N go to the bottom
* `left N` Make the turret N go to the left
* `right N` Make the turret N go to the right
* `fire N` Make the turret N fire
* `serial N M` Control turrets from serial N at M bauds
* `serial` Control turrets from /dev/ttyACM0 at 9600 bauds
* `exit` Exit this program

### What should my Arduino send ?

Each time the program query the serial port, it sends one character '?'. Then the Arduino (or any other serial device) respond by a single character describing which turrets will be controlled and how.

* Bit 0..3 :Turret selected (0 for none 1 for turret 1, 2 for turret 2, 3 for turret 1 & 2, etc)
* Bit 4 : Selected turret will fire (1 for fire 0 for nothing)
* Bit 5..7 : (0 for nothing, 1 for left, 2 for right, 3 for bottom, 4 for top)

If Bit 4..7 are set to zero, the selected turrets stop.

### Is it possible to use more than 4 turrets ?

If you compile the software as it is, no. Plugging a fifth turret will just do nothing. But technically, it's possible. If you want to increase this limit, you can edit the MAX_TOURET contsant in turret.h, but this is not very well supported. In fact all `top``left``right``bottom``stop``fire` commands will work but `serial` will bot use more than 4 turrets and the User  Interface will suerly be altered
