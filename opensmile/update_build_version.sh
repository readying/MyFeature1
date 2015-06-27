#!/bin/sh

echo -n "#define OPENSMILE_SOURCE_REVISION \"" > src/include/core/svn_version.hpp
svnversion -n >>  src/include/core/svn_version.hpp
echo "\"" >> src/include/core/svn_version.hpp
echo -n "#define OPENSMILE_BUILD_DATE \"" >> src/include/core/svn_version.hpp
date | tr '\n' '"' >>  src/include/core/svn_version.hpp

