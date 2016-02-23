#ifndef ILABEL_H
#define ILABEL_H

#include <QLabel>
#include <QMouseEvent>
class ILabel : public QLabel
{
    Q_OBJECT
public:
    ILabel();

public:
    virtual void mouseReleaseEvent(QMouseEvent * event);
signals:
    void click(void);
};

#endif // ILABEL_H
