#include <string.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#include "markdown.h"
#include "html.h"
#include "buffer.h"

#define SUNDOWN_LIBRARY "sundown"

static int sundown_markdown(lua_State *L) {
    const char *indoc;
    size_t indocSize;
    const char *outdoc;
    struct buf inbuf, *outbuf, *spbuf;
    struct mkd_renderer renderer;
    int targetSize;

    indoc = luaL_checklstring(L, 1, &indocSize);
    memset(&inbuf, 0x0, sizeof(struct buf));   
    inbuf.data = indoc;
    inbuf.size = indocSize;

    // Overhead guess inspired by Redcarpet.
    targetSize = (int)indocSize * 1.3f;
    outbuf = bufnew(128);
    bufgrow(outbuf, targetSize);

    sdhtml_renderer(&renderer, 0, NULL);
    sd_markdown(outbuf, &inbuf, &renderer, 0);

    // Smartypants -- should maybe make this optional, but it's
    // standard in most Markdown libraries so it's the default.
    spbuf = bufnew(128);
    bufgrow(spbuf, targetSize);
    sdhtml_smartypants(spbuf, outbuf);

    lua_pushlstring(L, spbuf->data, spbuf->size);
    
    bufrelease(outbuf);
    bufrelease(spbuf);
    sdhtml_free_renderer(&renderer);
    
    return 1;
}


static const struct luaL_reg sundown[] = {
    {"markdown", sundown_markdown},
    {NULL, NULL}
};

int luaopen_sundown(lua_State *L) {
    /* register library */
    luaL_register(L, SUNDOWN_LIBRARY, sundown);
    
    return 1;
}
