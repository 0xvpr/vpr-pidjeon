#!/usr/bin/env python3.exe

from .types import (
    Resource,
    Injector,
)

from .c_bindings import (
    create_remote_thread,
    log_basic
)

import platform
import ctypes
import psutil
import sys

from argparse import ArgumentParser, Namespace

from PyQt5.QtCore import Qt

from PyQt5.QtWidgets import (
    QApplication,
    QMenu,
    QMainWindow,
    QPushButton,
    QTableWidget,
    QTableWidgetItem,
    QHBoxLayout,
    QVBoxLayout,
    QWidget,
    QLineEdit,
    QFileDialog,
    QGroupBox
)

class GUI(QMainWindow):
    def __init__(self):
        super().__init__()

        self.initUI()
        self.tableWidget.setContextMenuPolicy(Qt.CustomContextMenu)
        self.tableWidget.customContextMenuRequested.connect(self.contextMenu)
    
    def initUI(self):
        self.setWindowTitle('vpr-pidjeon')
        self.setGeometry(100, 100, 400, 600)
        self.setMinimumSize(600, 400)



        self.tableWidget = QTableWidget()
        self.tableWidget.setColumnCount(2)
        self.tableWidget.setHorizontalHeaderLabels(['Process Name', 'PID'])
        self.tableWidget.setSortingEnabled(True)

        self.outer_layout = QVBoxLayout()
        self.outer_layout.setContentsMargins(0, 0, 0, 0)
        self.outer_layout.setSpacing(0)

        self.payload_layout = QHBoxLayout()
        self.payload_layout.setContentsMargins(0, 0, 0, 0)
        self.payload_layout.setSpacing(0)

        process_box   = QGroupBox("Processes")
        self.process_layout = QVBoxLayout()
        self.process_layout.setContentsMargins(0, 0, 0, 0)
        self.process_layout.setSpacing(0) 
        process_box.setLayout(self.process_layout)

        # Create the filter bar
        self.filter = QLineEdit()
        self.filter.setPlaceholderText("filter...")

        self.refreshButton = QPushButton("Refresh")
        self.refreshButton.clicked.connect(self.updateProcessList) 


        # Layout box order
        self.process_layout.addWidget(self.filter)
        self.process_layout.addWidget(self.tableWidget)
        self.process_layout.addWidget(self.refreshButton)

        self.container = QWidget()
        self.container.setLayout(self.outer_layout)
        self.setCentralWidget(self.container)

        # Must happen first
        self.updateProcessList()

        # Sizing
        row_height = self.tableWidget.rowHeight(0)
        self.filter.setFixedHeight(row_height)
        self.filter.setFixedWidth(400)
        self.filter.setMinimumWidth(300)

        self.refreshButton.setFixedHeight(row_height)
        self.refreshButton.setFixedWidth(400)
        self.refreshButton.setMinimumWidth(300)

        self.tableWidget.setFixedHeight((row_height * 11) + 4)
        self.tableWidget.setFixedWidth(400)
        self.tableWidget.setMinimumWidth(300)
        self.tableWidget.setMinimumHeight(200)

        # Actions
        self.tableWidget.cellDoubleClicked.connect(self.double_clicked_cell)

        payload_box   = QGroupBox("Payloads")
        payload_box.setLayout(self.payload_layout)

        parameter_box = QGroupBox("Parameters")
        parameter_layout = QHBoxLayout()
        parameter_box.setLayout(parameter_layout)

        self.targetBox = QLineEdit()
        self.targetBox.isEnabled = False
        self.targetBox.setPlaceholderText("Target...")
        parameter_layout.addWidget(self.targetBox)

        self.injectButton = QPushButton("Inject")
        self.injectButton.clicked.connect(self.inject_clicked)
        parameter_layout.addWidget(self.injectButton)

        self.payloadBox = QLineEdit()
        self.payloadBox.setPlaceholderText("Select payload...")
        self.payload_layout.addWidget(self.payloadBox)

        self.openFileDialogButton = QPushButton("Browse")
        self.openFileDialogButton.clicked.connect(self.openFileDialog)
        self.payload_layout.addWidget(self.openFileDialogButton)


        self.outer_layout.addWidget(process_box)
        self.outer_layout.addWidget(payload_box)
        self.outer_layout.addWidget(parameter_box)

    def inject_clicked(self):
        target = self.targetBox.text()
        payload = self.payloadBox.text()

        resource = Resource(
            process_id=self.process_id,
            target_process=target.encode(),
            relative_payload_path=payload.encode()
        )
        injector = Injector(
            logger=log_basic
        )

        create_remote_thread(
            ctypes.byref(resource),
            ctypes.byref(injector)
        )

    def double_clicked_cell(self, row, col):
        self.name = self.tableWidget.item(row, 0).text().encode()
        self.process_id = ctypes.c_uint32( int(self.tableWidget.item(row, 1).text()) )

        self.targetBox.setText(self.name.decode())

    def updateProcessList(self):
        searchQuery = self.filter.text().lower()
        self.tableWidget.setSortingEnabled(False) 
        self.tableWidget.setRowCount(0)
        for process in psutil.process_iter(['pid', 'name']):
            processName = process.info['name']
            if searchQuery in processName.lower():
                rowPosition = self.tableWidget.rowCount()
                self.tableWidget.insertRow(rowPosition)
                nameItem = QTableWidgetItem(processName)
                pidItem = QTableWidgetItem(str(process.info['pid']))
                
                nameItem.setFlags(nameItem.flags() & ~Qt.ItemIsEditable)
                pidItem.setFlags(pidItem.flags() & ~Qt.ItemIsEditable)
                
                self.tableWidget.setItem(rowPosition, 0, nameItem)
                self.tableWidget.setItem(rowPosition, 1, pidItem)
        self.tableWidget.setSortingEnabled(True)
    
    def contextMenu(self, position):
        menu = QMenu()
        copyAction = menu.addAction("Copy")
        action = menu.exec_(self.tableWidget.viewport().mapToGlobal(position))
        if action == copyAction:
            selected = self.tableWidget.selectedItems()
            if selected:
                clipboard = QApplication.clipboard()
                clipboard.setText(selected[0].text())

    def openFileDialog(self):
       options = QFileDialog.Options()
       options |= QFileDialog.DontUseNativeDialog
       fileName, _ = QFileDialog.getOpenFileName(self, "Open File", "", "All Files (*);;Text Files (*.txt)", options=options)
       if fileName:
           self.payloadBox.setText(fileName)

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
    ex = GUI()
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