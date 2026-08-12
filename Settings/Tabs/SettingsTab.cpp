// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "SettingsTab.h"

#include "../../3RVX/LanguageTranslator.h"
#include "../../3RVX/Settings.h"

SettingsTab::SettingsTab(
        HINSTANCE hInstance, LPCWSTR tabTemplate, LPCWSTR title) :
TabPage(hInstance, tabTemplate,
    Settings::Instance()->Translator()->Translate(title).c_str()) {

}

SettingsTab::~SettingsTab() {

}

INT_PTR SettingsTab::DialogProc(
        HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg) {
    case WM_INITDIALOG:
        Initialize();
        LoadSettings();
        return FALSE;

    case WM_COMMAND:
        PropSheet_Changed(GetParent(hwndDlg), NULL);
        break;

    case WM_NOTIFY:
        /* Catches changes from controls that don't send WM_COMMAND, such as
         * spinner (up-down) controls, which report UDN_DELTAPOS via
         * WM_NOTIFY instead. Without this, adjusting a spinner alone would
         * leave the Apply button disabled and the change would never be
         * saved. */
        PropSheet_Changed(GetParent(hwndDlg), NULL);
        break;
    }

    return Dialog::DialogProc(hwndDlg, uMsg, wParam, lParam);
}