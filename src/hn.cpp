#include "hn.h"

#include "net/http.h"
#include "util/compiler.h"
#include "util/string2.h"
#include "util/string-view.h"
#include "util/string.h"

/**
 * Parse a hexadecimal number in a string.
 */
static bool
parseHex32(U32& u, StringView s) noexcept {
    if (s.size == 0) {
        return false;
    }
    u = 0;
    for (Size i = 0; i < s.size; i++) {
        char c = s[i];
        u <<= 4;
        if ('0' <= c && c <= '9') {
            u |= 0 + c - '0';
        }
        else if ('A' <= c && c <= 'F') {
            u |= 10 + c - 'A';
        }
        else if ('a' <= c && c <= 'f') {
            u |= 10 + c - 'a';
        }
        else {
            return false;
        }
    }
    return true;
}

/**
 * @param out - The destination
 * @param ref - XML character entity reference without the starting & and ending ;
 */
static void
unescapeOne(String& out, StringView ref) noexcept {
    if (ref.size == 0) {
        goto fail;
    }
    if (ref[0] == '#') {
        U32 u;
        bool ok;
        if (ref.size > 1 && ref[1] == 'x') {
            // Hexadecimal
            ok = parseHex32(u, ref.substr(2));
        }
        else {
            // Decimal
            String s(ref.substr(1));
            ok = parseUInt(u, s);
        }
        if (!ok) {
            goto fail;
        }
        if (u >= 128) {
            // TODO: `u` is a Unicode code point. Write UTF-8 bytes into `out`.
            goto fail;
        }
        out << static_cast<char>(u);
        return;
    }
    if (ref == "amp") {
        out << '&';
        return;
    }
    if (ref == "gt") {
        out << '>';
        return;
    }
    if (ref == "lt") {
        out << '<';
        return;
    }
    if (ref == "quot") {
        out << '"';
        return;
    }

fail:
    out << '&' << ref << ';';
}

/**
 * Expand XML character references.
 */
void
unescape(String& out, StringView in) noexcept {
    out.size = 0;
    if (in.size > out.capacity) {
        out.reserve(in.size);
    }
    for (Size i = 0; i < in.size; i++) {
        char c = in[i];
        if (c != '&' || i + 1 == in.size) {
            out << c;
        }
        else {
            StringPosition semi = in.find(';', i + 1);
            if (semi != SV_NOT_FOUND) {
                unescapeOne(out, in.substr(i + 1, semi - 1 - i));
                i = semi;
            }
            else {
                out << c;
            }
        }
    }
}

HackerNews*
hnParse(String& response) noexcept {
    HackerNews* hn = new HackerNews;

    if (response.view().find("<rss version=\"2.0\">") != 0) {
        hn->errmsg = "Could not parse RSS";
        return hn;
    }

    HNItem* item = 0;

    Splits<'<'> tags(response);
    for (StringView tag = tags.next(); tag.data; tag = tags.next()) {
        if (tag == "item>") {
            hn->items.resize(hn->items.size + 1);
            item = &hn->items[hn->items.size - 1];
        }
        else if (!item) {
            // Do nothing.
        }
        else if (tag.startsWith("title>")) {
            // For some reason, at least titles are double-escaped. Are the other values also double-escaped?
            String tmp;
            unescape(tmp, tag.substr(6));
            unescape(item->title, tmp);
        }
        else if (tag.startsWith("link>")) {
            unescape(item->link, tag.substr(5));
        }
        else if (tag.startsWith("comments>")) {
            unescape(item->commentsHref, tag.substr(9));
        }
    }

    return hn;
}

HackerNews*
hnFetch() noexcept {
    StringView url = "https://news.ycombinator.com/rss";
    String response;

    Http* http = httpMake();
    String errmsg = httpGet(http, &response, url, 0, 0);
    httpDestroy(http);

    if (errmsg.size) {
        HackerNews* hn = new HackerNews;
        hn->errmsg = errmsg;
        return hn;
    }

    return hnParse(response);
}
