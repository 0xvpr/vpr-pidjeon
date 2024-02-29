#!/usr/bin/env python3.exe

import platform
import ctypes
import sys

from argparse import ArgumentParser, Namespace

from PyQt5.QtWidgets import (
    QApplication,
    QPushButton,
    QWidget
)

def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument(
        "-d",
        dest="debug",
        action="store_true",
        required=False,
        default=False
    )

    flags = parser.parse_args()

    return flags

def main() -> None:
    app = QApplication(sys.argv)
    widget = QWidget()

    button = QPushButton("MessageBoxA", widget)
    button.clicked.connect(lambda: print("test"))
    button.setGeometry(0, 0, 150, 45)

    widget.setGeometry(50, 50, 320, 200)
    widget.setWindowTitle("App")
    widget.show()

    sys.exit(app.exec_())

if __name__ == "__main__":
    flags = parse_args()

    if flags.debug:
        sys.stderr.write(f"[!] Platform:\t{ platform.platform() }\n")
        sys.stderr.flush()

    if flags.debug:
        sys.stderr.write("[!] Loading native libraries...\n")
        sys.stderr.flush()

    if flags.debug:
        sys.stderr.write("[+] Native libraries loaded successfully.\n")
        sys.stderr.flush()

    main()
