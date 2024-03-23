from ..types import (
    Resource,
    Injector
)

from pathlib import Path
from argparse import ArgumentParser
from typing import Tuple

from ..c_bindings import (
    get_process_id_by_process_name,
    log_basic,
    log_advanced
)

def handle_command_line() -> Tuple[Resource,Injector]:
    parser = ArgumentParser(description="Python module for creating a remote thread.")
    parser.add_argument(
        dest="target",
        type=str,
        help="Case-sensitive name of the target process."
    )
    parser.add_argument(
        dest="payload",
        type=str,
        help="Relative or absolute path of the target payload."
    )
    parser.add_argument(
        "-v",
        dest="verbosity",
        action="count",
        default=0,
        help="Set the verbosity level of the program (-v | -vv)"
    )

    args = parser.parse_args()

    process_id = get_process_id_by_process_name(args.target.encode())
    if not process_id:
        parser.print_help()
        print()
        parser.exit(status=-1, message=f"Process '{args.target}' not found.")

    payload = Path(args.payload)
    if not payload.is_file():
        parser.print_help()
        print()
        parser.exit(status=-1, message=f"Payload '{args.payload}' not found.")

    if args.verbosity > 2:
        parser.print_help()
        print()
        parser.exit(status=-1, message=f"Incorrect usage of -v flags.")

    # Determine architecture of target
    
    resource, injector = (
        Resource(
            process_id = get_process_id_by_process_name(args.target.encode()),
            target_process=args.target.encode(),
            relative_payload_path=args.payload.encode()
        ),
        Injector(
            logger=(lambda index: (0,log_basic,log_advanced)[index])(args.verbosity)
        )
    )

    return resource, injector

