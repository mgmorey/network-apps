import argparse
import json
import shlex
import subprocess
import tempfile
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--clang-tidy", required=True)
    parser.add_argument("--compile-commands", required=True)
    parser.add_argument("--source-directory", required=True)
    parser.add_argument("--config-file", required=True)
    parser.add_argument("--header-filter", required=True)
    parser.add_argument("--extra-arg-before", action="append", default=[])
    parser.add_argument("--sources", nargs="+", required=True)
    args = parser.parse_args()

    database_path = Path(args.compile_commands)
    database = json.loads(database_path.read_text(encoding="utf-8"))

    for entry in database:
        if "arguments" not in entry:
            entry["arguments"] = shlex.split(entry.pop("command"))

    with tempfile.TemporaryDirectory(prefix="clang-tidy-") as directory:
        Path(directory, "compile_commands.json").write_text(
            json.dumps(database),
            encoding="utf-8",
        )
        command = [
            args.clang_tidy,
            f"--config-file={args.config_file}",
            f"--header-filter={args.header_filter}",
        ]
        command.extend(
            f"--extra-arg-before={arg}"
            for arg in args.extra_arg_before
        )
        command.extend(["-p", directory, *args.sources])
        return subprocess.run(command, cwd=args.source_directory).returncode


if __name__ == "__main__":
    raise SystemExit(main())
