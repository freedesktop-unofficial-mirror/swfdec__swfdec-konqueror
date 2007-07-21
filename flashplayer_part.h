#ifndef FLASHPLAYER_PART_H
#define FLASHPLAYER_PART_H

// KDE
#include <kparts/factory.h>
#include <kparts/part.h>


class QStringList;

namespace FlashPlayer
{

class PartFactory : public KParts::Factory
{
protected:
    /** Reimplemented to create Konsole parts. */
    virtual KParts::Part* createPartObject(QWidget* parentWidget = 0,
                                           const char* something = 0,
                                           QObject* parent = 0,
                                           const char* classname = "KParts::Part",
                                           const char* something_2 = 0,
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
