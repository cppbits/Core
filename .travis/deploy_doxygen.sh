#!/usr/bin/env bash

set -e

##############################################################################
# Functions
##############################################################################

function usage()
{
  echo "usage: ${0}"
}

##############################################################################
# Validation
##############################################################################

if [[ $# -ne 0 ]]; then
  echo >&2 "${0}: error: too many arguments specified"
  usage
  exit 1
fi

##############################################################################
# Main
##############################################################################

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
repo_root=$(dirname $script_dir)

git_hash=$(git rev-parse HEAD)

mkdir -p $repo_root/build-doc
(
  cd $repo_root/build-doc
  doxygen "../doc/Doxyfile"

  cd html
  git init
  git remote add origin "https://cppbits:${GITHUB_TOKEN}@github.com/cppbits/Core.git"
  git checkout -b gh-pages
  git add *

  git commit -m "Update github pages" \
             -m "This updates github pages documentation to hash ${git_hash}" \
             --author "Deployment Bot <deploy@travis-ci.org>" \
             --no-gpg-sign
  git push --force origin gh-pages
)
