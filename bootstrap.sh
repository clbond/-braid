#!/bin/sh

if test ! -d boost;
then
  echo 'boost is missing from the source repository' >&2;
  exit 1;
fi;

cd boost;

exec ./bootstrap.sh --with-toolset=clang --with-libraries=asio,exception,system,iostreams,thread,date_time,program_options,filesystem --without-libraries=python;
