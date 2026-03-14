You can exclude any module by #defining `RAI_NO_<MODULE_NAME>` before #including `rriftt_ai.h`:

```c
#define RAI_NO_MATH
#define RAI_IMPLEMENTATION
#include "rriftt_ai.h"
```

But be aware, these modules are interdependent.

