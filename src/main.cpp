#include "hn.h"

#include "net/html.h"
#include "util/compiler.h"
#include "util/io.h"

const char* header = "<!DOCTYPE html><html><body>";
const char* trailer = "</body></html>";

static void
a(String& out, StringView title, StringView href) noexcept {
    out << "<a href=\"";
    htmlEscape(out, href);
    out << "\">";
    htmlEscape(out, title);
    out << "</a>";
}

int
main() noexcept {
    Flusher f2(serr);
    Flusher f1(sout);

    sout << header;

    // sout << "Fetching Hacker News... " << Flush();
    HackerNews* hn = hnFetch();
    // sout << " done\n" << Flush();

    if (hn->errmsg.size) {
        sout << "err: " << hn->errmsg << '\n';
        return 1;
    }

    //for (HNItem& item : hn->items) {
    //    buf << item.title << "\n\t" << item.link << '\n';
    //}

    String buf;
    buf << "<ol>";
    for (HNItem& item : hn->items) {
        buf << "<li>";
        a(buf, item.title, item.link);
        buf << "</li>";
    }
    buf << "</ol>";
    sout << buf;

    sout << trailer;

    return 0;
}
