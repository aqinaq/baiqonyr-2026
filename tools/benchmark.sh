#!/bin/sh
# Current 1280x720 frame-loop measurements. No video encoder required.
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
binary=${1:-"$root/build/baikonur"}
out=${2:-"$root/docs/evidence/current"}
case "$binary" in /*) ;; *) binary="$PWD/$binary" ;; esac
case "$out" in /*) ;; *) out="$PWD/$out" ;; esac
if [ ! -x "$binary" ]; then
    echo "Executable not found: $binary. Build with the README commands first." >&2
    exit 1
fi
mkdir -p "$out"
cd "$root"
{
    date -u '+Measured UTC: %Y-%m-%dT%H:%M:%SZ'
    uname -smr
    git rev-parse HEAD
    git status --short
} > "$out/environment.txt"
run_scene() {
    name=$1
    shift
    echo "Measuring $name at 1280x720..."
    "$binary" --width 1280 --height 720 --no-retina --no-vsync --duration 15 \
        --csv "$out/$name.csv" "$@" > "$out/$name.log" 2>&1 || {
        cat "$out/$name.log" >&2
        return 1
    }
    cat "$out/$name.log"
    if ! grep -q 'GL errors: 0' "$out/$name.log"; then
        echo "No successful frame summary for $name" >&2
        return 1
    fi
}
run_scene ready
run_scene ignition_and_ascent --launch --start-time -3
run_scene separation_and_stars --launch --start-time 40
printf 'Raw CSVs and logs saved to %s\n' "$out"
