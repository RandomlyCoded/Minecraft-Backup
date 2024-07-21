#include "worldiconprovider.h"

#include <QDebug>

namespace randomly {

WorldIconProvider::WorldIconProvider(QObject *parent, Options &opt)
    : QQuickImageProvider(Image)
    , m_options(opt)
{
    setParent(parent);
}

QImage WorldIconProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage icon;
    auto path = m_options.savesDirectory / id.toStdString() / "icon.png";

    icon.load(QString::fromStdString(path.string()));

    if (icon.isNull())
        icon.load(":/noWorldIcon.png");

    return icon;
}

} // namespace randomly
