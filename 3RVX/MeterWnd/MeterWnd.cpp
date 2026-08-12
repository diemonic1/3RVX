// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "MeterWnd.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include <VersionHelpers.h>
#include <sstream>

#include "../DisplayManager.h"
#include "../Skin/AccentColor.h"
#include "Animation.h"
#include "AnimationFactory.h"

MeterWnd::MeterWnd(LPCWSTR className, LPCWSTR title, HINSTANCE hInstance) :
LayeredWnd(className, title, hInstance, NULL, WINDOW_STYLES) {

}

MeterWnd::~MeterWnd() {
    delete _hideAnimation;
    delete _composite;
    DeleteClones();
}

void MeterWnd::Update() {
    CLOG(L"Updating meter window");
    using namespace Gdiplus;

    bool dirty = (_composite == NULL);

    for (Meter *meter : _meters) {
        if (meter->Dirty() == true) {
            dirty = true;
            break;
        }
    }

    if (dirty) {
        QCLOG(L"Contents have changed; redrawing");

        if (_composite) {
            delete _composite;
        }

        Rect bgRect(0, 0, _background->GetWidth(), _background->GetHeight());
        Gdiplus::Bitmap *drawn
            = _background->Clone(bgRect, PixelFormat32bppARGB);
        Graphics graphics(drawn);

        for (Meter *meter : _meters) {
            CLOG(L"Drawing meter:\n%s", meter->ToString().c_str());
            meter->Draw(drawn, &graphics);
        }

        if (_scale != 1.0f) {
            int scaledW = (int) (drawn->GetWidth() * _scale + 0.5f);
            int scaledH = (int) (drawn->GetHeight() * _scale + 0.5f);
            if (scaledW < 1) {
                scaledW = 1;
            }
            if (scaledH < 1) {
                scaledH = 1;
            }

            Gdiplus::Bitmap *scaled
                = new Gdiplus::Bitmap(scaledW, scaledH, PixelFormat32bppARGB);
            Graphics scaleGraphics(scaled);
            scaleGraphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
            scaleGraphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);
            scaleGraphics.DrawImage(drawn, 0, 0, scaledW, scaledH);
            delete drawn;
            _composite = scaled;
        } else {
            _composite = drawn;
        }
    }

    Bitmap(_composite);
    UpdateClones();

    if (_secondaryTwin != nullptr) {
        _secondaryTwin->Update();
    }
}

void MeterWnd::AddMeter(Meter *meter) {
    _meters.push_back(meter);
}

const std::list<Meter*>& MeterWnd::Meters() {
    return _meters;
}

void MeterWnd::MeterLevels(float value) {
    for (Meter *meter : _meters) {
        meter->Value(value);
    }

    if (_secondaryTwin != nullptr) {
        _secondaryTwin->MeterLevels(value);
    }
}

void MeterWnd::HideAnimation(AnimationTypes::HideAnimation anim, int speed) {
    delete _hideAnimation;
    _hideAnimation = AnimationFactory::Create(anim, speed);
}

void MeterWnd::VisibleDuration(int duration) {
    _visibleDuration = duration;
}

void MeterWnd::BackgroundImage(Gdiplus::Bitmap *background) {
    _background = background;
}

bool MeterWnd::EnableGlass(Gdiplus::Bitmap *mask) {
    bool result = LayeredWnd::EnableGlass(mask);
    ApplyClonesGlass();
    return result;
}

void MeterWnd::Show(bool animate) {
    if (_active) {
        if (_visible == false) {
            UpdateWindowPosition();

            bool disabled = false;
            if (_disableFullscreen
                    && DisplayManager::IsFullscreen(Window::Handle())) {
                CLOG(L"not showing (fs)");
                disabled = true;
            }

            if (_disableDirectX
                    && _d3dDevice->Occluded()) {
                CLOG(L"not showing (occluded)");
                disabled = true;
            }


            if (disabled == false) {
                ShowWindow(Window::Handle(), SW_SHOW);
                _visible = true;
            }
        }

        ShowClones();

        if (_visibleDuration > 0) {
            SetTimer(Window::Handle(), TIMER_HIDE, _visibleDuration, NULL);
            KillTimer(Window::Handle(), TIMER_OUT);

            if (_hideAnimation) {
                _hideAnimation->Reset(this);
            }
        }
    }

    if (_secondaryTwin != nullptr) {
        _secondaryTwin->Show(animate);
    }
}

void MeterWnd::Hide(bool animate) {
    if (_active && _visible) {
        if (animate && _hideAnimation) {
            SetTimer(Window::Handle(),
                TIMER_OUT, _hideAnimation->UpdateInterval(), NULL);
        } else {
            ShowWindow(Window::Handle(), SW_HIDE);
            _visible = false;
            HideClones();
        }
    }

    if (_secondaryTwin != nullptr) {
        _secondaryTwin->Hide(animate);
    }
}

void MeterWnd::Active(bool active) {
    _active = active;
}

bool MeterWnd::Active() {
    return _active;
}

void MeterWnd::SecondaryTwin(MeterWnd *twin) {
    _secondaryTwin = twin;
}

void MeterWnd::Scale(float scale) {
    if (_scale == scale) {
        return;
    }

    _scale = scale;
    if (_composite != NULL) {
        delete _composite;
        _composite = NULL;
    }
}

void MeterWnd::AnimateOut() {
    bool animOver = _hideAnimation->Animate(this);
    if (animOver) {
        CLOG(L"Finished hide animation.");
        KillTimer(Window::Handle(), TIMER_OUT);
        ShowWindow(Window::Handle(), SW_HIDE);
        _visible = false;
        HideClones();
    }
}

byte MeterWnd::Transparency() {
    return LayeredWnd::Transparency();
}

void MeterWnd::Transparency(byte transparency) {
    LayeredWnd::Transparency(transparency);
    UpdateClonesTransparency(transparency);
}

LayeredWnd *MeterWnd::Clone() {
    size_t numClones = _clones.size() + 1;
    std::wstringstream cloneClass;
    cloneClass << Window::ClassName() << L":" << numClones;
    std::wstringstream cloneTitle;
    cloneTitle << Window::Title() << L":" << numClones;

    LayeredWnd *clone = new LayeredWnd(
        cloneClass.str().c_str(),
        cloneTitle.str().c_str(),
        Window::InstanceHandle(),
        _composite,
        GetWindowLong(Window::Handle(), GWL_EXSTYLE));

    if (_glassMask) {
        clone->EnableGlass(_glassMask);
    }

    _clones.push_back(clone);
    CLOG(L"Created meter window clone: %s/%s",
        cloneClass.str().c_str(), cloneTitle.str().c_str());
    return clone;
}

std::vector<LayeredWnd *> MeterWnd::Clones() {
    return _clones;
}

void MeterWnd::DeleteClones() {
    for (LayeredWnd *clone : _clones) {
        delete clone;
    }
    _clones.clear();
}

void MeterWnd::UpdateClones() {
    for (LayeredWnd *clone : _clones) {
        clone->Bitmap(_composite);
    }
}

void MeterWnd::UpdateClonesTransparency(byte transparency) {
    for (LayeredWnd *clone : _clones) {
        clone->Transparency(transparency);
    }
}

void MeterWnd::ShowClones() {
    for (LayeredWnd *clone : _clones) {
        if (_disableFullscreen
                && DisplayManager::IsFullscreen(clone->Handle())) {
            continue;
        }
        if (_disableDirectX
                && DisplayManager::Direct3DOccluded(clone->Handle())) {
            continue;
        }

        clone->Show();
    }
}

void MeterWnd::HideClones() {
    for (LayeredWnd *clone : _clones) {
        clone->Hide();
    }
}

void MeterWnd::ApplyClonesGlass() {
    if (_glassMask == NULL) {
        return;
    }

    for (LayeredWnd *clone : _clones) {
        clone->EnableGlass(_glassMask);
    }
}

LRESULT MeterWnd::WndProc(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    if (message == WM_TIMER) {
        switch (wParam) {
        case TIMER_HIDE:
            CLOG(L"Display duration has elapsed. Hiding window.");
            Hide();
            KillTimer(hWnd, TIMER_HIDE);
            break;

        case TIMER_OUT:
            AnimateOut();
            break;
        }
    } else if (message == WM_DWMCOLORIZATIONCOLORCHANGED) {
        CLOG(L"updating meter color maps");
        AccentColor::Instance()->Refresh();
        UINT32 color = AccentColor::Instance()->Color();
        for (Meter *m : _meters) {
            if (m->HasColorTransform()) {
                m->UpdateColorTransform(color);
            }
        }
    }

    return LayeredWnd::WndProc(hWnd, message, wParam, lParam);
}

