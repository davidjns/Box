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
#include <QDesktopWidget>

using namespace std;

BoxImage::BoxImage(QWidget *_parent, ImageBox *_parentBox) :
    QGridLayout(_parent),
    parent(_parent),
    parentBox(_parentBox)
{
    parent->resize(800, 600);

    setSpacing(4);
    setAlignment(Qt::AlignTop | Qt::AlignLeft);

    stack = new QUndoStack();
    selected = NULL;
    copied = NULL;

    clipboard = QApplication::clipboard();

    nam = new QNetworkAccessManager(this);
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    numFinished = 0;

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

        int size = urls.size();
        for(int i = 0; i < size; i++)
        {
            QString string = urls.front();
            urls.pop_front();
            QUrl url(string);
            QNetworkReply *reply = nam->get(QNetworkRequest(url));
        }
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

    if(!waitingLists.empty())
    {
        QStringList urls = waitingLists.front();
        waitingLists.pop_front();
        qDebug() << "UndoAddFlickr pushed from waitingList";

        int size = urls.size();
        for(int i = 0; i < size; i++)
        {
            QString string = urls.front();
            urls.pop_front();
            QUrl url(string);
            QNetworkReply *reply = nam->get(QNetworkRequest(url));
        }
        flickrReady = false;
    }
}

void BoxImage::replyFinished(QNetworkReply *reply)
{
    numFinished++;
    QImageReader imageReader(reply);
    imageReader.setAutoDetectImageFormat(true);
    QImage image = imageReader.read();

    ImageLabel *newImage = new ImageLabel(this);
    QPixmap pix = QPixmap::fromImage(image);
    newImage->setPixmap(pix.scaledToWidth(150));

    newImages.push_back(newImage);

    if(numFinished == 10)
    {
        stack->push(new UndoAddFlickr(this, copyImages(), newImages));
        newImages.clear();
        numFinished = 0;
        setFlickrReady();
    }
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
    parentBox->togglePaste(getCopied());
    parentBox->showSelected(NULL);
    qDebug("cut called");
}

void BoxImage::copy()
{
    if(selected != NULL)
    {
        stack->push(new UndoCopy(this, selected));
        parentBox->togglePaste(getCopied());
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

    fixBox();
}

vector<ImageLabel*> BoxImage::copyImages()
{
    vector<ImageLabel*> tempImages;

    int num = (int)images.size();
    for(int i = 0; i < num; i++)
    {
        tempImages.push_back(images.at(i));
    }

    return tempImages;
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
    parentBox->toggleCutCopy(selected);
}

void BoxImage::fixBox()
{
    resizeGrid();
    parentBox->toggleRemove((int)images.size());

    if((int)images.size() == 0)
        resetSelected();

    parentBox->toggleCutCopy(selected);
    parentBox->togglePaste(getCopied());
    //TODO: check out the togglePaste stuff, don't let it paste a null, bitch!
}

void BoxImage::resizeGrid()
{
    QDesktopWidget desktop;
    int maxHeight = desktop.size().height() - 50;
    int maxWidth = desktop.size().width();
    int currHeight = parent->size().height();
    int currWidth = parent->size().width();

    int newHeight = (numImages/5 + 1) * 155 + 30;
    int newWidth;
    if(numImages < 5)
        newWidth = 375 + (numImages * 155);
    else
        newWidth = 375 + (5 * 155);

    if(maxHeight < newHeight)
    {
        if(maxWidth < newWidth)
            parent->resize(maxWidth, maxHeight);
        else if(currWidth < newWidth)
            parent->resize(newWidth, maxHeight);
        else
            parent->resize(currWidth, maxHeight);
    }
    else if(currHeight < newHeight)
    {
        if(maxWidth < newWidth)
            parent->resize(maxWidth, newHeight);
        else if(currWidth < newWidth)
            parent->resize(newWidth, newHeight);
        else
            parent->resize(currWidth, newHeight);
    }
    else
    {
        if(maxWidth < newWidth)
            parent->resize(maxWidth, currHeight);
        if(currWidth < newWidth)
            parent->resize(newWidth, currHeight);
        else
            parent->resize(currWidth, currHeight);
    }
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
