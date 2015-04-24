#include "transition_qt4_to_5.h"

#if QT_VERSION >= 0x050000
bool QUrl_hasQueryItem(const QUrl& url, const QString& item) {
    const QUrlQuery query(url.query());
    return query.hasQueryItem(item);
}

void QUrl_removeQueryItem(QUrl& url, const QString& item) {
    QUrlQuery query(url.query());
    query.removeQueryItem(item);
    url.setQuery(query);
}

void QUrl_addQueryItem(QUrl& url, const QString& itemKey, const QString& itemValue) {
    QUrlQuery query(url.query());
    query.addQueryItem(itemKey, itemValue);
    url.setQuery(query);
}
#endif
