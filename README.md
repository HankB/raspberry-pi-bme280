
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

## Issues

There are times when the readings are clearly in error. Often these are accompanied by recognizable patterns such as all bits set in some of the coefficients. Code has been added to retry the measurement when recognized and the coefficients are now included with the output (along with the retry count.) The recognized bad readings are also reported to STDERR should further investigation be required.
