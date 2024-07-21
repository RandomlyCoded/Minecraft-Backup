#ifndef RANDOMLY_WORLDICONPROVIDER_H
#define RANDOMLY_WORLDICONPROVIDER_H

#include "commandLine.h"

#include <QQuickImageProvider>

namespace randomly {

class WorldIconProvider : public QQuickImageProvider
{
    Q_OBJECT

public:
    WorldIconProvider(QObject *parent, Options &opt);

    // the id holds the world name
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

private:
    Options &m_options;
};

} // namespace randomly

#endif // RANDOMLY_WORLDICONPROVIDER_H
