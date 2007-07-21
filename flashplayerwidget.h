
#ifndef FLASHPLAYERWIDGET_H
#define FLASHPLAYERWIDGET_H

// Qt
#include <qwidget.h>

class QUrl;

class FlashPlayerWidget : public QWidget
{
Q_OBJECT

    public:
        FlashPlayerWidget(QWidget* parent = 0);
        virtual ~FlashPlayerWidget();

        QSize movieSize() const;

    public slots:
        void play();
        void pause(); 
        void load(const QUrl& url);

    protected:
        virtual void paintEvent( QPaintEvent* event );
        virtual void mousePressEvent( QMouseEvent* event );
        virtual void mouseReleaseEvent( QMouseEvent* event );
        virtual void mouseMoveEvent( QMouseEvent* event );
        virtual void resizeEvent( QResizeEvent* event );

    private slots:
        void advance();

    private:
        void handleMouseEvent( QMouseEvent* event ); 
        void prepareSurface(); 
        void render();

    private:
        class Private;
        Private* d;
};

#endif //FLASHPLAYERWIDGET_H

