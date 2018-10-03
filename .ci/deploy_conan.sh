#!/usr/bin/env bash

set -e

##############################################################################
# Functions
##############################################################################

function usage()
{
  echo "usage: ${0} [stable|edge|testing]"
  echo " - stable:  for master branch. Every merge must be unique version."
  echo " - edge:    for release branches. Suffixed by number of merges to release not on develop."
  echo " - testing: for develop branch. Suffixed by number of merges to develop not on master."
}

##############################################################################
# Validation
##############################################################################

if [[ $# -lt 1 ]]; then
  echo >&2 "${0}: error: too few arguments specified"
  usage
  exit 1
fi

if [[ $# -gt 1 ]]; then
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

if [[ "$1" == "stable" ]]; then
  :
elif [[ "$1" == "edge" ]]; then
  # First release branch will always have 0 merges (from the initial push)
  count=$(git rev-list --count --merges HEAD ^develop)
  count=$((count+1))
  export BUILD_SUFFIX="beta.${count}"
elif [[ "$1" == "testing" ]]; then
  hash=$(git rev-parse HEAD)
  count=$(git rev-list --count --merges HEAD ^master)
  count=$((count+1))
  export BUILD_SUFFIX="alpha.${count}+${hash}"
else
  echo >&2 "${0}: error: 1st positional argument must be 1 of 'stable', 'edge', or 'testing'"
  usage
  exit 1
fi

(

  cd "${repo_root}"
  # Deploy to conan
  conan user -p "${BINTRAY_API_KEY}" -r cppbits cppbits

  conan create . "cppbits/${channel}"
  conan upload "CppBitsCore/*" -r cppbits --all --confirm
)

