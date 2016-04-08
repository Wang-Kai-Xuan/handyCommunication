#include "ilabel.h"

ILabel::ILabel()
{

}

void ILabel::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit click();
}

