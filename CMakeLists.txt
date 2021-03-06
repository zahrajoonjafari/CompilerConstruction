cmake_minimum_required(VERSION 3.13)
project(compiler C)

set(CMAKE_C_STANDARD 11)

include_directories(mcc/builddir/mcc@sha)
include_directories(mcc/include)
include_directories(mcc/include/mcc)
include_directories(mcc/src/utils)
include_directories(mcc/vendor/cutest)

add_executable(compiler
        mcc/app/mc_ast_to_dot.c
        mcc/app/mcc.c
        mcc/builddir/mcc@sha/parser.tab.c
        mcc/builddir/mcc@sha/parser.tab.h
        mcc/builddir/mcc@sha/scanner.c
        mcc/builddir/mcc@sha/scanner.h
        mcc/builddir/meson-private/sanitycheckc.c
        mcc/include/mcc/ast.h
        mcc/include/mcc/ast_print.h
        mcc/include/mcc/ast_visit.h
        mcc/include/mcc/parser.h
        mcc/resources/mc_builtins.c
        mcc/src/utils/unused.h
        mcc/src/ast.c
        mcc/src/ast_print.c
        mcc/src/ast_visit.c
        mcc/test/unit/parser_test.c
        mcc/vendor/cutest/AllTests.c
        mcc/vendor/cutest/CuTest.c
        mcc/vendor/cutest/CuTest.h
        mcc/vendor/cutest/CuTestTest.c)
