#ifndef SPRITECANVAS_H
#define SPRITECANVAS_H

#include <QPoint>
#include <QPixmap>
#include <QLabel>
#include <QPointF>
#include <QColor>
#include <QMouseEvent>
#include <QDebug>
#include "undoredomanager.h"
#include "framepreviewui.h"

class SpriteCanvas
{
public:
    UndoRedoManager* undoRedoManager;

    SpriteCanvas(QLabel *spriteCanvas, int spriteSize);

    FramePreviewUi *previewFrameUi;

    void changePixmap(QPixmap newPixmap);

    void mousePress(QPoint globalMousePos);
    void mouseMove(QPoint globalMousePos);

    void mousePress(QPoint globalMousePos, bool isDraw);
    void mouseMove(QPoint globalMousePos, bool isDraw);

    void mouseRelease();

    void setPixelColor(QColor color);
    void clearCanvas();

    void changePixmap(QPixmap *newPixmap);

    void displayAnimationFrame(QPixmap *animationFramePixmap, bool actualSize);

    void undoAction();
    void redoAction();
private:
    QPixmap *spritePixmap;
    QPainter painter;
    QLabel *spriteCanvas;
    QColor pixelColor;

    int spriteCanvasSize;
    int spriteSize;

    // Used to make sure if the mouse is held down on the same pixel it doesnt set the pixel repeatedly without moving first.
    QPoint lastDrawnPixel;

    // Draws a pixel at a given position
    void drawPixel(QPoint pixelPosition);
    void erasePixel (QPoint pixelPoisiton);
    bool mouseOnSpriteCanvas(QPoint globalMousePos);
    QPoint getPixelPosition(QPoint mousePos);

    bool drawing;
};

#endif // SPRITECANVAS_H
