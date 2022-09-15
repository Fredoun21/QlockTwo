
#ifndef LedMapping_H
#define LedMapping_H

#include "LedWord.h"

const LedWord PREFIX_IL = LedWord(0, 0, 2);  // IL
const LedWord PREFIX_EST = LedWord(3, 0, 3); // EST

const LedWord INFIX_MOINS = LedWord(0, 6, 5); // MOINS
const LedWord INFIX_LE = LedWord(6, 6, 2);    // LE
const LedWord INFIX_ET = LedWord(0, 7, 2);    // ET

const LedWord SUFFIX_HEURE = LedWord(5, 5, 5); // HEURE

const LedWord MINUTE_CINQ = LedWord(6, 8, 4);       // CINQ
const LedWord MINUTE_DIX = LedWord(8, 6, 3);        // DIX
const LedWord MINUTE_QUART = LedWord(3, 7, 5);      // QUART
const LedWord MINUTE_VINGT = LedWord(0, 8, 5);      // VINGT
const LedWord MINUTE_VINGTCINQ = LedWord(0, 8, 10); // VINGT-CINQ
const LedWord MINUTE_DEMI = LedWord(3, 9, 4);       // DEMI

const LedWord HOURS[] = {
    LedWord(4, 2, 3), // UNE
    LedWord(7, 0, 4), // DEUX
    LedWord(6, 1, 5), // TROIS
    LedWord(0, 1, 6), // QUATRE
    LedWord(7, 3, 4), // CINQ
    LedWord(4, 3, 3), // SIX
    LedWord(7, 2, 4), // SEPT
    LedWord(0, 3, 4), // HUIT
    LedWord(0, 2, 4), // NEUF
    LedWord(2, 4, 3), // DIX
    LedWord(0, 5, 4), // ONZE
    LedWord(0, 4, 4), // MIDI
    LedWord(5, 4, 6)  // MINUIT
};

const LedWord WORD_WIFI = LedWord(8, 9, 3);

#endif /* LedMapping_H */
