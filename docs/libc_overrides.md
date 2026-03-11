`rriftt_ai.h` does not depend on C libc at all. Hence the unconditional `#includes`s are only `<stddef.h>`, `<stdint.h>` and so on, aka those that must come with any standard-compliant C compiler as per the C23 standard.
However, `rriftt_ai.h` does need memory allocations, math operations, assertions and so on. For these `rriftt_ai.h` uses libc out of the box, but wraps them in override macros. For example:

```c
#ifndef RAI_MALLOC
#include <stdlib.h>
#define RAI_MALLOC(size) malloc(size)
#endif // RAI_MALLOC
```

You can strip off dependency on libc by, for example, defining your own malloc like this:

```c
#define RAI_MALLOC(size) my_malloc(size)
#include "rriftt_ai.h"
```

Please see this section of the header for all overrideable libc dependencies.
