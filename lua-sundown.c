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
    struct buf inbuf, *outbuf;
    struct mkd_renderer renderer;

    indoc = luaL_checklstring(L, 1, &indocSize);
    memset(&inbuf, 0x0, sizeof(struct buf));   
    inbuf.data = indoc;
    inbuf.size = indocSize;

    // 20% HTML overhead guess inspired by Redcarpet.
    outbuf = bufnew(128);
    bufgrow(outbuf, indocSize * 1.2f);

    sdhtml_renderer(&renderer, 0, NULL);
    sd_markdown(outbuf, &inbuf, &renderer, 0);

    //FIXME smartypants?

    lua_pushlstring(L, outbuf->data, outbuf->size);
    
    bufrelease(outbuf);
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
