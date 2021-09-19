#include "main.h"

// Alphanumeric keyboard base layer
uint8_t ALPHA_MAP[56] = {
    // Register 1
    0x3e, // graph  -> f5
    0x3d, // trace  -> f4
    0x3c, // zoom   -> f3
    0x3b, // window -> f2
    0x3a, // y =    -> f1
    0x00, // 2nd is used as a modifier
    0x00, // mode exits the program
    0x2a, // del  -> delete

    // Register 2
    0x00, // unused
    0x1b, // sto  -> X
    0x16, // ln   -> S
    0x11, // log  -> N
    0x0c, // x^2  -> I
    0x07, // x^-1 -> D
    0x04, // math -> A
    0x00, // alpha is used as a modifier

    // Register 3
    0x2c, // 0    -> space
    0x1c, // 1    -> Y
    0x17, // 4    -> T
    0x12, // 7    -> O
    0x0d, // ,    -> J
    0x08, // sin  -> E
    0x05, // apps -> B
    0x00, // xt0n -> ???

    // Register 4
    0x33, // .    -> ;     (it should be :, but that doesn't have its own scancode)
    0x1d, // 2    -> Z
    0x18, // 5    -> U
    0x13, // 8    -> P
    0x0e, // (    -> K
    0x09, // cos  -> F
    0x06, // prgm -> C
    0x00, // stat -> ???

    // Register 5
    0x38, // (-)  -> /     (it should be ?, but that doesn't have its own scancode)
    0x00, // 3    -> ???   (marked as theta on calculator)
    0x19, // 6    -> V
    0x14, // 9    -> Q
    0x0f, // )    -> L
    0x0a, // tan  -> G
    0x00, // vars -> ???
    0x00, // unused

    // Register 6
    0x28, // enter -> enter
    0x34, // +     -> '    (it should be ", but that doesn't have its own scancode)
    0x1a, // 7     -> W
    0x15, // X     -> R
    0x10, // /     -> M
    0x0b, // ^     -> H
    0x00, // clear -> ???
    0x00, // unused

    // Register 7
    0x51, // down  -> down
    0x50, // left  -> left
    0x4f, // right -> right
    0x42, // up    -> up
    0x00, // unused
    0x00, // unused
    0x00, // unused
    0x00, // unused
};

// Shift layer: capitalizes letters and changes behavior of some keys
uint8_t ALPHA_MAP_SHIFT[56] = {
    // Register 1
    0x3e, // table     -> f5
    0x3d, // calc      -> f4
    0x3c, // format    -> f3
    0x3b, // tblset    -> f2
    0x3a, // stat plot -> f1
    0x00, // 2nd is used as a modifier
    0x00, // mode exits the program
    0x2a, // ins       -> insert    CHANGED

    // Register 2
    0x00, // unused
    0x1b, // sto  -> X
    0x16, // ln   -> S
    0x11, // log  -> N
    0x0c, // x^2  -> I
    0x07, // x^-1 -> D
    0x04, // math -> A
    0x00, // alpha is used as a modifier

    // Register 3
    0x2c, // 0    -> space
    0x1c, // 1    -> Y
    0x17, // 4    -> T
    0x12, // 7    -> O
    0x0d, // ,    -> J
    0x08, // sin  -> E
    0x05, // apps -> B
    0x00, // xt0n -> ???

    // Register 4
    0x33, // .    -> :
    0x1d, // 2    -> Z
    0x18, // 5    -> U
    0x13, // 8    -> P
    0x0e, // (    -> K
    0x09, // cos  -> F
    0x06, // prgm -> C
    0x00, // stat -> ???

    // Register 5
    0x38, // (-)  -> ?
    0x00, // 3    -> ???   (marked as theta on calculator)
    0x19, // 6    -> V
    0x14, // 9    -> Q
    0x0f, // )    -> L
    0x0a, // tan  -> G
    0x00, // vars -> ???
    0x00, // unused

    // Register 6
    0x28, // enter -> enter
    0x34, // +     -> "
    0x1a, // 7     -> W
    0x15, // X     -> R
    0x10, // /     -> M
    0x0b, // ^     -> H
    0x00, // clear -> ???
    0x00, // unused

    // Register 7
    0x4e, // down  -> pagedown
    0x4a, // left  -> home
    0x4d, // right -> end
    0x4b, // up    -> pageup
    0x00, // unused
    0x00, // unused
    0x00, // unused
    0x00, // unused
};


// Alpha layer: revert to numerical layout
uint8_t ALPHA_MAP_ALPHA[56] = {
    // Register 1
    0x3e, // graph  -> f5
    0x3d, // trace  -> f4
    0x3c, // zoom   -> f3
    0x3b, // window -> f2
    0x3a, // y =    -> f1
    0x00, // 2nd is used as a modifier
    0x00, // mode exits the program
    0x2a, // del  -> delete

    // Register 2
    0x00, // unused
    0x2e, // sto  -> =
    0x00, // ln   -> ???
    0x00, // log  -> ???
    0x00, // x^2  -> ???
    0x00, // x^-1 -> ???
    0x00, // math -> ???
    0x00, // alpha is used as a modifier

    // Register 3
    0x27, // 0    -> 0
    0x1e, // 1    -> 1
    0x21, // 4    -> 4
    0x24, // 7    -> 7
    0x36, // ,    -> ,
    0x00, // sin  -> ???
    0x00, // apps -> ??? (this should be META, but that's a modifier, not a scancode)
    0x00, // xt0n -> ???

    // Register 4
    0x37, // .    -> .
    0x1f, // 2    -> 2
    0x22, // 5    -> 5
    0x25, // 8    -> 6
    0x2f, // (    -> [
    0x00, // cos  -> ???
    0x00, // prgm -> ???
    0x00, // stat -> ???

    // Register 5
    0x2d, // (-)  -> -
    0x20, // 3    -> 3
    0x23, // 6    -> 6
    0x26, // 9    -> 9
    0x30, // )    -> ]
    0x00, // tan  -> ???
    0x00, // vars -> ???
    0x00, // unused

    // Register 6
    0x28, // enter -> enter
    0x2e, // +     -> = (this should be +, but that doesn't have its own scancode)
    0x2d, // -     -> -
    0x25, // X     -> 8 (this should be *, but that doesn't have its own scancode)
    0x38, // /     -> /
    0x23, // ^     -> 6 (this should be ^, but that doesn't have its own scancode)
    0x00, // clear -> ???
    0x00, // unused

    // Register 7
    0x51, // down  -> down
    0x50, // left  -> left
    0x4f, // right -> right
    0x42, // up    -> up
    0x00, // unused
    0x00, // unused
    0x00, // unused
    0x00, // unused
};


const uint8_t ALPHA_MODIFIER_MAP[8] = {
    0xFF, // left CTRL unused
    0x05, // 2nd   -> left SHIFT
    0xFF, // left ALT unused
    0xFF, // left META unused
    0xFF, // right CTRL unused
    0xFF, // right SHIFT unused
    0xFF, // right ALT unused
    0xFF, // right META unused
};


#define SCAN_CODES        ALPHA_MAP
#define SCAN_CODES_2ND    ALPHA_MAP_SHIFT
#define SCAN_CODES_ALPHA  ALPHA_MAP_ALPHA
#define MODIFIER_MAP      ALPHA_MODIFIER_MAP
