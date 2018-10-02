#!/usr/bin/env bash

set -e

##############################################################################
# Functions
##############################################################################

function usage()
{
  echo "usage: ${0} <channel> [version suffix]"
}

##############################################################################
# Validation
##############################################################################

if [[ $# -lt 1 ]]; then
  echo >&2 "${0}: error: too few arguments specified"
  usage
  exit 1
fi

if [[ $# -gt 2 ]]; then
  echo >&2 "${0}: error: too many arguments specified"
  usage
  exit 1
fi

##############################################################################
# Main
##############################################################################

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
repo_root=$(dirname $script_dir)

channel="$1"
if [[ $# -eq 2 ]]; then
  export BUILD_SUFFIX="$2"
fi

(
  cd "${repo_root}"
  # Deploy to conan
  conan user -p "${BINTRAY_API_KEY}" -r cppbits cppbits

  conan create . "cppbits/${channel}"
  conan upload "CppBitsCore/*" -r cppbits --all --confirm
)

