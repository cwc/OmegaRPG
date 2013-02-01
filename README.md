Omega
=====

A text-based roguelike game

[![Build Status](https://travis-ci.org/cwc/OmegaRPG.png)](https://travis-ci.org/cwc/OmegaRPG)

About the Game
-----
David Kinder sums it up pretty well:

> Omega is a game written by Laurence Brothers in the late 1980s for Unix workstations. It derives principally from the tradition of earlier games such as Rogue and Hack, where the player explores a labyrinth of dungeons and monsters through the medium of an ASCII map display. 

> Omega is interesting as it was one of the first such games to show the influence of more story and quest led games, particularly the Ultima series that were released commercially by Origin Systems for various micros during the 1980s. Rather than simply wandering round a dungeon, in Omega the player has various paths open to them to appease the gods of Omega's world. 

About this Project
-----
Omega is a unique part of roguelike history, and deserves to be preserved.

The primary goal of this project is to make the codebase more maintainable. To that end, the code will be cleaned up and refactored from pure C to object-oriented C++. This will be no small task, but once accomplished, will greatly ease the barrier of entry for developers who might wish to contribute.

Future goals include extensibility by way of a popular scripting language such as Python or Lua (an apparently dead fork already exists that added a Scheme interpreter for certain components), a tile-based renderer (a la [David Kinder's port](http://github.com/DavidKinder/Omega)), and of course an expanded game world with new gameplay elements.
