
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

## 2024-03-27 testing

Has been running minute by minute from `cron` on `sodus` using

```text
* * * * * /home/hbarta/bin/bme280 2>>/home/hbarta/Downloads/temp_humidity.txt | /bin/mosquitto_pub -s -t "HA/$(/bin/hostname)/master_bedroom/temp_humidity" -h mqtt>>/tmp/temp_humidity_cron.txt 2>&1
```

Found in `/home/hbarta/Downloads/temp_humidity.txt` - need to look at that code and ID the entry in MQTT capture

```text
1711547341, 5c 0d 00 7b d6 00 5d cd 68.792000 991.538574 38.265625 0 4
Can't open file for reading: No such file or directory
1711547402, 80 00 00 80 00 00 80 00 78.584000 748.081970 89.565430 0 5
post fprintf: Invalid argument
put_vals() 33 chars written
post fclose: Invalid argument
fclose() 0, errno 22
1711547578, 5c 4d 00 7c c5 00 5d 02 70.987999 991.580261 37.070312 0 4
```

These errors may have been produced during the first execution following reboot - yes, confirmed.

```text
HA/sodus/master_bedroom/temp_humidity {"t":1711547341, "humid":38.27, "press":991.54, "temp":68.79, "coeffs":"5c 0d 00 7b d6 00 5d cd"}
...
HA/sodus/master_bedroom/temp_humidity {"t":1711547578, "humid":37.07, "press":991.58, "temp":70.99, "coeffs":"5c 4d 00 7c c5 00 5d 02"}
HA/sodus/master_bedroom/temp_humidity {"t":1711547579, "humid":37.59, "press":991.60, "temp":71.53, "coeffs":"5c 5c 00 7c ff 00 5d 5a"}
HA/sodus/master_bedroom/temp_humidity {"t":1711547581, "humid":37.34, "press":991.70, "temp":71.58, "coeffs":"5c 5a 00 7d 05 00 5d 32"}
```

That search was not fruitful. But looking at otheMWTT messages I find

```text
HA/sodus/master_bedroom/temp_humidity {"t":1711232101, "humid":32.15, "press":999.35, "temp":73.63, "coeffs":"5b 7f 00 7d e6 00 59 c0"}
HA/sodus/master_bedroom/temp_humidity {"t":1711232221, "humid":0.00, "press":642.60, "temp":32.00, "coeffs":"5b 80 00 7d eb 00 59 cf"}
HA/sodus/master_bedroom/temp_humidity {"t":1711232281, "humid":32.01, "press":999.32, "temp":73.80, "coeffs":"5b 85 00 7d f7 00 59 a8"}
...
HA/sodus/master_bedroom/temp_humidity {"t":1711233001, "humid":31.99, "press":999.52, "temp":73.85, "coeffs":"5b 7f 00 7d fc 00 59 a5"}
HA/sodus/master_bedroom/temp_humidity {"t":1711233302, "humid":28.09, "press":739.89, "temp":-272.87, "coeffs":"5b 81 00 7e 04 00 59 73"}
HA/sodus/master_bedroom/temp_humidity {"t":1711233361, "humid":31.52, "press":999.87, "temp":73.87, "coeffs":"5b 73 00 7d ff 00 59 55"}
...
HA/sodus/master_bedroom/temp_humidity {"t":1711503302, "humid":42.22, "press":982.11, "temp":67.95, "coeffs":"5d 4f 00 7b 7a 00 60 74"}
HA/sodus/master_bedroom/temp_humidity {"t":1711503361, "humid":42.06, "press":953.70, "temp":32.31, "coeffs":"5d 49 00 7b 77 00 60 72"}
HA/sodus/master_bedroom/temp_humidity {"t":1711503421, "humid":42.38, "press":982.06, "temp":67.78, "coeffs":"5d 4c 00 7b 68 00 60 8f"}
```

Sanity checks that come out of this are

* humidity == 0.0
* delta pressure > 100
* delta temperature > 20

Pressure and temperature tests implemented at present.
