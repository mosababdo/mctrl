/*
 * mCtrl: Additional Win32 controls
 * <https://github.com/mity/mctrl>
 * <https://mctrl.org>
 *
 * Copyright (c) 2008-2020 Martin Mitas
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef MC_MISC_H
#define MC_MISC_H

#include <limits.h>
#include <malloc.h>
#include <stdio.h>
#include <stddef.h>
#include <tchar.h>
#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <vssym32.h>
#include <vsstyle.h>
#include <uxtheme.h>
#include <wingdi.h>

/*#include <windowsx.h>*/
/* This header is polluting namespace of macros too much. We are interested
 * only in macros GET_[XY]_LPARAM. */
#ifndef GET_X_LPARAM
    #define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
    #define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))
#endif

#include "compat.h"
#include "debug.h"
#include "resource.h"
#include "version.h"

#include "mCtrl/_defs.h"
#include "mCtrl/_common.h"


/****************************
 *** Miscellaneous Macros ***
 ****************************/

/* Whether we are Unicode build */
#ifdef UNICODE
    #define MC_IS_UNICODE      TRUE
#else
    #define MC_IS_UNICODE      FALSE
#endif

/* Minimum and maximum et al. */
#define MC_MIN(a,b)            ((a) < (b) ? (a) : (b))
#define MC_MAX(a,b)            ((a) > (b) ? (a) : (b))
#define MC_MIN3(a,b,c)         MC_MIN(MC_MIN((a), (b)), (c))
#define MC_MAX3(a,b,c)         MC_MAX(MC_MAX((a), (b)), (c))
#define MC_MID3(a,b,c)         MC_MIN(MC_MAX((a), (b)), (c))
#define MC_ABS(a)              ((a) >= 0 ? (a) : -(a))
#define MC_SIGN(a)             ((a) > 0 ? +1 : ((a) < 0 ? -1 : 0))

/* Get count of records in an array */
#define MC_SIZEOF_ARRAY(array)   (sizeof((array)) / sizeof((array)[0]))

/* Specifier of flexible array member size */
#if __STDC_VERSION__ >= 199901L  ||  defined MC_COMPILER_MSVC
    #define MC_ARRAY_FLEXIBLE_SIZE_SPEC       /* empty */
#elif defined MC_COMPILER_GCC
    #define MC_ARRAY_FLEXIBLE_SIZE_SPEC       0
#else
    #define MC_ARRAY_FLEXIBLE_SIZE_SPEC       1
#endif

/* Pi (float) */
#define MC_PIf     3.14159265358979323846f

/* Offset of structure member */
#if defined offsetof
    #define MC_OFFSETOF(type, member)   offsetof(type, member)
#elif defined MC_COMPILER_GCC  &&  MC_COMPILER_GCC >= 40000
    #define MC_OFFSETOF(type, member)   __builtin_offsetof(type, member)
#else
    #define MC_OFFSETOF(type, member)   ((size_t) &((type*)0)->member)
#endif

/* Pointer to container structure */
#define MC_CONTAINEROF(ptr, type, member)                                \
            ((type*)((BYTE*)(ptr) - MC_OFFSETOF(type, member)))


/* Macros telling the compiler that the condition is likely or unlikely to
 * be true. (If supported) it allows better optimization. Use only sparingly
 * in important loops where it really matters. Programmers are often bad in
 * this kind of prediction. */
#if defined MC_COMPILER_GCC  &&  MC_COMPILER_GCC >= 30000
    #define MC_LIKELY(condition)       __builtin_expect((condition) != 0, TRUE)
    #define MC_UNLIKELY(condition)     __builtin_expect((condition) != 0, FALSE)
#else
    #define MC_LIKELY(condition)       (condition)
    #define MC_UNLIKELY(condition)     (condition)
#endif

/* Macro for wrapping error conditions. */
#define MC_ERR(condition)              MC_UNLIKELY(condition)


/* Send and PostMessage() wrappers to save some typing and casting. */
#define MC_SEND(win, msg, wp, lp)                                         \
            SendMessage((win), (msg), (WPARAM)(wp), (LPARAM)(lp))
#define MC_POST(win, msg, wp, lp)                                         \
            PostMessage((win), (msg), (WPARAM)(wp), (LPARAM)(lp))


/***************************
 *** Miscellaneous Types ***
 ***************************/

typedef struct mc_point16_tag mc_point16_t;
struct mc_point16_tag {
    SHORT x;
    SHORT y;
};


/***************
 *** Globals ***
 ***************/

extern HINSTANCE mc_instance;
extern HINSTANCE mc_instance_kernel32;


/* Checking OS version (compare with normal operators: ==, <, <= etc.) */

#define MC_WIN_VER(major, minor, sp)                                      \
    (((major) << 16) | ((minor) << 8) | (sp) << 0)

#define MC_WIN_10          MC_WIN_VER(10, 0, 0)
#define MC_WIN_8_1         MC_WIN_VER(6, 3, 0)
#define MC_WIN_8           MC_WIN_VER(6, 2, 0)
#define MC_WIN_7_SP1       MC_WIN_VER(6, 1, 1)
#define MC_WIN_7           MC_WIN_VER(6, 1, 0)
#define MC_WIN_VISTA_SP2   MC_WIN_VER(6, 0, 2)
#define MC_WIN_VISTA_SP1   MC_WIN_VER(6, 0, 1)
#define MC_WIN_VISTA       MC_WIN_VER(6, 0, 0)
#define MC_WIN_XP64_SP2    MC_WIN_VER(5, 2, 2)
#define MC_WIN_XP64_SP1    MC_WIN_VER(5, 2, 1)
#define MC_WIN_XP64        MC_WIN_VER(5, 2, 0)
#define MC_WIN_XP_SP3      MC_WIN_VER(5, 1, 3)
#define MC_WIN_XP_SP2      MC_WIN_VER(5, 1, 2)
#define MC_WIN_XP_SP1      MC_WIN_VER(5, 1, 1)
#define MC_WIN_XP          MC_WIN_VER(5, 1, 0)
#define MC_WIN_2000_SP4    MC_WIN_VER(5, 0, 4)
#define MC_WIN_2000_SP3    MC_WIN_VER(5, 0, 3)
#define MC_WIN_2000_SP2    MC_WIN_VER(5, 0, 2)
#define MC_WIN_2000_SP1    MC_WIN_VER(5, 0, 1)
#define MC_WIN_2000        MC_WIN_VER(5, 0, 0)
#define MC_WIN_NT4_SP6     MC_WIN_VER(4, 0, 6)
#define MC_WIN_NT4_SP5     MC_WIN_VER(4, 0, 5)
#define MC_WIN_NT4_SP4     MC_WIN_VER(4, 0, 4)
#define MC_WIN_NT4_SP3     MC_WIN_VER(4, 0, 3)
#define MC_WIN_NT4_SP2     MC_WIN_VER(4, 0, 2)
#define MC_WIN_NT4_SP1     MC_WIN_VER(4, 0, 1)
#define MC_WIN_NT4         MC_WIN_VER(4, 0, 0)

/* Server editions */
#define MC_WIN_S2016       MC_WIN_10
#define MC_WIN_S2012R2     MC_WIN_8_1
#define MC_WIN_S2012       MC_WIN_8
#define MC_WIN_S2008R2_SP1 MC_WIN_7_SP1
#define MC_WIN_S2008R2     MC_WIN_7
#define MC_WIN_S2008_SP2   MC_WIN_VISTA_SP2
#define MC_WIN_S2008_SP1   MC_WIN_VISTA_SP1
#define MC_WIN_S2008       MC_WIN_VISTA
#define MC_WIN_S2003_SP2   MC_WIN_XP64_SP2
#define MC_WIN_S2003_SP1   MC_WIN_XP64_SP1
#define MC_WIN_S2003       MC_WIN_XP64

extern DWORD mc_win_version;


/* Checking version of COMCTRL32.DLL */

#define MC_DLL_VER(major, minor)                                          \
    (((major) << 16) | ((minor) << 0))

extern DWORD mc_comctl32_version;


/* Image list of glyphs used throughout mCtrl */

#define MC_BMP_GLYPH_W               9   /* glyph image size */
#define MC_BMP_GLYPH_H               9

#define MC_BMP_GLYPH_CLOSE           0   /* indexes of particular glyphs */
#define MC_BMP_GLYPH_MORE_OPTIONS    1
#define MC_BMP_GLYPH_CHEVRON_L       2
#define MC_BMP_GLYPH_CHEVRON_R       3
#define MC_BMP_GLYPH_EXPANDED        4
#define MC_BMP_GLYPH_COLLAPSED       5

extern HIMAGELIST mc_bmp_glyphs;


/**************************
 *** Light-Weight Mutex ***
 **************************/

/* Our mutex is implemented as a SRWLock (strictly with the exclusive locking).
 * This is more light-weight then CRITICAL_SECTION, and also fail-free.
 *
 * Beware: Our mutex is not recursive.
 */

typedef SRWLOCK mc_mutex_t;


#define MC_MUTEX_INIT       SRWLOCK_INIT


static inline void
mc_mutex_lock(mc_mutex_t* mutex)
{
    AcquireSRWLockExclusive(mutex);
}

static inline void
mc_mutex_unlock(mc_mutex_t* mutex)
{
    ReleaseSRWLockExclusive(mutex);
}


#if 0
    /* Enable this for debugging locking issues. */

    #define mc_mutex_lock(m)                                                \
            do {                                                            \
                MC_TRACE("LOCK(%p, %s:%d)", m, __FILE__, __LINE__);         \
                mc_mutex_lock(m);                                           \
            } while(0)

    #define mc_mutex_unlock(m)                                              \
            do {                                                            \
                MC_TRACE("UNLOCK(%p, %s:%d)", m, __FILE__, __LINE__);       \
                mc_mutex_unlock(m);                                         \
            } while(0)
#endif


/************************
 *** String Utilities ***
 ************************/

/* String type identifier. */

typedef enum mc_str_type_tag mc_str_type_t;
enum mc_str_type_tag {
    MC_STRA = 1,   /* Multibyte (ANSI) string */
    MC_STRW = 2,   /* Unicode (UTF-16LE) string */
#ifdef UNICODE
    MC_STRT = MC_STRW
#else
    MC_STRT = MC_STRA
#endif
};


/* Loading strings from resources */

const TCHAR* mc_str_load(UINT id);


/* Copying/converting strings into provided buffer */

void mc_str_inbuf_A2A(const char* restrict from_str, char* restrict to_str, int to_str_bufsize);
void mc_str_inbuf_W2W(const WCHAR* restrict from_str, WCHAR* restrict to_str, int to_str_bufsize);
void mc_str_inbuf_A2W(const char* restrict from_str, WCHAR* restrict to_str, int to_str_bufsize);
void mc_str_inbuf_W2A(const WCHAR* restrict from_str, char* restrict to_str, int to_str_bufsize);

static inline void
mc_str_inbuf(const void* restrict from_str, mc_str_type_t from_type,
             void* restrict to_str, mc_str_type_t to_type, int to_str_bufsize)
{
    if(from_type == to_type) {
        if(from_type == MC_STRA)
            mc_str_inbuf_A2A(from_str, to_str, to_str_bufsize);
        else
            mc_str_inbuf_W2W(from_str, to_str, to_str_bufsize);
    } else {
        if(from_type == MC_STRA)
            mc_str_inbuf_A2W(from_str, to_str, to_str_bufsize);
        else
            mc_str_inbuf_W2A(from_str, to_str, to_str_bufsize);
    }
}


/* Copying/converting strings into malloc'ed buffer */

char* mc_str_n_A2A(const char* restrict from_str, int from_len, int* restrict ptr_to_len);
WCHAR* mc_str_n_W2W(const WCHAR* restrict from_str, int from_len, int* restrict ptr_to_len);
WCHAR* mc_str_n_A2W(const char* restrict from_str, int from_len, int* restrict ptr_to_len);
char* mc_str_n_W2A(const WCHAR* restrict from_str, int from_len, int* restrict ptr_to_len);

static inline void*
mc_str_n(const void* restrict from_str, mc_str_type_t from_type, int from_len,
         mc_str_type_t to_type, int* restrict ptr_to_len)
{
    if(from_type == to_type) {
        if(from_type == MC_STRA)
            return mc_str_n_A2A(from_str, from_len, ptr_to_len);
        else
            return mc_str_n_W2W(from_str, from_len, ptr_to_len);
    } else {
        if(from_type == MC_STRA)
            return mc_str_n_A2W(from_str, from_len, ptr_to_len);
        else
            return mc_str_n_W2A(from_str, from_len, ptr_to_len);
    }
}

static inline char* mc_str_A2A(const char* from_str)
    { return mc_str_n_A2A(from_str, -1, NULL); }
static inline WCHAR* mc_str_W2W(const WCHAR* from_str)
    { return mc_str_n_W2W(from_str, -1, NULL); }
static inline WCHAR* mc_str_A2W(const char* from_str)
    { return mc_str_n_A2W(from_str, -1, NULL); }
static inline char* mc_str_W2A(const WCHAR* from_str)
    { return mc_str_n_W2A(from_str, -1, NULL); }

static inline void*
mc_str(const void* from_str, mc_str_type_t from_type, mc_str_type_t to_type)
{
    if(from_type == to_type) {
        if(from_type == MC_STRA)
            return mc_str_A2A(from_str);
        else
            return mc_str_W2W(from_str);
    } else {
        if(from_type == MC_STRA)
            return mc_str_A2W(from_str);
        else
            return mc_str_W2A(from_str);
    }
}


/***************************
 *** Loading System DLLs ***
 ***************************/

/* Safer way of loading system DLLs. */
HMODULE mc_load_sys_dll(const TCHAR* dll_name);


/*********************************
 *** Atomic Reference Counting ***
 *********************************/

#if defined MC_COMPILER_GCC  &&  MC_COMPILER_GCC >= 40100
    typedef int32_t mc_ref_t;
#else
    typedef LONG mc_ref_t;
#endif

static inline mc_ref_t
mc_ref(mc_ref_t* i)
{
#if defined MC_COMPILER_GCC  &&  MC_COMPILER_GCC >= 40700
    return __atomic_add_fetch(i, 1, __ATOMIC_RELAXED);
#elif defined MC_COMPILER_GCC  &&  MC_COMPILER_GCC >= 40100
    return __sync_add_and_fetch(i, 1);
#elif defined MC_COMPILER_MSVC  &&  MC_COMPILER_MSVC > 1200
    return _InterlockedIncrement(i);
#else
    return InterlockedIncrement(i);
#endif
}

static inline mc_ref_t
mc_unref(mc_ref_t* i)
{
#if defined MC_COMPILER_GCC  &&  MC_COMPILER_GCC >= 40700
    /* See http://stackoverflow.com/questions/10268737/ */
    mc_ref_t ref = __atomic_sub_fetch(i, 1, __ATOMIC_RELEASE);
    if(ref == 0)
        __atomic_thread_fence(__ATOMIC_ACQUIRE);
    return ref;
#elif defined MC_COMPILER_GCC  &&  MC_COMPILER_GCC >= 40100
    return __sync_sub_and_fetch(i, 1);
#elif defined MC_COMPILER_MSVC  &&  MC_COMPILER_MSVC > 1200
    return _InterlockedDecrement(i);
#else
    return InterlockedDecrement(i);
#endif
}


/**********************
 *** Rect Utilities ***
 **********************/

/* These are so trivial that inlining these is probably always better then
 * calling Win32API functions like InflateRect() etc. */

static inline LONG mc_rect_width(const RECT* r)
    { return (r->right - r->left); }
static inline LONG mc_rect_height(const RECT* r)
    { return (r->bottom - r->top); }

/* These are too common, so lets save typing. */
#define mc_width   mc_rect_width
#define mc_height  mc_rect_height

static inline BOOL mc_rect_is_empty(const RECT* r)
    { return (r->left >= r->right  ||  r->top >= r->bottom); }

static inline void mc_rect_set(RECT* r, LONG x0, LONG y0, LONG x1, LONG y1)
    { r->left = x0; r->top = y0; r->right = x1; r->bottom = y1; }
static inline void mc_rect_copy(RECT* r0, const RECT* r1)
    { mc_rect_set(r0, r1->left, r1->top, r1->right, r1->bottom); }

static inline void mc_rect_offset(RECT* r, LONG dx, LONG dy)
    { r->left += dx; r->top += dy; r->right += dx; r->bottom += dy; }
static inline void mc_rect_inflate(RECT* r, LONG dx, LONG dy)
    { r->left -= dx; r->top -= dy; r->right += dx; r->bottom += dy; }

static inline BOOL mc_rect_contains_xy(const RECT* r, LONG x, LONG y)
    { return (r->left <= x  &&  x < r->right  &&  r->top <= y  &&  y < r->bottom); }
static inline BOOL mc_rect_contains_pt(const RECT* r, const POINT* pt)
    { return mc_rect_contains_xy(r, pt->x, pt->y); }
static inline BOOL mc_rect_contains_pos(const RECT* r, DWORD pos)
    { return mc_rect_contains_xy(r, GET_X_LPARAM(pos), GET_Y_LPARAM(pos)); }
static inline BOOL mc_rect_contains_rect(const RECT* r0, const RECT* r1)
    { return (r0->left <= r1->left  &&  r1->right <= r0->right  &&
              r0->top <= r1->top  &&  r1->bottom <= r0->bottom); }
static inline BOOL mc_rect_overlaps_rect(const RECT* r0, const RECT* r1)
    { return (r0->left < r1->right  &&  r0->top < r1->bottom  &&
              r0->right > r1->left  &&  r0->bottom > r1->top); }


/**************************
 *** Clipping Utilities ***
 **************************/

HRGN mc_clip_get(HDC dc);
void mc_clip_set(HDC dc, LONG left, LONG top, LONG right, LONG bottom);
void mc_clip_reset(HDC dc, HRGN old_clip);


/*******************************
 *** Message Blocker Utility ***
 *******************************/

/* In some cases, controls may need to block certain auto-repeat from
 * happening. Imagine e.g. a button which triggers a popup menu. If user
 * clicks on the button for the 2nd time, it leads to:
 *
 * (1) canceling the popup menu (because it happened outside the popup menu
 *     window); and
 * (2) re-creation of the popup menu (because that's what the button does).
 *
 * But that's likely not what the user intended. He wanted to just cancel the
 * the popup menu. I.e. the control may want to block temporarily the (2) so
 * the single mouse click does only one action.
 *
 * mc_msgblocker_start() starts some short time period when any call to
 * mc_msgblocker_query() with the same window handle and id parameter shall
 * return FALSE. Otherwise it shall return TRUE. (The id can be used to
 * distinguish e.g. when a single control manages multiple popup menus. Use
 * zero if you do not need that.)
 *
 * Note only one blocker can be running at the same time. That's actually fine:
 * If multiple controls use a popup menu, opening different one means the
 * button for the former can be used for sure even without any blocking like
 * this.
 *
 * Hence, mc_msgblocker_start() invalidates any former blocker if there is an
 * active one.
 */
void mc_msgblocker_start(HWND win, UINT_PTR id);
BOOL mc_msgblocker_query(HWND win, UINT_PTR id);


/**************************
 *** Assorted Utilities ***
 **************************/

/* InitCommonControlsEx() wrapper. */
int mc_init_comctl32(DWORD icc);

/* Check whether the window uses right-to-left layout. */
BOOL mc_is_rtl_exstyle(DWORD exstyle);

/* Detect icon size */
void mc_icon_size(HICON icon, SIZE* size);
static inline UINT mc_icon_width(HICON icon)
    { SIZE s; mc_icon_size(icon, &s); return s.cx; }
static inline UINT mc_icon_height(HICON icon)
    { SIZE s; mc_icon_size(icon, &s); return s.cy; }

/* Determine approximate font on-screen dimension in pixels (height and avg.
 * char width). Used to auto-adjust item size in complex controls. */
void mc_font_size(HFONT font, SIZE* size, BOOL include_external_leading);

void mc_string_size(const TCHAR* str, HFONT font, SIZE* size);
static inline UINT mc_string_width(const TCHAR* str, HFONT font)
    { SIZE s; mc_string_size(str, font, &s); return s.cx; }
static inline UINT mc_string_height(const TCHAR* str, HFONT font)
    { SIZE s; mc_string_size(str, font, &s); return s.cy; }

/* Converting pixels <--> DLUs */
int mc_pixels_from_dlus(HFONT font, int dlus, BOOL vert);
int mc_dlus_from_pixels(HFONT font, int pixels, BOOL vert);

/* Send simple (i.e. using only NMHDR) notification */
static inline LRESULT
mc_send_notify(HWND parent, HWND win, UINT code)
{
    NMHDR hdr;

    hdr.hwndFrom = win;
    hdr.idFrom = GetWindowLong(win, GWL_ID);
    hdr.code = code;

    return MC_SEND(parent, WM_NOTIFY, hdr.idFrom, &hdr);
}

/* TrackMouseEvent() convenient wrapper */
static inline void
mc_track_mouse(HWND win, DWORD flags)
{
    TRACKMOUSEEVENT tme;

    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = flags;
    tme.hwndTrack = win;
    tme.dwHoverTime = HOVER_DEFAULT;

    TrackMouseEvent(&tme);
}


static inline uint32_t
mc_round_up_to_power_of_two_32(uint32_t val)
{
    val--;
    val |= val >> 1; val |= val >> 2; val |= val >> 4;
    val |= val >> 8; val |= val >> 16;
    val++;
    return val;
}

static inline uint32_t
mc_round_down_to_power_of_two_32(uint32_t val)
{
    val |= val >> 1; val |= val >> 2; val |= val >> 4;
    val |= val >> 8; val |= val >> 16;
    return (val + 1) >> 1;
}

static inline uint64_t
mc_round_up_to_power_of_two_64(uint64_t val)
{
    val--;
    val |= val >> 1; val |= val >> 2; val |= val >> 4;
    val |= val >> 8; val |= val >> 16; val |= val >> 32;
    val++;
    return val;
}

static inline uint64_t
mc_round_down_to_power_of_two_64(uint64_t val)
{
    val |= val >> 1; val |= val >> 2; val |= val >> 4;
    val |= val >> 8; val |= val >> 16; val |= val >> 32;
    return (val + 1) >> 1;
}


#endif  /* MC_MISC_H  */
