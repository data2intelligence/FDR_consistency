from distutils.core import setup, Extension
import os, numpy

module = Extension('FDR_consistency', sources = ['main.c', 'FDR.c'])

setup(name = 'FDR_consistency',
      version = '1.0',
      description = 'Compute FDR for genes based on the consistency between two lists.',
      ext_modules = [module],
      include_dirs = [os.path.join(numpy.get_include(), 'numpy')],
      )
