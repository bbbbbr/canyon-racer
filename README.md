
# Canyon Racer

A small Game Boy Racing Demo with Vertical Parallax Effect.

This project started by experimenting with a vertical parallax effect on the original Game Boy, similar to the kind in the "[Demotronic](https://demozoo.org/productions/20662/)" Game Boy Color demo by [1.000.000 Boys](https://demozoo.org/productions/20662/). It was inspired by a [tweet](https://twitter.com/_Kimimi/status/1493231550040793092) about a canyon effect in the game Étoile Princess. Could it be done on the Game Boy?

![Canyon Racer Intro Splash Screen](/info/canyon_racer_intro_splash.png)
![Canyon Racer Gameplay Screen](/info/canyon_racer_gameplay.png)

### Download ROMs and Play Online

Downloads and online playable version are at:
* https://bbbbbr.itch.io/canyon-racer


### Game Play
A short (yet infinite), very simple, kinda hard demo game with cool FX and music.

- Guide the racer through the canyon without crashing into walls and jump over rocks.
- Create Save States and Rewind to them after crashing (both actions use up a Life).
- Collect `+1` items to gain more Lives.

Give your eyes frequent rests. There can be some temporary sensation of visual movement after playing due to the vertical parallax.

### Controls
- **Jumping**
  - Jump: **A** button (tap for short or hold for longer)
  - Extra Long Jump: **A** + **Up** (required for some long jumps)
- **Save & Rewind / Lives**
  - Add Save State: **B** (any time during gameplay, but uses 1 Life)
  - Crash -> Rewind to Save State: Automatic when crashed (uses 1 Life, also rewinds Score)
  - Lives: There is a limited supply of Lives (top readout in lower-right)
- Pause: **Start**

### Techniques
- **Save & Rewind:** Creating Save States uses 1 life each time, so add them carefully.
- **Short hops:** Make a series of short, fast taps with **Jump** to hop over several close obstacles in a row.
- **Longer Jump:** Press and hold **Jump**.
- **Extra Long Jump:** Press and hold **Jump** + **Up** to make extra long jumps. The ship will land farther up on the screen if possible.
- **Shortened Jump:** While jumping, use **Down** to reduce jump distance. The ship will land farther down on the screen if possible.
- If the D-Pad is not used during jumps the ship will land in the same location it started in.


### Cartridge Compatibility
- Rumble effect on ship land and crash: [Inside Gadgets](https://shop.insidegadgets.com/) MBC5 Rumble carts (probably other MBC5 rumble carts too)
- Save support:
  - MBC5 carts
  - [Ferrante Crafts](https://ferrantecrafts.com/) 31k + 1k save carts
- Mega Duck:
  - [Inside Gadgets 32K Mega Duck](https://shop.insidegadgets.com/product/megaduck-32kb-flash-cart/) cart. (no save support)
  - [3D Printable cart shell](https://github.com/bbbbbr/megaduck_cartridge_shell)


### Effect Accuracy
Some notes about the visual effects if you aren't playing on an actual Game Boy or chip clone:
  - Use an accurate emulator (Emulicious, BGB, Sameboy, etc) and it should look ok.
  - Currently the Analogue Pocket does not match OEM hardware for the column scroll effect, so timing is off a little.


### MegaDuck
On this platform the full vertical parallax effect is turned off since it may damage some generations of OEM Mega Duck screens (leaving a permanent vertical band) due to the rapid on/off cycling of pixels. Most likely the effect could be enabled safely for aftermarket screens, for the few people who have modded their devices.


### Credits, Contributors and Thanks
Code & Gameplay: [bbbbbr](https://twitter.com/0xbbbbbr)

Music & SFX: [BeatScribe](https://twitter.com/beatscribemusic)

Additional code from:
  - SFX Driver: [Coffee "Valen" Bat](https://twitter.com/cofebbat) : [CBTFX Driver](https://github.com/datguywitha3ds/CBT-FX)
  - Music Driver: [SuperDisk](https://github.com/SuperDisk) : [hUGETracker](https://github.com/SuperDisk/hUGETracker) / [hUGEDriver](https://github.com/SuperDisk/)
  - Ferrante Crafts 31k ROM + 1k Flash cart support - [toxa](https://github.com/untoxa/)

Assets:
  - Some canyon tiles are variations on this tileset: [sondanielson](https://sondanielson.itch.io/gameboy-simple-rpg-tileset). CCA License
  - Ship tiles are variations on this tileset: [Jerom](https://opengameart.org/content/retro-spaceships). CC-BY-3 License.
  - Title Logo letter shapes are re-stylized versions of the [Vermin Vibes 1989](https://nalgames.com/fonts/vermin-vibes-1989) font. Purchased Business Lite license. Non-transferrable to other parties.

Docs & Tools:
  - [Pan Docs](https://gbdev.io/pandocs/) are a useful resource
  - Lots of debugging and optimizing in the [Emulicious](https://emulicious.net/) emulator
  - Built with [GBDK-2020](https://github.com/gbdk-2020/gbdk-2020)


### Building:
* [GBDK-2020](https://github.com/gbdk-2020/gbdk-2020) (4.2.0+)
* For SFX converversion from .sav files use the patched converter:
  - https://github.com/bbbbbr/CBT-FX/blob/length_fix/hammer2cbt.py


# About the Effects

### Vertical Parallax
Vertical parallax on the Game Boy takes far more processing resources than Horizontal parallax.

Two likely approaches would be mid-scanline Y axis scrolling and animated background tile updates (with some limitations on how many tiles can be changed per frame). This demo uses mid-scanline Y axis scrolling.

For Horizontal parallax it can be sufficient to modify the X scroll position just once at the start of every new parallax region. For Vertical parallax it has to run every single scanline and modify the Y scroll position for each parallax section _as the pixels for each region are being drawn_, for the entire width _and_ height of the screen. The timing has to be precisely aligned in order to take effect in the right locations.

For this demo only a couple instruction cycles are available to run between each Vertical parallax region to modify the scroll value and load it. It runs at 60 frames per second. As rendering progresses from left to right the Y scroll position is bit shifted down (divided by 2) once for each vertical region as it approaches the center. From the center onward the Y scroll position is bit shifted back up to it's original value until it reaches the right edge of the screen. Repeating tile patterns are used so that scrolling appears relatively seamless when each region wraps around.

### Effect Timing and Artifacts
Horizontal scrolling offsets and sprites will both alter the required Vertical parallax timing. While it is possible to somewhat compensate for the horizontal offsets, the cpu cost is higher and leaves less room for gameplay. 

There is additional timing variation based on the opcode being executed at the time each scanline interrupt triggers. If it's during an opcode which takes several cycles to complete before the interrupt can jump then the timing may be off by that amount of cycles. The alignment of Vertical parallax timing is sensitive down to 1-2 cycles, so this can also cause artifacts.

This demo game tries to work with those limitations and does not compensate for artifacts.

### Sine Waves
In addition to the Vertical parallax, scrolling horizontal sine waves of various sizes are used to distort the background. This looks better and gives the canyon a shape to navigate through.

#### How They Look
The two separate effects used to create the canyon environment:

* **LEFT** Y Axis Vertical Parallax that scrolls the separate background columns.
* **RIGHT** X Axis scrolling Sine Waves that distort the background and change the canyon bottom's shape.
![Canyon Racer Intro Splash Screen](/info/bg_scy_parallax_scrolling.gif)
![Canyon Racer Intro Splash Screen](/info/bg_scx_wave_scrolling.gif)
