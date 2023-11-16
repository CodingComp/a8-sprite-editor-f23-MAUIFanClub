#include "fileSystem.h"
#include "qforeach.h"
#include <QTextStream>
FileSystem::FileSystem(AnimationManager* animManager, SpriteCanvas* spriteCanv)
{
    animationManager = animManager;
    spriteCanvas = spriteCanv;
}

FileSystem::FileSystem(){}

FileSystem& FileSystem::operator= (FileSystem other)
{
    std::swap(animationManager, other.animationManager);
    std::swap(spriteCanvas, other.spriteCanvas);
    return *this;
}

void FileSystem::loadJson(QString filepath)
{
    QFile loadFile(filepath);
    if(!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning("Could not open file.");
        return;
    }
    QByteArray loadData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(loadData));
    readSpritefromJson(loadDoc.object());
}
void FileSystem::readSpritefromJson(const QJsonObject &sprite)
{
    int size = sprite["frameSize"].toInt();
    int frameCount = sprite["frameCount"].toInt();

    QLabel *tempLabel = spriteCanvas->getSpriteCanvas();
    spriteCanvas->refreshSpriteCanvas(tempLabel, size);

    animationManager->clearAnimationFrames();
    QScrollArea *tempScroll = animationManager->getFramesPanel();
    animationManager = new AnimationManager(spriteCanvas, tempScroll, size, false);



    QJsonArray frameArray = sprite["frames"].toArray();



    for(int frameIndex = 0; frameIndex < frameArray.size(); frameIndex++)
    {
        QImage tempImage(size, size, QImage::Format_ARGB32);

        QJsonObject frameval = frameArray[frameIndex].toObject();
        QString label = QStringLiteral("frame").append(QString::number(frameIndex));
        QJsonArray framevalues = frameval[label].toArray();

        foreach(const QJsonValue &v, framevalues)
        {
            QJsonObject value = v.toObject();
            tempImage.setPixelColor(value["x"].toInt(),
                                    value["y"].toInt(),
                                    QColor(value["red"].toInt(),
                                           value["green"].toInt(),
                                           value["blue"].toInt(),
                                           value["alpha"].toInt()));
        }
        QPixmap map = QPixmap::fromImage(tempImage);
        animationManager->createNewFrame(map);
    }
    animationManager->changeDisplayedFrame(0);
}

void FileSystem::saveSprite(QString filename, int size)
{
    QFile saveFile(filename);
    if(!saveFile.open(QIODevice::WriteOnly))
    {
        qWarning("Could not save file.");
        return;
    }

    QJsonObject JsonSprite;
    JsonSprite["frameSize"] = size;
    writeSpriteToJson(JsonSprite);
    QJsonDocument JsonDoc(JsonSprite);
    saveFile.write(JsonDoc.toJson());
}

void FileSystem::writeSpriteToJson(QJsonObject &sprite)
{
    sprite["frameCount"] = animationManager->getSize();
    QJsonArray frames;

    int frameIndex = 0;
    foreach(AnimationManager::AnimationFrame frame, animationManager->animationFrames){
        QJsonObject frameObject;
        writeFrameToJson(frameObject, *frame.animationPixmap, frameIndex);
        frames.append(frameObject);
        frameIndex++;
    }

    sprite["frames"] = frames;
}
void FileSystem::writeFrameToJson(QJsonObject &frame, QPixmap pixmap, int index)
{
    QJsonArray pixels;
    QImage image = pixmap.toImage();

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color(image.pixel(x, y));

            QJsonObject pixelInfo;
            pixelInfo["x"] = x;
            pixelInfo["y"] = y;
            pixelInfo["red"] = color.red();
            pixelInfo["green"] = color.green();
            pixelInfo["blue"] = color.blue();
            pixelInfo["alpha"] = color.alpha();

            pixels.append(pixelInfo);
        }
    }

    frame[QStringLiteral("frame").append(QString::number(index))] = pixels;
}
