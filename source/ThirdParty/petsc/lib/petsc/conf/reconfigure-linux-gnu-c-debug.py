#!/usr/bin/python
if __name__ == '__main__':
  import sys
  import os
  sys.path.insert(0, os.path.abspath('config'))
  import configure
  configure_options = [
    '--download-fblaslapack',
    '--download-mpich',
    '--prefix=/home/biswajit/project/sFVM/source/ThirdParty/petsc',
    '--with-cc=gcc',
    '--with-cxx=g++',
    '--with-fc=gfortran',
    'PETSC_ARCH=linux-gnu-c-debug',
    'PETSC_DIR=/home/biswajit/Downloads/petsc-3.11.3',
  ]
  configure.petsc_configure(configure_options)
