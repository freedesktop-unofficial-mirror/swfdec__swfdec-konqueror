
// Own
#include "flashplayer_part.h"

// Local
#include "flashplayerwidget.h"

extern "C"
{
    // entry point for the swfdec-based flash player part library,
    // returns a new factory which can be used to construct Konsole parts
    KDE_EXPORT void* init_libflashplayerpart()
    {
        return new FlashPlayer::PartFactory;
    }
}

using namespace FlashPlayer;

KParts::Part* PartFactory::createPartObject( QWidget* parentWidget,
                                             QObject* parent,
                                             const char* /*classname*/,
                                             const QStringList& /*args*/)
{
    return new Part(parentWidget,parent);
}

class Part::Private
{
public:
    FlashPlayerWidget* player;        
};

Part::Part(QWidget* parentWidget , QObject* parent)
 : KParts::ReadOnlyPart(parent)
 , d(new Private)
{
    d->player = new FlashPlayerWidget(parentWidget);
    
    setWidget(d->player);
}
bool Part::openFile()
{
    d->player->load(localFilePath());
    d->player->play();

    return true;
}

#include "flashplayer_part.moc"
