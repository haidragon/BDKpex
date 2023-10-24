#include "overlay.h"

GdiSelectBrush_t GdiSelectBrush = NULL;
PatBlt_t NtGdiPatBlt = NULL;
NtUserGetDC_t NtUserGetDC = NULL;
NtGdiCreateSolidBrush_t NtGdiCreateSolidBrush = NULL;
ReleaseDC_t NtUserReleaseDC = NULL;
DeleteObjectApp_t NtGdiDeleteObjectApp = NULL;
SelectBrush_t NtGdiSelectBrush = NULL;
/* ==== DRAW TEXT ==== */
SelectFont_t NtGdiSelectFont = NULL;
ExtTextOutW_t NtGdiExtTextOutW = NULL;
HfontCreate_t NtGdiHfontCreate = NULL;
/* ==== TEXT FINISH =====*/
BOOL initOverlay()
{
    GdiSelectBrush = (GdiSelectBrush_t)overlay::get_system_module_export(skCrypt(L"win32kfull.sys"), skCrypt("NtGdiSelectBrush"));
    NtGdiCreateSolidBrush = (NtGdiCreateSolidBrush_t)overlay::get_system_module_export(skCrypt(L"win32kfull.sys"), skCrypt("NtGdiCreateSolidBrush"));
    NtGdiPatBlt = (PatBlt_t)overlay::get_system_module_export(skCrypt(L"win32kfull.sys"), skCrypt("NtGdiPatBlt"));
    NtUserGetDC = (NtUserGetDC_t)overlay::get_system_module_export(skCrypt(L"win32kbase.sys"), skCrypt("NtUserGetDC"));
    NtUserReleaseDC = (ReleaseDC_t)overlay::get_system_module_export(skCrypt(L"win32kbase.sys"), skCrypt("NtUserReleaseDC"));
    NtGdiDeleteObjectApp = (DeleteObjectApp_t)overlay::get_system_module_export(skCrypt(L"win32kbase.sys"), skCrypt("NtGdiDeleteObjectApp"));
    NtGdiSelectBrush = (SelectBrush_t)overlay::get_system_module_export(skCrypt(L"win32kbase.sys"), skCrypt("GreSelectBrush"));
    /* ==== DRAW TEXT ==== */
    NtGdiSelectFont = (SelectFont_t)overlay::get_system_module_export(skCrypt(L"win32kfull.sys"), skCrypt("NtGdiSelectFont"));
    NtGdiExtTextOutW = (ExtTextOutW_t)overlay::get_system_module_export(skCrypt(L"win32kfull.sys"), skCrypt("NtGdiExtTextOutW"));
    NtGdiHfontCreate = (HfontCreate_t)overlay::get_system_module_export(skCrypt(L"win32kfull.sys"), skCrypt("hfontCreate"));
    /* ==== TEXT FINISH =====*/
    if (GdiSelectBrush && NtGdiCreateSolidBrush && NtGdiPatBlt 
        && NtUserGetDC && NtUserReleaseDC && NtGdiDeleteObjectApp
        && NtGdiSelectFont && NtGdiExtTextOutW && NtGdiHfontCreate
        )
    {
        return true;
    }
    else 
    {
        return false;
    }
}

namespace overlay
{
    PVOID getSystemRoutineAddress(PCWSTR routine_name)
    {
        UNICODE_STRING name;
        RtlInitUnicodeString(&name, routine_name);
        return MmGetSystemRoutineAddress(&name);
    }

    PVOID get_system_module_export(LPCWSTR module_name, LPCSTR routine_name)
    {
        PLIST_ENTRY module_list = reinterpret_cast<PLIST_ENTRY>(getSystemRoutineAddress(skCrypt(L"PsLoadedModuleList")));
        if (!module_list)
            return NULL;
        for (PLIST_ENTRY link = module_list; link != module_list->Blink; link = link->Flink)
        {
            overlayOnly::LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(link, overlayOnly::LDR_DATA_TABLE_ENTRY, InLoadOrderModuleList);
            UNICODE_STRING name;
            RtlInitUnicodeString(&name, module_name);
            if (RtlEqualUnicodeString(&entry->BaseDllName, &name, TRUE))
            {
                return (entry->DllBase) ? RtlFindExportedRoutineByName(entry->DllBase, routine_name) : NULL;

            }
        }
    }

    INT FrameRect(HDC hDC, CONST RECT* lprc, HBRUSH hbr, int thickness)
    {
        HBRUSH oldbrush;
        RECT r = *lprc;

        if (!(oldbrush = GdiSelectBrush(hDC, hbr))) return 0;

        NtGdiPatBlt(hDC, r.left, r.top, thickness, r.bottom - r.top, PATCOPY);
        NtGdiPatBlt(hDC, r.right - thickness, r.top, thickness, r.bottom - r.top, PATCOPY);
        NtGdiPatBlt(hDC, r.left, r.top, r.right - r.left, thickness, PATCOPY);
        NtGdiPatBlt(hDC, r.left, r.bottom - thickness, r.right - r.left, thickness, PATCOPY);
        if (oldbrush)
        {
            GdiSelectBrush(hDC, oldbrush);
        }
        return TRUE;
    }

    NTSTATUS DrawBox(int r, int g, int b, int x, int y, int w, int h, int t) //int r, g, b, x, y, w, h, t;
    {
        HDC hdc = NtUserGetDC(NULL);
        if (!hdc)
            return STATUS_UNSUCCESSFUL;
        HBRUSH brush = NtGdiCreateSolidBrush(RGB(r, g, b), NULL);
        if (!brush)
            return STATUS_UNSUCCESSFUL;
        RECT rect = { x, y, x + w, y + h };
        FrameRect(hdc, &rect, brush, t);
        NtUserReleaseDC(hdc);
        NtGdiDeleteObjectApp(brush);
        return STATUS_SUCCESS;
    }

    NTSTATUS DrawMultipleBox(BoxInfo MBoxInfo[15], DWORD BoxAmount)
    {
        HDC hdc = NtUserGetDC(NULL);
        if (!hdc)
            return STATUS_UNSUCCESSFUL;
        
        for (int i = 0; i < BoxAmount; i++) /* Draw 15 Box Maxium */
        {
            if (MBoxInfo[i].T <= 0)
            {
                print(skCrypt("[ERROR] Draw Box Thickness Less than Zero"));
                continue;
            }
            
            HBRUSH Brush = NtGdiCreateSolidBrush(RGB(MBoxInfo[i].R, MBoxInfo[i].G, MBoxInfo[i].B), NULL);
            if (!Brush)
                return STATUS_UNSUCCESSFUL;

            RECT rect = { MBoxInfo[i].X , MBoxInfo[i].Y, MBoxInfo[i].X + MBoxInfo[i].W, MBoxInfo[i].Y + MBoxInfo[i].H };
            FrameRect(hdc, &rect, Brush, MBoxInfo[i].T);
            NtGdiDeleteObjectApp(Brush);
        }
        NtUserReleaseDC(hdc);
        return STATUS_SUCCESS;
    }

    NTSTATUS DrawText(DrawStringInfo stringInfo)
    {
        HDC hdc = NtUserGetDC(NULL);
        if (!hdc) return STATUS_UNSUCCESSFUL;
        HBRUSH Brush = NtGdiCreateSolidBrush(RGB(100, 0, 200), NULL);
        if (!Brush)
            return STATUS_UNSUCCESSFUL;
        HFONT font = TextString::CreateFontW_(stringInfo.size, 0, 0, 0, FW_NORMAL, 0, 0, 0, CHINESEBIG5_CHARSET, 
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, L"Arial");
        HFONT oldfont = NtGdiSelectFont(hdc, font);
        bool textSuccess = TextString::extTextOutW_(hdc, stringInfo.X, stringInfo.Y, 0, NULL, stringInfo.string, wcslen(stringInfo.string), 0);
        NtGdiDeleteObjectApp(Brush);
        NtUserReleaseDC(hdc);
        NtGdiDeleteObjectApp(font);
    }

    BOOL FillRect(HDC hDC, int x, int y, int w, int h, HBRUSH hbr)
    {
        BOOL Ret;
        HBRUSH prevhbr = NULL;

        RECT lprc = { x, y, x + w, y + h };
        prevhbr = NtGdiSelectBrush(hDC, hbr);
        Ret = NtGdiPatBlt(hDC, lprc.left, lprc.top, lprc.right - lprc.left,
            lprc.bottom - lprc.top, PATCOPY);
        if (prevhbr)
            NtGdiSelectBrush(hDC, prevhbr);
        return Ret;
    }

    BOOL DrawHealthBar(BoxInfo BoxInfo, DWORD MaxHealth, DWORD CurrentHealth, int BarR, int BarG, int BarB)
    {
        HDC hdc = NtUserGetDC(NULL);
        if (!hdc)
            return STATUS_UNSUCCESSFUL;
        HBRUSH brush = NtGdiCreateSolidBrush(RGB(BoxInfo.R, BoxInfo.G, BoxInfo.B), NULL);
        if (!brush)
            return STATUS_UNSUCCESSFUL;
        RECT rect = { BoxInfo.X, BoxInfo.Y, BoxInfo.X + BoxInfo.W, BoxInfo.Y + BoxInfo.H };
        FrameRect(hdc, &rect, brush, BoxInfo.T); // draw out line
        NtGdiDeleteObjectApp(brush);
        HBRUSH BarBrush = NtGdiCreateSolidBrush(RGB(BarR, BarG, BarB), NULL);
        HBRUSH prevhbr = NULL;  
        prevhbr = NtGdiSelectBrush(hdc, BarBrush);
        RECT lprc = { BoxInfo.X + BoxInfo.T, BoxInfo.Y + BoxInfo.T, BoxInfo.X + (int)(BoxInfo.W * CurrentHealth / MaxHealth) - BoxInfo.T, BoxInfo.Y + BoxInfo.H - BoxInfo.T };
        NtGdiPatBlt(hdc, lprc.left, lprc.top, lprc.right - lprc.left, lprc.bottom - lprc.top, PATCOPY);
        if (prevhbr)
            NtGdiSelectBrush(hdc, prevhbr);
        NtUserReleaseDC(hdc);
        NtGdiDeleteObjectApp(BarBrush);
        return STATUS_SUCCESS;
    }
}

namespace CompositeOverlay
{

    BOOL Draw2(LPWSTR ConsoleMessage[5])
    {
        HDC hdc = NtUserGetDC(NULL);
        if (!hdc)
            return false;
        HFONT font = TextString::CreateFontW_(40 /* size */ , 0, 0, 0, FW_NORMAL, 0, 0, 0, CHINESEBIG5_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, L"Arial");
        HFONT oldfont = NtGdiSelectFont(hdc, font);
        
        for (int i = 0; i < 5; i++)
        {
            if (!ConsoleMessage[i]) /* You're dereferencing a null pointer~*/
                continue;
            if (!wcslen(ConsoleMessage[i]))
                continue;
            TextString::extTextOutW_(hdc, 0, i*40 /* y-axis */ , 0, NULL, ConsoleMessage[i], wcslen(ConsoleMessage[i]), 0);
        }
        NtGdiDeleteObjectApp(font);
        NtGdiDeleteObjectApp(oldfont);
        NtUserReleaseDC(hdc);
    }

    BOOL Draw(DrawHealthBarInfo DHBInfo[20], DWORD BarAmount) /* required: BoxList, HealhBarList*/
    {
        HDC hdc = NtUserGetDC(NULL);
        if (!hdc)
            return STATUS_UNSUCCESSFUL;
        HBRUSH brush = NtGdiCreateSolidBrush(RGB(DHBInfo[0].R, DHBInfo[0].G, DHBInfo[0].B), NULL); // Set Default Colour for health bonding Box
        HBRUSH BarBrush = NtGdiCreateSolidBrush(RGB(DHBInfo[0].BarR, DHBInfo[0].BarG, DHBInfo[0].BarB), NULL); // Set Default Colour for Filler
        if (!brush)
            return STATUS_UNSUCCESSFUL;
        RECT rect;
        for (int i = 0; i < BarAmount; i++)
        {
            rect = { DHBInfo[i].X, DHBInfo[i].Y, DHBInfo[i].X + DHBInfo[i].W, DHBInfo[i].Y + DHBInfo[i].H };
            overlay::FrameRect(hdc, &rect, brush, DHBInfo[i].T);

        } /* Draw All the box frames */
        HBRUSH prevhbr = NULL;
        prevhbr = NtGdiSelectBrush(hdc, BarBrush);
        for (int i = 0; i < BarAmount; i++)
        {
            RECT lprc = { DHBInfo[i].X + DHBInfo[i].T, DHBInfo[i].Y + DHBInfo[i].T, DHBInfo[i].X 
                + (int)(DHBInfo[i].W * DHBInfo[i].CurrentHealth / DHBInfo[i].MaxiumHealth) 
                - DHBInfo[i].T, DHBInfo[i].Y + DHBInfo[i].H - DHBInfo[i].T };
            NtGdiPatBlt(hdc, lprc.left, lprc.top, lprc.right - lprc.left, lprc.bottom - lprc.top, PATCOPY);
        }
        if (prevhbr)
            NtGdiSelectBrush(hdc, prevhbr);
        NtUserReleaseDC(hdc);
        NtGdiDeleteObjectApp(BarBrush);
        NtGdiDeleteObjectApp(brush);
        NtGdiDeleteObjectApp(prevhbr);
        return true;
    }
}


namespace ModularOverlay
{
    BOOL drawBox(HDC hdc, int r, int g, int b, int x, int y, int w, int h, int t)
    {
        if (!hdc)
            return false;
        HBRUSH brush = NtGdiCreateSolidBrush(RGB(r, g, b), NULL);
        if (!brush)
            return false;
        RECT rect = { x, y, x + w, y + h };
        overlay::FrameRect(hdc, &rect, brush, t);
        NtGdiDeleteObjectApp(brush);
        return true;
    }

    BOOL drawFillBox(HDC hdc, int r, int g, int b, int x, int y, int w, int h, int t)
    {
        if (!hdc)
            return false;
        if (!t)
            return false;
        HBRUSH brush = NtGdiCreateSolidBrush(RGB(r, g, b), NULL);
        if (!brush)
            return false;
        HBRUSH prevhbr = NtGdiSelectBrush(hdc, brush);
        RECT lprc = { x, y, x + w, y + h };
        BOOL ret = NtGdiPatBlt(hdc, lprc.left, lprc.top, lprc.right - lprc.left,
            lprc.bottom - lprc.top, PATCOPY);
        if (prevhbr)
            NtGdiSelectBrush(hdc, prevhbr);
        NtGdiDeleteObjectApp(brush);
        return ret;

    }
    /*
      BOOL FillRect(HDC hDC, int x, int y, int w, int h, HBRUSH hbr)
    {
        BOOL Ret;
        HBRUSH prevhbr = NULL;

        RECT lprc = { x, y, x + w, y + h };
        prevhbr = NtGdiSelectBrush(hDC, hbr);
        Ret = NtGdiPatBlt(hDC, lprc.left, lprc.top, lprc.right - lprc.left,
            lprc.bottom - lprc.top, PATCOPY);
        if (prevhbr)
            NtGdiSelectBrush(hDC, prevhbr);
        return Ret;
    }
    */
    BOOL drawBar(HDC hdc, BoxInfo BoxInfo, DWORD MaxHealth, DWORD CurrentHealth, int R, int G, int B)
    {
        if (!hdc)
            return false;
        HBRUSH brush = NtGdiCreateSolidBrush(RGB(BoxInfo.R, BoxInfo.G, BoxInfo.B), NULL);
        if (!brush)
            return false;
        RECT rect = { BoxInfo.X, BoxInfo.Y, BoxInfo.X + BoxInfo.W, BoxInfo.Y + BoxInfo.H };
        overlay::FrameRect(hdc, &rect, brush, BoxInfo.T); // draw out line
        NtGdiDeleteObjectApp(brush);
        HBRUSH BarBrush = NtGdiCreateSolidBrush(RGB(R, G, B), NULL);
        HBRUSH prevhbr = NULL;
        prevhbr = NtGdiSelectBrush(hdc, BarBrush);
        RECT lprc = { BoxInfo.X + BoxInfo.T, BoxInfo.Y + BoxInfo.T, BoxInfo.X + (int)(BoxInfo.W * CurrentHealth / MaxHealth) - BoxInfo.T, BoxInfo.Y + BoxInfo.H - BoxInfo.T };
        NtGdiPatBlt(hdc, lprc.left, lprc.top, lprc.right - lprc.left, lprc.bottom - lprc.top, PATCOPY);
        if (prevhbr)
            NtGdiSelectBrush(hdc, prevhbr);
        NtGdiDeleteObjectApp(BarBrush);
        return STATUS_SUCCESS;
    }
    BOOL drawText(HDC hdc, LPWSTR string, int x, int y, int Size)
    {
        if (!hdc) return false;
        if (!string)
            return false;
        if (!wcslen(string))
            return false;
        HBRUSH Brush = NtGdiCreateSolidBrush(RGB(100, 0, 200), NULL);
        if (!Brush)
            return false;
        HFONT font = TextString::CreateFontW_(Size, 0, 0, 0, FW_NORMAL, 0, 0, 0, CHINESEBIG5_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, skCrypt(L"Arial"));
        HFONT oldfont = NtGdiSelectFont(hdc, font);
        bool textSuccess = TextString::extTextOutW_(hdc, x, y, 0, NULL, string, wcslen(string), 0);
        NtGdiDeleteObjectApp(Brush);
        NtGdiDeleteObjectApp(font);
        NtGdiDeleteObjectApp(oldfont);
    }
    HDC GetHDC()
    {
        return NtUserGetDC(NULL);
    }
    BOOL ReleaseHDC(HDC hdc) /* handle to device context */
    {
        NtUserReleaseDC(hdc);
        return true;
    }
}
namespace TextString
{
    HFONT
        CreateFontIndirectExW_(const ENUMLOGFONTEXDVW* elfexd)
    {
        /* Msdn: Note, this function ignores the elfDesignVector member in
                 ENUMLOGFONTEXDV.
         */
        if (elfexd)
        {
            return NtGdiHfontCreate((PENUMLOGFONTEXDVW)elfexd, 0, 0, 0, NULL);
        }
        else {
            print(skCrypt("[-] CreateFontIndirectExW elfexd is missing"));
            return NULL;
        }
    }

    HFONT CreateFontIndirectW_(CONST LOGFONTW* lplf)
    {
        if (lplf)
        {
            ENUMLOGFONTEXDVW Logfont;

            RtlCopyMemory(&Logfont.elfEnumLogfontEx.elfLogFont, lplf, sizeof(LOGFONTW));
            // Need something other than just cleaning memory here.
            // Guess? Use caller data to determine the rest.
            RtlZeroMemory(&Logfont.elfEnumLogfontEx.elfFullName,
                sizeof(Logfont.elfEnumLogfontEx.elfFullName));
            RtlZeroMemory(&Logfont.elfEnumLogfontEx.elfStyle,
                sizeof(Logfont.elfEnumLogfontEx.elfStyle));
            RtlZeroMemory(&Logfont.elfEnumLogfontEx.elfScript,
                sizeof(Logfont.elfEnumLogfontEx.elfScript));

            Logfont.elfDesignVector.dvNumAxes = 0; // No more than MM_MAX_NUMAXES

            RtlZeroMemory(&Logfont.elfDesignVector, sizeof(DESIGNVECTOR));

            return CreateFontIndirectExW_(&Logfont);
        }
        else {
            print(skCrypt("[-] CreateFontIndirectW lplf is missing"));
            return NULL;
        }
    }

    HFONT CreateFontW_(int nHeight,
        int nWidth,
        int nEscapement,
        int nOrientation,
        int nWeight,
        DWORD   fnItalic,
        DWORD   fdwUnderline,
        DWORD   fdwStrikeOut,
        DWORD   fdwCharSet,
        DWORD   fdwOutputPrecision,
        DWORD   fdwClipPrecision,
        DWORD   fdwQuality,
        DWORD   fdwPitchAndFamily,
        LPCWSTR lpszFace) {
        LOGFONTW logfont;
        logfont.lfHeight = nHeight;
        logfont.lfWidth = nWidth;
        logfont.lfEscapement = nEscapement;
        logfont.lfOrientation = nOrientation;
        logfont.lfWeight = nWeight;
        logfont.lfItalic = (BYTE)fnItalic;
        logfont.lfUnderline = (BYTE)fdwUnderline;
        logfont.lfStrikeOut = (BYTE)fdwStrikeOut;
        logfont.lfCharSet = (BYTE)fdwCharSet;
        logfont.lfOutPrecision = (BYTE)fdwOutputPrecision;
        logfont.lfClipPrecision = (BYTE)fdwClipPrecision;
        logfont.lfQuality = (BYTE)fdwQuality;
        logfont.lfPitchAndFamily = (BYTE)fdwPitchAndFamily;
        if (NULL != lpszFace)
        {
            int Size = sizeof(logfont.lfFaceName) / sizeof(WCHAR);
            wcsncpy((wchar_t*)logfont.lfFaceName, lpszFace, Size - 1);
            /* Be 101% sure to have '\0' at end of string */
            logfont.lfFaceName[Size - 1] = '\0';
        }
        else
        {
            logfont.lfFaceName[0] = L'\0';
        }
        return CreateFontIndirectW_(&logfont);
    }

    PVOID AllocateVirtualMemory(SIZE_T Size)
    {
        PVOID pMem = NULL;
        NTSTATUS statusAlloc = ZwAllocateVirtualMemory(NtCurrentProcess(), &pMem, 0, &Size, MEM_COMMIT, PAGE_READWRITE);
        //print("[+] AllocateVirtualMemory statusAlloc: %x \n", statusAlloc);
        return pMem;
    }

    VOID FreeVirtualMemory(PVOID VirtualAddress, SIZE_T Size)
    {
        if (MmIsAddressValid(VirtualAddress))
        {
            NTSTATUS Status = ZwFreeVirtualMemory(NtCurrentProcess(), &VirtualAddress, &Size, MEM_RELEASE);

            if (!NT_SUCCESS(Status)) {
                print(skCrypt("[-] GDI.cpp Warning : Released memory failed.FreeVirtualMemory Internal Function\r\n"));
            }
            return;
        }
        print(skCrypt("[-] GDI.cpp Warning: Released memory does not exist.FreeVirtualMemory Internal Function\r\n"));
    }

    BOOL extTextOutW_(HDC hdc, INT x, INT y, UINT fuOptions, RECT* lprc, LPWSTR lpString, UINT cwc, INT* lpDx)
    {
        BOOL		nRet = FALSE;
        PVOID       local_lpString = NULL;
        RECT* local_lprc = NULL;
        INT* local_lpDx = NULL;

        if (lprc != NULL)
        {
            SIZE_T Len = sizeof(RECT);
            local_lprc = (RECT*)AllocateVirtualMemory(Len);
            if (local_lprc != NULL)
            {
                __try
                {
                    RtlZeroMemory(local_lprc, Len);
                    RtlCopyMemory(local_lprc, lprc, Len);
                }
                __except (1)
                {
                    print(skCrypt("GDI.cpp Line RtlCopyMemory  Triggers An Error.ExtTextOutW Internal Function\r\n"));
                    goto $EXIT;
                }
            }
            else
            {
                print(skCrypt("GDI.cpp Line local_lprc = null  Triggers An Error.ExtTextOutW Internal Function\r\n"));
                goto $EXIT;
            }
        }

        if (cwc != 0)
        {
            SIZE_T     AllocSize = sizeof(WCHAR) * cwc + 1;
            local_lpString = AllocateVirtualMemory(AllocSize);

            if (local_lpString != NULL)
            {
                __try
                {
                    RtlZeroMemory(local_lpString, AllocSize);
                    RtlCopyMemory(local_lpString, lpString, AllocSize);
                }
                __except (1)
                {
                    print(skCrypt("[-] GDI.cpp Line RtlCopyMemory  Triggers An Error.ExtTextOutW Internal Function\r\n"));
                    goto $EXIT;
                }
            }
            else
            {
                print(skCrypt("[-] GDI.cpp Line local_lpString = null  Triggers An Error.ExtTextOutW Internal Function\r\n"));
                goto $EXIT;
            }
        }

        if (local_lpDx != NULL)
        {
            SIZE_T     AllocSize = sizeof(INT);
            local_lpDx = (INT*)AllocateVirtualMemory(AllocSize);
            if (local_lpDx != NULL)
            {
                __try
                {
                    RtlZeroMemory(local_lpString, AllocSize);
                    *local_lpDx = *lpDx;
                }
                __except (1)
                {
                    print(skCrypt("[-] GDI.cpp Line RtlCopyMemory  Triggers An Error.ExtTextOutW Internal Function\r\n"));
                    goto $EXIT;
                }
            }
            else
            {
                print(skCrypt("[-] GDI.cpp Line local_lpDx = null  Triggers An Error.ExtTextOutW Internal Function\r\n"));
            }
        }

        if (NtGdiExtTextOutW != NULL) {
            nRet = NtGdiExtTextOutW(hdc, x, y, fuOptions, local_lprc, (LPWSTR)local_lpString, cwc, local_lpDx, 0);
        }
        else {
            print(skCrypt("[-] GDI.cpp Line NtGdiExtTextOutW = NULL Triggers An Error.TextOutW Internal Function\r\n"));
        }
    $EXIT:
        if (lprc != NULL)
        {
            FreeVirtualMemory(lprc, sizeof(RECT));
            lprc = NULL;
        }

        if (local_lpDx != NULL)
        {
            FreeVirtualMemory(local_lpDx, sizeof(INT));
            local_lpDx = NULL;
        }

        if (local_lpString != NULL)
        {
            FreeVirtualMemory(local_lpString, cwc);
            local_lpString = NULL;
        }

        return nRet;
    }


}

