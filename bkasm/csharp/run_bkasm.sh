#!/bin/bash
DIR="$(dirname "$0")"
exec mono "$DIR/bkasm.exe" "$@"
