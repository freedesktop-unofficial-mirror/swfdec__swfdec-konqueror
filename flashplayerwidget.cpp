
// Local
#include "flashplayerwidget.h"

// Qt
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QX11Info>
#include <QtDebug>

// Cairo
#include <X11/Xlib.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

// swfdec
#include <libswfdec/swfdec.h>

class FlashPlayerWidget::Private
{
public:
    Private()
    {
        player = 0;
        loader = 0;
        surface = 0;
        timer = 0;
        mouseButton = 0;
    }

    SwfdecPlayer* player;
    SwfdecLoader* loader;
    cairo_surface_t* surface;
    QPixmap backingPixmap;

    QTimer* timer;

    int mouseButton;

    static bool swfDecStarted; 
};

bool FlashPlayerWidget::Private::swfDecStarted = false;

FlashPlayerWidget::FlashPlayerWidget(QWidget* parent)
    : QWidget(parent)
    , d(new Private)
{
    // widget attributes
    setAttribute(Qt::WA_OpaquePaintEvent);
    setMouseTracking(true);
    
    // load swfdec 
    if ( !Private::swfDecStarted )
    {
        swfdec_init();
        Private::swfDecStarted = true;

        qDebug() << "Started swfdec";
    }
    
    // setup player
    d->player = swfdec_player_new();

    // setup timer to advance movie
    d->timer = new QTimer(this);
    d->timer->setInterval(40);
    connect( d->timer , SIGNAL(timeout()) , this , SLOT(advance()) );
}
FlashPlayerWidget::~FlashPlayerWidget()
{
    cairo_surface_destroy( d->surface );
}
void FlashPlayerWidget::resizeEvent( QResizeEvent* event )
{
    prepareSurface();
}
void FlashPlayerWidget::prepareSurface()
{
    // Qt backing pixmap
    d->backingPixmap = QPixmap(width(),height());
    
    // cairo surface
    if ( d->surface )
        cairo_surface_destroy( d->surface );

    d->surface = cairo_xlib_surface_create( QX11Info::display() , d->backingPixmap.handle() , 
                                            (Visual*)(d->backingPixmap.x11Info().visual()) , 
                                            d->backingPixmap.width() , 
                                            d->backingPixmap.height() );
}
void FlashPlayerWidget::paintEvent( QPaintEvent* event )
{
        // create cairo painter
        cairo_t* painter = cairo_create(d->surface);
        
        // render movie
        swfdec_player_render( d->player , painter , 0 , 0 , width() , height() );
        
        // cleanup
        cairo_show_page(painter);
        cairo_destroy(painter);

        // copy pixmap to screen
        QPainter qtPainter(this);
        qtPainter.drawPixmap(0,0,d->backingPixmap);
}

void FlashPlayerWidget::mousePressEvent( QMouseEvent* event )
{
    d->mouseButton = 1;
    handleMouseEvent(event);
}
void FlashPlayerWidget::mouseMoveEvent( QMouseEvent* event )
{
    handleMouseEvent(event);
}
void FlashPlayerWidget::mouseReleaseEvent( QMouseEvent* event )
{
    d->mouseButton = 0;
    handleMouseEvent(event);
}
void FlashPlayerWidget::handleMouseEvent( QMouseEvent* event )
{
    swfdec_player_handle_mouse( d->player , event->pos().x() , event->pos().y() ,
                                d->mouseButton );

    event->accept();
}
void FlashPlayerWidget::advance()
{
    swfdec_player_advance( d->player , swfdec_player_get_next_event(d->player) );
    update(); 
}

QSize FlashPlayerWidget::movieSize() const
{
    int width;
    int height;

    swfdec_player_get_image_size(d->player,&width,&height);
    return QSize(width,height);
}

void FlashPlayerWidget::play()
{
    if ( !d->timer->isActive() )
        d->timer->start();
}
void FlashPlayerWidget::pause()
{
    d->timer->stop();
}
void FlashPlayerWidget::load(const QUrl& url)
{
    d->loader = swfdec_loader_new_from_file(url.toLocalFile().toUtf8().constData());
    swfdec_player_set_loader(d->player,d->loader);
}

#include "flashplayerwidget.moc"

