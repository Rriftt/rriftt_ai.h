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
    filepath = $3
    mode = "file"

    dir = filepath; if (sub(/\/[^\/]*$/, "", dir)) system("mkdir -p " dir)
    print "Extracting " filepath " ..."

    extracting = 1
    next
}

# C routing (#ifdef tests & examples)
/^#ifdef RAI__FILE_(TESTS|EXAMPLES)__/ && !extracting {
    macro = $2

    # Strip RAI__FILE_ prefix, replace __ with /, replace last _ with .
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

    # Inject C boilerplate
    print "#define RAI_IMPLEMENTATION" > filepath
    print "#include \"../" hdr "\"" > filepath
    print "" > filepath
    next
}

# Extracting content
extracting {
    # Handle termination conditions based on the current mode
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
            close(filepath)
            next
        }

        # Strip polyglot C/Markdown wrappers
        if ($0 ~ /^[[:space:]]*```c[[:space:]]*\*\//) { print "```c" > filepath; next }
        if ($0 ~ /^[[:space:]]*\/\*[[:space:]]*```[[:space:]]*$/) { print "```" > filepath; next }
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

