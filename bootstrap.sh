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

pushd v8/v8 >/dev/null;

../depot_tools/gn gen out || die 'Failed to generate V8 build scripts';

make x64.debug || die 'V8 build failed';

exit $?;
