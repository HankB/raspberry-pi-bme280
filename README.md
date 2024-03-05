
This piece of code was combined from several sources:

https://github.com/adafruit/Adafruit_BME280_Library
https://cdn-shop.adafruit.com/datasheets/BST-BME280_DS001-10.pdf
https://projects.drogon.net/raspberry-pi/wiringpi/i2c-library/

Tested on RASPBIAN JESSIE LITE
https://www.raspberrypi.org/downloads/raspbian/

```
git clone https://github.com/andreiva/raspberry-pi-bme280.git
git clone git@github.com:HankB/raspberry-pi-bme280.git # post fork
```

### Dependencies

```text
sudo apt-get install libi2c-dev i2c-tools wiringpi
```

### Requirements

Enable i2c using `raspi-config` or similar.

### Compiling

```text
make
```

### Usage

Copy binary to /usr/bin

```text
sudo cp bme280 /usr/bin
```

Now you should be able to run the program, simply by typing

```text
bme280
```

Output should look like this

```text
{"sensor":"bme280", "humidity":54.36, "pressure":1011.89, "temperature":25.58, "altitude":9.23, "timestamp":1469568295}
```

## Status

Logic for dealing with bad values is pretty tentative at present.

## Issues

There are times when the readings are clearly in error. Often these are accompanied by recognizable patterns such as all bits set in some of the coefficients. Code has been added to retry the measurement when recognized and the coefficients are now included with the output (along with the retry count.) The recognized bad readings are also reported to STDERR should further investigation be required.

Bad readings continue to cause problems. Some additional logic is being added and the response to bad readings modified. The following rules will be employed.

* `ALL_BITS_SET` - If "all bits set" is identified, the values will be written to STDERR and the program will exit w/out writing results to STDOUT. If this turns out to be common, the logic could ne modified to include retries.
* `INSANE_VALUE` - Values will be checked for "sanity." If something is wildly out of range, the values will not be recorded or reported.
* `EXCESS_DEVIATION` - A record of previous computed values will be stored in a disk file in `/tmp/vals` from one execution to the next. If the new values differ from the previous values by a significant amount, the readings and calculated values will be reported to STDERR and the program will exit w/out writing results to STDOUT. (Likewise WRT retries.) The "discard" logic regarding previous values is complicated by the following.
    * The first execution following reboot may produce wonky values. This could cause subsequent good values to be discarded. For this reason, when the disk file does not exist, (e.g. first execution following reboot) the values 0.0 will be stored in the disk file and the results will not be reported.
    * In other circumstances, if the values are close enough to the previous values, they will be recorded and reported. This might cause a string of bad values to be reported and also the first good falue following a bad value will not be reported.

One big change here is to not perform retries. Depending on the impact of this change, that decision may need to be revisited.
