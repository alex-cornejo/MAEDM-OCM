#!/bin/bash
find . \
  \( -path "./.git" -o -path "./build" -o -path "./.vscode" -o -path "./.idea" \) -prune \
  -o \
  \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.cc" \) \
  ! -name "json.hpp" \
  -print0 | xargs -0 clang-format -i