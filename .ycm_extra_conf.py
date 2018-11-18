import os

def FlagsForFile(filename, **kwargs):
  return {
    'flags': [
      '-x', 'c11',
      '-Wall', '-Wpointer-arith',
    ],
  }
