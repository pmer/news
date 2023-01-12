#include "hn.h"

#include "util/assert.h"
#include "util/compiler.h"
#include "util/string-view.h"
#include "util/string.h"

void
expand(String& out, StringView in) noexcept;

HackerNews*
hnParse(String& response) noexcept;

void
testHn() noexcept {
    String out;

    expand(out, "a");
    assert_(out == "a");

    expand(out, "&#x2F;");
    assert_(out == "/");

    expand(out, "a&#x2F;b&#x27;c");
    assert_(out == "a/b'c");
}
