#!/bin/bash

# ==============================================================================
# TS-M1N3 — Main Build Wrapper (delegates to _build-mac.sh)
# Ramer Digital 2026
# ==============================================================================

set -e
cd "$(dirname "$0")"
exec ./_build-mac.sh "$@"
