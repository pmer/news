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
    expand(out, "?a=b&amp;c=d");
    assert_(out == "?a=b&c=d");

    String response = "<rss version=\"2.0\"><channel><title>Hacker News</title><link>https://news.ycombinator.com/</link><description>Links for the intellectually curious, ranked by readers.</description><item><title>NYC Slice</title><link>https://elkue.com/nyc-slice/</link><pubDate>Wed, 11 Jan 2023 19:00:54 +0000</pubDate><comments>https://news.ycombinator.com/item?id=34342871</comments><description><![CDATA[<a href=\"https://news.ycombinator.com/item?id=34342871\">Comments</a>]]></description></item></channel></rss>";
    HackerNews* hn = hnParse(response);
    assert_(hn->errmsg.data == 0);
    assert_(hn->items.size == 1);
    assert_(hn->items[0].title == "NYC Slice");
    assert_(hn->items[0].link == "https://elkue.com/nyc-slice/");
    assert_(hn->items[0].commentsHref == "https://news.ycombinator.com/item?id=34342871");
}
