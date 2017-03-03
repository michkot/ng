Angie project
=============

![alt](https://cdn.travis-ci.org/images/favicon-076a22660830dc325cc8ed70e7146a59.png)
[![Travis CI](https://api.travis-ci.org/michkot/ng.svg?branch=master)](https://travis-ci.org/michkot/ng)
[![AppVeyor](https://ci.appveyor.com/api/projects/status/4edca1b2f3u0bxhb?svg=true)](https://ci.appveyor.com/project/michkot/ng)

Build & Run
-----------
```sh
# set to your executables
COPT=opt
CLANGXX=clang++
CLANG=clang
export COPT CLANGXX CLANG
(bash ./compile-inputs-to-ll.sh; mkdir tmp; cd tmp; cmake ../; make; cd .. ; ./tmp/ng)
```
