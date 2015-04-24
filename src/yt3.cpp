#include "yt3.h"

#include <algorithm>
#include <ctime>

#include "jsfunctions.h"
#include "networkaccess.h"
#include "constants.h"

#ifdef APP_EXTRA
#include "extra.h"
#endif

#define STR(x) #x
#define STRINGIFY(x) STR(x)

namespace The {
NetworkAccess* http();
}

YT3 &YT3::instance() {
    static YT3 *i = new YT3();
    return *i;
}

YT3::YT3() {
    QByteArray customApiKey = qgetenv("GOOGLE_API_KEY");
    if (!customApiKey.isEmpty()) {
        keys << QString::fromUtf8(customApiKey);
        qDebug() << "API key from environment" << keys;
    }

    if (keys.isEmpty()) {
        QSettings settings;
        if (settings.contains("googleApiKey")) {
            keys << settings.value("googleApiKey").toString();
            qWarning() << "API key from settings" << keys;
        }
    }

#ifdef APP_GOOGLE_API_KEY
    qWarning() << "KEY="<<STRINGIFY(APP_GOOGLE_API_KEY);
    if (keys.isEmpty()) {
        keys << STRINGIFY(APP_GOOGLE_API_KEY);
        qWarning() << "built-in API key" << keys;
    } else {
        qWarning() << "KEY isnt empty";
    }
#endif

#ifdef APP_EXTRA
    if (keys.isEmpty())
        keys << Extra::apiKeys();
#endif

    if (keys.isEmpty()) {
        qWarning() << "No available API keys";
    } else {
        key = keys.takeFirst();
        testApiKey();
    }
}

const QString &YT3::baseUrl() {
    static const QString base = "https://www.googleapis.com/youtube/v3/";
    return base;
}

void YT3::testApiKey() {
    QUrl url = method("videos");
#if QT_VERSION >= 0x050000
    {
        QUrl &u = url;
        QUrlQuery url(u);
#endif
        url.addQueryItem("part", "id");
        url.addQueryItem("chart", "mostPopular");
        url.addQueryItem("maxResults", "1");
#if QT_VERSION >= 0x050000
        u.setQuery(url);
    }
#endif
    qWarning() << "testApiKey URL=" << url.toString();
    QObject *reply = The::http()->get(url);
    connect(reply, SIGNAL(finished(QNetworkReply*)), SLOT(testResponse(QNetworkReply*)));
}

void YT3::addApiKey(QUrl &url) {
    qWarning() << "CURRENT API key" << key;
    if (key.isEmpty()) {
        qWarning() << __PRETTY_FUNCTION__ << "empty key";
        return;
    }
#if QT_VERSION >= 0x050000
    {
        QUrl &u = url;
        QUrlQuery url_(u);
#endif
        url_.addQueryItem("key", key);
#if QT_VERSION >= 0x050000
        u.setQuery(url_);
    }
#endif
    qWarning() << "URL=" << url.query();
}

QUrl YT3::method(const QString &name) {
    QUrl url(baseUrl() + name);
    addApiKey(url);
    qWarning() << "METHOD URL=" << url.toString();
    return url;
}

void YT3::testResponse(QNetworkReply *reply) {
    qWarning() << "222 built-in API key" << keys;
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (status != 200) {
        if (keys.isEmpty()) {
            qWarning() << "Fatal error: No working API keys!";
            return;
        }
        key = keys.takeFirst();
        testApiKey();
    } else {
        qWarning() << "Using key" << key;
    }
}
