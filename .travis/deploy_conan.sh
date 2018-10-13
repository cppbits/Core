#!/usr/bin/env bash

set -e

##############################################################################
# Functions
##############################################################################

function usage()
{
  echo "usage: ${0} [stable|prerelease|edge]"
  echo " - stable:  for master branch. Every merge must be unique version."
  echo " - prerelease: for develop branch. Suffixed by number of merges to develop not on master."
  echo " - edge:    for release branches. Suffixed by number of merges to release not on develop."
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

if [[ "$1" == "stable" ]]; then
  channel="stable"
elif [[ "$1" == "prerelease" ]]; then
  channel="stable"
  # First release branch will always have 0 merges (from the initial push)
  count=$(git rev-list --count --merges HEAD ^develop)
  count=$((count+1))
  export CONAN_VERSION_SUFFIX="prerelease.${count}"
elif [[ "$1" == "edge" ]]; then
  channel="testing"
  hash=$(git rev-parse HEAD)
  # First release branch will always have 0 merges (from the initial push)
  count=$(git rev-list --count --merges HEAD ^master)
  count=$((count+1))
  export CONAN_VERSION_SUFFIX="snapshot.${count}+sha.${hash}"
else
  echo >&2 "${0}: error: 1st positional argument must be 1 of 'stable', 'edge', or 'prerelease'"
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

