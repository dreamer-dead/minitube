#ifndef TRANSITION_QT4_TO_5_H
#define TRANSITION_QT4_TO_5_H

#include <QtCore>

QT_FORWARD_DECLARE_CLASS(QUrl)

#if QT_VERSION >= 0x050000
bool QUrl_hasQueryItem(const QUrl& url, const QString& item);
void QUrl_removeQueryItem(QUrl& url, const QString& itemKey);
void QUrl_addQueryItem(QUrl& url, const QString& itemKey, const QString& itemValue);
#else
#define QUrl_hasQueryItem(url, item) url.hasQueryItem(item)
QUrl_removeQueryItem(url, key, value) url.removeQueryItem(key)
QUrl_addQueryItem(url, key, value) url.addQueryItem(key, value)
#endif

#endif // TRANSITION_QT4_TO_5_H

