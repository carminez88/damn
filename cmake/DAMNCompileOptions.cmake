if(MSVC)
    add_compile_options (/permissive-)
    add_compile_options (/DNOGDI)
    add_compile_options (/DNOMINMAX)
    add_compile_options (/MP12)
    add_compile_options ("$<IF:$<CONFIG:Debug>,/MDd,/MD>")
    add_compile_options (/Zc:__cplusplus)
    add_compile_options ("$<IF:$<CONFIG:Debug>,/Od,/O2>")
    add_compile_options (/EHsc)
    add_compile_options (/nologo)
    #add_compile_options (/W4)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-W -Wall -pipe -fPIC -Wextra -msse -msse2 -msse3 -mssse3 -msahf $<IF:$<CONFIG:Debug>,-O0,-O3> $<IF:$<CONFIG:Debug>,-g3,-g0>)
    add_compile_options("$<$<CONFIG:DEBUG>:-Wno-system-headers>")
endif()