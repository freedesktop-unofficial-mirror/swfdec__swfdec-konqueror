
// Local
#include "flashplayerwidget.h"

// Qt
#include "qapplication.h"
#include "qpainter.h"
#include "qpixmap.h"
#include "qtimer.h"
#include "qurl.h"

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
        backingPixmap = 0;
        surface = 0;
        timer = 0;
        mouseButton = 0;
    }

    SwfdecPlayer* player;
    SwfdecLoader* loader;
   
    QPixmap backingPixmap;
    cairo_surface_t* surface;

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
    setWFlags( getWFlags() | Qt::WNoAutoErase );
    setMouseTracking(true);
    
    // load swfdec 
    if ( !Private::swfDecStarted )
    {
        swfdec_init();
        Private::swfDecStarted = true;
    }
    
    // setup player
    d->player = swfdec_player_new();

    // setup timer to advance movie
    d->timer = new QTimer(this);
    d->timer->changeInterval(40);
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
    // cairo surface
    if ( d->surface )
        cairo_surface_destroy( d->surface );

  d->backingPixmap = QPixmap(width(),height());

  // TODO - Find a way to get correct screen, visual rather
  // than just the default ones
  Display* display = d->backingPixmap.x11Display();
  int screen = d->backingPixmap.x11Screen();
  void* visual = d->backingPixmap.x11Visual();

  d->surface = cairo_xlib_surface_create(   display , 
                                            d->backingPixmap.handle(), 
                                            (Visual*)visual, 
                                            width(), 
                                            height() );
}
void FlashPlayerWidget::paintEvent( QPaintEvent* event )
{

        // copy to screen
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
        
    render();
    update();
}
void FlashPlayerWidget::render()
{
    // create cairo painter
    cairo_t* painter = cairo_create(d->surface);                             
                                                                             
    // render movie                                                          
    swfdec_player_render( d->player , painter , 0 , 0 , width() , height() );
                                                                             
    // cleanup
    cairo_show_page(painter);                                                
    cairo_destroy(painter); 
}
QSize FlashPlayerWidget::movieSize() const
{
    int width;
    int height;

    swfdec_player_get_image_size( d->player , &width , &height );

    return QSize(width,height);
}
void FlashPlayerWidget::play()
{
    if ( !d->timer->isActive() )
        d->timer->start(40,FALSE);
}
void FlashPlayerWidget::pause()
{
    d->timer->stop();
}
void FlashPlayerWidget::load(const QUrl& url)
{
    d->loader = swfdec_loader_new_from_file(url.path().utf8().data());
    swfdec_player_set_loader(d->player,d->loader);
}

#include "flashplayerwidget.moc"

