#!/usr/bin/env python3.exe

import ctypes
import sys

from ctypes import CDLL

def inject(process_id: int, payload_path: bytes) -> int:
    result = -1

    loadlibrary = CDLL("lib/vpr-pidjeon.dll");
    if loadlibrary:
        result = loadlibrary.inject(
            ctypes.c_uint32(process_id),
            payload_path
        )

    return result

if __name__ == "__main__":
    # Parse arguments
    target, payload = int(sys.argv[1]), sys.argv[2].encode()

    # Verify target

    # Verify payload

    # Determine architecture of target

    # Inject
    print(inject(target, payload))
