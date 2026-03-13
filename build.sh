#!/bin/bash
set -e

CC=gcc
CFLAGS="-std=c23 -Wall -Wextra -Wpedantic"
LDFLAGS="-lm"

echo "================================"
echo "  Extracting Resources          " 
echo "================================"

awk -v hdr="rriftt_ai.h" '
BEGIN { extracting = 0; mode = ""; filepath = ""; depth = 0 }

# Markdown routing (Native Fences)
/^[[:space:]]*\/\*[[:space:]]*```markdown[[:space:]]+/ && !extracting {
    path_arg = $3
    mode = "file"
    extracting = 1

    # If it explicitly ends in /, treat as a directory and auto-detect name.
    if (path_arg ~ /\/$/) {
        dirpath = path_arg
        defer_filename = 1
        held_c_fence = 0
        buf_idx = 0
        buffering_struct = 0
    } else {
        # Otherwise, use the exact provided filename
        filepath = path_arg
        defer_filename = 0
        dir = filepath; if (sub(/\/[^\/]*$/, "", dir)) system("mkdir -p " dir)
        print "Extracting " filepath " ..."
    }
    next
}

# C routing (#ifdef tests & examples)
/^#ifdef RAI__FILE_(TESTS|EXAMPLES)__/ && !extracting {
    macro = $2

    # Strip RAI__FILE_ prefix, replace __ with /, replace last _.
    filepath = substr(macro, 11)
    gsub(/__/, "/", filepath)
    match(filepath, /_[^_]*$/)
    if (RSTART > 0) {
        filepath = substr(filepath, 1, RSTART-1) "." substr(filepath, RSTART+1)
    }
    filepath = tolower(filepath)

    dir = filepath; if (sub(/\/[^\/]*$/, "", dir)) system("mkdir -p " dir)
    print "Extracting " filepath " ..."

    mode = "macro"
    extracting = 1
    depth = 1

    print "#define RAI_IMPLEMENTATION" > filepath
    print "#include \"../" hdr "\"" > filepath
    print "" > filepath
    next
}

# Extracting content
extracting {
    if (mode == "macro") {
        if (/^[[:space:]]*#(if|ifdef|ifndef)/) { depth++ }
        if (/^[[:space:]]*#endif/) {
            depth--
            if (depth == 0) {
                extracting = 0
                close(filepath)
                next
            }
        }
    } else if (mode == "file") {
        # Check for Markdown end boundary
        if (/^[[:space:]]*```[[:space:]]*\*\//) {
            extracting = 0
            if (filepath != "") close(filepath)
            next
        }

        # Strip polyglot C/Markdown wrappers
        if ($0 ~ /^[[:space:]]*```c[[:space:]]*\*\//) {
            if (defer_filename) {
                held_c_fence = 1
                next
            }
            print "```c" > filepath
            next
        }
        if ($0 ~ /^[[:space:]]*\/\*[[:space:]]*```[[:space:]]*$/) { 
            print "```" > filepath
            next 
        }

        # Dynamic Filename Extraction
        if (defer_filename) {

            # If we are currently inside a multi-line struct block
            if (buffering_struct) {
                buffer[buf_idx++] = $0

                # Look for `} StructName;`
                if (match($0, /\}[[:space:]]*[A-Za-z0-9_]+[[:space:]]*;/)) {
                    match($0, /[A-Za-z0-9_]+[[:space:]]*;/)
                    name = substr($0, RSTART, RLENGTH)
                    sub(/[[:space:]]*;/, "", name)

                    filepath = dirpath name ".md"
                    system("mkdir -p " dirpath)
                    print "Extracting " filepath " (auto-detected struct)..."
                    defer_filename = 0
                    buffering_struct = 0

                    # Dump the held fences and buffered lines into the file
                    if (held_c_fence) print "```c" > filepath
                    for (i=0; i<buf_idx; i++) print buffer[i] > filepath
                }
                next
            }

            # Hold empty lines until we hit the actual definition
            if ($0 ~ /^[[:space:]]*$/) {
                buffer[buf_idx++] = $0
                next
            }

            name = ""
            # Macro
            if (match($0, /^[[:space:]]*#define[[:space:]]+[A-Za-z0-9_]+/)) {
                name = substr($0, RSTART, RLENGTH)
                sub(/^[[:space:]]*#define[[:space:]]+/, "", name)
            } 
            # Function
            else if (match($0, /[A-Za-z0-9_]+[[:space:]]*\(/)) {
                name = substr($0, RSTART, RLENGTH)
                sub(/[[:space:]]*\($/, "", name)
            } 
            # Multi-line Typedef block (Trigger buffering)
            else if ($0 ~ /^[[:space:]]*typedef[[:space:]]+(struct|enum|union)/) {
                buffering_struct = 1
                buffer[buf_idx++] = $0
                next
            }
            # Standard Named block
            else if (match($0, /(struct|enum|union)[[:space:]]+[A-Za-z0-9_]+/)) {
                name = substr($0, RSTART, RLENGTH)
                sub(/(struct|enum|union)[[:space:]]+/, "", name)
            }

            if (name != "") {
                filepath = dirpath name ".md"
            } else if (!buffering_struct) {
                filepath = dirpath "unnamed_block_" NR ".md"
            }

            # If it was a 1-liner definition (macro, func), flush immediately
            if (!buffering_struct) {
                system("mkdir -p " dirpath)
                print "Extracting " filepath " (auto-detected)..."
                defer_filename = 0

                if (held_c_fence) print "```c" > filepath
                for (i=0; i<buf_idx; i++) print buffer[i] > filepath
                print $0 > filepath
                next
            }
        }
    }

    # Dump raw content
    print $0 > filepath
}
' rriftt_ai.h

shopt -s nullglob
TEST_SRCS=(tests/*.c)
EXAMPLE_SRCS=(examples/*.c)

echo ""
echo "================================"
echo "  Building and Executing Tests  " 
echo "================================"
for TEST_SRC in "${TEST_SRCS[@]}"; do
    TEST_EXE="${TEST_SRC%.c}"
    echo "Building  $TEST_EXE ..."
    $CC $CFLAGS -o "$TEST_EXE" "$TEST_SRC" $LDFLAGS
    echo "Executing $TEST_EXE ..."
    ./"$TEST_EXE" > /dev/null
    rm "$TEST_EXE"
done

echo ""
echo "================================"
echo "  Building Examples             "
echo "================================"
for EXAMPLE_SRC in "${EXAMPLE_SRCS[@]}"; do
    EXAMPLE_EXE="${EXAMPLE_SRC%.c}"
    echo "Building $EXAMPLE_EXE ..."
    $CC $CFLAGS -o "$EXAMPLE_EXE" "$EXAMPLE_SRC" $LDFLAGS
    rm "$EXAMPLE_EXE"
done

echo ""
echo "================================"
echo "  All Tests Passed Successfully "
echo "================================"

