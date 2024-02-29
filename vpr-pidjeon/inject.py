#!/usr/bin/env python3.exe

import ctypes
import sys
import os

from ctypes import CDLL

def inject(process_id: bytes, payload_path: bytes) -> int:
    current_dir = os.path.dirname(os.path.abspath(__file__))
    dll = CDLL( current_dir + "\\lib\\vpr-pidjeon.dll");

    if not dll:
        return -1

    process_id = dll.GetProcessIdByProcessName(target)
    result = dll.inject(
        process_id,
        payload_path
    )

    return result

if __name__ == "__main__":
    # Parse arguments
    target, payload = sys.argv[1].encode(), sys.argv[2].encode()

    # Verify target

    # Verify payload

    # Determine architecture of target

    # Inject
    print( inject(target, payload) )
