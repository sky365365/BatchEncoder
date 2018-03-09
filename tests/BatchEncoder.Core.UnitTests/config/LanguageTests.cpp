﻿// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Language.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CLanguage_Tests)
    {
    public:
        TEST_METHOD(CLanguage_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::CLanguage m_Language;
            #pragma warning(pop)
        }

        TEST_METHOD(CLanguage_GetLanguageById)
        {
            std::vector<config::CLanguage> languages
            {
                { L"en-US" },
                { L"es-ES" },
                { L"ja-JP" },
                { L"ko-KR" },
                { L"pl-PL" }
            };

            auto& language = config::CLanguage::GetLanguageById(languages, L"es-es");

            Assert::AreEqual(L"es-ES", language.szId);
        }
    };
}
