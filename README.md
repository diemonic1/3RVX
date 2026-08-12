<a href="https://3RVX.com"><img align="right" src="Assets/MainIcon-PNGs/64.png"/></a>

3RVX
====
3RVX is a customizable on-screen display (OSD) and hotkey tool for Windows systems.

Homepage: [3RVX.com](https://3RVX.com)

This is a fork of the original [3RVX](https://github.com/malensek/3RVX) project. Compared to upstream, this fork adds:

- **"All Monitors Except Primary" display option** — in addition to showing the OSD on the primary monitor, a single specific monitor, or all monitors, you can now show it on every monitor *except* the primary one. This is handy on multi-monitor setups where you don't want notifications popping up on your main/primary screen. See [New Display Option](#new-display-option-all-monitors-except-primary) below.
- **Per-monitor skins** — a second skin can be assigned specifically to secondary (non-primary) monitors, independent of the skin used on the primary monitor. See [Per-Monitor Skins](#per-monitor-skins) below.
- **Per-monitor notification scale** — the primary and secondary monitor notifications can each be scaled independently, from 50% to 150%. See [Per-Monitor Notification Scale](#per-monitor-notification-scale) below.
- **Two new skins**: `Win11` and `Win11 Larger (+20%)`, styled after the native Windows 11 volume/brightness OSD. See [New Skins](#new-skins) below.
- **New translations**: Turkish, Russian, German, Italian, Spanish, Belarusian, Polish, Portuguese, and Kazakh localization files.


New Display Option: All Monitors Except Primary
-------------------------------------------------
The **Display** settings tab's "Display on Monitor" dropdown now includes an **All Monitors Except Primary** option, alongside the existing "Primary Monitor", "All Monitors", and individual monitor choices. When selected, the OSD is shown on every connected monitor other than the primary/main one. On a single-monitor system, it falls back to showing on that monitor.


Per-Monitor Skins
------------------
The **General** settings tab's "Skin" section now has two skin pickers:

- **Primary Monitor Skin** — the skin shown on the primary/main monitor (this is the same skin selector that existed before).
- **Secondary Monitors Skin** — the skin shown on every other monitor. Defaults to **Same as Primary Monitor**, which keeps the previous single-skin behavior.

This works consistently no matter which "Display on Monitor" option is chosen:
- **Primary Monitor**: only the primary skin is used.
- **All Monitors**: the primary monitor shows the primary skin, and every other monitor shows the secondary skin.
- **All Monitors Except Primary**: every shown monitor uses the secondary skin (the primary monitor is never included in this mode).
- **A specific monitor**: that monitor uses the primary skin if it *is* the primary monitor, or the secondary skin otherwise.

If the secondary skin is left as "Same as Primary Monitor", every monitor renders identically, matching upstream behavior.


Per-Monitor Notification Scale
-------------------------------
Next to each skin picker in the **General** tab's "Skin" section, a **Notification Scale (%)** spinner lets you resize the notification independently for the primary monitor and for secondary monitors, from **50%** to **150%** (default: **100%**, the original size). This follows the exact same primary/secondary split described above for skins — the primary monitor always uses the primary scale, and every other shown monitor uses the secondary scale, regardless of which "Display on Monitor" option is active.


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
