# apt-like-ProgressBar
A simple header only Progress Bar, implemented to be similar to that during apt upgrade process, with the progressbar showing at the bottom, allowing further print statements to continue at the top.

Allows the creation and usage of simple progress bars for generic C codes. The updation of progress is to be done manually, thereby allowing usage across all sorts of looping constructs

## Usage Instructions
Before the start of the loop, create a ProgressBar object
```
ProgressBar pb;
```

Set it up with the total parameter as the number of iterations of the loop
```
PB_Init(total, &pb);
```

Update the ProgressBar with the iteration number as the val parameter
```
PB_Update(val, &pb);
```
Clear the ProgressBar once the loop has finished exeuting
```
PB_Clear(&pb);
``` 

Now, the ProgressBar object is also reset. To use it again, make sure to set it up again.

### Example
```
#include "progressbar.h"
#include <unistd.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    int total = 100;
    ProgressBar pb;

    PB_Init(total, &pb);
    for (int i = 0; i < 100; i++) {
        printf("Iteration: %d / total\n");
        sleep(1);
        PB_Update(i + 1, &pb);
    }
    PB_Clear(&pb);
    return 0;
}
```
