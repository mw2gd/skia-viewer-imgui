#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GN_BIN="${ROOT_DIR}/.deps/bin/gn"
SKIA_DIR="${ROOT_DIR}/ext/skia"
OPENGL_OUT_DIR="${SKIA_DIR}/out/opengl"
TARGET="${1:-viewer}"

if [[ ! -x "${GN_BIN}" ]]; then
  printf 'GN was not found at %s\n' "${GN_BIN}" >&2
  printf 'Run ./deps.sh first, then rerun ./build.sh.\n' >&2
  exit 1
fi

if ! command -v ninja >/dev/null 2>&1; then
  printf 'ninja was not found on PATH.\n' >&2
  printf 'Run ./deps.sh first, then rerun ./build.sh.\n' >&2
  exit 1
fi

# Replace Skia's build file only when a repo-specific override exists.
if [[ -f "${ROOT_DIR}/APPBUILD.gn" ]]; then
  cp "${ROOT_DIR}/APPBUILD.gn" "${SKIA_DIR}/BUILD.gn"
fi

# Generate and build the desktop Skia output.
cd "${SKIA_DIR}"
"${GN_BIN}" gen "${OPENGL_OUT_DIR}" --args='
is_debug=false
is_official_build=false
skia_enable_tools=true
skia_use_gl=true
skia_use_metal=true
skia_enable_gpu=true
skia_enable_ganesh=true
skia_enable_svg=false
skia_use_system_libjpeg_turbo=false
skia_use_system_libpng=false 
skia_use_system_libwebp=false
skia_use_system_zlib=false
skia_use_system_icu=false
skia_use_system_harfbuzz=false
skia_use_system_expat=false
'

if [[ ! -e "${SKIA_DIR}/third_party/externals/icu/scripts/make_data_assembly.py" ]]; then
  printf 'Skia third-party dependencies are incomplete: ICU is missing.\n' >&2
  printf 'Run ./deps.sh first, then rerun ./build.sh %s.\n' "${TARGET}" >&2
  exit 1
fi

ninja -C "${OPENGL_OUT_DIR}" "${TARGET}"

printf '\nBuilt target: %s\n' "${TARGET}"
if [[ -x "${OPENGL_OUT_DIR}/${TARGET}" ]]; then
  printf 'Executable: %s/%s\n' "${OPENGL_OUT_DIR}" "${TARGET}"
fi
