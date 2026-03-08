#!/bin/bash
set -e

CC=gcc
CFLAGS="-std=c23 -Wall -Wextra -Wpedantic"
LDFLAGS="-lm"

echo "================================"
echo "  Extracting Resources          " 
echo "================================"
mkdir -p tests examples docs

awk -v hdr="rriftt_ai.h" '
/^#ifdef RAI__FILE_/ {
    macro = $2
    if (macro == "RAI__FILE_README_MD") { filepath = "README.md" }
    else if (macro == "RAI__FILE_LICENSE") { filepath = "LICENSE" }
    else {
        filepath = tolower(substr(macro, 11))
        if (filepath ~ /^tests_/) sub(/^tests_/, "tests/", filepath)
        else if (filepath ~ /^examples_/) sub(/^examples_/, "examples/", filepath)
        else if (filepath ~ /^docs_/) sub(/^docs_/, "docs/", filepath)
        sub(/_c$/, ".c", filepath)
        sub(/_md$/, ".md", filepath)
    }
    
    print "Extracting " filepath " ..."
    
    extracting = 1
    depth = 1
    if (filepath ~ /\.c$/) {
        print "#define RRIFTT_AI_IMPLEMENTATION" > filepath
        print "#include \"../" hdr "\"" > filepath
        print "" > filepath
    }
    next
}
extracting {
    if (/^[[:space:]]*#(if|ifdef|ifndef)/) { depth++ }
    if (/^[[:space:]]*#endif/) {
        depth--
        if (depth == 0) { extracting = 0; close(filepath); next }
    }
    if ((filepath ~ /\.md$/ || filepath == "LICENSE" || filepath == "README.md") && ($0 ~ /^\/\*/ || $0 ~ /^\*\//)) {
        next
    }
    print $0 > filepath
}' rriftt_ai.h

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

