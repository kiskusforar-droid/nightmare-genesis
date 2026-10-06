#!/usr/bin/env bash
set -eu

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SGDK_PATH="${SGDK_PATH:-/opt/sgdk}"

if [ ! -d "$SGDK_PATH" ]; then
  echo "SGDK not found at: $SGDK_PATH"
  echo "Set SGDK_PATH=/path/to/sgdk and run this script again."
  exit 1
fi

mkdir -p "$ROOT_DIR/bin"

make -C "$ROOT_DIR" SGDK="$SGDK_PATH"

for file in "$ROOT_DIR"/out/*.bin; do
  if [ -f "$file" ]; then
    cp "$file" "$ROOT_DIR/bin/"
  fi
done

if [ -d "$ROOT_DIR/bin" ]; then
  echo "ROM generated in: $ROOT_DIR/bin"
  ls -l "$ROOT_DIR/bin"
fi
