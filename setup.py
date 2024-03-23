#!/usr/bin/env python3.exe

from setuptools import setup, find_packages

setup(
    name='vpr-pidjeon',
    version='1.0.0',
    packages=find_packages(),
    package_data={ "vpr-pidejon": ["lib/*"] },
    include_package_data=True,
    entry_points={
        'console_scripts': [
            'vpr-pidjeon = vpr_pidjeon:main',
        ],
    },
)
