// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "SkinManager.h"

#include "Shlwapi.h"

#include "../Error.h"
#include "../Settings.h"

#include "ErrorSkin.h"
#include "MeterComponent.h"
#include "OSDComponent.h"
#include "Skin.h"
#include "SkinV2.h"
#include "SkinV3.h"
#include "SliderComponent.h"

SkinManager *SkinManager::instance;

SkinManager *SkinManager::Instance() {
    if (instance == NULL) {
        instance = new SkinManager();
    }
    return instance;
}

void SkinManager::LoadSkin(std::wstring skinXML, std::wstring secondarySkinXML) {
    DisposeComponents();

    /* First, make sure the skin directory exists. */
    Settings *settings = Settings::Instance();
    std::wstring skinDir = settings->SkinDir();
    if (PathFileExists(skinDir.c_str()) == FALSE) {
        Error::ErrorMessageDie(Error::SKINERR_SKINDIR, skinDir);
    }

    LoadComponentSet(skinXML, false);

    /* Resolve "same as primary" (empty string) to the primary skin's path.
     * A secondary component set is always loaded (even when it resolves to
     * the same file as the primary skin) so that the secondary MeterWnd
     * always has its own independent Meter instances -- these must never be
     * shared with the primary MeterWnd, since both may be redrawn/scaled
     * independently. */
    std::wstring resolvedSecondaryXML =
        (secondarySkinXML == L"") ? skinXML : secondarySkinXML;
    _hasSecondarySkin = (resolvedSecondaryXML != skinXML);
    LoadComponentSet(resolvedSecondaryXML, true);
}

void SkinManager::LoadComponentSet(std::wstring skinXML, bool secondary) {
    Settings *settings = Settings::Instance();

    Skin *skin;
    SkinInfo info(skinXML, false);
    if (info.FormatVersion() == 2) {
        skin = new SkinV2(skinXML);
    } else {
        skin = new SkinV3(skinXML);
    }

    std::vector<Skin *> skins;
    skins.push_back(skin);
    skins.push_back(new SkinV3(settings->SkinXML(L"Classic")));
    skins.push_back(new ErrorSkin());

    OSDComponent *volumeOSD = nullptr;
    std::vector<HICON> volumeIconset;
    SliderComponent *volumeSlider = nullptr;
    OSDComponent *muteOSD = nullptr;
    OSDComponent *ejectOSD = nullptr;
    HICON ejectIcon = nullptr;
    OSDComponent *brightnessOSD = nullptr;

    for (Skin *skin : skins) {
        if (volumeOSD == nullptr) {
            volumeOSD = skin->VolumeOSD();
        }

        if (secondary == false && volumeIconset.size() == 0) {
            volumeIconset = skin->VolumeIconset();
        }

        if (secondary == false && volumeSlider == nullptr) {
            volumeSlider = skin->VolumeSlider();
        }

        if (muteOSD == nullptr) {
            muteOSD = skin->MuteOSD();
        }

        if (ejectOSD == nullptr) {
            ejectOSD = skin->EjectOSD();
        }

        if (secondary == false && ejectIcon == nullptr) {
            ejectIcon = skin->EjectIcon();
        }

        if (brightnessOSD == nullptr) {
            brightnessOSD = skin->BrightnessOSD();
        }
    }

    for (Skin *skin : skins) {
        delete skin;
    }

    if (secondary) {
        _volumeOSDSecondary = volumeOSD;
        _muteOSDSecondary = muteOSD;
        _ejectOSDSecondary = ejectOSD;
        _brightnessOSDSecondary = brightnessOSD;
    } else {
        _volumeOSD = volumeOSD;
        _volumeIconset = volumeIconset;
        _volumeSlider = volumeSlider;
        _muteOSD = muteOSD;
        _ejectOSD = ejectOSD;
        _ejectIcon = ejectIcon;
        _brightnessOSD = brightnessOSD;
    }
}

bool SkinManager::HasSecondarySkin() {
    return _hasSecondarySkin;
}

OSDComponent *SkinManager::VolumeOSD(bool secondary) {
    if (secondary && _volumeOSDSecondary != nullptr) {
        return _volumeOSDSecondary;
    }
    return _volumeOSD;
}

std::vector<HICON> &SkinManager::VolumeIconset() {
    return _volumeIconset;
}

SliderComponent *SkinManager::VolumeSlider() {
    return _volumeSlider;
}

OSDComponent *SkinManager::MuteOSD(bool secondary) {
    if (secondary && _muteOSDSecondary != nullptr) {
        return _muteOSDSecondary;
    }
    return _muteOSD;
}

OSDComponent *SkinManager::EjectOSD(bool secondary) {
    if (secondary && _ejectOSDSecondary != nullptr) {
        return _ejectOSDSecondary;
    }
    return _ejectOSD;
}

HICON &SkinManager::EjectIcon() {
    return _ejectIcon;
}

OSDComponent * SkinManager::BrightnessOSD(bool secondary) {
    if (secondary && _brightnessOSDSecondary != nullptr) {
        return _brightnessOSDSecondary;
    }
    return _brightnessOSD;
}

SkinManager::~SkinManager() {
    DisposeComponents();
}

void SkinManager::DisposeComponents() {
    delete _volumeOSD;
    _volumeOSD = nullptr;
    delete _volumeOSDSecondary;
    _volumeOSDSecondary = nullptr;
    for (HICON icon : _volumeIconset) {
        DestroyIcon(icon);
    }
    _volumeIconset.clear();
    delete _volumeSlider;
    _volumeSlider = nullptr;

    delete _muteOSD;
    _muteOSD = nullptr;
    delete _muteOSDSecondary;
    _muteOSDSecondary = nullptr;

    delete _ejectOSD;
    _ejectOSD = nullptr;
    delete _ejectOSDSecondary;
    _ejectOSDSecondary = nullptr;
    DestroyIcon(_ejectIcon);
    _ejectIcon = nullptr;

    delete _brightnessOSD;
    _brightnessOSD = nullptr;
    delete _brightnessOSDSecondary;
    _brightnessOSDSecondary = nullptr;

    _hasSecondarySkin = false;
}