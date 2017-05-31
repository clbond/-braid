#!/bin/sh

if test ! -d boost;
then
  echo 'boost is missing from the source repository' >&2;
  exit 1;
fi;

pushd boost >/dev/null;

./bootstrap.sh \
  --with-toolset=clang
  --with-libraries=exception,system,iostreams,thread,date_time,program_options,filesystem \
|| die 'boost bootstrap failed';

./b2 || die 'boost build failed';

popd >/dev/null;

pushd v8 >/dev/null;

exec make x64.debug;
