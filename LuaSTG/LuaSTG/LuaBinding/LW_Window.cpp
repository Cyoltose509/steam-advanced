#include "LuaBinding/LuaWrapper.hpp"
#include "LuaBinding/lua_utility.hpp"
#include "AppFrame.h"

inline Core::Graphics::IWindow* _get_window()
{
    return LuaSTGPlus::AppFrame::GetInstance().GetAppModel()->getWindow();
}

#define getwindow(__NAME__) auto* __NAME__ = _get_window()

static int lib_setMouseEnable(lua_State* L)
{
    getwindow(window);
    bool const enable = lua_toboolean(L, 1);
    if (enable)
        window->setCursor(Core::Graphics::WindowCursor::Arrow);
    else
        window->setCursor(Core::Graphics::WindowCursor::None);
    return 0;
}
static int lib_setCursorStyle(lua_State* L)
{
    getwindow(window);
    Core::Graphics::WindowCursor const style = (Core::Graphics::WindowCursor)luaL_checkinteger(L, 1);
    window->setCursor(style);
    return 0;
}
static int lib_setCentered(lua_State* L)
{
    getwindow(window);
    if (lua_gettop(L) > 0)
    {
        uint32_t const index = (uint32_t)luaL_checkinteger(L, 1);
        window->setMonitorCentered(index);
    }
    else
    {
        window->setMonitorCentered(0);
    }
    return 0;
}

static int lib_setCustomMoveSizeEnable(lua_State* L)
{
    getwindow(window);
    window->setCustomSizeMoveEnable(lua_toboolean(L, 1));
    return 0;
}
static int lib_setCustomMinimizeButtonRect(lua_State* L)
{
    getwindow(window);
    window->setCustomMinimizeButtonRect(Core::RectI(
        (int32_t)luaL_checkinteger(L, 1),
        (int32_t)luaL_checkinteger(L, 2),
        (int32_t)luaL_checkinteger(L, 3),
        (int32_t)luaL_checkinteger(L, 4)
    ));
    return 0;
}
static int lib_setCustomCloseButtonRect(lua_State* L)
{
    getwindow(window);
    window->setCustomCloseButtonRect(Core::RectI(
        (int32_t)luaL_checkinteger(L, 1),
        (int32_t)luaL_checkinteger(L, 2),
        (int32_t)luaL_checkinteger(L, 3),
        (int32_t)luaL_checkinteger(L, 4)
    ));
    return 0;
}
static int lib_setCustomMoveButtonRect(lua_State* L)
{
    getwindow(window);
    window->setCustomMoveButtonRect(Core::RectI(
        (int32_t)luaL_checkinteger(L, 1),
        (int32_t)luaL_checkinteger(L, 2),
        (int32_t)luaL_checkinteger(L, 3),
        (int32_t)luaL_checkinteger(L, 4)
    ));
    return 0;
}

static int compat_SetSplash(lua_State* L)
{
    LAPP.SetSplash(lua_toboolean(L, 1));
    return 0;
}
static int compat_SetTitle(lua_State* L)
{
    LAPP.SetTitle(luaL_checkstring(L, 1));
    return 0;
}

#define makefname(__X__) { #__X__ , &lib_##__X__ }

static const luaL_Reg compat[] = {
    { "SetSplash", &compat_SetSplash },
    { "SetTitle" , &compat_SetTitle  },
    {NULL, NULL},
};

static const luaL_Reg lib[] = {
    makefname(setMouseEnable),
    makefname(setCursorStyle),
    makefname(setCentered),

    makefname(setCustomMoveSizeEnable),
    makefname(setCustomMinimizeButtonRect),
    makefname(setCustomCloseButtonRect),
    makefname(setCustomMoveButtonRect),
    
    {NULL, NULL},
};

void LuaSTGPlus::LuaWrapper::WindowWrapper::Register(lua_State* L)noexcept
{
    luaL_register(L, LUASTG_LUA_LIBNAME, compat); // ? t
    // Window
    lua_pushstring(L, "Window");                  // ? t k
    lua_newtable(L);                              // ? t k t
    luaL_register(L, NULL, lib);                  // ? t k t
    lua_settable(L, -3);                          // ? t
    lua_pop(L, 1);                                // ?
};
