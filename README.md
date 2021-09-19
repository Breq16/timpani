# timpani
the Texas Instruments Machine for Percussion And Noisy Instruments.

...or, alternatively, the Totally Improvised Macro Pad And Novel Interface

## What is it?

Timpani allows a TI-84 Plus CE calculator to become a USB keyboard.


## Setup Guide

This project is built (i.e. compiled, linked, and dumped as an `.8xp`) using the [CE C Toolchain](https://github.com/CE-Programming/toolchain). In general, follow the [Getting Started](https://ce-programming.github.io/toolchain/static/getting-started.html) guide to set up the toolchain.

However, **this project requires the `usbdrvce` library.** As of this writing, this library is not included with the release version of the toolchain. It can currently be found in the `usbsdrvce` [branch](https://github.com/CE-Programming/toolchain/tree/usbdrvce) of the toolchain repo.

The easiest way to install this toolchain build is by downloading the build artifacts from the CI build. At time of writing, the most recent CI run can be found [here](https://github.com/CE-Programming/toolchain/runs/3457640141). You need to install the `CEdev` folder for your OS, *and load the `clibs.8xg` file onto your calculator/emulator.* You NEED this version of `clibs` -- the release version does not contain the `usbdrvce` library!

