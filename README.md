<a href="https://3RVX.com"><img align="right" src="Assets/MainIcon-PNGs/64.png"/></a>

3RVX
====
3RVX is a customizable on-screen display (OSD) and hotkey tool for Windows systems.

Homepage: [3RVX.com](https://3RVX.com)

This is a fork of the original [3RVX](https://github.com/malensek/3RVX) project. Compared to upstream, this fork adds:

- **"All Monitors Except Primary" display option** — in addition to showing the OSD on the primary monitor, a single specific monitor, or all monitors, you can now show it on every monitor *except* the primary one. This is handy on multi-monitor setups where you don't want notifications popping up on your main/primary screen. See [New Display Option](#new-display-option-all-monitors-except-primary) below.
- **Two new skins**: `Win11` and `Win11 Larger (+20%)`, styled after the native Windows 11 volume/brightness OSD. See [New Skins](#new-skins) below.
- **New translations**: Turkish, Russian, German, Italian, Spanish, Belarusian, Polish, Portuguese, and Kazakh localization files.


New Display Option: All Monitors Except Primary
-------------------------------------------------
The **Display** settings tab's "Display on Monitor" dropdown now includes an **All Monitors Except Primary** option, alongside the existing "Primary Monitor", "All Monitors", and individual monitor choices. When selected, the OSD is shown on every connected monitor other than the primary/main one. On a single-monitor system, it falls back to showing on that monitor.


New Skins
---------
- **Win11** — a skin styled after the native Windows 11 volume/brightness OSD.
- **Win11 Larger (+20%)** — the same Win11 skin, scaled up 20% for larger or high-DPI displays.


Building
--------
You can build 3RVX with [Visual Studio](https://www.visualstudio.com/) community edition or higher. There are no external dependencies; clone and compile.


Contributing
------------
Bug reports, feature suggestions, and code contributions are always welcome. If you've helped out, feel free to add yourself to the [contributors](CONTRIBUTORS.md) list as part of your pull request.
