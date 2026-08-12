// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>

#include <string>
#include <vector>

class Skin;
struct OSDComponent;
struct SliderComponent;

class SkinManager {
public:
    static SkinManager *Instance();

    /// <summary>
    /// Loads the primary skin, and (optionally) a secondary skin used for
    /// notifications shown on non-primary monitors. Pass an empty string for
    /// <paramref name="secondarySkinXML"/> (or the same path as
    /// <paramref name="skinXML"/>) to indicate that secondary monitors should
    /// just use the primary skin.
    /// </summary>
    void LoadSkin(std::wstring skinXML, std::wstring secondarySkinXML = L"");

    /// <summary>
    /// Indicates whether a distinct secondary skin (for non-primary monitors)
    /// is currently loaded. When false, the "secondary" component accessors
    /// simply return the primary components.
    /// </summary>
    bool HasSecondarySkin();

    OSDComponent *VolumeOSD(bool secondary = false);
    std::vector<HICON> &VolumeIconset();
    SliderComponent *VolumeSlider();

    OSDComponent *MuteOSD(bool secondary = false);

    OSDComponent *EjectOSD(bool secondary = false);
    HICON &EjectIcon();

    OSDComponent *BrightnessOSD(bool secondary = false);

private:
    bool _hasSecondarySkin = false;

    OSDComponent *_volumeOSD;
    OSDComponent *_volumeOSDSecondary;
    std::vector<HICON> _volumeIconset;
    SliderComponent *_volumeSlider;
    OSDComponent *_muteOSD;
    OSDComponent *_muteOSDSecondary;

    OSDComponent *_ejectOSD;
    OSDComponent *_ejectOSDSecondary;
    HICON _ejectIcon;

    OSDComponent *_brightnessOSD;
    OSDComponent *_brightnessOSDSecondary;

    ~SkinManager();
    void DisposeComponents();
    void LoadComponentSet(std::wstring skinXML, bool secondary);

private:
    static SkinManager *instance;

};