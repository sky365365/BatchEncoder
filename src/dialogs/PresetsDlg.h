﻿// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxcmn.h>
#include <afxwin.h>
#include "controls\MyListCtrl.h"
#include "controls\MyButton.h"
#include "controls\MyComboBox.h"
#include "controls\MyEdit.h"
#include "controls\MyStatic.h"
#include "controls\MyDialogEx.h"
#include "configuration\FormatsList.h"
#include "configuration\Configuration.h"

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

    class CPresetsDlg : public CMyDialogEx
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
        CString szPresetsDialogResize;
        CString szPresetsListColumns;
        volatile bool bUpdate;
        int nSelectedFormat;
        config::CFormatsList m_Formats;
    public:
        CMyStatic m_StcName;
        CMyStatic m_StcOptions;
        CMyListCtrl m_LstPresets;
        CMyComboBox m_CmbFormat;
        CMyEdit m_EdtName;
        CMyEdit m_EdtOptions;
        CMyButton m_BtnOK;
        CMyButton m_BtnCancel;
        CMyButton m_BtnDuplicate;
        CMyButton m_BtnRemoveAll;
        CMyButton m_BtnRemove;
        CMyButton m_BtnAdd;
        CMyButton m_BtnMoveUp;
        CMyButton m_BtnMoveDown;
        CMyButton m_BtnUpdate;
        CMyButton m_BtnLoad;
        CMyButton m_BtnSave;
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
        bool LoadPresets(CString szFileXml);
        bool LoadPresets(XmlDocumnent &doc);
        bool SavePresets(CString szFileXml, config::CFormat &format);
    };
}
