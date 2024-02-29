#!/usr/bin/env python3.exe

import ctypes
import sys
import os

from ctypes import CDLL

def inject(target: bytes, payload_path: bytes) -> int:
    current_dir = os.path.dirname(os.path.abspath(__file__))
    dll = CDLL( current_dir + "\\lib\\vpr-pidjeon.dll");

    get_process_id_by_process_name = dll.GetProcessIdByProcessName
    get_process_id_by_process_name.restype = ctypes.c_uint32
    get_process_id_by_process_name.argtypes = [ctypes.c_char_p]

    loadlibrarya = dll["inject"]
    loadlibrarya.restype = ctypes.c_uint32
    loadlibrarya.argtypes = [ctypes.c_uint32, ctypes.c_char_p]

    if not dll or not get_process_id_by_process_name or not loadlibrarya:
        return -1

    process_id = get_process_id_by_process_name(target)
    result = loadlibrarya(
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
