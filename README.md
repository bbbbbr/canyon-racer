
# Canyon Racer

A Top Down Racing Game for the Game Boy built around a Vertical Parallax effect.

This project started with wanting to try a vertical parallax effect on the original Game Boy, similar to the kind seen in the "[Demotronic](https://demozoo.org/productions/20662/)" Game Boy Color demo by [1.000.000 Boys](https://demozoo.org/productions/20662/). It has since grown into a mini-game.

If you aren't playing on original hardware you may need an accurate emulator (Emulicious, BGB, Sameboy, etc)


![Canyon Racer Intro Splash Screen](/info/canyon_racer_intro_splash.png)
![Canyon Racer Gameplay Screen](/info/canyon_racer_gameplay.png)

### Credits, Contributors and Thanks

Code & Gameplay: [bbbbbr](https://twitter.com/0xbbbbbr)

Music & SFX: [BeatScribe](https://twitter.com/beatscribemusic)

Additional code from:
  - [Coffee "Valen" Bat](https://twitter.com/cofebbat) : [CBTFX Driver](https://github.com/datguywitha3ds/CBT-FX)
  - [SuperDisk](https://github.com/SuperDisk) : [hUGETracker](https://github.com/SuperDisk/hUGETracker)  / [hUGEDriver](https://github.com/SuperDisk/)
  - [toxa](https://github.com/untoxa/) : GBDK HugeDriver integration

Assets:
  - Some canyon tiles are based on this tileset: [sondanielson](https://sondanielson.itch.io/gameboy-simple-rpg-tileset). CCA License
  - Ship tiles are based on this tileset: [Jerom](https://opengameart.org/content/retro-spaceships). CC-BY-3 License.
  - Title Logo letter shapes are stylized versions of the [Vermin Vibes 1989](https://nalgames.com/fonts/vermin-vibes-1989) font. Non-commercial license

Docs & Tools:
  - [Pan Docs](https://gbdev.io/pandocs/) are a useful resource
  - Lots of debugging and optimizing in the [Emulicious](https://emulicious.net/) emulator
  - Built with [GBDK-2020](https://github.com/gbdk-2020/gbdk-2020)


### Builing:
* [GBDK-2020](https://github.com/gbdk-2020/gbdk-2020) (4.1.0-Pre+)
* For SFX converversion from .sav files use the patched converter:
  - https://github.com/bbbbbr/CBT-FX/blob/length_fix/hammer2cbt.py


# About the Effects

### Vertical Parallax
Vertical parallax on the Game Boy takes far more processing resources than Horizontal parallax.

Two likely approaches would be mid-scanline Y axis scrolling and animated background tile updates (with some limitations on how many tiles can be changed per frame). This demo uses mid-scanline Y axis scrolling.

For Horizontal parallax it can be sufficient to modify the X scroll position just once at the start of every new parallax region. For Vertical parallax it has to run every single scanline and modify the Y scroll position for each parallax section _as the pixels for each region are being rendered_, for the entire width _and_ height of the screen. The timing has to be precisely aligned in order to take effect in the right locations.

In this demo only a couple instruction cycles are available to run between each Vertical parallax region to modify the scroll value and load it. The demo runs at a full 60 frames per second.

### Effect Timing and Artifacts
Horizontal scrolling offsets and sprites will both alter the required Vertical parallax timing. While it is possible to somewhat compensate for the horizontal offsets, the computational cost is higher and leaves less room for gameplay. 

There is additional timing variation based on the opcode being executed at the time each scanline interrupt triggers. If it's during an opcode which takes several cycles to complete before the interrupt can jump then the timing may be off by that amount of cycles. The Vertical parallax timing is sensitive down to 1-2 cycles of alignment, so this can also cause artifacts.

This mini game tries to work with those limitations in a way that's visually pleasing when possible and does not try to compensate for them.

### Sine Waves
In addition to the above, scrolling sine waves of various sizes are used to distort the background. This makes it both more visually interesting and more fun to navigate through the canyon.

### How They Look
The two separate effects used to create the canyon environment:

* **LEFT** Y Axis Vertical Parallax that scrolls the separate background columns.
* **RIGHT** X Axis scrolling Sine Waves that distort the background and change the canyon bottom's shape.
![Canyon Racer Intro Splash Screen](/info/bg_scy_parallax_scrolling.gif)
![Canyon Racer Intro Splash Screen](/info/bg_scx_wave_scrolling.gif)

