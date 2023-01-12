#ifndef HN_H_
#define HN_H_

#include "util/compiler.h"
#include "util/string.h"
#include "util/vector.h"

struct HNItem {
    String title;
    String link;
    String commentsHref;
};

// Clean up with operator delete.
struct HackerNews {
    Vector<HNItem> items;
    String errmsg;
};

HackerNews*
hnFetch() noexcept;

#endif
