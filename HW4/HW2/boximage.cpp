#include "boximage.h"
#include "imagebox.h"
#include "imagelabel.h"
#include "undoaddimages.h"
#include "undoaddflickr.h"
#include "undoremoveimage.h"
#include "undoremoveimages.h"
#include "undocopy.h"
#include "undocut.h"
#include "undopaste.h"
#include "flickrcollector.h"
#include <algorithm>
#include <QClipboard>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QMessageBox>
#include <QPixmap>
#include <QLabel>
#include <QUrl>
#include <QNetworkRequest>

using namespace std;

BoxImage::BoxImage(QWidget *_parent, ImageBox *_parentBox) :
    QGridLayout(_parent),
    parent(_parent),
    parentBox(_parentBox)
{
    setSpacing(4);
    setAlignment(Qt::AlignTop | Qt::AlignLeft);

    stack = new QUndoStack();
    selected = NULL;
    copied = NULL;

    clipboard = QApplication::clipboard();

    collector = new FlickrCollector(parent);
    connect(collector, SIGNAL(ready()), this, SLOT(addFlickrReady()));
    collectorReady = true;
    waitingCalls = 0;
    flickrReady = true;
    //end

    numImages = 0;
}

void BoxImage::addFlickr()
{
    waitingCalls++;
    if(collectorReady)
    {
        qDebug("execute called");
        collectorReady = false;
        collector->execute();
        waitingCalls--;
    }
}

void BoxImage::addFlickrReady()
{
    QStringList urls = collector->list();
    qDebug("list recieved");

    if(flickrReady)
    {
        qDebug() << "UndoAddFlickr called";
        stack->push(new UndoAddFlickr(this, urls));
        flickrReady = false;
    }
    else
    {
        qDebug() << "URL list pushed to waiting";
        waitingLists.push_back(urls);
    }

    if(waitingCalls > 0)
    {
        qDebug("execute called again...");
        collector->execute();
        waitingCalls--;
    }
    else
        collectorReady = true;
}

void BoxImage::setFlickrReady()
{
    flickrReady = true;


}

void BoxImage::addCollection()
{
    vector<ImageLabel*> tempImages;

    QFileDialog dialog(parent);
    QString directory = dialog.getExistingDirectory(parent, tr("Select Directory"), "/", QFileDialog::ShowDirsOnly);
    dialog.setDirectory(directory);
    QStringList filters;
    filters << "*.bmp" << "*.gif" << "*.jpg" << "*.jpeg" << "*.png" << "*.pbm" << "*.pgm" << "*.ppm" << "*.xbm" <<  "*.xpm";
    QFileInfoList fileNames = dialog.directory().entryInfoList(filters);

    if(fileNames.empty())
        return;

    int sizeList = fileNames.size();
    for(int i = 0; i < sizeList; i++)
    {
        QFileInfo info = fileNames.front();
        fileNames.pop_front();

        QString file = info.filePath();

        ImageLabel *newImage = new ImageLabel(this);

        QPixmap newPixmap(file);
        if(newPixmap.isNull())
        {
            badImage(file);
        }
        else
        {
            newImage->setPixmap(newPixmap.scaledToWidth(150));
            tempImages.push_back(newImage);
        }
    }

    stack->push(new UndoAddImages(this, tempImages));
}

void BoxImage::addImage()
{
    vector<ImageLabel*> tempImages;

    qDebug("addImage start");
    QString searchFilter = "Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)";

    QFileDialog dialog(parent);
    QString fileName = dialog.getOpenFileName(parent, tr("Open Image"), "/", searchFilter);
    if(fileName == NULL)
        return;

    ImageLabel *newImage =  new ImageLabel(this);

    QPixmap newPixmap(fileName);
    if(newPixmap.isNull())
    {
        badImage(fileName);
        return;
    }

    newImage->setPixmap(newPixmap.scaledToWidth(150));

    tempImages.push_back(newImage);
    stack->push(new UndoAddImages(this, tempImages));
}

void BoxImage::removeAll()
{
    if(images.empty())
        return;

    vector<ImageLabel*> tempImages;
    int num = (int)images.size();
    for(int i = 0; i < num; i++)
    {
        tempImages.push_back(images.at(i));
    }

    stack->push(new UndoRemoveImages(this, tempImages));
    parentBox->showSelected(NULL);
}

void BoxImage::removeImage()
{
    if(images.empty() || selected == NULL)
        return;

    vector<ImageLabel*> tempImages;
    int num = (int)images.size();
    for(int i = 0; i < num; i++)
    {
        tempImages.push_back(images.at(i));
    }

    stack->push(new UndoRemoveImage(this, tempImages));
    parentBox->showSelected(NULL);
}

void BoxImage::cut()
{
    if(images.empty() || selected == NULL)
        return;

    vector<ImageLabel*> tempImages;
    try
    {
        int num = (int)images.size();
        for(int i = 0; i < num; i++)
        {
            tempImages.push_back(images.at(i));
        }
    }
    catch(...)
    {
        qDebug("BoxImage cut: indexing error");
    }

    stack->push(new UndoCut(this, tempImages));
    parentBox->togglePasteOn();
    parentBox->showSelected(NULL);
    qDebug("cut called");
}

void BoxImage::copy()
{
    if(selected != NULL)
    {
        stack->push(new UndoCopy(this, selected));
        parentBox->togglePasteOn();
    }

    qDebug("copied set?");
}

void BoxImage::paste()
{
    if(getCopied() == NULL)
        return;

    vector<ImageLabel*> tempImages;
    try
    {
        int num = (int)images.size();
        for(int i = 0; i < num; i++)
        {
            tempImages.push_back(images.at(i));
        }
    }
    catch(...)
    {
        qDebug("BoxImage paste: indexing error");
    }
    stack->push(new UndoPaste(this, tempImages, getCopied()));
    qDebug("paste called");
}

void BoxImage::clearGrid()
{
    ImageLabel *image;
    try
    {
        int num = numImages;
        for(int i = (num - 1); i >= 0; i--)
        {
            image = images.at(i);

            image->hide();
            removeWidget(image);
            images.pop_back();
            numImages--;
        }
    }
    catch(...)
    {
        qDebug() << "BoxImage clearGrid: indexing exception thrown";
    }
}

void BoxImage::fillGrid(std::vector<ImageLabel *> newImages)
{
    ImageLabel *image;
    try
    {
        int num = (int)newImages.size();
        for(int i = 0; i < num; i++)
        {
            image = newImages.at(i);
            image->show();
            addWidget(image, (i/5)+1, (i%5)+1);
            images.push_back(image);
            numImages++;
        }
    }
    catch(...)
    {
        qDebug() << "BoxImage fillGrid: indexing exception thrown";
    }
}

void BoxImage::imageClick(ImageLabel *nextSelect)
{
    if(selected)
        selected->undoBorder();

    if(selected == nextSelect)
        selected = NULL;
    else
        selected = nextSelect;

    parentBox->showSelected(selected);
}

void BoxImage::fixBox()
{
    resizeGrid();
    parentBox->toggleRemove((int)images.size());

    if((int)images.size() == 0)
        resetSelected();
}

void BoxImage::resizeGrid()
{
    if(numImages <= 5)
        parent->resize(175 + ((775 * numImages) / 5), 200);
    else
        parent->resize(950, (numImages/5 + 1)*200);
}

void BoxImage::badImage(QString fileName)
{
    QMessageBox *errorBox = new QMessageBox();
    errorBox->setModal(true);
    errorBox->setText("Error reading file : " + fileName);
    errorBox->setWindowTitle("Error Reading File");
    errorBox->exec();
}

ImageLabel* BoxImage::getCopied()
{
    ImageLabel *newImage = new ImageLabel(this);
    newImage->setPixmap(clipboard->pixmap());
    return newImage;
}

void BoxImage::setCopied(ImageLabel *image)
{
    clipboard->setPixmap(*(image->pixmap()));
}
