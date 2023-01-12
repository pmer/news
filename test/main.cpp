#include "util/compiler.h"
#include "util/io.h"

void
testHn() noexcept;

I32
main() noexcept {
    Flusher f1(sout);
    Flusher f2(serr);

    testHn();

    return 0;
}
