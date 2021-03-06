#include <string.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#include "markdown.h"
#include "html.h"
#include "buffer.h"

#define SUNDOWN_LIBRARY "sundown"
#define OVERHEAD_GUESS 1.3f

static int sundown_markdown(lua_State *L) {
    const char *indoc;
    size_t indocSize;
    struct buf *inbuf, *outbuf, *spbuf;
    struct mkd_renderer renderer;
    int targetSize;

    indoc = luaL_checklstring(L, 1, &indocSize);
    inbuf = bufnew(128);
    bufput(inbuf, indoc, indocSize);

    // Overhead guess inspired by Redcarpet.
    targetSize = (int)(indocSize * OVERHEAD_GUESS);
    outbuf = bufnew(128);
    bufgrow(outbuf, targetSize);

    sdhtml_renderer(&renderer, 0, NULL);
    sd_markdown(outbuf, inbuf, &renderer, 0);

    // Smartypants -- should maybe make this optional, but it's
    // standard in most Markdown libraries so it's the default.
    spbuf = bufnew(128);
    bufgrow(spbuf, targetSize);
    sdhtml_smartypants(spbuf, outbuf);

    lua_pushlstring(L, spbuf->data, spbuf->size);
    
    bufrelease(inbuf);
    bufrelease(outbuf);
    bufrelease(spbuf);
    sdhtml_free_renderer(&renderer);
    
    return 1;
}

// Register the library.
static const struct luaL_reg sundown[] = {
    {"markdown", sundown_markdown},
    {NULL, NULL}
};
int luaopen_sundown(lua_State *L) {
    luaL_register(L, SUNDOWN_LIBRARY, sundown);
    return 1;
}
