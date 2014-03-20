#include "imagelabel.h"
#include "boximage.h"

ImageLabel::ImageLabel(QObject *parent) :
    QLabel()
{
    setLineWidth(3);
    setMidLineWidth(3);
    setFrameStyle(QFrame::Panel);
}

ImageLabel::ImageLabel(BoxImage *_box) :
    QLabel(),
    box(_box)
{
    setLineWidth(1);
    setMidLineWidth(1);
    setFrameStyle(QFrame::Panel | QFrame::Plain);
    connect(this, SIGNAL(clicked()), this, SLOT(clickedSlot()));
}

void ImageLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    emit clicked();
}

void ImageLabel::clickedSlot()
{
    setLineWidth(3);
    setMidLineWidth(3);
    box->imageClick(this);
}

void ImageLabel::undoBorder()
{
    setLineWidth(1);
    setMidLineWidth(1);
}
