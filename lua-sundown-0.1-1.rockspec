package = "lua-sundown"
version = "0.1-1"
source = {
   url = "http://github.com/sampsyo/lua-sundown/tarball/master"
}
description = {
   summary = "Process Markdown safely and quickly.",
   detailed = [[
        Sundown is a C library for processing Markdown documents. This
        is a simple Lua binding for Sundown.
   ]],
   homepage = "http://github.com/sampsyo/lua-sundown/",
   license = "BSD"
}
dependencies = {
   "lua >= 5.1"
}
build = {
    type = "builtin",
    modules = {
        sundown = {
            sources = {
                "sundown/array.c",
                "sundown/autolink.c",
                "sundown/buffer.c",
                "sundown/html.c",
                "sundown/html_smartypants.c",
                "sundown/markdown.c",
                "lua-sundown.c"
            },
            incdirs = {"sundown"},
        },
    }
}
