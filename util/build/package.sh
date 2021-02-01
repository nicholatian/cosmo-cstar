#!/bin/sh
#-*-mode:sh;indent-tabs-mode:nil;tab-width:2;coding:utf-8-*-┐
#───vi: set net ft=sh ts=2 sts=2 fenc=utf-8 :vi─────────────┘

if [ "$TOOL_BUILD_PACKAGE" ]; then
  set -- "$TOOL_BUILD_PACKAGE" "$@"
else
  if [ -x "o/tool/build/package.com.dbg" ]; then
    set -- "o/tool/build/package.com.dbg" "$@"
  else
    MKDIR=${MKDIR:-$(command -v mkdir) -p} || exit
    CP=${CP:-$(command -v cp) -f} || exit
    if [ ! -x o/build/bootstrap/package.com ]; then
      $MKDIR o/build/bootstrap &&
        $CP -a build/bootstrap/package.com \
            o/build/bootstrap/package.com || exit
    fi
    set -- o/build/bootstrap/package.com "$@"
  fi
fi

printf "$LOGFMT" "${ACTION:-PACKAGE}" "$3" >&2
# if [ "$SILENT" = "0" ]; then
#   COLUMNS=${COLUMNS:-80}
#   COLUMNS=$((COLUMNS - 4))
#   printf "%s\n" "$*" |
#     /usr/bin/fold -s -w $COLUMNS |
#     sed -e '1bb' -e 's/^/  /' -e ':b' -e '$b' -e 's/$/ \\/' >&2
# else
#   printf "$LOGFMT" "${ACTION:-PACKAGE}" "$3" >&2
# fi

exec "$@"
