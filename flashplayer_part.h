#ifndef FLASHPLAYER_PART_H
#define FLASHPLAYER_PART_H

// KDE
#include <KParts/Factory>
#include <KParts/Part>


class QStringList;

namespace FlashPlayer
{

class PartFactory : public KParts::Factory
{
protected:
    /** Reimplemented to create Konsole parts. */
    virtual KParts::Part* createPartObject(QWidget* parentWidget = 0,
                                           QObject* parent = 0,
                                           const char* classname = "KParts::Part",
                                           const QStringList& args = QStringList());
};

class Part : public KParts::ReadOnlyPart 
{
Q_OBJECT

public:
    Part(QWidget* parentWidget , QObject* parent = 0);

protected:
    /** Reimplemented from KParts::PartBase. */
    virtual bool openFile();

private:
    class Private;
    Private* d;
};

}

#endif // FLASHPLAYER_PART_H
