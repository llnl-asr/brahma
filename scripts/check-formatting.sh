#!/bin/bash
# Check (or, with --fix, apply) clang-format on every C/C++ file git tracks.
#
#   scripts/check-formatting.sh [--fix] [clang-format executable]
#
# CI pins the version below (pip install clang-format==<version>); another
# version can format differently, so the check warns when it is not the one.
# Generated interface files are formatted too: regenerating one means running
# this with --fix afterwards.
set -euo pipefail

SUPPORTED_VERSION="19.1.7"
FIX=0
if [ "${1:-}" = "--fix" ]; then
  FIX=1
  shift
fi
CLANG_FORMAT=${1:-clang-format}

command -v "$CLANG_FORMAT" >/dev/null 2>&1 || {
  echo "$CLANG_FORMAT not found; pip install clang-format==$SUPPORTED_VERSION" >&2
  exit 2
}
version=$("$CLANG_FORMAT" --version | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -1)
if [ "$version" != "$SUPPORTED_VERSION" ]; then
  echo "WARNING: clang-format $version; CI uses $SUPPORTED_VERSION and may disagree." >&2
fi

cd "$(git rev-parse --show-toplevel)"
mapfile -t files < <(git ls-files '*.c' '*.cc' '*.cpp' '*.h' '*.hpp')
if [ "$FIX" = 1 ]; then
  printf '%s\0' "${files[@]}" | xargs -0 -n 8 -P "$(nproc)" "$CLANG_FORMAT" -i
  echo "formatted ${#files[@]} files"
else
  printf '%s\0' "${files[@]}" | xargs -0 -n 8 -P "$(nproc)" "$CLANG_FORMAT" --dry-run --Werror
  echo "${#files[@]} files are formatted"
fi
