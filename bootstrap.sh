#!/bin/sh

if test ! -d boost;
then
  echo 'boost is missing from the source repository' >&2;
  exit 1;
fi;

pushd boost;

./bootstrap.sh --with-toolset=clang --with-libraries=exception,system,iostreams,thread,date_time,program_options,filesystem \
  || die 'boost bootstrap failed';

popd;

pushd webkit;

cmake -DPORT=JSCOnly . -Wno-dev || die 'webkit cmake failed';

exec make;
