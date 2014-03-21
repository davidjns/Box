#ifndef IMAGELABEL_H
#define IMAGELABEL_H

class BoxImage;
#include <QLabel>

class ImageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ImageLabel(QObject *parent = 0);
    ImageLabel(BoxImage *_box);
    ImageLabel(BoxImage *_box, QString _path);

    void undoBorder();
    QString getPath()
        { return path; }

protected:
    void mouseReleaseEvent(QMouseEvent *ev);

private:
    BoxImage *box;
    QString path;

signals:
    void clicked();

public slots:
    void clickedSlot();

};

#endif // IMAGELABEL_H
