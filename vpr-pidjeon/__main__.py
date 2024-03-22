#!/usr/bin/env python3.exe

import platform
import ctypes
import psutil
import sys

from argparse import ArgumentParser, Namespace

from PyQt5.QtWidgets import (
    QApplication,
    QMainWindow,
    QTableWidget,
    QTableWidgetItem,
    QVBoxLayout,
    QWidget,
)
from PyQt5.QtCore import QTimer

class TaskManager(QMainWindow):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Real-Time Task Manager')
        self.setGeometry(100, 100, 400, 600)

        self.tableWidget = QTableWidget()
        self.tableWidget.setColumnCount(2)
        self.tableWidget.setHorizontalHeaderLabels(['Process Name', 'PID'])

        self.layout = QVBoxLayout()
        self.layout.addWidget(self.tableWidget)

        self.container = QWidget()
        self.container.setLayout(self.layout)
        self.setCentralWidget(self.container)

        self.timer = QTimer(self)
        self.timer.timeout.connect(self.updateProcessList)
        self.timer.start(1000)  # Refreshes every 1000 milliseconds

    def updateProcessList(self):
        self.tableWidget.setRowCount(0)  # Clear the table
        for process in psutil.process_iter(['pid', 'name']):
            rowPosition = self.tableWidget.rowCount()
            self.tableWidget.insertRow(rowPosition)
            self.tableWidget.setItem(rowPosition, 0, QTableWidgetItem(process.info['name']))
            self.tableWidget.setItem(rowPosition, 1, QTableWidgetItem(str(process.info['pid'])))


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
    ex = TaskManager()
    ex.show()
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
