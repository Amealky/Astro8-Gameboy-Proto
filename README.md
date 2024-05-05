``Reupload project from 2017 : This project was recovered so don't take commits into accounts``

<h1 align="center"> Astro8 Prototype <h1>

<img src="https://img.shields.io/badge/platform-gameboy-yellowgreen" />
<img src="https://img.shields.io/badge/language-c-lightgrey" />


## Preview
<p float="left" align="center">
  <img src="https://i.ibb.co/5FMPBS0/astro8-0.png" width="300" />
</p>



## Overview
``This project is a non finished Gameboy game prototype`` you play a starship, you can shoot on them to destroy them and you can also be kill by them.


The github file already provide the ``.gb``, so just drag n drop it into a Gameboy Emulator [Bgb for windows user](http://bgb.bircd.org/) or [mGBA for linux and mac user](https://mgba.io/)


This prototype was made in C using the [GBDK devkit](https://github.com/gbdk-2020/gbdk-2020) for Nintendo Gameboy and more


## Controls
Beware to set your emulator control befeore !

A -> Go to (rule ?) screen
B -> Shoot
Up/Down/Right/Left -> Move the ship
Start -> Start the game


## Build the project

If you want to build the project you can follow the step below :
- Ensure you have GCC installed
- Download the [GBDK devkit](https://github.com/gbdk-2020/gbdk-2020)
- Put the GBDK folder where you want but copy the path to it
- Then open the Makefile and set the ``GBDK_FOLDER`` var at the path you got (keep the / at the end of the GBDK path)
- Finally just run ``make`` command this will replace the .gb by a freshly builded one
- Then you can just drag n drop it into a GB emulator
- (Optionnal) if you got lot of compilation file you can run ``make clean``
