#!/usr/bin/env python

import os
import re
import sys
import glob
import subprocess

HEADER_FILE = "rriftt_ai.h"


# Attempts to find the name of the function, struct, or macro in a C block.
def extract_identifier(c_text):
    if match := re.search(r"([A-Za-z0-9_]+)\s*\(", c_text):
        return match.group(1)
    if match := re.search(r"#define\s+([A-Za-z0-9_]+)", c_text):
        return match.group(1)
    if match := re.search(r"\}\s*([A-Za-z0-9_]+)\s*;", c_text):
        return match.group(1)
    if match := re.search(r"(?:struct|enum|union)\s+([A-Za-z0-9_]+)", c_text):
        return match.group(1)
    return None


# Converts TESTS__TEST_ARENA_C to tests/test_arena.c
def format_filename(macro_str):
    parts = macro_str.lower().split("__")
    folder_path = "/".join(parts[:-1])
    file_part = parts[-1]

    last_underscore = file_part.rfind("_")
    if last_underscore != -1:
        file_part = file_part[:last_underscore] + "." + file_part[last_underscore + 1 :]

    return os.path.join(folder_path, file_part) if folder_path else file_part


def main():
    if not os.path.exists(HEADER_FILE):
        print(f"Error: {HEADER_FILE} not found.")
        sys.exit(1)

    path_stack = []
    macro_stack = []

    state = "NORMAL"
    doc_buffer = []
    c_buffer = []
    target_filename = None

    # Used for dynamic file extraction (e.g., tests, examples)
    out_file_handle = None

    print("================================")
    print("  Extracting Resources          ")
    print("================================")

    with open(HEADER_FILE, "r", encoding="utf-8") as f:
        lines = f.readlines()

    for i, line in enumerate(lines):
        # ---------------------------------------------------------
        # Preprocessor Stack Management (Global across all states)
        # ---------------------------------------------------------
        if re.match(r"^\s*#\s*if", line):
            macro_match = re.search(
                r"#\s*(?:ifdef|ifndef|if\s+defined\()?\s*([A-Za-z0-9_]+)", line
            )
            macro_name = macro_match.group(1) if macro_match else ""

            if macro_name.startswith("RAI__DIR_"):
                dir_name = macro_name[9:].lower()
                path_stack.append(dir_name)
                macro_stack.append("DIR")

            elif macro_name.startswith("RAI__FILE_"):
                raw_file_macro = macro_name[10:]
                filename = format_filename(raw_file_macro)
                out_dir = os.path.join(*path_stack) if path_stack else "."
                filepath = os.path.join(out_dir, filename)

                macro_stack.append("FILE")
                state = "IN_FILE"

                # Fix 1: Ensure we create the directory for the full filepath, not just the stack
                os.makedirs(os.path.dirname(filepath), exist_ok=True)
                out_file_handle = open(filepath, "w", encoding="utf-8")

                # Fix 2: Calculate depth using path_stack PLUS the nested folders from the macro
                macro_folder_depth = len(raw_file_macro.split("__")) - 1
                total_depth = len(path_stack) + macro_folder_depth
                include_depth = "../" * total_depth

                out_file_handle.write("#define RAI_IMPLEMENTATION\n")
                out_file_handle.write(f'#include "{include_depth}{HEADER_FILE}"\n\n')
                print(f"Extracting {filepath} ...")

            else:
                macro_stack.append("IGNORE")
                if state == "IN_FILE" and out_file_handle:
                    out_file_handle.write(line)

        elif re.match(r"^\s*#\s*endif", line):
            if macro_stack:
                closed_macro = macro_stack.pop()
                if closed_macro == "DIR":
                    path_stack.pop()
                elif closed_macro == "FILE":
                    if out_file_handle:
                        out_file_handle.close()
                        out_file_handle = None
                    state = "NORMAL"
                elif state == "IN_FILE" and out_file_handle:
                    out_file_handle.write(line)

        # ---------------------------------------------------------
        # File Extraction State (e.g. tests, examples)
        # ---------------------------------------------------------
        elif state == "IN_FILE":
            if out_file_handle:
                out_file_handle.write(line)

        # ---------------------------------------------------------
        # Normal State (Looking for Docs)
        # ---------------------------------------------------------
        elif state == "NORMAL":
            if doc_match := re.match(r"^\s*/\*\*(.*)", line):
                state = "IN_DOC"
                doc_buffer = []
                c_buffer = []

                doc_header = doc_match.group(1).strip()
                target_filename = doc_header if doc_header else None

        # ---------------------------------------------------------
        # Documentation Gathering State
        # ---------------------------------------------------------
        elif state == "IN_DOC":
            if re.match(r"^\s*\*/", line):
                state = "GATHERING_C"
            else:
                doc_text = line.rstrip("\n")
                # Carefully strip only the single alignment space/tab
                # to preserve the asterisk for Markdown bullet points
                if doc_text.startswith(" *") or doc_text.startswith("\t*"):
                    doc_text = doc_text[1:]
                doc_buffer.append(doc_text)

        # ---------------------------------------------------------
        # C Code Gathering State
        # ---------------------------------------------------------
        elif state == "GATHERING_C":
            if re.match(r"^\s*$", line):
                c_text_full = "".join(c_buffer).strip()

                if not target_filename:
                    detected_name = extract_identifier(c_text_full)
                    target_filename = (
                        f"{detected_name}.md"
                        if detected_name
                        else f"unnamed_block_{i}.md"
                    )

                out_dir = os.path.join(*path_stack) if path_stack else "."
                out_path = os.path.normpath(os.path.join(out_dir, target_filename))

                # Ensure the full directory path exists, including any nested folders from target_filename
                final_dir = os.path.dirname(out_path)
                if final_dir:
                    os.makedirs(final_dir, exist_ok=True)

                with open(out_path, "w", encoding="utf-8") as out_f:
                    out_f.write("\n".join(doc_buffer) + "\n\n")

                    if c_text_full:
                        fence = "`" * 3
                        out_f.write(f"{fence}c\n")
                        out_f.write(c_text_full + "\n")
                        out_f.write(f"{fence}\n")

                print(f"Extracting {out_path} ...")

                state = "NORMAL"
                target_filename = None
            else:
                c_buffer.append(line)

    # ---------------------------------------------------------
    # Build and Test Execution Phase
    # ---------------------------------------------------------
    compiler = "gcc"
    cflags = ["-std=c23", "-Wall", "-Wextra", "-Wpedantic"]
    ldflags = ["-lm"]

    print("")
    print("================================")
    print("  Building and Executing Tests  ")
    print("================================")

    test_srcs = glob.glob("tests/*.c")
    for test_src in test_srcs:
        test_exe = os.path.splitext(test_src)[0]

        print(f"Building  {test_exe} ...")
        compile_cmd = [compiler] + cflags + ["-o", test_exe, test_src] + ldflags
        subprocess.run(compile_cmd, check=True)

        print(f"Executing {test_exe} ...")
        run_cmd = [f"./{test_exe}"]
        subprocess.run(run_cmd, check=True, stdout=subprocess.DEVNULL)

        if os.path.exists(test_exe):
            os.remove(test_exe)

    print("")
    print("================================")
    print("  Building Examples             ")
    print("================================")

    example_srcs = glob.glob("examples/*.c")
    for example_src in example_srcs:
        example_exe = os.path.splitext(example_src)[0]

        print(f"Building {example_exe} ...")
        compile_cmd = [compiler] + cflags + ["-o", example_exe, example_src] + ldflags
        subprocess.run(compile_cmd, check=True)

        if os.path.exists(example_exe):
            os.remove(example_exe)

    print("")
    print("================================")
    print("  All Tests Passed Successfully ")
    print("================================")


if __name__ == "__main__":
    main()
