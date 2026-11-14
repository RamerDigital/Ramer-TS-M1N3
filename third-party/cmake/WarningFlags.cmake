add_library(warning_flags INTERFACE)

target_compile_options(warning_flags INTERFACE
    -Werror # Treat warnings as errors
    -Wno-pass-failed # Suppress warnings/errors when compiler fails to auto-vectorize loops (prevents build failure under -Werror and avoids flooding terminal with optimization reports)
    -Wall -Wshadow-all -Wshorten-64-to-32 -Wstrict-aliasing -Wuninitialized
    -Wunused-parameter -Wconversion -Wsign-compare -Wint-conversion
    -Wconditional-uninitialized -Woverloaded-virtual -Wreorder
    -Wconstant-conversion -Wsign-conversion -Wunused-private-field
    -Wbool-conversion -Wunreachable-code
    -Wzero-as-null-pointer-constant -Wcast-align
    -Wshift-sign-overflow -Wnullable-to-nonnull-conversion -Wpedantic

    # Suppress warnings for redundant/extra semicolons
    -Wno-extra-semi

    # Suppress warnings for missing override keywords in overridden destructors (frequent in JUCE framework headers)
    -Wno-inconsistent-missing-destructor-override

    # Suppress warnings when structs are partially initialized (common in configuration structs)
    -Wno-missing-field-initializers

    # Suppress warnings when qualifiers like const are ignored on value return types (common in legacy APIs)
    -Wno-ignored-qualifiers
)
