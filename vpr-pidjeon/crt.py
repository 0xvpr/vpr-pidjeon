#!/usr/bin/env python3.exe

from .util import handle_command_line

from .types import (
    Resource,
    Injector
)

from .c_bindings import (
    vpr_pidjeon_dll,
    get_process_id_by_process_name,
    create_remote_thread,
)

from ctypes import (
    POINTER,
    byref,
    c_int32,
)

def _create_remote_thread(resource: POINTER(Resource), injector: POINTER(Injector)) -> c_int32:
    if not vpr_pidjeon_dll or not get_process_id_by_process_name or not create_remote_thread:
        Exception(ImportError)

    return create_remote_thread(
        resource,
        injector
    )

if __name__ == "__main__":
    resource, injector = handle_command_line()
    _create_remote_thread(
        byref(resource),
        byref(injector)
    )
