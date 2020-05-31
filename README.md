MAX30105 - Particle sensor and oximeter sensor (i2c)
====================================================

This is a reference workbench for `ehwe`
([project link](https://github.3)

## How to use this git

See this document in master-branch


## Bus-pirate session

### Read temperture

```
m
1. HiZ
2. 1-WIRE
3. UART
4. I2C
5. SPI
6. 2WIRE
7. 3WIRE
8. LCD
9. DIO
x. exit(without change)

(1)>4
Set speed:
 1. ~5KHz
 2. ~50KHz
 3. ~100KHz
 4. ~400KHz

(1)>4
Ready
I2C>W
Power supplies ON
I2C>A
AUX HIGH
I2C>P
Pull-up resistors ON
I2C>(1)
Searching I2C address space. Found devices at:
0xAE(0x57 W) 0xAF(0x57 R)

I2C>[0xAE 0x21 0x01]
I2C START BIT
WRITE: 0xAE ACK
WRITE: 0x21 ACK
WRITE: 0x01 ACK
I2C STOP BIT
I2C>[0xAE 0x1F [0xAF r:2]
I2C START BIT
WRITE: 0xAE ACK
WRITE: 0x1F ACK
I2C START BIT
WRITE: 0xAF ACK
READ: 0x1A  ACK 0x07
NACK
I2C STOP BIT
I2C>
```



