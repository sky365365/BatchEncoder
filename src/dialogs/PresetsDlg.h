﻿// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxcmn.h>
#include <afxwin.h>
#include <string>
#include <utility>
#include "controls\MyListCtrl.h"
#include "controls\MyButton.h"
#include "controls\MyComboBox.h"
#include "controls\MyEdit.h"
#include "controls\MyStatic.h"
#include "controls\MyDialogEx.h"
#include "configuration\FormatsList.h"
#include "configuration\Configuration.h"
#include "utilities\StringHelper.h"

#define PRESET_COLUMN_NAME      0
#define PRESET_COLUMN_OPTIONS   1

namespace app
{
    class CPresetsDlg;

    typedef struct tagPresetsDlgDropContext
    {
        CPresetsDlg *pDlg = nullptr;
        HDROP hDrop = nullptr;
        HANDLE hThread = nullptr;
        DWORD dwThreadID = -1;
        volatile bool bHandled = true;
    } PresetsDlgDropContext;

    class CPresetsDlg : public controls::CMyDialogEx
    {
        DECLARE_DYNAMIC(CPresetsDlg)
    public:
        CPresetsDlg(CWnd* pParent = nullptr);
        virtual ~CPresetsDlg();
        enum { IDD = IDD_DIALOG_PRESETS };
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);
        DECLARE_MESSAGE_MAP()
    protected:
        HICON m_hIcon;
        afx_msg void OnPaint();
        afx_msg HCURSOR OnQueryDragIcon();
    public:
        virtual BOOL OnInitDialog();
    public:
        PresetsDlgDropContext m_DD;
        config::CConfiguration *pConfig;
        std::wstring szPresetsDialogResize;
        std::wstring szPresetsListColumns;
        volatile bool bUpdate;
        int nSelectedFormat;
        config::CFormatsList m_Formats;
    public:
        controls::CMyStatic m_StcName;
        controls::CMyStatic m_StcOptions;
        controls::CMyListCtrl m_LstPresets;
        controls::CMyComboBox m_CmbFormat;
        controls::CMyEdit m_EdtName;
        controls::CMyEdit m_EdtOptions;
        controls::CMyButton m_BtnOK;
        controls::CMyButton m_BtnCancel;
        controls::CMyButton m_BtnDuplicate;
        controls::CMyButton m_BtnRemoveAll;
        controls::CMyButton m_BtnRemove;
        controls::CMyButton m_BtnAdd;
        controls::CMyButton m_BtnMoveUp;
        controls::CMyButton m_BtnMoveDown;
        controls::CMyButton m_BtnUpdate;
        controls::CMyButton m_BtnLoad;
        controls::CMyButton m_BtnSave;
    public:
        afx_msg void OnDropFiles(HDROP hDropInfo);
        afx_msg void OnBnClickedOk();
        afx_msg void OnBnClickedCancel();
        afx_msg void OnLvnItemchangedListPresets(NMHDR *pNMHDR, LRESULT *pResult);
        afx_msg void OnCbnSelchangeComboPresetFormat();
        afx_msg void OnBnClickedButtonDuplicate();
        afx_msg void OnBnClickedButtonRemoveAllPresets();
        afx_msg void OnBnClickedButtonRemovePreset();
        afx_msg void OnBnClickedButtonAddPreset();
        afx_msg void OnBnClickedButtonPresetUp();
        afx_msg void OnBnClickedButtonPresetDown();
        afx_msg void OnBnClickedButtonUpdatePreset();
        afx_msg void OnEnChangeEditPresetName();
        afx_msg void OnEnChangeEditPresetOptions();
        afx_msg void OnBnClickedButtonLoadPresets();
        afx_msg void OnBnClickedButtonSavePresets();
        afx_msg void OnClose();
    public:
        void LoadWindowSettings();
        void SaveWindowSettings();
        void SetLanguage();
        void AddToList(config::CPreset &preset, int nItem);
        void InsertPresetsToListCtrl();
        void HandleDropFiles(HDROP hDropInfo);
        void UpdateFields(config::CPreset &preset);
        void ListSelectionChange();
    public:
        bool LoadPresets(const std::wstring& szFileXml);
        bool LoadPresets(xml::XmlDocumnent &doc);
        bool SavePresets(const std::wstring& szFileXml, config::CFormat &format);
    };
}
