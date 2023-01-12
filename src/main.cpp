#include "hn.h"

#include "util/compiler.h"
#include "util/io.h"

int
main() noexcept {
    Flusher f2(serr);
    Flusher f1(sout);

    sout << "Fetching Hacker News... " << Flush();
    HackerNews* hn = hnFetch();
    sout << " done\n" << Flush();

    if (hn->errmsg.size) {
        sout << "err: " << hn->errmsg << '\n';
        return 1;
    }

    for (HNItem& item : hn->items) {
        sout << item.title << ' ' << item.link << '\n';
    }
    return 0;
}
