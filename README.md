## FunLeds

A tiny project to drive LED lights connected to an (Arduino Nano)[https://www.arduino.cc/en/pmwiki.php?n=Main/ArduinoBoardNano].

The motivation for the project came from a small Christmas tree that had a
broken [COB](https://en.wikipedia.org/wiki/Chip_on_board) circuit.

I have created a new circuit on a proto board and hooked up an Arduino Nano
that was recycled from a previous project.

I wanted to create a board that could be used by my kids if they ever fancy to
play and learn about basic embedded development.

The result is not an award winning high-tech "product", but it works:

![Picture 1](https://photos.app.goo.gl/C262GftGPov8SX3q8)

### Firmware

The code that runs on the Nano is rather simple. It allows to play light shows
that are defined in a simple [data structure](https://en.wikipedia.org/wiki/Struct_(C_programming_language)):

```
{{black, 0, 0}, {red, 1, 0}, {green, 1, 0}, {blue, 1, 0}, {white, 2, 2}};
```

A more verbose format is also allowed:

```
{
  {{0, 0, 0}, 1, 0},
  {{40, 0, 0}, 1, 0},
  {{80, 0, 0}, 1, 0},
  {{120, 0, 0}, 1, 0}
}
```

The data scheme of a single light show element (ie. a light setting at a given
time) is the following:

```
{r, g, b}, fadein, fadeout}

r, g, b: the intensity of the red, green and blue LEDs
fadein : how fast the rgb values should appear (fade in)
fadeout: how fast the rgb values should disappear (fadeout)
```

### LED control

There are 2 LEDs for each (red, green, blue) color. These LEDs are connected in
parallel. The Nano drives each LED pairs via [digital pins](https://www.arduino.cc/en/reference/board)
that can be used in [PWM mode](https://www.arduino.cc/en/Tutorial/Foundations/PWM).

The pins used are:

* red  : pin 9
* green: pin 10
* blue : pin 11

The firmware sets values between 0 and 255, that affects the duty cycle
of the output signal on those pins. 0 represents 0% duty cycle (ie. LED
off), while 255 means 100% duty cycle (ie. LED with full brightness).

### To-do

* Would be nice to write a simple web based composer for creating light shows
and produce the required data structure for the firmware to play the show.

* The hardware could also include an SD card to read the show definition from.
There would be no need to reflash every time somebody wants to have a different
light show.

* The fade out values are not used at the moment, I was even thinking to remove
them. Need a bit of thinking how to combine fade out and fade in in a sequence.

* Create a [Fritzing](https://fritzing.org/) file with the board schematics.

* Add these to-dos to Github issues. :)

### License

MIT license (see [LICENSE](LICENSE))
