let &makeprg="ninja -C build"
" let &makeprg="make -j8 -C ~/src/mmorpg2/build"
let &errorformat="%s ../%f:%l:%c: %m,../%f:%l:%c: %m,%s ../%f:%l: %m,../%f:%l: %m"
