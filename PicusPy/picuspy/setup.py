#!/usr/bin/env python

from setuptools import setup, find_packages

setup(
    name='picuspy',
    version='0.1',
    description='PicusPy shit',
    author = 'Tim Olson',
    author_email = 'timjolson@user.noreplay.github.com',
    packages = find_packages(),
    install_requires = ['numpy', 'pygame', 'matplotlib'],
)
