#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DEPS_DIR="${ROOT_DIR}/.deps"
BIN_DIR="${DEPS_DIR}/bin"
GN_DIR="${DEPS_DIR}/gn"
GN_REPO="https://gn.googlesource.com/gn"
GN_REVISION="${GN_REVISION:-b2afae122eeb6ce09c52d63f67dc53fc517dbdc8}"

log() {
  printf '\n==> %s\n' "$*"
}

require_command() {
  if ! command -v "$1" >/dev/null 2>&1; then
    printf 'Missing required command: %s\n' "$1" >&2
    exit 1
  fi
}

install_brew_package() {
  local package="$1"

  if brew list --versions "${package}" >/dev/null 2>&1; then
    printf '%s already installed\n' "${package}"
  else
    brew install "${package}"
  fi
}

if [[ "$(uname -s)" != "Darwin" ]]; then
  printf 'This dependency script is currently intended for macOS builds.\n' >&2
  exit 1
fi

log "Checking Xcode command line tools"
if ! xcode-select -p >/dev/null 2>&1; then
  printf 'Xcode command line tools are required. Run: xcode-select --install\n' >&2
  exit 1
fi

log "Checking Homebrew"
if ! command -v brew >/dev/null 2>&1; then
  printf 'Homebrew is required to install build prerequisites.\n' >&2
  printf 'Install it from https://brew.sh, then rerun ./deps.sh.\n' >&2
  exit 1
fi

log "Installing build prerequisites"
install_brew_package cmake
install_brew_package ninja
install_brew_package pkg-config
install_brew_package glew

require_command git
require_command python3
require_command ninja

log "Syncing git submodules"
git -C "${ROOT_DIR}" submodule update --init --recursive

log "Syncing Skia third-party dependencies"
(
  cd "${ROOT_DIR}/ext/skia"
  python3 tools/git-sync-deps
)

log "Fetching GN"
mkdir -p "${DEPS_DIR}" "${BIN_DIR}"
if [[ -d "${GN_DIR}/.git" ]]; then
  git -C "${GN_DIR}" fetch --prune
else
  rm -rf "${GN_DIR}"
  git clone "${GN_REPO}" "${GN_DIR}"
fi
git -C "${GN_DIR}" checkout --detach "${GN_REVISION}"

log "Building GN"
(
  cd "${GN_DIR}"
  python3 build/gen.py
  ninja -C out gn
)

if [[ ! -x "${GN_DIR}/out/gn" ]]; then
  printf 'GN build finished but no executable was found at %s\n' "${GN_DIR}/out/gn" >&2
  exit 1
fi

ln -sf "${GN_DIR}/out/gn" "${BIN_DIR}/gn"

log "Done"
printf 'GN is available at %s\n' "${BIN_DIR}/gn"
printf 'For this shell, run: export PATH="%s:$PATH"\n' "${BIN_DIR}"
