#ifndef LEDWORD_H
#define LEDWORD_H

/**
 * LedWord représentant un mot (uniquement horizontal) sur la matrice, défini par le premier pixel et la longueur du mot.
 */
class LedWord
{
public:
    LedWord();
    LedWord(int firstPixelX, int firstPixelY, int length);
    ~LedWord();
    int getFirstPixelX() const;
    void setFirstPixelX(int firstPixelX);
    int getFirstPixelY() const;
    void setFirstPixelY(int firstPixelX);
    int getLength() const;
    void setLength(int length);

private:
    int firstPixelX;
    int firstPixelY;
    int length;

protected:
};

#endif // LEDWORD_H
