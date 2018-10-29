#pragma once

/*
     Packing:
     Cross platform way of ignore the default packing from the compiler.
     I use this for accessing file headers such as bitmaps etc.
     Example:
     
     PACKED(
     struct Foo { class details };
     )
 */

#ifdef __GNUC__
#define PACKED( class_to_pack ) class_to_pack __attribute__((__packed__))
#elif defined(_MSC_VER)
#define PACKED( class_to_pack ) __pragma( pack(push, 1) ) class_to_pack __pragma( pack(pop) )
#endif

/* Example:

PACKED(
struct Bar
{
     float baz;
     float boo;
}
);*/